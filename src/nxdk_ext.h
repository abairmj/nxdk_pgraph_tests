#ifndef NXDK_ZBUFFER_TESTS_NXDK_MISSING_DEFINES_H
#define NXDK_ZBUFFER_TESTS_NXDK_MISSING_DEFINES_H

// TODO: upstream missing nv2a defines
// See
// https://github.com/xqemu/xqemu/blob/f9b9a9bad88b3b2df53a1b01db6a37783aa2eb27/hw/xbox/nv2a/nv2a_shaders.c#L296-L312
#define NV2A_VERTEX_ATTR_POSITION 0
#define NV2A_VERTEX_ATTR_WEIGHT 1
#define NV2A_VERTEX_ATTR_NORMAL 2
#define NV2A_VERTEX_ATTR_DIFFUSE 3
#define NV2A_VERTEX_ATTR_SPECULAR 4
#define NV2A_VERTEX_ATTR_FOG_COORD 5
#define NV2A_VERTEX_ATTR_POINT_SIZE 6
#define NV2A_VERTEX_ATTR_BACK_DIFFUSE 7
#define NV2A_VERTEX_ATTR_BACK_SPECULAR 8
#define NV2A_VERTEX_ATTR_TEXTURE0 9
#define NV2A_VERTEX_ATTR_TEXTURE1 10
#define NV2A_VERTEX_ATTR_TEXTURE2 11
#define NV2A_VERTEX_ATTR_TEXTURE3 12

#define NV097_SET_TEXTURE_FORMAT_COLOR_LU_IMAGE_G8B8 0x17
#define NV097_SET_TEXTURE_FORMAT_COLOR_SZ_B8G8R8A8 0x3B
#define NV097_SET_TEXTURE_FORMAT_COLOR_LC_IMAGE_CR8YB8CB8YA8 0x24
#define NV097_SET_TEXTURE_FORMAT_COLOR_LC_IMAGE_YB8CR8YA8CB8 0x25
#define NV097_SET_TEXTURE_FORMAT_COLOR_D16 0x2C      // TODO: proper nvidia name
#define NV097_SET_TEXTURE_FORMAT_COLOR_LIN_F16 0x31  // TODO: proper nvidia name
#define NV097_SET_CONTROL0_COLOR_SPACE_CONVERT 0xF0000000
#define NV097_SET_CONTROL0_COLOR_SPACE_CONVERT_CRYCB_TO_RGB 0x1

#define NV097_SET_TEXTURE_CONTROL0_ENABLE (1 << 30)
#define NV097_SET_TEXTURE_CONTROL0_MIN_LOD_CLAMP 0x3FFC0000
#define NV097_SET_TEXTURE_CONTROL0_MAX_LOD_CLAMP 0x0003FFC0

#define NV097_SET_CONTROL0_Z_FORMAT_FIXED 0
#define NV097_SET_CONTROL0_Z_FORMAT_FLOAT 1

#define NV097_SET_DEPTH_FUNC_V_NEVER 0x00000200
#define NV097_SET_DEPTH_FUNC_V_LESS 0x00000201
#define NV097_SET_DEPTH_FUNC_V_EQUAL 0x00000202
#define NV097_SET_DEPTH_FUNC_V_LEQUAL 0x00000203
#define NV097_SET_DEPTH_FUNC_V_GREATER 0x00000204
#define NV097_SET_DEPTH_FUNC_V_NOTEQUAL 0x00000205
#define NV097_SET_DEPTH_FUNC_V_GEQUAL 0x00000206
#define NV097_SET_DEPTH_FUNC_V_ALWAYS 0x00000207

#define NV097_SET_ZMIN_MAX_CONTROL_CULL_NEAR_FAR_EN_FALSE 0
#define NV097_SET_ZMIN_MAX_CONTROL_CULL_NEAR_FAR_EN_TRUE 1
#define NV097_SET_ZMIN_MAX_CONTROL_ZCLAMP_EN_CULL 0
#define NV097_SET_ZMIN_MAX_CONTROL_ZCLAMP_EN_CLAMP (1 << 4)
#define NV097_SET_ZMIN_MAX_CONTROL_CULL_IGNORE_W_FALSE 0
#define NV097_SET_ZMIN_MAX_CONTROL_CULL_IGNORE_W_TRUE (1 << 8)

