/*
 *  Render.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Render__
#define __Render__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>
#include <GMath.h>
NAMESPACE_USE(Math)

//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	#include <GLES2/gl2.h>
	#pragma comment(lib,"libGLESv2.lib")
#elif __APPLE__
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#else
#error Unknown platform!
#endif

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////
// Render Interface
//////////////////////////////////////////////////////////////////////////
  
  enum VERTEX_TYPE 
  { 
    VT_1I, 
    VT_2I, 
    VT_3I, 
    VT_4I, 
    VT_1F, 
    VT_2F, 
    VT_3F, 
    VT_4F 
  };

  HANDLE RICreateVertexBuffer(U32 count, U32 stride, U8 attrib[16], const VOID* data);
  VOID RIUpdateVertexBuffer(HANDLE handle, U32 count, U32 stride, U8 attrib[16], const VOID* data);
  VOID RIDestroyVertexBuffer(HANDLE handle);
  VOID RIBindVertexBuffer(HANDLE handle);

  //////////////////////////////////////////////////////////////////////////

  HANDLE RICreateIndexBuffer(U32 count, U32 stride, const VOID* data);
  VOID RIUpdateIndexBuffer(HANDLE handle, U32 count, U32 stride, const VOID* data);  
  VOID RIDestroyIndexBuffer(HANDLE handle);
  VOID RIBindIndexBuffer(HANDLE handle);

  //////////////////////////////////////////////////////////////////////////

  enum PIXEL_FORMAT
  {
    PF_ALPHA	    = 0x1906,
    PF_RGB		    = 0x1907,
    PF_RGBA		    = 0x1908,
    PF_L			    = 0x1909,
    PF_LA			    = 0x190A,
    PF_RGB_DXT1   = 0x83F0,
    PF_RGBA_DXT1  = 0x83F1,
    PF_RGBA_DXT3  = 0x83F2,
    PF_RGBA_DXT5  = 0x83F3,
    PF_RGB_PVRTC_4BPPV1_IMG   = 0x8C00,
    PF_RGB_PVRTC_2BPPV1_IMG   = 0x8C01,
    PF_RGBA_PVRTC_4BPPV1_IMG  = 0x8C02,
    PF_RGBA_PVRTC_2BPPV1_IMG  = 0x8C03,
  };

  enum DATA_TYPE
  {
    DT_BYTE						        = 0x1400,
    DT_UNSIGNED_BYTE			    = 0x1401,
    DT_SHORT					        = 0x1402,
    DT_UNSIGNED_SHORT			    = 0x1403,
    DT_INT						        = 0x1404,
    DT_UNSIGNED_INT				    = 0x1405,
    DT_FLOAT					        = 0x1406,
    DT_UNSIGNED_SHORT_4_4_4_4	= 0x8033,
    DT_UNSIGNED_SHORT_5_5_5_1	= 0x8034,
    DT_UNSIGNED_SHORT_5_6_5		= 0x8363,
  };

  HANDLE RICreateTexture(U32 level, U32 width, U32 height, U32 bits, U32 format, U32 type, const VOID* pixels);
  VOID RIUpdateTexture(HANDLE handle, U32 level, I32 x, I32 y, U32 width, U32 height, U32 bits, U32 format, U32 type, const VOID* pixels);
	HANDLE RICreateCubeTexture(U32 level, U32 width, U32 height, U32 bits, U32 format, U32 type, const VOID* pixels);
	VOID RIUpdateCubeTexture(HANDLE handle, U32 level, I32 x, I32 y, U32 width, U32 height, U32 bits, U32 format, U32 type, const VOID* pixels);
  VOID RIDestroyTexture(HANDLE handle);
  VOID RIBindTexture(HANDLE handle, U32 unit);

  //////////////////////////////////////////////////////////////////////////

  HANDLE RICreateRenderTarget(HANDLE color, HANDLE depth);
  VOID RIDestroyRenderTarget(HANDLE handle);
  VOID RIBindRenderTarget(HANDLE handle);

  //////////////////////////////////////////////////////////////////////////

  HANDLE RICreateShader(VOID* data, U32 size);
  VOID RIDestroyShader(HANDLE handle);
  VOID RIBindShader(HANDLE handle, const CHAR* pass);
  VOID RISetShaderParameter1f(HANDLE handle, const CHAR* pname, F32 v);
  VOID RISetShaderParameter2f(HANDLE handle, const CHAR* pname, F32 v0, F32 v1);
  VOID RISetShaderParameter3f(HANDLE handle, const CHAR* pname, F32 v0, F32 v1, F32 v2);
  VOID RISetShaderParameter4f(HANDLE handle, const CHAR* pname, F32 v0, F32 v1, F32 v2, F32 v3);
  VOID RISetShaderParameter1i(HANDLE handle, const CHAR* pname, I32 v);
  VOID RISetShaderParameter2i(HANDLE handle, const CHAR* pname, I32 v0, I32 v1);
  VOID RISetShaderParameter3i(HANDLE handle, const CHAR* pname, I32 v0, I32 v1, I32 v2);
  VOID RISetShaderParameter4i(HANDLE handle, const CHAR* pname, I32 v0, I32 v1, I32 v2, I32 v3);
  VOID RISetShaderParameter1fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v);
  VOID RISetShaderParameter2fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v);
  VOID RISetShaderParameter3fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v);
  VOID RISetShaderParameter4fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v);
  VOID RISetShaderParameter1iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v);
  VOID RISetShaderParameter2iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v);
  VOID RISetShaderParameter3iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v);
  VOID RISetShaderParameter4iv(HANDLE handle, const CHAR* pname, U32 count, const I32 * v);
  VOID RISetShaderParameterMatrix2fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v);
  VOID RISetShaderParameterMatrix3fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v);
  VOID RISetShaderParameterMatrix4fv(HANDLE handle, const CHAR* pname, U32 count, const F32 * v);
  VOID RISetShaderParameterSampler(HANDLE handle, const CHAR* pname, HANDLE texture);

  //////////////////////////////////////////////////////////////////////////

  VOID RIReset(U32 fbo = 0); 

  //////////////////////////////////////////////////////////////////////////

  enum CLEAR_TYPE
  {
    CT_DEPTH = 0x00000100,
    CT_COLOR = 0x00004000,
  };

  VOID RIClear(U32 flags, F32 red, F32 green, F32 blue, F32 alpha, F32 depth);

  //////////////////////////////////////////////////////////////////////////

  VOID RISetViewport(I32 x, I32 y, I32 width, I32 height);
  VOID RISetScissor(BOOL enable, I32 x, I32 y, I32 width, I32 height);

  //////////////////////////////////////////////////////////////////////////

  enum PRIMITIVE_TYPE
  {
    PT_POINTS			      = 0x0000,
    PT_LINES			      = 0x0001,
    PT_LINE_LOOP		    = 0x0002,
    PT_LINE_STRIP       = 0x0003,
    PT_TRIANGLES        = 0x0004,
    PT_TRIANGLE_STRIP   = 0x0005,
    PT_TRIANGLE_FAN     = 0x0006,
  };
  VOID RIDrawPrimitive(U32 mode, I32 count, U32 type);

  //////////////////////////////////////////////////////////////////////////

  U32 RIExec();
  VOID RISwap();

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Render__

//////////////////////////////////////////////////////////////////////////