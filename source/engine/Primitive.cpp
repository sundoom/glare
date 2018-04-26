/*
 *  Primitive.cpp
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

	Primitive::Primitive() 
	{
		mType = PT_TRIANGLES;
	}

	Primitive::~Primitive() 
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	type
	====================
	*/
	VOID Primitive::SetType(U32 t)
	{
		mType = t;
	}
	
	U32 Primitive::GetType() const
	{
		return mType;
	}

	/*
	====================
	Shader
	====================
	*/
	VOID Primitive::SetShader(const ShaderData* s, const CHAR* pass)
	{
		CHECK(s!=NULL&&pass!=NULL);
		if(mShaderPtr != s)
		{
			mShaderPtr = (ShaderData*)s;
      mConstants.clear();
		}
		mPassName = pass;
	}
	
	const ShaderData* Primitive::GetShader() const
	{
		return mShaderPtr.Ptr();
	}

	ShaderData* Primitive::GetShader()
	{
		return mShaderPtr.Ptr();
	}

	/*
	====================
	constant
	====================
	*/
	VOID Primitive::SetConstant(const CHAR* name, const Constant* c)
	{
		CHECK(name&&c);
		if(mConstants.find(name)!=mConstants.end())GAssert(VA("The constant is already exists : %s.",name));
		mConstants.insert(std::make_pair(name, (Constant*)c));
	}

	const Constant* Primitive::GetConstant(const CHAR *name) const
	{
		CHECK(name);
		std::map<Str, ConstantPtr>::const_iterator it = mConstants.find(name);
		if(it == mConstants.end()) return NULL;
		return it->second.Ptr();
	}

	Constant* Primitive::GetConstant(const CHAR *name)
	{
		CHECK(name);
		std::map<Str, ConstantPtr>::iterator it = mConstants.find(name);
		if(it == mConstants.end()) return NULL;
		return it->second.Ptr();
	}

	/*
	====================
	vertex
	====================
	*/
	VOID Primitive::SetVertexData(const VertexData* vd)
	{
		CHECK(vd);
		mVDPtr = (VertexData*)vd;
	}

	const VertexData* Primitive::GetVertexData() const
	{
		return mVDPtr.Ptr();
	}

	VertexData* Primitive::GetVertexData()
	{
		return mVDPtr.Ptr();
	}

	/*
	====================
	index
	====================
	*/
	VOID Primitive::SetIndexData(const IndexData* id)
	{
		CHECK(id);
		mIDPtr = (IndexData*)id;
	}

	const IndexData* Primitive::GetIndexData() const
	{
		return mIDPtr.Ptr();
	}

	IndexData* Primitive::GetIndexData()
	{
		return mIDPtr.Ptr();
	}
	
	/*
	====================
	box
	====================
	*/
	VOID Primitive::SetBox(const BoundingBox& box)
	{
		mBox = box;
	}

	const BoundingBox& Primitive::GetBox() const
	{
		return mBox;
	}	

	BoundingBox& Primitive::GetBox()
	{
		return mBox;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	operator
	====================
	*/
	VOID Primitive::Draw()
	{
	  // bind the vertex buffer
    CHECK(mVDPtr&&mVDPtr->mHandle);
    RIBindVertexBuffer(mVDPtr->mHandle);

		// bind the index buffer
    CHECK(mIDPtr&&mIDPtr->mHandle);
    RIBindIndexBuffer(mIDPtr->mHandle);
		U32 count = mIDPtr->mCount;
		U32 stride = mIDPtr->mStride;
		GLenum type;
		if(stride == sizeof(U16))type = DT_UNSIGNED_SHORT;
		else if(stride == sizeof(U32)) type = DT_UNSIGNED_INT;
		else GAssert("The index stride is error!");

		// update the value of the uniform
    CHECK(mShaderPtr&&mShaderPtr->mHandle);
		for(std::map<Str, ConstantPtr>::iterator it = mConstants.begin(); it!=mConstants.end(); ++it)
		{
      const CHAR* name = it->first.c_str();
			ConstantPtr& constant = it->second;
      switch(constant->mType)
      {
      case Constant::CT_INT:
        RISetShaderParameter1iv(mShaderPtr->mHandle, name, constant->mCount, (I32*)constant->mData);
        break;
      case Constant::CT_INT_ARRAY:
        RISetShaderParameter1iv(mShaderPtr->mHandle, name, constant->mCount, (I32*)constant->mData);
        break;
      case Constant::CT_FLOAT:
        RISetShaderParameter1fv(mShaderPtr->mHandle, name, constant->mCount, (F32*)constant->mData);
        break;
      case Constant::CT_FLOAT_ARRAY:
        RISetShaderParameter1fv(mShaderPtr->mHandle, name, constant->mCount, (F32*)constant->mData);
        break;
      case Constant::CT_VECTOR:
        RISetShaderParameter4fv(mShaderPtr->mHandle, name, constant->mCount, (F32*)constant->mData);
        break;
      case Constant::CT_VECTOR_ARRAY:
        RISetShaderParameter4fv(mShaderPtr->mHandle, name, constant->mCount, (F32*)constant->mData);
        break;
      case Constant::CT_MATRIX:
        RISetShaderParameterMatrix4fv(mShaderPtr->mHandle, name, constant->mCount, (F32*)constant->mData);
        break;
      case Constant::CT_MATRIX_ARRAY:
        RISetShaderParameterMatrix4fv(mShaderPtr->mHandle, name, constant->mCount, (F32*)constant->mData);
        break;
      case Constant::CT_TEXTURE:
        RISetShaderParameterSampler(mShaderPtr->mHandle, name, constant->mTexturePtr->mHandle);
        break;
      default:
        GAssert(VA("The constant type is error : %s", it->first.c_str()));
        break;
      }
		}

    // bind the shader
    RIBindShader(mShaderPtr->mHandle, mPassName.c_str());

		// draw the elements
    RIDrawPrimitive(mType, count, type);
  }

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