// Naming from xemu's nv2a_regs.h, to be used with nxdk's NV_IMAGE_BLIT definitions.
#define NV09F_SET_OPERATION_BLEND_AND 0x02
#define NV09F_SET_OPERATION_SRCCOPY 0x03
#define NV09F_SET_OPERATION_SRCCOPY_PREMULT 0x04
#define NV09F_SET_OPERATION_BLEND_AND_PREMULT 0x05

#define NV_IMAGE_BLIT_SET_BETA 0x0194
#define NV_IMAGE_BLIT_SET_BETA4 0x0198

#define NV012_BETA 0x00000012  // Alpha factor
#define NV012_SET_BETA 0x0300

#define NV072_BETA_4 0x00000072  // RGBA factor
#define NV072_SET_BETA 0x0300

#define NV04_SURFACE_2D_FORMAT_Y8 0x00000001
#define NV04_SURFACE_2D_FORMAT_X1R5G5B5_Z1R5G5B5 0x00000002
#define NV04_SURFACE_2D_FORMAT_X1R5G5B5_X1R5G5B5 0x00000003
#define NV04_SURFACE_2D_FORMAT_R5G6B5 0x00000004
#define NV04_SURFACE_2D_FORMAT_Y16 0x00000005
#define NV04_SURFACE_2D_FORMAT_X8R8G8B8_Z8R8G8B8 0x00000006
#define NV04_SURFACE_2D_FORMAT_X8R8G8B8_X8R8G8B8 0x00000007
#define NV04_SURFACE_2D_FORMAT_X1A7R8G8B8_Z1A7R8G8B8 0x00000008
#define NV04_SURFACE_2D_FORMAT_X1A7R8G8B8_X1A7R8G8B8 0x00000009
#define NV04_SURFACE_2D_FORMAT_A8R8G8B8 0x0000000a
#define NV04_SURFACE_2D_FORMAT_Y32 0x0000000b

#define NV097_SET_COLOR_MATERIAL 0x298
#define NV097_SET_COLOR_MATERIAL_ALL_FROM_MATERIAL 0
#define NV097_SET_COLOR_MATERIAL_EMISSIVE_FROM_MATERIAL 0
#define NV097_SET_COLOR_MATERIAL_EMISSIVE_FROM_VERTEX_DIFFUSE (1 << 0)
#define NV097_SET_COLOR_MATERIAL_EMISSIVE_FROM_VERTEX_SPECULAR (2 << 0)
#define NV097_SET_COLOR_MATERIAL_AMBIENT_FROM_MATERIAL 0
#define NV097_SET_COLOR_MATERIAL_AMBIENT_FROM_VERTEX_DIFFUSE (1 << 2)
#define NV097_SET_COLOR_MATERIAL_AMBIENT_FROM_VERTEX_SPECULAR (2 << 2)
#define NV097_SET_COLOR_MATERIAL_DIFFUSE_FROM_MATERIAL 0
#define NV097_SET_COLOR_MATERIAL_DIFFUSE_FROM_VERTEX_DIFFUSE (1 << 4)
#define NV097_SET_COLOR_MATERIAL_DIFFUSE_FROM_VERTEX_SPECULAR (2 << 4)
#define NV097_SET_COLOR_MATERIAL_SPECULAR_FROM_MATERIAL 0
#define NV097_SET_COLOR_MATERIAL_SPECULAR_FROM_VERTEX_DIFFUSE (1 << 6)
#define NV097_SET_COLOR_MATERIAL_SPECULAR_FROM_VERTEX_SPECULAR (2 << 6)
#define NV097_SET_COLOR_MATERIAL_BACK_EMISSIVE_FROM_MATERIAL 0
#define NV097_SET_COLOR_MATERIAL_BACK_EMISSIVE_FROM_VERTEX_DIFFUSE (1 << 8)
#define NV097_SET_COLOR_MATERIAL_BACK_EMISSIVE_FROM_VERTEX_SPECULAR (2 << 8)
#define NV097_SET_COLOR_MATERIAL_BACK_AMBIENT_FROM_MATERIAL 0
#define NV097_SET_COLOR_MATERIAL_BACK_AMBIENT_FROM_VERTEX_DIFFUSE (1 << 10)
#define NV097_SET_COLOR_MATERIAL_BACK_AMBIENT_FROM_VERTEX_SPECULAR (2 << 10)
#define NV097_SET_COLOR_MATERIAL_BACK_DIFFUSE_FROM_MATERIAL 0
#define NV097_SET_COLOR_MATERIAL_BACK_DIFFUSE_FROM_VERTEX_DIFFUSE (1 << 12)
#define NV097_SET_COLOR_MATERIAL_BACK_DIFFUSE_FROM_VERTEX_SPECULAR (2 << 12)
#define NV097_SET_COLOR_MATERIAL_BACK_SPECULAR_FROM_MATERIAL 0
#define NV097_SET_COLOR_MATERIAL_BACK_SPECULAR_FROM_VERTEX_DIFFUSE (1 << 14)
#define NV097_SET_COLOR_MATERIAL_BACK_SPECULAR_FROM_VERTEX_SPECULAR (2 << 14)

