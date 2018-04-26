/*
 *  Constant.cpp
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
	
	Constant::Constant() 
	{
		mType = CT_NONE;		
		mData = NULL;
		mCount = 0;
	}

	Constant::Constant(I32 n)
	{
		mType = CT_INT;		
		mData = GNEW(U8[sizeof(I32)]); CHECK(mData);
		mCount = 1;
		*((I32*)mData) = n;
	}

	Constant::Constant(const I32* pn, U32 count)
	{
		CHECK(pn);
		mType = CT_INT_ARRAY;
		mData = GNEW(U8[count*sizeof(I32)]); CHECK(mData);
		mCount = count;
		::memcpy(mData,pn,count*sizeof(I32));
	}

	Constant::Constant(F32 f)
	{
		mType = CT_FLOAT;
		mData = GNEW(U8[sizeof(F32)]); CHECK(mData);
		mCount = 1;
		*((F32*)mData) = f;
	}

	Constant::Constant(const F32* pf, U32 count)
	{
		CHECK(pf);
		mType = CT_FLOAT_ARRAY;
		mData = GNEW(U8[count*sizeof(F32)]); CHECK(mData);
		mCount = count;
		::memcpy(mData,pf,count*sizeof(F32));
	}

	Constant::Constant(const Vector4& vec)
	{
		mType = CT_VECTOR;
		mData = GNEW(U8[sizeof(Vector4)]); CHECK(mData);
		mCount = 1;
		*((Vector4*)mData) = vec;
	}

	Constant::Constant(const Vector4* pvec, U32 count)
	{
		CHECK(pvec);
		mType = CT_VECTOR_ARRAY;
		mData = GNEW(U8[count*sizeof(Vector4)]); CHECK(mData);
		mCount = count;
		::memcpy(mData,pvec,count*sizeof(Vector4));
	}

	Constant::Constant(const Matrix& mat)
	{
		mType = CT_MATRIX;
		mData = GNEW(U8[sizeof(Matrix)]); CHECK(mData);
		mCount = 1;
		*((Matrix*)mData) = mat;
	}

	Constant::Constant(const Matrix* pmat, U32 count)
	{
		CHECK(pmat);
		mType = CT_MATRIX_ARRAY;
		mData = GNEW(U8[count*sizeof(Matrix)]); CHECK(mData);
		mCount = count;
		::memcpy(mData,pmat,count*sizeof(Matrix));
	}

	Constant::Constant(const TextureData* texture)
	{
		CHECK(texture);
		mType = CT_TEXTURE;
		mTexturePtr = (TextureData*)texture;
		mData = NULL;
		mCount = 0;
	}

	Constant::~Constant() 
	{
		if(mData){ GDELETE([]mData), mData = NULL; }
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	SetInt
	====================
	*/
	VOID Constant::SetInt(I32 n)
	{
		if(mType == CT_NONE) mType = CT_INT;
		CHECK(mType == CT_INT);
		if(mCount != 1)
		{
			if(mData){ GDELETE([]mData), mData = NULL; }
			mData = GNEW(U8[sizeof(I32)]); CHECK(mData);
			mCount = 1;
		}		
		*((I32*)mData) = n;
	}

	/*
	====================
	GetInt
	====================
	*/
	I32 Constant::GetInt() const
	{
		CHECK(mType == CT_INT);
		return *((I32*)mData);
	}

	/*
	====================
	SetIntArray
	====================
	*/
	VOID Constant::SetIntArray( const I32* pn, U32 count )
	{
		CHECK(pn);
		if(mType == CT_NONE) mType = CT_INT_ARRAY;
		CHECK(mType == CT_INT_ARRAY);
		if(mCount != count)
		{
			if(mData){ GDELETE([]mData), mData = NULL; }
			mData = GNEW(U8[count*sizeof(I32)]); CHECK(mData);
			mCount = count;
		}
		::memcpy(mData,pn,count*sizeof(I32));
	}

	/*
	====================
	GetIntArray
	====================
	*/
	const I32* Constant::GetIntArray( U32& count ) const
	{
		CHECK(mType == CT_INT_ARRAY);
		count = mCount;
		return (I32*)mData;
	}

	/*
	====================
	SetFloat
	====================
	*/
	VOID Constant::SetFloat( F32 f )
	{
		if(mType == CT_NONE) mType = CT_FLOAT;
		CHECK(mType == CT_FLOAT);
		if(mCount != 1)
		{
			if(mData){ GDELETE([]mData), mData = NULL; }
			mData = GNEW(U8[sizeof(F32)]); CHECK(mData);
			mCount = 1;
		}
		*((F32*)mData) = f;
	}

	/*
	====================
	GetFloat
	====================
	*/
	F32 Constant::GetFloat() const
	{
		CHECK(mType == CT_FLOAT);
		return *((F32*)mData);
	}

	/*
	====================
	SetFloatArray
	====================
	*/
	VOID Constant::SetFloatArray( const F32* pf, U32 count )
	{
		CHECK(pf);
		if(mType == CT_NONE) mType = CT_FLOAT_ARRAY;
		CHECK(mType == CT_FLOAT_ARRAY);
		if(mCount != count)
		{
			if(mData){ GDELETE([]mData), mData = NULL; }
			mData = GNEW(U8[count*sizeof(F32)]); CHECK(mData);
			mCount = count;
		}
		::memcpy(mData,pf,count*sizeof(F32));
	}

	/*
	====================
	GetFloatArray
	====================
	*/
	const F32* Constant::GetFloatArray(U32& count) const
	{
		CHECK(mType == CT_FLOAT_ARRAY);
		count = mCount;
		return (F32*)mData;
	}

	/*
	====================
	SetVector
	====================
	*/
	VOID Constant::SetVector(const Vector4& vec)
	{
		if(mType == CT_NONE) mType = CT_VECTOR;
		CHECK(mType == CT_VECTOR);
		if(mCount != 1)
		{
			if(mData){ GDELETE([]mData), mData = NULL; }
			mData = GNEW(U8[sizeof(Vector4)]); CHECK(mData);
			mCount = 1;
		}
		*((Vector4*)mData) = vec;
	}

	/*
	====================
	GetVector
	====================
	*/
	const Vector4& Constant::GetVector() const
	{
		CHECK(mType == CT_VECTOR);
		return *((Vector4*)mData);
	}

	/*
	====================
	SetVectorArray
	====================
	*/
	VOID Constant::SetVectorArray(const Vector4* pvec, U32 count)
	{
		CHECK(pvec);
		if(mType == CT_NONE) mType = CT_VECTOR_ARRAY;
		CHECK(mType == CT_VECTOR_ARRAY);
		if(mCount != count)
		{
			if(mData){ GDELETE([]mData), mData = NULL; }
			mData = GNEW(U8[count*sizeof(Vector4)]); CHECK(mData);
			mCount = count;
		}
		::memcpy(mData,pvec,count*sizeof(Vector4));
	}

	/*
	====================
	GetVectorArray
	====================
	*/
	const Vector4* Constant::GetVectorArray(U32& count) const
	{
		CHECK(mType == CT_VECTOR_ARRAY);
		count = mCount;
		return (Vector4*)mData;
	}

	/*
	====================
	SetMatrix
	====================
	*/
	VOID Constant::SetMatrix(const Matrix& mat)
	{
		if(mType == CT_NONE) mType = CT_MATRIX;
		CHECK(mType == CT_MATRIX);
		if(mCount != 1)
		{
			if(mData){ GDELETE([]mData), mData = NULL; }
			mData = GNEW(U8[sizeof(Matrix)]); CHECK(mData);
			mCount = 1;
		}
		*((Matrix*)mData) = mat;
	}

	/*
	====================
	GetMatrix
	====================
	*/
	const Matrix& Constant::GetMatrix() const
	{
		CHECK(mType == CT_MATRIX);
		return *((Matrix*)mData);
	}

	/*
	====================
	SetMatrixArray
	====================
	*/
	VOID Constant::SetMatrixArray(const Matrix* pmat, U32 count)
	{
		CHECK(pmat);
		if(mType == CT_NONE) mType = CT_MATRIX_ARRAY;
		CHECK(mType == CT_MATRIX_ARRAY);
		if(mCount != count)
		{
			if(mData){ GDELETE([]mData), mData = NULL; }
			mData = GNEW(U8[count*sizeof(Matrix)]); CHECK(mData);
			mCount = count;
		}
		::memcpy(mData,pmat,count*sizeof(Matrix));
	}

	/*
	====================
	GetMatrixArray
	====================
	*/
	const Matrix* Constant::GetMatrixArray(U32& count) const
	{
		CHECK(mType == CT_MATRIX_ARRAY);
		count = mCount;
		return (Matrix*)mData;
	}
	
	/*
	====================
	SetTexture
	====================
	*/
	VOID Constant::SetTexture(const TextureData* texture)
	{
		CHECK(texture);
		if(mType == CT_NONE) mType = CT_TEXTURE;
		CHECK(mType == CT_TEXTURE);
		mTexturePtr = (TextureData*)texture;
	}

	/*
	====================
	GetTexture
	====================
	*/
	const TextureData* Constant::GetTexture() const
	{
		CHECK( mType == CT_TEXTURE );
		return mTexturePtr.Ptr();
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
