#include "texture_stage.h"

#include "debug_output.h"
#include "nxdk_ext.h"
#include "pbkit_ext.h"
#include "swizzle.h"

// bitscan forward
static int bsf(int val) { __asm bsf eax, val }

bool TextureStage::RequiresColorspaceConversion() const {
  return format_.xbox_format == NV097_SET_TEXTURE_FORMAT_COLOR_LC_IMAGE_CR8YB8CB8YA8 ||
         format_.xbox_format == NV097_SET_TEXTURE_FORMAT_COLOR_LC_IMAGE_YB8CR8YA8CB8;
}

void TextureStage::Commit(uint32_t memory_dma_offset, uint32_t palette_dma_offset) const {
  if (!enabled_) {
    auto p = pb_begin();
    p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_TX_ENABLE(stage_), false);
    pb_end(p);
    return;
  }

  if (!format_.xbox_bpp) {
    PrintMsg("No texture format specified. This will cause an invalid pgraph state exception and a crash.");
    ASSERT(!"No texture format specified. This will cause an invalid pgraph state exception and a crash.");
  }

  auto p = pb_begin();

  // NV097_SET_TEXTURE_CONTROL0
  p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_TX_ENABLE(stage_),
               NV097_SET_TEXTURE_CONTROL0_ENABLE | NV097_SET_TEXTURE_CONTROL0_MAX_LOD_CLAMP);

  uint32_t dimensionality = GetDimensionality();

  uint32_t size_u = bsf((int)width_);
  uint32_t size_v = bsf((int)height_);
  uint32_t size_p = 0;
  if (dimensionality > 2) {
    size_p = bsf((int)depth_);
  }

  const uint32_t DMA_A = 1;
  const uint32_t DMA_B = 2;

  uint32_t format =
      MASK(NV097_SET_TEXTURE_FORMAT_CONTEXT_DMA, DMA_A) | MASK(NV097_SET_TEXTURE_FORMAT_CUBEMAP_ENABLE, 0) |
      MASK(NV097_SET_TEXTURE_FORMAT_BORDER_SOURCE, NV097_SET_TEXTURE_FORMAT_BORDER_SOURCE_COLOR) |
      MASK(NV097_SET_TEXTURE_FORMAT_DIMENSIONALITY, dimensionality) |
      MASK(NV097_SET_TEXTURE_FORMAT_COLOR, format_.xbox_format) |
      MASK(NV097_SET_TEXTURE_FORMAT_MIPMAP_LEVELS, mipmap_levels_) |
      MASK(NV097_SET_TEXTURE_FORMAT_BASE_SIZE_U, size_u) | MASK(NV097_SET_TEXTURE_FORMAT_BASE_SIZE_V, size_v) |
      MASK(NV097_SET_TEXTURE_FORMAT_BASE_SIZE_P, size_p);

  uint32_t offset = reinterpret_cast<uint32_t>(memory_dma_offset) + texture_memory_offset_;
  uint32_t texture_addr = offset & 0x03ffffff;
  // NV097_SET_TEXTURE_OFFSET
  p = pb_push2(p, NV20_TCL_PRIMITIVE_3D_TX_OFFSET(stage_), texture_addr, format);

  uint32_t pitch_param = (format_.xbox_bpp * width_) << 16;
  ;
  uint32_t size_param = size_param = (width_ << 16) | (height_ & 0xFFFF);
  // NV097_SET_TEXTURE_CONTROL1
  p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_TX_NPOT_PITCH(stage_), pitch_param);
  // NV097_SET_TEXTURE_IMAGE_RECT
  p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_TX_NPOT_SIZE(stage_), size_param);

  // (0x0W0V0U wrapping: 1=wrap 2=mirror 3=clamp 4=border 5=clamp to edge)
  // NV097_SET_TEXTURE_ADDRESS
  p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_TX_WRAP(stage_), 0x00030303);

  // NV097_SET_TEXTURE_FILTER
  p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_TX_FILTER(stage_), texture_filter_);

  p = pb_push4(p, NV097_SET_TEXTURE_MATRIX_ENABLE, 0, 0, 0, 0);

  uint32_t palette_config = 0;
  if (format_.xbox_format == NV097_SET_TEXTURE_FORMAT_COLOR_SZ_I8_A8R8G8B8) {
    ASSERT(palette_length_ <= 3 && "Invalid attempt to use paletted format without setting palette.");
    uint32_t palette_offset = palette_dma_offset + palette_memory_offset_;
    palette_offset &= 0x03ffffc0;
    palette_config = MASK(NV097_SET_TEXTURE_PALETTE_CONTEXT_DMA, DMA_A) |
                     MASK(NV097_SET_TEXTURE_PALETTE_LENGTH, palette_length_) | palette_offset;
  }

  // NV097_SET_TEXTURE_PALETTE
  p = pb_push1(p, NV20_TCL_PRIMITIVE_3D_TX_PALETTE_OFFSET(stage_), palette_config);

  p = pb_push1(p, NV097_SET_TEXTURE_BORDER_COLOR, border_color_);

  p = pb_push4f(p, NV097_SET_TEXTURE_SET_BUMP_ENV_MAT, bump_env_material[0], bump_env_material[1], bump_env_material[2],
                bump_env_material[3]);
  p = pb_push1f(p, NV097_SET_TEXTURE_SET_BUMP_ENV_SCALE, bump_env_scale);
  p = pb_push1f(p, NV097_SET_TEXTURE_SET_BUMP_ENV_OFFSET, bump_env_offset);

  pb_end(p);
}

