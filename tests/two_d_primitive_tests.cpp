#include "two_d_primitive_tests.h"

#include <SDL_image.h>
#include <pbkit/pbkit.h>

#include "../nxdk_ext.h"
#include "../pbkit_ext.h"
#include "../test_host.h"
#include "debug_output.h"
#include "vertex_buffer.h"

// From pbkit.c
#define MAXRAM 0x03FFAFFF

// See pb_init in pbkit.c, where the channel contexts are set up.
// SUBCH_3 == GR_CLASS_9F, which contains the IMAGE_BLIT commands.
//#define SUBCH_CLASS_9F SUBCH_3
// SUBCH_4 == GR_CLASS_62, which contains the NV10 2D surface commands.
#define SUBCH_CLASS_62 SUBCH_4

static constexpr uint32_t SUBCH_CLASS_5C = kNextSubchannel;

static constexpr uint32_t kStartX = 25;
static constexpr uint32_t kStartY = 50;
static constexpr uint32_t kEndX = 100;
static constexpr uint32_t kEndY = 100;


static std::string OperationName(uint32_t operation);
static std::string ColorFormatName(uint32_t format);

static constexpr TwoDPrimitiveTests::BlitTest kTests[] = {
    {NV09F_SET_OPERATION_BLEND_AND, NV04_SURFACE_2D_FORMAT_X8R8G8B8_X8R8G8B8, 0x80000000},

};

TwoDPrimitiveTests::TwoDPrimitiveTests(TestHost& host, std::string output_dir) : TestSuite(host, std::move(output_dir)) {
  for (auto test : kTests) {
    std::string name = MakeTestName(test);

    auto test_method = [this, test]() { this->Test(test); };
    tests_[name] = test_method;
  }
}

void TwoDPrimitiveTests::Initialize() {
  SetDefaultTextureFormat();

  auto channel = kNextContextChannel;

  pb_create_gr_ctx(channel, 0x5C, &solid_lin_ctx_);
  pb_bind_channel(&solid_lin_ctx_);
  pb_bind_subchannel(SUBCH_CLASS_5C, &solid_lin_ctx_);

  // set up to not have shader
  // vertex shader (transform shader)..
  // fragment or pixel shader (color from triangle)..
  host_.SetShaderProgram(nullptr);

}

//void TwoDPrimitiveTests::ImageBlit(uint32_t operation, uint32_t beta, uint32_t source_channel, uint32_t destination_channel,
//                               uint32_t surface_format, uint32_t source_pitch, uint32_t destination_pitch,
//                               uint32_t source_offset, uint32_t source_x, uint32_t source_y,
//                               uint32_t destination_offset, uint32_t destination_x, uint32_t destination_y,
//                               uint32_t width, uint32_t height, uint32_t clip_x, uint32_t clip_y, uint32_t clip_width,
//                               uint32_t clip_height) const {
//  PrintMsg("ImageBlit: %d beta: 0x%X src: %d dest: %d\n", operation, beta, source_channel, destination_channel);
//
//  pb_target_back_buffer();
//
//  /* Clear depth & stencil buffers */
//  pb_erase_depth_stencil_buffer(0, 0, width, height);
//  pb_fill(0, 0, width, height, 0x00000000);
//  pb_erase_text_screen();
//
//  uint32_t *p = pb_begin();
//  p = pb_push1_to(SUBCH_CLASS_62, p, NV097_SET_BEGIN_END, NV097_SET_BEGIN_END_OP_LINES);
//
//  p = pb_push1_to(SUBCH_CLASS_5C, p, NV10_CONTEXT_SURFACES_2D_SET_DMA_IN_MEMORY0, source_channel);
//  p = pb_push1_to(SUBCH_CLASS_5C, p, NV10_CONTEXT_SURFACES_2D_SET_DMA_IN_MEMORY1, destination_channel);
//  p= pb_push1_to (SUBCH_CLASS_5C, p,  NV04_SOLID_LINE_OPERATION, 0x03);
//  p= pb_push1_to (SUBCH_CLASS_5C, p,  NV04_SOLID_LINE_COLOR_VALUE, 0xFFFFFF);
//
//  p= pb_push1_to (SUBCH_CLASS_5C, p,   NV04_SOLID_LINE_START, (source_y << 16) | source_x);
//  p= pb_push1_to (SUBCH_CLASS_5C, p,   NV04_SOLID_LINE_END, (source_y+30 <<16) | source_x + 30);
//
////
////  p = pb_push1_to(SUBCH_CLASS_19, p, NV01_CONTEXT_CLIP_RECTANGLE_SET_POINT, (clip_y << 16) | clip_x);
////  p = pb_push1_to(SUBCH_CLASS_19, p, NV01_CONTEXT_CLIP_RECTANGLE_SET_SIZE, (clip_height << 16) | clip_width);
//  //p = pb_push1_to(SUBCH_CLASS_9F, p, NV_IMAGE_BLIT_CLIP_RECTANGLE, clip_rect_ctx_.ChannelID);
//
////  p = pb_push1_to(SUBCH_CLASS_9F, p, 0x0120, 0);  // Sync read
////  p = pb_push1_to(SUBCH_CLASS_9F, p, 0x0124, 1);  // Sync write
////  p = pb_push1_to(SUBCH_CLASS_9F, p, 0x0128, 2);  // Modulo
////
//  p = pb_push1_to(SUBCH_CLASS_62, p, NV10_CONTEXT_SURFACES_2D_FORMAT, surface_format);
//
//  p = pb_push1_to(SUBCH_CLASS_62, p, NV10_CONTEXT_SURFACES_2D_PITCH, source_pitch | (destination_pitch << 16));
//  p = pb_push1_to(SUBCH_CLASS_62, p, NV10_CONTEXT_SURFACES_2D_OFFSET_SRC, source_offset);
//  p = pb_push1_to(SUBCH_CLASS_62, p, NV10_CONTEXT_SURFACES_2D_OFFSET_DST, destination_offset);
//
//  p = pb_push1(p, NV097_SET_BEGIN_END, NV097_SET_BEGIN_END_OP_END);
//
//  pb_end(p);
//
//}

