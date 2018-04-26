/*
 *  State.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "State.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////	

  U32 s_default_fbo = 0;
  static U32 s_fbo = 0;
  static U32 s_rbo = 0;
  static U32 s_program;
  #define MAX_TEXTURE_UNITS 4
  static U32 s_texture_unit = 0;
  static U32 s_texture_object[MAX_TEXTURE_UNITS];
  static U32 s_vertex_object = 0;		
  static U32 s_vertex_attrib_count = 0;
  static U32 s_index_object = 0;
  static U32 s_blend_src = GL_ONE;
  static U32 s_blend_dst = GL_ZERO;
  static U32 s_depth_func = GL_LEQUAL;
  static BOOL s_depth_mask = TRUE;
  static U32 s_cull_mode = GL_NONE;
  static BOOL s_scissor_test = FALSE;

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Reset
	====================
	*/
	VOID Reset(U32 fbo)
	{    
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glClearDepthf(1.0f);
		glDepthRangef(0.0f, 1.0f);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);

    s_default_fbo = fbo;
		s_fbo = 0;
		s_rbo = 0;
		s_program = 0;
		s_vertex_object = 0;
		s_vertex_attrib_count = 0;
		s_index_object = 0;
		s_texture_unit = 0;
		memset(s_texture_object, 0, sizeof(s_texture_object));
		s_depth_func = GL_LEQUAL;	
		s_depth_mask = TRUE;
		s_blend_src = GL_ONE;
		s_blend_dst = GL_ZERO;
		s_cull_mode = GL_NONE;
    s_scissor_test = FALSE;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	BindFramebuffer
	====================
	*/
	VOID BindFramebuffer(U32 fbo)
	{
		if(fbo != s_fbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			s_fbo = fbo;
		}
	}

	/*
	====================
	BindRenderbuffer
	====================
	*/
	VOID BindRenderbuffer(U32 rbo)
	{
		if(rbo != s_rbo)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			s_rbo = rbo;
		}
	}

	/*
	====================
	BindProgram
	====================
	*/
	VOID BindProgram(U32 program)
	{
		if(program != s_program)
		{
			glUseProgram(program);
			s_program = program;
		}
	}

	/*
	====================
	BindTexture
	====================
	*/	
	VOID BindTexture(U32 target, U32 object, U32 unit)
	{
		CHECK(unit < MAX_TEXTURE_UNITS);

		// active the texture unit
		if(unit != s_texture_unit)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			s_texture_unit = unit;
		}

		// bind textures to texture image units as needed
		if(object != s_texture_object[unit])
		{
			glBindTexture(target, object);
			s_texture_object[unit] = object;
		}
	}

  /*
	====================
	BindVertexAttribArray
	====================
	*/
  VOID BindVertexAttribArray(U32 count)
  {
    if(s_vertex_attrib_count < count) { for(U32 i = s_vertex_attrib_count; i < count; i++) glEnableVertexAttribArray(i); }
    if(s_vertex_attrib_count > count) { for(U32 i = count; i < s_vertex_attrib_count; i++) glDisableVertexAttribArray(i); }
    s_vertex_attrib_count = count;
  }

	/*
	====================
	BindVertexBuffer
	====================
	*/
	VOID BindVertexBuffer(U32 object)
	{
		if(object != s_vertex_object)
    {
      glBindBuffer(GL_ARRAY_BUFFER, object);
      s_vertex_object = object;
    }
	}

	/*
	====================
	BindIndexBuffer
	====================
	*/	
	VOID BindIndexBuffer(U32 object)
	{
		if(object != s_index_object)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object);
			s_index_object = object;
		}
	}

	/*
	====================
	BindBlendFunc
	====================
	*/	
	VOID BindBlendFunc(U32 src, U32 dst)
	{
		if(src == s_blend_src && dst == s_blend_dst) return;

		if(src == GL_ONE && dst == GL_ZERO)
		{
			glDisable(GL_BLEND);
		}
		else
		{
			if(s_blend_src == GL_ONE && s_blend_dst == GL_ZERO)
			{
				glEnable(GL_BLEND);
			}

			glBlendFunc(src, dst);
		}

		s_blend_src = src, s_blend_dst = dst;
	}
	
	/*
	====================
	BindDepthFunc
	====================
	*/	
	VOID BindDepthFunc(U32 func)
	{
		if(func == s_depth_func) return;

		if(func == GL_ALWAYS) 
		{
			glDisable(GL_DEPTH_TEST);
		} 
		else 
		{
			if(s_depth_func == GL_ALWAYS)
			{
				glEnable(GL_DEPTH_TEST);
			}

			glDepthFunc(func);
		}

		s_depth_func = func;
	}

	/*
	====================
	BindDepthMask
	====================
	*/	
	VOID BindDepthMask(BOOL mask)
	{
		if(mask != s_depth_mask)
		{
			glDepthMask(mask ? GL_TRUE : GL_FALSE);
			s_depth_mask = mask;
		}
	}

	/*
	====================
	BindCullFunc
	====================
	*/	
	VOID BindCullFunc(U32 mode)
	{
		if(mode == s_cull_mode) return;

		if(mode == GL_NONE)
		{
			glDisable(GL_CULL_FACE);
		} 
		else 
		{
			if(s_cull_mode == GL_NONE) 
			{
				glEnable(GL_CULL_FACE);
			}

			glCullFace(mode);
		}

		s_cull_mode = mode;
	}

  /*
	====================
	BindScissorTest
	====================
	*/	
	VOID BindScissorTest(BOOL enable)
	{
    if(s_scissor_test != enable)
    {
      if(enable)
      {
        glEnable(GL_SCISSOR_TEST);
      }
      else
      {
        glDisable(GL_SCISSOR_TEST);
      }
      s_scissor_test = enable;
    }
  }

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