#define NV097_SET_LIGHT_CONTROL NV20_TCL_PRIMITIVE_3D_LIGHT_CONTROL
#define NV097_SET_MATERIAL_ALPHA 0x3B4
#define NV097_SET_SPECULAR_ENABLE 0x03b8
#define NV097_SET_MATERIAL_EMISSION 0x03a8
#define NV097_SET_SPECULAR_PARAMS 0x9E0

#define NV042_SET_PITCH 0x304
#define NV042_SET_COLOR_FORMAT NV04_CONTEXT_SURFACES_2D_FORMAT
#define NV042_SET_COLOR_FORMAT_LE_A8R8G8B8 0x0A

#define NV05C_SET_COLOR_FORMAT_LE_X16R5G6B5 0x00000001
#define NV05C_SET_COLOR_FORMAT_LE_X17R5G5B5 0x00000002
#define NV05C_SET_COLOR_FORMAT_LE_X8R8G8B8 0x00000003

// This is sent whenever a vertex buffer is drawn after being locked.
// Failing to send it in a tight modification loop will cause the hardware to re-use previously set data even if new
// values are set (likely it skips fetching the updated memory from system RAM).
#define NV097_BREAK_VERTEX_BUFFER_CACHE 0x1710

#define NV097_SET_WEIGHT1F 0x169C
#define NV097_SET_WEIGHT4F 0x16C0
#define NV097_SET_NORMAL3F 0x1530
#define NV097_SET_NORMAL3S 0x1540
#define NV097_SET_DIFFUSE_COLOR4F 0x1550
#define NV097_SET_DIFFUSE_COLOR 0x156C
#define NV097_SET_SPECULAR_COLOR4F 0x1570
#define NV097_SET_SPECULAR_COLOR 0x158C
#define NV097_SET_FOG_COORD 0x1698
#define NV097_SET_POINT_SIZE 0x43C
#define NV097_SET_TEXCOORD0_2F 0x1590
#define NV097_SET_TEXCOORD0_4F 0x15A0
#define NV097_SET_TEXCOORD1_2F 0x15B8
#define NV097_SET_TEXCOORD1_4F 0x15C8
#define NV097_SET_TEXCOORD2_2F 0x15E0
#define NV097_SET_TEXCOORD2_4F 0x15F0
#define NV097_SET_TEXCOORD3_2F 0x1608
#define NV097_SET_TEXCOORD3_4F 0x1620

#define NV097_SET_LIGHT_ENABLE_MASK_LIGHT1_OFF 0
#define NV097_SET_LIGHT_ENABLE_MASK_LIGHT1_INFINITE (1 << 2)
#define NV097_SET_LIGHT_ENABLE_MASK_LIGHT1_LOCAL (2 << 2)
#define NV097_SET_LIGHT_ENABLE_MASK_LIGHT1_SPOT (3 << 2)

#define NV097_SET_POINT_PARAMS_ENABLE 0x318
#define NV097_SET_POINT_SMOOTH_ENABLE 0x31C

#endif  // NXDK_ZBUFFER_TESTS_NXDK_MISSING_DEFINES_H