void TwoDPrimitiveTests::Test(const BlitTest& test) {
  host_.PrepareDraw(0xFF440011);


//  uint32_t clip_w = host_.GetFramebufferWidth();
//  uint32_t clip_h = host_.GetFramebufferHeight();
//
//  ImageBlit(test.blit_operation, test.beta, image_src_dma_ctx_.ChannelID,
//            11,  // DMA channel 11 - 0x1117
//            test.buffer_color_format, image_pitch_, 4 * host_.GetFramebufferWidth(), 0, START_X, START_Y, 0,
//            DESTINATION_X, DESTINATION_Y, SOURCE_WIDTH, SOURCE_HEIGHT, clip_x, clip_y, clip_w, clip_h);

  auto p = pb_begin();
  p = pb_push1_to (SUBCH_CLASS_5C, p,  NV04_SOLID_LINE_OPERATION, 0x03);
  p = pb_push1_to (SUBCH_CLASS_5C, p,  NV04_SOLID_LINE_COLOR_VALUE, 0xFFFFFF);
  p = pb_push1_to (SUBCH_CLASS_5C, p,  NV04_SOLID_LINE_SURFACE, 11);
  p = pb_push1_to (SUBCH_CLASS_5C, p,  NV04_SOLID_LINE_COLOR_FORMAT, 0x3);

  p = pb_push1_to (SUBCH_CLASS_5C, p,   NV04_SOLID_LINE_START, (kStartY << 16) | kStartX);
  p = pb_push1_to (SUBCH_CLASS_5C, p,   NV04_SOLID_LINE_END, (kEndY <<16) | kEndX);

  pb_end(p);
  std::string op_name = OperationName(test.blit_operation);
  pb_print("Op: %s\n", op_name.c_str());
  std::string color_format_name = ColorFormatName(test.buffer_color_format);
  pb_print("BufFmt: %s\n", color_format_name.c_str());

  pb_draw_text_screen();
//  std::string name = MakeTestName(test);

  host_.FinishDraw();
  //host_.FinishDrawAndSave(output_dir_.c_str(), name.c_str());
}

std::string TwoDPrimitiveTests::MakeTestName(const BlitTest& test) {
  char buf[256] = {0};
  snprintf(buf, 255, "ImageBlt_%s_%s_B%08X", OperationName(test.blit_operation).c_str(),
           ColorFormatName(test.buffer_color_format).c_str(), test.beta);
  return buf;
}

static std::string OperationName(uint32_t operation) {
  if (operation == NV09F_SET_OPERATION_BLEND_AND) {
    return "BLENDAND";
  }
  if (operation == NV09F_SET_OPERATION_SRCCOPY) {
    return "SRCCOPY";
  }

  char buf[16] = {0};
  snprintf(buf, 15, "%X", operation);
  return buf;
}

static std::string ColorFormatName(uint32_t format) {
  switch (format) {
    case NV04_SURFACE_2D_FORMAT_X8R8G8B8_X8R8G8B8:
      return "XRGB";
    case NV04_SURFACE_2D_FORMAT_A8R8G8B8:
      return "ARGB";
    default:
      break;
  }

  char buf[16] = {0};
  snprintf(buf, 15, "%X", format);
  return buf;
}
