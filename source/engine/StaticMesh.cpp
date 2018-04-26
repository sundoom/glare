/*
 *  StaticMesh.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////	

	StaticMesh::StaticMesh(const CHAR* name) : Resource(name)
	{
		
	}

	StaticMesh::~StaticMesh()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID StaticMesh::Load(const GData* data)
	{
		CHECK(data);

		// get the mesh config
		GConfigPtr mesh_config_ptr = GConfig::Load((CHAR*)data->Ptr()); CHECK(mesh_config_ptr);

		// check it if it is a mesh?
		CHECK(Str(mesh_config_ptr->GetValue()) == "static_mesh");

		// prase the mesh`s children
		const GConfig* mesh_child_config_ptr = NULL;
		for( U32 i = 0; mesh_child_config_ptr = mesh_config_ptr->GetChild(i); i++ )
		{			
			// prase the primitive
			if( Str(mesh_child_config_ptr->GetValue()) == "primitive" )
			{
				const GConfig *primitive_config_ptr = (const GConfig *)mesh_child_config_ptr;

				// build a new primitive
				Primitive* primitive = GNEW(Primitive); CHECK(primitive);

				// get the primitive type
				Str type = primitive_config_ptr->GetAttribute("type");
				if(type=="PT_POINTS") primitive->SetType(Primitive::PT_POINTS);
				else if(type=="PT_LINES") primitive->SetType(Primitive::PT_LINES);
				else if(type=="PT_LINE_LOOP") primitive->SetType(Primitive::PT_LINE_LOOP);
				else if(type=="PT_LINE_STRIP") primitive->SetType(Primitive::PT_LINE_STRIP);
				else if(type=="PT_TRIANGLES") primitive->SetType(Primitive::PT_TRIANGLES);
				else if(type=="PT_TRIANGLE_STRIP") primitive->SetType(Primitive::PT_TRIANGLE_STRIP);
				else if(type=="PT_TRIANGLE_FAN") primitive->SetType(Primitive::PT_TRIANGLE_FAN);
				else if(type=="PT_QUADS") primitive->SetType(Primitive::PT_QUADS);
				else if(type=="PT_QUAD_STRIP") primitive->SetType(Primitive::PT_QUAD_STRIP);
				else if(type=="PT_POLYGON") primitive->SetType(Primitive::PT_POLYGON);
				else GAssert( VA("The primitive`s type(%s) is unknown!", type.c_str()) );

				const GConfig* primitive_child_config_ptr = NULL;
				for( U32 j = 0; primitive_child_config_ptr = primitive_config_ptr->GetChild(j); j++ )
				{
					if( Str(primitive_child_config_ptr->GetValue()) == "vertex" )
					{
						const GConfig *vertex_config_ptr = (const GConfig *)primitive_child_config_ptr;

						// get the vertex format
						Str format_string = vertex_config_ptr->GetAttribute("format"); CHECK(format_string!= "");
						std::vector<U8>formats;
            formats.push_back(0);
						U32 elem_count = 0, elem_size = 0;
						{
							std::vector<Str>tokens = GParse(format_string);
							U32 size = tokens.size();
							for(U32 k = 0; k < size; k++)
							{
								if(tokens[k]=="VT_1I")		formats.push_back(VT_1I), elem_count+=1, elem_size+=1*sizeof(I32);
								else if(tokens[k]=="VT_2I") formats.push_back(VT_2I), elem_count+=2, elem_size+=2*sizeof(I32);
								else if(tokens[k]=="VT_3I") formats.push_back(VT_3I), elem_count+=3, elem_size+=3*sizeof(I32);
								else if(tokens[k]=="VT_4I") formats.push_back(VT_4I), elem_count+=4, elem_size+=4*sizeof(I32);
								else if(tokens[k]=="VT_1F") formats.push_back(VT_1F), elem_count+=1, elem_size+=1*sizeof(F32);
								else if(tokens[k]=="VT_2F") formats.push_back(VT_2F), elem_count+=2, elem_size+=2*sizeof(F32);
								else if(tokens[k]=="VT_3F") formats.push_back(VT_3F), elem_count+=3, elem_size+=3*sizeof(F32);
								else if(tokens[k]=="VT_4F") formats.push_back(VT_4F), elem_count+=4, elem_size+=4*sizeof(F32);
								else GAssert( VA("The vertex`s format(%s) is unknown!", tokens[k].c_str()) );;
							}
						}
            CHECK(formats.size() < 16);
            U8 format_count = formats.size() - 1;
            formats[0] = format_count;           

						// get the vertex data
						std::vector<Str>tokens = GParse(vertex_config_ptr->GetText());
						U32 size = tokens.size();
						CHECK(size % elem_count == 0);
            U32 count = size/elem_count;
            U32 stride = elem_size;
            U8 attrib[16];
            ::memcpy(attrib, &formats[0], formats.size()*sizeof(U8));
            std::vector<U8>buffer(size/elem_count*elem_size);
            U8*ptr = &buffer[0];
						Str*token = &tokens[0];
						for(U32 k = 0; k < count; k++)
						{
							for( U32 m = 1; m <= format_count; m++ )
							{
								switch(formats[m])
								{
								case VT_1I:
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									break;
								case VT_2I:
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									break;
								case VT_3I:
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									break;
								case VT_4I:
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									(*((I32*)ptr)) = ::atoi(token->c_str()), ptr+=sizeof(I32), token++;
									break;
								case VT_1F:
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									break;			   
								case VT_2F:
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									break;			   
								case VT_3F:
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									break;			   
								case VT_4F:
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									(*((F32*)ptr)) = (F32)::atof(token->c_str()), ptr+=sizeof(F32), token++;
									break;
								}
							}
						}

						// build the vertex data for the primitive
						VertexData* vd_ptr = GNEW(VertexData); CHECK(vd_ptr);
						vd_ptr->Load(count, stride, attrib, &buffer[0]);
						primitive->SetVertexData(vd_ptr);
					}
					// parse the index
					else if( Str(primitive_child_config_ptr->GetValue()) == "index" )
					{
						const GConfig *index_config_ptr = (const GConfig *)primitive_child_config_ptr;

						std::vector<Str>tokens = GParse(index_config_ptr->GetText());

						// get the index stride
						U32 stride = ::atoi(index_config_ptr->GetAttribute("stride"));

						// get the index count
						U32 count = tokens.size(); CHECK(count!=0);

						// get the index data
            IndexData* id_ptr = GNEW(IndexData); CHECK(id_ptr);
						if(stride==sizeof(U16))
						{
              U16*ptr = GNEW(U16[count]); CHECK(ptr);
							for(U32 k = 0; k < tokens.size(); k++)ptr[k] = ::atoi(tokens[k].c_str());
              id_ptr->Load(count, stride, ptr);
              if(ptr){GDELETE([]ptr), ptr = NULL;}
						}
						else if(stride==sizeof(U32))
						{
              U32*ptr = GNEW(U32[count]); CHECK(ptr);							
							for(U32 k = 0; k < tokens.size(); k++)ptr[k] = ::atoi(tokens[k].c_str());
              id_ptr->Load(count, stride, ptr);
              if(ptr){GDELETE([]ptr), ptr = NULL;}
						}
						else
						{
							GAssert( VA("The index`s type(%s) is unknown!", type.c_str()) );
						}						
						primitive->SetIndexData(id_ptr);
					}
					// parse the shader
					else if( Str(primitive_child_config_ptr->GetValue()) == "shader" )
					{
						const GConfig *shader_config_ptr = (const GConfig *)primitive_child_config_ptr;

						// set the shader
						Str shader_name = shader_config_ptr->GetAttribute("name"); CHECK(shader_name != "");
						Str pass_name = shader_config_ptr->GetAttribute("pass"); CHECK(pass_name != "");
            ShaderData* shader = Resource::Find<ShaderData>(shader_name.c_str());
            CHECK(shader);
						primitive->SetShader(shader,pass_name.c_str());

						// parse the constant of the shader
						const GConfig *constant_config_ptr = NULL;
						for( U32 k = 0; constant_config_ptr = shader_config_ptr->GetChild(k); k++ )
						{
							CHECK( Str(constant_config_ptr->GetValue()) == "constant" );

							// get the name of the constant
							Str constant_name = constant_config_ptr->GetAttribute("name"); CHECK(constant_name!= "");

							// get the type of the constant
							Str constant_type = constant_config_ptr->GetAttribute("type"); CHECK(constant_type!= "");

							// get the value of the constant
							Str constant_value = constant_config_ptr->GetText(); CHECK(constant_value!= "");

							// the new constant
							ConstantPtr constant_ptr = GNEW(Constant); CHECK(constant_ptr);

							// get the constant type and the value
							if(constant_type == "CT_INT")
							{
								I32 value;
								value = ::atoi(constant_value.c_str());
								constant_ptr->SetInt(value);
							}
							else if(constant_type == "CT_INT_ARRAY") 
							{
								std::vector<I32>value;
								std::vector<Str>tokens = GParse(constant_value);
								value.resize(tokens.size());
								for( U32 l = 0; l < tokens.size(); l++ ) value[l] = ::atoi(tokens[l].c_str());
								constant_ptr->SetIntArray((I32*)(&value[0]), value.size());
							}
							else if(constant_type == "CT_FLOAT")
							{
								F32 value;
								value = (F32)::atof(constant_value.c_str());
								constant_ptr->SetFloat(value);
							}
							else if(constant_type == "CT_FLOAT_ARRAY")
							{
								std::vector<F32>value;
								std::vector<Str>tokens = GParse(constant_value);
								value.resize(tokens.size());
								for( U32 l = 0; l < tokens.size(); l++ ) value[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetFloatArray((F32*)(&value[0]), value.size());
							}
							else if(constant_type == "CT_VECTOR")
							{
								Vector4 value;
								std::vector<Str>tokens = GParse(constant_value); CHECK(tokens.size() == 4);
								for(U32 l = 0; l < 4; l++) value.ptr()[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetVector(value);
							}
							else if( constant_type == "CT_VECTOR_ARRAY" )
							{
								std::vector<F32>value;
								std::vector<Str>tokens = GParse(constant_value); CHECK(tokens.size()%4 == 0);
								value.resize(tokens.size());
								for( U32 l = 0; l < tokens.size(); l++ ) value[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetVectorArray((Vector4*)(&value[0]), value.size()/4);
							}
							else if( constant_type == "CT_MATRIX" ) 
							{
								Matrix value;
								std::vector<Str>tokens = GParse(constant_value); CHECK(tokens.size() == 16);
								for(U32 l = 0; l < 16; l++) value.ptr()[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetMatrix(value);
							}
							else if( constant_type == "CT_MATRIX_ARRAY" ) 
							{
								std::vector<F32>value;
								std::vector<Str>tokens = GParse(constant_value); CHECK(tokens.size()%16 == 0);
								value.resize(tokens.size());
								for( U32 l = 0; l < tokens.size(); l++ ) value[l] = (F32)::atof(tokens[l].c_str());
								constant_ptr->SetMatrixArray((Matrix*)(&value[0]), value.size()/16);
							}
							else if( constant_type == "CT_TEXTURE" ) 
							{
								// build the texture
								Str tex_name = constant_value;
                TextureData* texture = Resource::Find<TextureData>(tex_name.c_str());
                CHECK(texture);
                constant_ptr->SetTexture(texture);
							}
							else
							{
								GAssert( VA("The type(%s) of the constant(%s) is error.\n", constant_type.c_str() , constant_name.c_str() ) );
							}

							// add the constant to the map
							primitive->SetConstant(constant_name.c_str(), constant_ptr.Ptr());
						}
					}
					// parse the bounding box
					else if( Str(primitive_child_config_ptr->GetValue()) == "bounding_box" )
					{
						const GConfig *box_config_ptr = (const GConfig *)primitive_child_config_ptr;
						std::vector<Str>tokens = GParse(box_config_ptr->GetText());
						CHECK(tokens.size() == 6);
						BoundingBox box;
						box.set((F32)::atof(tokens[0].c_str()),(F32)::atof(tokens[1].c_str()),(F32)::atof(tokens[2].c_str()),(F32)::atof(tokens[3].c_str()),(F32)::atof(tokens[4].c_str()),(F32)::atof(tokens[5].c_str()));
						primitive->SetBox(box);
					}
					else
					{
						GAssert( VA("The primitive`s keyword(%s) is unknown!", primitive_child_config_ptr->GetValue()) );
					}
				}

				// add the primitive to the table
				mPrimitives.push_back(primitive);
			}
			else
			{
				GAssert( VA("The static mesh`s keyword(%s) is unknown!", mesh_child_config_ptr->GetValue()) );
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Draw
	====================
	*/
	VOID StaticMesh::Draw(const Matrix& wvp)
	{
    // get the frustum
    Frustum frustum(wvp);

    // draw all of the primitive
    for(U32 i = 0; i < mPrimitives.size(); i++)
    {
      PrimitivePtr& primitive = mPrimitives[i];
      const BoundingBox& box = primitive->GetBox();
      if(!frustum.boxInFrustum(box.min(),box.max()))continue;
      primitive->GetConstant("gWVP")->SetMatrix(wvp);
      primitive->Draw();
    }
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

