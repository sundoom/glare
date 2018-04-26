/*
 *  State.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __State__
#define __State__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

  // reset the state
  VOID Reset(U32 fbo);

  //////////////////////////////////////////////////////////////////////////

  // bind the frame buffer
  VOID BindFramebuffer(U32 fbo);

  // bind the render buffer
  VOID BindRenderbuffer(U32 rbo);

  // bind the shader program
  VOID BindProgram(U32 program);

  // bind the texture object
  VOID BindTexture(U32 target, U32 object, U32 unit = 0);

  // bind the vertex attrib array
  VOID BindVertexAttribArray(U32 count);

  // bind the vertex buffer
  VOID BindVertexBuffer(U32 object);

  // bind the index buffer
  VOID BindIndexBuffer(U32 object);

  // bind the blend func
  VOID BindBlendFunc(U32 src, U32 dst);

  // bind the depth func
  VOID BindDepthFunc(U32 func);

  // bind the depth mask
  VOID BindDepthMask(BOOL mask);

  // bind the cull func
  VOID BindCullFunc(U32 mode);

  // bind the cull func
  VOID BindScissorTest(BOOL enable);

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __State__

//////////////////////////////////////////////////////////////////////////