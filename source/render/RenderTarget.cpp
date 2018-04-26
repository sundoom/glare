/*
 *  RenderTarget.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "RenderTarget.h"
#include "BaseTexture.h"
#include "State.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	RenderTarget::RenderTarget() 
	{
		mFBO = 0;
		mRBO = 0;
		mColorPtr = NULL;
    mDepthPtr = NULL;
	}

	RenderTarget::~RenderTarget() 
	{
		if(mFBO) { glDeleteFramebuffers(1, &mFBO); mFBO = 0; } 
	  if(mRBO) { glDeleteRenderbuffers(1, &mRBO); mRBO = 0; }
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Build
	====================
	*/
	VOID RenderTarget::Build(const BaseTexture* color, const BaseTexture* depth)
	{
    // create the fbo
    if(mFBO == 0) { glGenFramebuffers(1, &mFBO); CHECK(mFBO); }
    BindFramebuffer(mFBO);

    // attach the color texture
    if(color)
    {
      CHECK(color->mTarget == GL_TEXTURE_2D);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->mObject, 0);
      mColorPtr = color;
    }

    // attach the depth texture
    if(depth)
    {
      CHECK(depth->mTarget == GL_TEXTURE_2D);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->mObject, 0);
      mDepthPtr = depth;
    }
    else if(color)
    {
      // attach the depth render buffer
      if(mRBO == 0) { glGenRenderbuffers(1, &mRBO); CHECK(mRBO); }
      BindRenderbuffer(mRBO);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, color->Width(), color->Height());
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBO);
    }

    // check the frame buffer status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
      // GAssert("GL_FRAMEBUFFER_COMPLETE");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      GAssert("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      GAssert("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
      GAssert("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      GAssert("GL_FRAMEBUFFER_UNSUPPORTED");
      break;
    default:
      GAssert("Unknown FBO error");
      break;
    }
  }

  /*
	====================
	Bind
	====================
	*/
  VOID RenderTarget::Bind()
  {
    BindFramebuffer(mFBO);
  }

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