void TextureStage::SetFilter(uint32_t lod_bias, TextureStage::ConvolutionKernel kernel, TextureStage::MinFilter min,
                             TextureStage::MagFilter mag, bool signed_alpha, bool signed_red, bool signed_green,
                             bool signed_blue) {
  texture_filter_ =
      MASK(NV097_SET_TEXTURE_FILTER_MIPMAP_LOD_BIAS, lod_bias) |
      MASK(NV097_SET_TEXTURE_FILTER_CONVOLUTION_KERNEL, kernel) | MASK(NV097_SET_TEXTURE_FILTER_MIN, min) |
      MASK(NV097_SET_TEXTURE_FILTER_MAG, mag) | MASK(NV097_SET_TEXTURE_FILTER_ASIGNED, signed_alpha) |
      MASK(NV097_SET_TEXTURE_FILTER_RSIGNED, signed_red) | MASK(NV097_SET_TEXTURE_FILTER_GSIGNED, signed_green) |
      MASK(NV097_SET_TEXTURE_FILTER_BSIGNED, signed_blue);
}

int TextureStage::SetTexture(const SDL_Surface *surface, uint8_t *memory_base) const {
  auto pixels = static_cast<uint32_t *>(surface->pixels);

  // if conversion required, do so, otherwise use SDL to convert
  if (format_.require_conversion) {
    uint8_t *dest = memory_base + texture_memory_offset_;

    // TODO: potential reference material -
    // https://github.com/scalablecory/colors/blob/master/color.c
    switch (format_.xbox_format) {
      case NV097_SET_TEXTURE_FORMAT_COLOR_LC_IMAGE_CR8YB8CB8YA8:  // YUY2 aka
                                                                  // YUYV
      {
        uint32_t *source = pixels;
        for (int y = 0; y < surface->h; ++y)
          for (int x = 0; x < surface->w; x += 2, source += 2) {
            uint8_t R0, G0, B0, R1, G1, B1;
            SDL_GetRGB(source[0], surface->format, &R0, &G0, &B0);
            SDL_GetRGB(source[1], surface->format, &R1, &G1, &B1);
            dest[0] = (0.257f * R0) + (0.504f * G0) + (0.098f * B0) + 16;    // Y0
            dest[1] = -(0.148f * R1) - (0.291f * G1) + (0.439f * B1) + 128;  // U
            dest[2] = (0.257f * R1) + (0.504f * G1) + (0.098f * B1) + 16;    // Y1
            dest[3] = (0.439f * R1) - (0.368f * G1) - (0.071f * B1) + 128;   // V
            dest += 4;
          }
      } break;

      case NV097_SET_TEXTURE_FORMAT_COLOR_LC_IMAGE_YB8CR8YA8CB8:  // UYVY
      {
        uint32_t *source = pixels;
        for (int y = 0; y < surface->h; ++y)
          for (int x = 0; x < surface->w; x += 2, source += 2) {
            uint8_t R0, G0, B0, R1, G1, B1;
            SDL_GetRGB(source[0], surface->format, &R0, &G0, &B0);
            SDL_GetRGB(source[1], surface->format, &R1, &G1, &B1);
            dest[0] = -(0.148f * R1) - (0.291f * G1) + (0.439f * B1) + 128;  // U
            dest[1] = (0.257f * R0) + (0.504f * G0) + (0.098f * B0) + 16;    // Y0
            dest[2] = (0.439f * R1) - (0.368f * G1) - (0.071f * B1) + 128;   // V
            dest[3] = (0.257f * R1) + (0.504f * G1) + (0.098f * B1) + 16;    // Y1
            dest += 4;
          }
      } break;

      case NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_G8B8:
        // TODO: for now, just let default gradient happen
        break;

      default:
        return 3;
        break;
    }

    // TODO: swizzling

    return 0;
  }

  // standard SDL conversion to destination format
  SDL_Surface *new_surf = SDL_ConvertSurfaceFormat(const_cast<SDL_Surface *>(surface), format_.sdl_format, 0);
  if (!new_surf) {
    return 4;
  }

  SetRawTexture((const uint8_t *)new_surf->pixels, new_surf->w, new_surf->h, 1, new_surf->pitch,
                new_surf->format->BytesPerPixel, format_.xbox_swizzled, memory_base);

  SDL_FreeSurface(new_surf);
  return 0;
}

