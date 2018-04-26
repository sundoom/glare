/*
 *  Shader.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Shader__
#define __Shader__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Render.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented Shader interface.
	class Shader
	{		
	public:		
		// constructor
		Shader();
		
		// destructor
		virtual ~Shader();

		// load the data
		VOID Load(const VOID* data, U32 size);
	
		// bind the shader
		VOID Bind(const CHAR* name);

    // set the parameter value of the shader
    VOID SetParameter1f(const CHAR* pname, F32 v);
    VOID SetParameter2f(const CHAR* pname, F32 v0, F32 v1);
    VOID SetParameter3f(const CHAR* pname, F32 v0, F32 v1, F32 v2);
    VOID SetParameter4f(const CHAR* pname, F32 v0, F32 v1, F32 v2, F32 v3);
    VOID SetParameter1i(const CHAR* pname, I32 v);
    VOID SetParameter2i(const CHAR* pname, I32 v0, I32 v1);
    VOID SetParameter3i(const CHAR* pname, I32 v0, I32 v1, I32 v2);
    VOID SetParameter4i(const CHAR* pname, I32 v0, I32 v1, I32 v2, I32 v3);
    VOID SetParameter1fv(const CHAR* pname, U32 count, const F32 * v);
    VOID SetParameter2fv(const CHAR* pname, U32 count, const F32 * v);
    VOID SetParameter3fv(const CHAR* pname, U32 count, const F32 * v);
    VOID SetParameter4fv(const CHAR* pname, U32 count, const F32 * v);
    VOID SetParameter1iv(const CHAR* pname, U32 count, const I32 * v);
    VOID SetParameter2iv(const CHAR* pname, U32 count, const I32 * v);
    VOID SetParameter3iv(const CHAR* pname, U32 count, const I32 * v);
    VOID SetParameter4iv(const CHAR* pname, U32 count, const I32 * v);
    VOID SetParameterMatrix2fv(const CHAR* pname, U32 count, const F32 * v);
    VOID SetParameterMatrix3fv(const CHAR* pname, U32 count, const F32 * v);
    VOID SetParameterMatrix4fv(const CHAR* pname, U32 count, const F32 * v);
    VOID SetParameterSampler(const CHAR* pname, const class BaseTexture* texture);

	private:
		#pragma pack(1)
		struct SAMPLER
		{
			class BaseTexture* texture;
			U32 wrap_s, wrap_t, wrap_r;
			U32 min_filter, mag_filter;
		};
		struct UNIFORM
		{			
			U32	type;
			U32	count;
			std::vector<U8>data;
			SAMPLER sampler;
		};
		struct PROGRAM
		{
			Str name;
			U32	object;
			std::list< std::pair<I32, UNIFORM*> >uniforms;
		};
		struct PASS
		{
			PROGRAM* program;
			U32 blend_src, blend_dst;
			U32	depth_func;
			BOOL depth_mask;
			U32	cull_mode;
		};
		#pragma pack()

	private:
		std::list<PROGRAM>mPrograms;
		std::map<Str, UNIFORM>mUniforms;
		std::map<Str, PASS>mPasses;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Shader__

//////////////////////////////////////////////////////////////////////////