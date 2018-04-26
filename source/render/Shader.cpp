/*
 *  Shader.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "Shader.h"
#include "BaseTexture.h"
#include "State.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////	

	Shader::Shader()
	{
	
	}

	Shader::~Shader() 
	{
    for(std::list<PROGRAM>::iterator it = mPrograms.begin(); it != mPrograms.end(); ++it)
		{
      if((*it).object) {glDeleteProgram((*it).object);(*it).object = 0;}
		}
    mPrograms.clear();
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID Shader::Load(const VOID* data, U32 size)
	{
    // get the shader config
    GConfigPtr shader_config_ptr = GConfig::Load((const CHAR*)data);
    CHECK(shader_config_ptr);

    // check it if it is a shader?
    CHECK(Str(shader_config_ptr->GetValue()) == "shader");

    std::map<Str, SAMPLER>samplers;

    // prase the shader`s children
    const GConfig *shader_child_config_ptr = NULL;
    for(U32 i = 0; shader_child_config_ptr = shader_config_ptr->GetChild(i); i++)
    {
      // prase the sampler
      if(Str(shader_child_config_ptr->GetValue()) == "sampler")
      {
        const GConfig *sampler_config_ptr = (const GConfig *)shader_child_config_ptr;

        // get the sampler name
        Str name = sampler_config_ptr->GetAttribute("name"); CHECK(name!="");
        if(samplers.find(name)!=samplers.end())GAssert(VA("The sampler is already exists : %s.",name.c_str()));

        // the new sampler
        SAMPLER sampler;
        sampler.wrap_s = GL_REPEAT;
        sampler.wrap_t = GL_REPEAT;
        sampler.wrap_r = GL_REPEAT;
        sampler.min_filter = GL_REPEAT;
        sampler.mag_filter = GL_REPEAT;

        // get the sampler state
        const GConfig *sampler_child_config_ptr =  NULL;
        for(U32 j = 0; sampler_child_config_ptr = sampler_config_ptr->GetChild(j); j++)
        {
          if(Str(sampler_child_config_ptr->GetValue()) == "wrap_s")
          {
            // get the wrap: s
            Str s = sampler_child_config_ptr->GetText(); CHECK(s!="");
            if(s == "CLAMP_TO_EDGE") sampler.wrap_s = GL_CLAMP_TO_EDGE;
            else if(s == "REPEAT") sampler.wrap_s = GL_REPEAT;
            else if(s == "MIRRORED_REPEAT") sampler.wrap_s = GL_MIRRORED_REPEAT;
            else GAssert(VA("The texture`s wrap(s) value(%s) is unknown.\n", s.c_str()));
          }
          else if(Str(sampler_child_config_ptr->GetValue()) == "wrap_t")
          {
            // get the wrap: t
            Str t = sampler_child_config_ptr->GetText(); CHECK(t!="");
            if(t == "CLAMP_TO_EDGE") sampler.wrap_t = GL_CLAMP_TO_EDGE;
            else if(t == "REPEAT") sampler.wrap_t = GL_REPEAT;
            else if(t == "MIRRORED_REPEAT") sampler.wrap_t = GL_MIRRORED_REPEAT;
            else GAssert(VA("The texture`s wrap(t) value(%s) is unknown.\n", t.c_str()));
          }
          else if(Str(sampler_child_config_ptr->GetValue()) == "wrap_r")
          {
            // get the wrap: r
            Str r = sampler_child_config_ptr->GetText(); CHECK(r!="");
            if(r == "CLAMP_TO_EDGE") sampler.wrap_r = GL_CLAMP_TO_EDGE;
            else if(r == "REPEAT") sampler.wrap_r = GL_REPEAT;
            else if(r == "MIRRORED_REPEAT") sampler.wrap_r = GL_MIRRORED_REPEAT;
            else GAssert(VA("The texture`s wrap(r) value(%s) is unknown.\n", r.c_str()));
          }
          else if(Str(sampler_child_config_ptr->GetValue()) == "min_filter")
          {
            // get the min filter
            Str min = sampler_child_config_ptr->GetText(); CHECK(min!="");
            if(min == "NEAREST") sampler.min_filter = GL_NEAREST;
            else if(min == "LINEAR") sampler.min_filter = GL_LINEAR;
            else if(min == "NEAREST_MIPMAP_NEAREST") sampler.min_filter = GL_NEAREST_MIPMAP_NEAREST;
            else if(min == "NEAREST_MIPMAP_LINEAR") sampler.min_filter = GL_NEAREST_MIPMAP_LINEAR;
            else if(min == "LINEAR_MIPMAP_NEAREST") sampler.min_filter = GL_LINEAR_MIPMAP_NEAREST;
            else if(min == "LINEAR_MIPMAP_LINEAR") sampler.min_filter = GL_LINEAR_MIPMAP_LINEAR;
            else GAssert(VA("The texture`s min filter value(%s) is unknown.\n", min.c_str()));
          }
          else if(Str(sampler_child_config_ptr->GetValue()) == "mag_filter")
          {
            // get the mag filter
            Str mag = sampler_child_config_ptr->GetText(); CHECK(mag!="");
            if(mag == "NEAREST") sampler.mag_filter = GL_NEAREST;
            else if(mag == "LINEAR") sampler.mag_filter = GL_LINEAR;
            else GAssert(VA("The texture`s mag filter value(%s) is unknown.\n", mag.c_str()));
          }
          else
          {
            GAssert(VA("The shader`s sampler(%s)`s keyword(%s) is unknown!", name.c_str(), sampler_child_config_ptr->GetValue()));
          }
        }

        // add it to the table
        samplers.insert(std::make_pair(name, sampler));
      }
      // prase the program
      else if(Str(shader_child_config_ptr->GetValue()) == "program")
      {
        const GConfig*program_config_ptr = (const GConfig*)shader_child_config_ptr;

        // the new program
        PROGRAM program;

        // get the program name
        Str program_name = program_config_ptr->GetAttribute("name"); CHECK(program_name!="");
        for(std::list<PROGRAM>::iterator it = mPrograms.begin(); it != mPrograms.end(); ++it)
        { if(program_name == it->name) GAssert(VA("The program is already exists : %s.",program_name.c_str())); }
        program.name = program_name;

        // get the program attribs
        Str attribs = program_config_ptr->GetAttribute("attribs"); CHECK(attribs!= "");

        // prase the program`s children
        const CHAR *vs, *fs;
        const GConfig *program_child_config_ptr = NULL;
        for(U32 j = 0; program_child_config_ptr = program_config_ptr->GetChild(j); j++)
        {
          if(Str(program_child_config_ptr->GetValue()) == "vs")
          {
            // get the vs source
            vs = program_child_config_ptr->GetText();
          }
          else if(Str(program_child_config_ptr->GetValue()) == "fs")
          {
            // get the fs source
            fs = program_child_config_ptr->GetText();
          }
          else
          {
            GAssert(VA("The shader`s program(%s)`s keyword(%s) is unknown!", program_name.c_str(), program_child_config_ptr->GetValue()));
          }
        }

        GLint result = 0;

        // compile the program
        program.object = glCreateProgram(); CHECK(program.object);

        // compile the vertex shader
        if(vs && vs != "")
        {
          GLuint handle = glCreateShader(GL_VERTEX_SHADER); CHECK(handle);

          // load the vertex shader source
          glShaderSource(handle, 1, (const GLchar**)&vs, NULL);

          // compile the shader
          glCompileShader(handle);
          glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
          if(result == 0)
          {
            GLsizei size;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &size);
            if(size > 1)
            {
              Str log(size+1,0); GLsizei length;
              glGetShaderInfoLog(handle, size, &length, &log[0]);
              if(length > 0) GAssert(log.c_str());
            }
            GAssert("Fail to compile the vertex shader.\n");
          }

          // attach the shader to the gpu program
          glAttachShader(program.object, handle);
          glDeleteShader(handle);
        }

        // compile the fragment shader
        if(fs && fs != "")
        {
          GLuint handle = glCreateShader(GL_FRAGMENT_SHADER); CHECK(handle);

          // load the fragment shader source
          glShaderSource(handle, 1, (const GLchar**)&fs, NULL);

          // compile the shader
          glCompileShader(handle);
          glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
          if(result == 0) 
          {
            GLsizei size;
            glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &size);
            if(size > 1)
            {
              Str log(size+1,0); GLsizei length;
              glGetShaderInfoLog(handle, size, &length, &log[0]);
              if(length > 0) GAssert(log.c_str());
            }
            GAssert("Fail to compile the fragment shader.\n");
          }

          // attach the shader to the GPU Program
          glAttachShader(program.object, handle);
          glDeleteShader(handle);
        }

        // bind the attributes
        if(attribs.size())
        {
          std::vector<Str>attributes = GParse(attribs);
          for(U32 k = 0; k < attributes.size(); k++) glBindAttribLocation(program.object, k, attributes[k].c_str());
        }

        // link the shader to the gpu program
        glLinkProgram(program.object);
        glGetProgramiv(program.object, GL_LINK_STATUS, &result);
        if(result == 0) 
        {
          GLsizei size;
          glGetProgramiv(program.object, GL_INFO_LOG_LENGTH, &size);
          if(size > 1)
          {
            Str log(size+1,0); GLsizei length;
            glGetProgramInfoLog(program.object, size, &length, &log[0]);
            if(length > 0) GAssert(log.c_str());
          }
          GAssert("Fail to link the shader to the gpu program.\n");
        }

        // build the uniform map		
        GLint uniform_count, max_length;
        glGetProgramiv(program.object, GL_ACTIVE_UNIFORMS, &uniform_count);
        glGetProgramiv(program.object, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);
        CHAR* uniform_name = GNEW(CHAR[max_length+1]); CHECK(uniform_name);
        for( I32 k = 0; k < uniform_count; k++ )
        {
          // get the uniform info of the program
          GLenum type; GLint size;
          memset(uniform_name,0,(max_length+1)*sizeof(CHAR));
          glGetActiveUniform(program.object, k, max_length, 0, &size, &type, &uniform_name[0]);
          GLint location = glGetUniformLocation(program.object, &uniform_name[0]);
          if(location < 0) continue;

          // add the uniform to the table
          std::map<Str, UNIFORM>::iterator it = mUniforms.find(uniform_name);
          if(it == mUniforms.end())
          {
            UNIFORM uniform;
            uniform.type = type;
            uniform.count = 0;
            it = mUniforms.insert(mUniforms.begin(), std::make_pair(uniform_name,uniform));
          }
          CHECK(it!=mUniforms.end() && it->second.type==type);
          program.uniforms.push_back(std::make_pair(location,&it->second));
        }
        GDELETE([]uniform_name);

        // validate the program
        glValidateProgram(program.object);
        glGetProgramiv(program.object, GL_VALIDATE_STATUS, &result);
        if(result == 0)
        {
          GLsizei size;
          glGetProgramiv(program.object, GL_INFO_LOG_LENGTH, &size);
          if(size > 1)
          {
            Str log(size+1,0); GLsizei length;
            glGetProgramInfoLog(program.object, size, &length, &log[0]);
            if(length > 0) GAssert(log.c_str());
          }
        }

        // add the program to the map
        mPrograms.push_back(program);
      }
      // prase the pass
      else if(Str(shader_child_config_ptr->GetValue()) == "pass")
      {
        const GConfig*pass_config_ptr = (const GConfig*)shader_child_config_ptr;

        // get the pass name
        Str pass_name = pass_config_ptr->GetAttribute("name"); CHECK(pass_name!="");

        // the new pass
        PASS pass;
        pass.program = NULL;
        pass.blend_src = GL_ONE;
        pass.blend_dst = GL_ZERO;
        pass.depth_func = GL_LEQUAL;
        pass.depth_mask = TRUE;
        pass.cull_mode = GL_NONE;

        // prase the pass`s children
        const GConfig *pass_child_config_ptr = NULL;
        for(U32 j = 0; pass_child_config_ptr = pass_config_ptr->GetChild(j); j++)
        {
          if(Str(pass_child_config_ptr->GetValue()) == "program")
          {
            // get the program name
            Str program_name = pass_child_config_ptr->GetAttribute("name"); CHECK(program_name!="");

            // get the program of the pass
            for(std::list<PROGRAM>::iterator it = mPrograms.begin(); it != mPrograms.end(); ++it)
            { if(program_name == it->name) pass.program = &(*it); }
            CHECK(pass.program);
          }
          else if(Str(pass_child_config_ptr->GetValue()) == "blend")
          {
            // get the blend src
            Str src = pass_child_config_ptr->GetAttribute("src"); CHECK(src!="");
            if(src == "BF_ZERO") pass.blend_src = GL_ZERO;
            else if(src == "BF_ONE") pass.blend_src = GL_ONE;
            else if(src == "BF_DST_COLOR") pass.blend_src = GL_DST_COLOR;
            else if(src == "BF_ONE_MINUS_DST_COLOR") pass.blend_src = GL_ONE_MINUS_DST_COLOR;
            else if(src == "BF_SRC_ALPHA_SATURATE") pass.blend_src = GL_SRC_ALPHA_SATURATE;
            else if(src == "BF_SRC_ALPHA") pass.blend_src = GL_SRC_ALPHA;
            else if(src == "BF_ONE_MINUS_SRC_ALPHA") pass.blend_src = GL_ONE_MINUS_SRC_ALPHA;
            else if(src == "BF_DST_ALPHA") pass.blend_src = GL_DST_ALPHA;
            else if(src == "BF_ONE_MINUS_DST_ALPHA") pass.blend_src = GL_ONE_MINUS_DST_ALPHA;

            // get the blend dst
            Str dst = pass_child_config_ptr->GetAttribute("dst"); CHECK(dst!="");
            if(dst == "BF_ZERO") pass.blend_dst = GL_ZERO;
            else if(dst == "BF_ONE") pass.blend_dst = GL_ONE;
            else if(dst == "BF_SRC_COLOR") pass.blend_dst = GL_SRC_COLOR;
            else if(dst == "BF_ONE_MINUS_SRC_COLOR") pass.blend_dst = GL_ONE_MINUS_SRC_COLOR;
            else if(dst == "BF_SRC_ALPHA") pass.blend_dst = GL_SRC_ALPHA;						
            else if(dst == "BF_ONE_MINUS_SRC_ALPHA") pass.blend_dst = GL_ONE_MINUS_SRC_ALPHA;
            else if(dst == "BF_DST_ALPHA") pass.blend_dst = GL_DST_ALPHA;
            else if(dst == "BF_ONE_MINUS_DST_ALPHA") pass.blend_dst = GL_ONE_MINUS_DST_ALPHA;
          }					
          else if(Str(pass_child_config_ptr->GetValue()) == "depth")
          {
            // get the depth func
            Str func = pass_child_config_ptr->GetAttribute("func"); CHECK(func!="");
            if(func == "CF_NEVER") pass.depth_func = GL_NEVER;
            else if(func == "CF_LESS") pass.depth_func = GL_LESS;
            else if(func == "CF_EQUAL") pass.depth_func = GL_EQUAL;
            else if(func == "CF_LEQUAL") pass.depth_func = GL_LEQUAL;
            else if(func == "CF_GREATER") pass.depth_func = GL_GREATER;
            else if(func == "CF_NOTEQUAL") pass.depth_func = GL_NOTEQUAL;
            else if(func == "CF_GEQUAL") pass.depth_func = GL_GEQUAL;
            else if(func == "CF_ALWAYS") pass.depth_func = GL_ALWAYS;

            // get the depth mask
            Str mask = pass_child_config_ptr->GetAttribute("mask"); CHECK(mask!="");
            pass.depth_mask = (mask == "TRUE") ? TRUE : FALSE;
          }
          else if(Str(pass_child_config_ptr->GetValue()) == "cull")
          {
            // get the cull mode
            Str mode = pass_child_config_ptr->GetAttribute("mode"); CHECK(mode!="");
            if(mode == "CULL_NONE") pass.cull_mode = GL_NONE;
            else if (mode == "CULL_FRONT") pass.cull_mode = GL_FRONT;
            else if (mode == "CULL_BACK") pass.cull_mode = GL_BACK;
          }
          else
          {
            GAssert(VA("The effect`s pass keyword(%s) is unknown!", pass_child_config_ptr->GetValue()));
          }
        }

        // add the pass to the table
        mPasses.insert(std::make_pair(pass_name,pass));
      }
      else
      {
        GAssert(VA("The shader`s keyword(%s) is unknown!", shader_child_config_ptr->GetValue()));
      }
    }

    // reset the program`s samplers
    for(std::map<Str, UNIFORM>::iterator it1 = mUniforms.begin(); it1 != mUniforms.end(); ++it1 )
    {
      UNIFORM&uniform = it1->second;
      if(uniform.type >= GL_SAMPLER_2D && uniform.type <= GL_SAMPLER_CUBE)
      {
        std::map<Str, SAMPLER>::iterator it2 = samplers.find(it1->first);
        if(it2 == samplers.end()) continue;					
        SAMPLER& sampler = it2->second;
        uniform.sampler.wrap_s			= sampler.wrap_s;
        uniform.sampler.wrap_t			= sampler.wrap_t;
        uniform.sampler.wrap_r			= sampler.wrap_r;
        uniform.sampler.min_filter	= sampler.min_filter;
        uniform.sampler.mag_filter	= sampler.mag_filter;
      }
    }
	}

	/*
	====================
	Bind
	====================
	*/
	VOID Shader::Bind(const CHAR* name)
	{
    CHECK(name);

    // get the pass
    std::map<Str, PASS>::iterator it1 = mPasses.find(name);
    CHECK(it1 != mPasses.end());
    PASS& pass = it1->second;

    // bind the program
    PROGRAM* program = pass.program;
    CHECK(program&&program->object);
    BindProgram(program->object);

    // update the constant of the program
    GLint texture_unit = 0;
    for(std::list< std::pair<I32, UNIFORM*> >::iterator it = program->uniforms.begin(); it != program->uniforms.end(); ++it)
    {
      const I32& location = it->first;
      UNIFORM*uniform = it->second;
      switch(uniform->type)
      {
      case GL_FLOAT:
        glUniform1fv(location, uniform->count, (GLfloat*)&uniform->data[0]);
        break;
      case GL_FLOAT_VEC2:
        glUniform2fv(location, uniform->count, (GLfloat*)&uniform->data[0]);
        break;
      case GL_FLOAT_VEC3:
        glUniform3fv(location, uniform->count, (GLfloat*)&uniform->data[0]);
        break;
      case GL_FLOAT_VEC4:
        glUniform4fv(location, uniform->count, (GLfloat*)&uniform->data[0]);
        break;
      case GL_INT:
        glUniform1iv(location, uniform->count, (GLint*)&uniform->data[0]);
        break;
      case GL_INT_VEC2:
        glUniform2iv(location, uniform->count, (GLint*)&uniform->data[0]);
        break;
      case GL_INT_VEC3:
        glUniform3iv(location, uniform->count, (GLint*)&uniform->data[0]);
        break;
      case GL_INT_VEC4:
        glUniform4iv(location, uniform->count, (GLint*)&uniform->data[0]);
        break;
      case GL_FLOAT_MAT2:
        glUniformMatrix2fv(location, uniform->count, GL_FALSE, (GLfloat*)&uniform->data[0]);
        break;
      case GL_FLOAT_MAT3:
        glUniformMatrix3fv(location, uniform->count, GL_FALSE, (GLfloat*)&uniform->data[0]);
        break;
      case GL_FLOAT_MAT4:
        glUniformMatrix4fv(location, uniform->count, GL_FALSE, (GLfloat*)&uniform->data[0]);
        break;
      case GL_SAMPLER_2D:
      case GL_SAMPLER_CUBE:
        {
          // get the texture unit
          U32 unit = texture_unit++;

          // set the sampler
          CHECK(uniform->sampler.texture);
          uniform->sampler.texture->WrapS(uniform->sampler.wrap_s);
          uniform->sampler.texture->WrapT(uniform->sampler.wrap_t);
          uniform->sampler.texture->MinFilter(uniform->sampler.min_filter);
          uniform->sampler.texture->MagFilter(uniform->sampler.mag_filter);

          // bind the texture
          uniform->sampler.texture->Bind(unit);

          // set the unit to the shader
          glUniform1i(location, unit);
        }
        break;
#if 0
      case GL_SAMPLER_3D_OES:
        {
          // get the texture unit
          U32 unit = texture_unit++;

          // get the volume texture			
          VolumeTexture* volume_texture = dynamic_cast<VolumeTexture*>(uniform->sampler.texture.Ptr());
          CHECK(volume_texture);

          // set the sampler
          volume_texture->WrapS(uniform->sampler.wrap_s);
          volume_texture->WrapT(uniform->sampler.wrap_t);
          volume_texture->WrapR(uniform->sampler.wrap_r);
          volume_texture->MinFilter(uniform->sampler.min_filter);
          volume_texture->MagFilter(uniform->sampler.mag_filter);

          // bind the texture
          volume_texture->Bind(unit);

          // set the unit to the shader
          glUniform1i(location, unit);
        }
        break;
#endif
      default:
        GAssert(VA("The type(%d) of uniform is error!\n", uniform->type));
        break;	
      }
    }			

    // bind depth function and the depth mask if neccesary
    BindDepthFunc(pass.depth_func);
    BindDepthMask(pass.depth_mask);

    // bind blending function if neccesary
    BindBlendFunc(pass.blend_src, pass.blend_dst);

    // bind face culling if neccesary
    BindCullFunc(pass.cull_mode);
	}

  //////////////////////////////////////////////////////////////////////////

  /*
	====================
	SetParameter1f
	====================
	*/
	VOID Shader::SetParameter1f(const CHAR* pname, F32 v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
		UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT);
		if(uniform.count == 1)
		{
			if(*((F32*)&uniform.data[0]) != v)
			{
				*((F32*)&uniform.data[0]) = v;
			}
		}
		else
		{
			uniform.count = 1;
			uniform.data.resize(sizeof(F32));
			*((F32*)&uniform.data[0]) = v;
		}
	}

	/*
	====================
	SetParameter2f
	====================
	*/
	VOID Shader::SetParameter2f(const CHAR* pname, F32 v0, F32 v1)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT_VEC2);
		if( uniform.count == 1 )
		{
			if(((F32*)&uniform.data[0])[0] != v0 ||
				((F32*)&uniform.data[0])[1] != v1)
			{
				((F32*)&uniform.data[0])[0] = v0;
				((F32*)&uniform.data[0])[1] = v1;
			}
		}
		else
		{
			uniform.count = 1;
			uniform.data.resize( 2 * sizeof(F32) );
			((F32*)&uniform.data[0])[0] = v0;
			((F32*)&uniform.data[0])[1] = v1;
		}
	}

	/*
	====================
	SetParameter3f
	====================
	*/
	VOID Shader::SetParameter3f(const CHAR* pname, F32 v0, F32 v1, F32 v2)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT_VEC3);
		if(uniform.count == 1)
		{
			if( ((F32*)&uniform.data[0])[0] != v0 ||
				((F32*)&uniform.data[0])[1] != v1 ||
				((F32*)&uniform.data[0])[2] != v2 )
			{
				((F32*)&uniform.data[0])[0] = v0;
				((F32*)&uniform.data[0])[1] = v1;
				((F32*)&uniform.data[0])[2] = v2;
			}
		}
		else
		{
			uniform.count = 1;
			uniform.data.resize(3 * sizeof(F32));
			((F32*)&uniform.data[0])[0] = v0;
			((F32*)&uniform.data[0])[1] = v1;
			((F32*)&uniform.data[0])[2] = v2;
		}
	}

	/*
	====================
	SetParameter4f
	====================
	*/
	VOID Shader::SetParameter4f(const CHAR* pname, F32 v0, F32 v1, F32 v2, F32 v3)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT_VEC4);
		if(uniform.count == 1)
		{
			if( ((F32*)&uniform.data[0])[0] != v0 ||
				((F32*)&uniform.data[0])[1] != v1 ||
				((F32*)&uniform.data[0])[2] != v2 ||
				((F32*)&uniform.data[0])[3] != v3 )
			{
				((F32*)&uniform.data[0])[0] = v0;
				((F32*)&uniform.data[0])[1] = v1;
				((F32*)&uniform.data[0])[2] = v2;
				((F32*)&uniform.data[0])[3] = v3;
			}
		}
		else
		{
			uniform.count = 1;
			uniform.data.resize(4 * sizeof(F32));
			((F32*)&uniform.data[0])[0] = v0;
			((F32*)&uniform.data[0])[1] = v1;
			((F32*)&uniform.data[0])[2] = v2;
			((F32*)&uniform.data[0])[3] = v3;
		}
	}

	/*
	====================
	SetParameter1i
	====================
	*/
	VOID Shader::SetParameter1i(const CHAR* pname, I32 v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_INT);
		if(uniform.count == 1)
		{
			if(*((I32*)&uniform.data[0]) != v)
			{
				*((I32*)&uniform.data[0]) = v;
			}
		}
		else
		{
			uniform.count = 1;
			uniform.data.resize( sizeof(I32) );
			*((I32*)&uniform.data[0]) = v;
		}
	}

	/*
	====================
	SetParameter2i
	====================
	*/
	VOID Shader::SetParameter2i(const CHAR* pname, I32 v0, I32 v1)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_INT_VEC2);
		if(uniform.count == 1)
		{
			if( ((I32*)&uniform.data[0])[0] != v0 ||
				((I32*)&uniform.data[0])[1] != v1 )
			{
				((I32*)&uniform.data[0])[0] = v0;
				((I32*)&uniform.data[0])[1] = v1;
			}
		}
		else
		{
			uniform.count = 1;
			uniform.data.resize(2 * sizeof(I32));
			((I32*)&uniform.data[0])[0] = v0;
			((I32*)&uniform.data[0])[1] = v1;
		}
	}

	/*
	====================
	SetParameter3i
	====================
	*/
	VOID Shader::SetParameter3i(const CHAR* pname, I32 v0, I32 v1, I32 v2)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_INT_VEC3);
		if(uniform.count == 1)
		{
			if( ((I32*)&uniform.data[0])[0] != v0 ||
				((I32*)&uniform.data[0])[1] != v1 ||
				((I32*)&uniform.data[0])[2] != v2 )
			{
				((I32*)&uniform.data[0])[0] = v0;
				((I32*)&uniform.data[0])[1] = v1;
				((I32*)&uniform.data[0])[2] = v2;
			}
		}
		else
		{
			uniform.count = 1;
			uniform.data.resize(3 * sizeof(I32));
			((I32*)&uniform.data[0])[0] = v0;
			((I32*)&uniform.data[0])[1] = v1;
			((I32*)&uniform.data[0])[2] = v2;
		}
	}

	/*
	====================
	SetParameter4i
	====================
	*/
	VOID Shader::SetParameter4i(const CHAR* pname, I32 v0, I32 v1, I32 v2, I32 v3)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_INT_VEC4);
		if(uniform.count == 1)
		{
			if( ((I32*)&uniform.data[0])[0] != v0 ||
				((I32*)&uniform.data[0])[1] != v1 ||
				((I32*)&uniform.data[0])[2] != v2 ||
				((I32*)&uniform.data[0])[3] != v3 )
			{
				((I32*)&uniform.data[0])[0] = v0;
				((I32*)&uniform.data[0])[1] = v1;
				((I32*)&uniform.data[0])[2] = v2;
				((I32*)&uniform.data[0])[3] = v3;
			}
		}
		else
		{
			uniform.count = 1;
			uniform.data.resize(4 * sizeof(I32));
			((I32*)&uniform.data[0])[0] = v0;
			((I32*)&uniform.data[0])[1] = v1;
			((I32*)&uniform.data[0])[2] = v2;
			((I32*)&uniform.data[0])[3] = v3;
		}
	}

	/*
	====================
	SetParameter1fv
	====================
	*/
	VOID Shader::SetParameter1fv(const CHAR* pname, U32 count, const F32 * v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT);
		if(uniform.count == count)
		{
			if(::memcmp( &uniform.data[0], v, count * sizeof(F32) ))
			{
				::memcpy(&uniform.data[0], v, count * sizeof(F32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize( count * sizeof(F32) );
			::memcpy( &uniform.data[0], v, count * sizeof(F32) );
		}
	}

	/*
	====================
	SetParameter2fv
	====================
	*/
	VOID Shader::SetParameter2fv(const CHAR* pname, U32 count, const F32* v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT_VEC2);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * 2 * sizeof(F32)))
			{
				::memcpy(&uniform.data[0], v, count * 2 * sizeof(F32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize( count * 2 * sizeof(F32) );
			::memcpy(&uniform.data[0], v, count * 2 * sizeof(F32));
		}
	}

	/*
	====================
	SetParameter3fv
	====================
	*/
	VOID Shader::SetParameter3fv(const CHAR* pname, U32 count, const F32 * v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT_VEC3);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * 3 * sizeof(F32)))
			{
				::memcpy(&uniform.data[0], v, count * 3 * sizeof(F32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize(count * 3 * sizeof(F32));
			::memcpy(&uniform.data[0], v, count * 3 * sizeof(F32));
		}
	}

	/*
	====================
	SetParameter4fv
	====================
	*/
	VOID Shader::SetParameter4fv(const CHAR* pname, U32 count, const F32 * v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT_VEC4);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * 4 * sizeof(F32)))
			{
				::memcpy(&uniform.data[0], v, count * 4 * sizeof(F32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize(count * 4 * sizeof(F32));
			::memcpy(&uniform.data[0], v, count * 4 * sizeof(F32));
		}
	}

	/*
	====================
	SetParameter1iv
	====================
	*/
	VOID Shader::SetParameter1iv(const CHAR* pname, U32 count, const I32 * v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_INT);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * sizeof(I32)))
			{
				::memcpy(&uniform.data[0], v, count * sizeof(I32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize(count * sizeof(I32));
			::memcpy(&uniform.data[0], v, count * sizeof(I32));
		}
	}

	/*
	====================
	SetParameter2iv
	====================
	*/
	VOID Shader::SetParameter2iv(const CHAR* pname, U32 count, const I32 * v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_INT_VEC2);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * 2 * sizeof(I32)))
			{
				::memcpy(&uniform.data[0], v, count * 2 * sizeof(I32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize(count * 2 * sizeof(I32));
			::memcpy(&uniform.data[0], v, count * 2 * sizeof(I32));
		}
	}

	/*
	====================
	SetParameter3iv
	====================
	*/
	VOID Shader::SetParameter3iv(const CHAR* pname, U32 count, const I32* v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_INT_VEC3);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * 3 * sizeof(I32)))
			{
				::memcpy(&uniform.data[0], v, count * 3 * sizeof(I32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize(count * 3 * sizeof(I32));
			::memcpy(&uniform.data[0], v, count * 3 * sizeof(I32));
		}
	}

	/*
	====================
	SetParameter4iv
	====================
	*/
	VOID Shader::SetParameter4iv(const CHAR* pname, U32 count, const I32 * v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_INT_VEC4);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * 4 * sizeof(I32)))
			{
				::memcpy(&uniform.data[0], v, count * 4 * sizeof(I32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize(count * 4 * sizeof(I32));
			::memcpy(&uniform.data[0], v, count * 4 * sizeof(I32));
		}
	}

	/*
	====================
	SetParameterMatrix2fv
	====================
	*/
	VOID Shader::SetParameterMatrix2fv(const CHAR* pname, U32 count, const F32 * v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT_MAT2);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * 4 * sizeof(F32)))
			{
				::memcpy(&uniform.data[0], v, count * 4 * sizeof(F32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize(count * 4 * sizeof(F32));
			::memcpy(&uniform.data[0], v, count * 4 * sizeof(F32));
		}
	}

	/*
	====================
	SetParameterMatrix3fv
	====================
	*/
	VOID Shader::SetParameterMatrix3fv(const CHAR* pname, U32 count, const F32 * v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT_MAT3);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * 9 * sizeof(F32)))
			{
				::memcpy(&uniform.data[0], v, count * 9 * sizeof(F32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize(count * 9 * sizeof(F32));
			memcpy(&uniform.data[0], v, count * 9 * sizeof(F32));
		}
	}

	/*
	====================
	SetParameterMatrix4fv
	====================
	*/
	VOID Shader::SetParameterMatrix4fv(const CHAR* pname, U32 count, const F32 * v)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type == GL_FLOAT_MAT4);
		if(uniform.count == count)
		{
			if(::memcmp(&uniform.data[0], v, count * 16 * sizeof(F32)))
			{
				::memcpy(&uniform.data[0], v, count * 16 * sizeof(F32));
			}
		}
		else
		{
			uniform.count = count;
			uniform.data.resize(count * 16 * sizeof(F32));
			::memcpy(&uniform.data[0], v, count * 16 * sizeof(F32));
		}
	}

	/*
	====================
	SetParameterSampler
	====================
	*/
	VOID Shader::SetParameterSampler(const CHAR* pname, const BaseTexture* texture)
	{
    std::map<Str, UNIFORM>::iterator it = mUniforms.find(pname);
    CHECK(it != mUniforms.end());
    UNIFORM& uniform = it->second;
		CHECK(uniform.type>=GL_SAMPLER_2D && uniform.type<=GL_SAMPLER_CUBE);
		if(uniform.sampler.texture != texture)
		{
			uniform.sampler.texture = (BaseTexture*)texture;
		}
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