int TextureStage::SetVolumetricTexture(const SDL_Surface **layers, uint32_t depth, uint8_t *memory_base) const {
  ASSERT(format_.xbox_swizzled && "Volumetric textures using linear formats are not supported by XBOX.")

  auto **new_surfaces = new SDL_Surface *[depth];

  for (auto i = 0; i < depth; ++i) {
    auto *surface = const_cast<SDL_Surface *>(layers[i]);
    new_surfaces[i] = SDL_ConvertSurfaceFormat(surface, format_.sdl_format, 0);

    if (!new_surfaces[i]) {
      ASSERT(!"Failed to convert surface format.");
      // This will leak memory, but the ASSERT will halt execution anyway.
      return 4;
    }
  }

  const int32_t width = new_surfaces[0]->w;
  int32_t height = new_surfaces[0]->h;
  int32_t pitch = new_surfaces[0]->pitch;
  int32_t bytes_per_pixel = new_surfaces[0]->format->BytesPerPixel;

  uint32_t converted_surface_size = pitch * height;
  auto *flattened = new uint8_t[converted_surface_size * depth];
  uint8_t *dest = flattened;
  for (auto i = 0; i < depth; ++i) {
    SDL_Surface *s = new_surfaces[i];
    ASSERT(s->w == width && "Volumetric surface layers must have identical dimensions");
    ASSERT(s->h == height && "Volumetric surface layers must have identical dimensions");
    ASSERT(s->pitch == pitch && "Volumetric surface layers must have identical dimensions");
    ASSERT(s->format->BytesPerPixel == bytes_per_pixel && "Volumetric surface layers must have identical dimensions");

    memcpy(dest, s->pixels, converted_surface_size);
    dest += converted_surface_size;
  }

  for (auto i = 0; i < depth; ++i) {
    SDL_FreeSurface(new_surfaces[i]);
  }
  delete[] new_surfaces;

  int ret = SetRawTexture(flattened, width, height, depth, pitch, bytes_per_pixel, format_.xbox_swizzled, memory_base);

  delete[] flattened;

  return ret;
}

int TextureStage::SetRawTexture(const uint8_t *source, uint32_t width, uint32_t height, uint32_t depth, uint32_t pitch,
                                uint32_t bytes_per_pixel, bool swizzle, uint8_t *memory_base) const {
  uint8_t *dest = memory_base + texture_memory_offset_;

  if (swizzle) {
    if (depth > 1) {
      swizzle_box(source, width, height, depth, dest, pitch, pitch * height, bytes_per_pixel);
    } else {
      swizzle_rect(source, width, height, dest, pitch, bytes_per_pixel);
    }
  } else {
    memcpy(dest, source, pitch * height * depth);
  }

  return 0;
}

int TextureStage::SetPalette(const uint32_t *palette, uint32_t length, uint8_t *memory_base) {
  switch (length) {
    case 256:
      palette_length_ = 0;
      break;
    case 128:
      palette_length_ = 1;
      break;
    case 64:
      palette_length_ = 2;
      break;
    case 32:
      palette_length_ = 3;
      break;
    default:
      ASSERT(!"Invalid palette length. Must be 32|64|128|256");
      return 1;
  }

  uint8_t *dest = memory_base + palette_memory_offset_;
  memcpy(dest, palette, length * 4);
  return 0;
}
