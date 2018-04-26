/*
 *  Constant.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Constant__
#define __Constant__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented constant interface.
	class Constant : public GReference
	{
	public:
		// the constant type
		enum TYPE
		{
			CT_NONE,
			CT_INT,
			CT_INT_ARRAY,
			CT_FLOAT,
			CT_FLOAT_ARRAY,
			CT_VECTOR,
			CT_VECTOR_ARRAY,
			CT_MATRIX,
			CT_MATRIX_ARRAY,
			CT_TEXTURE,
		};

	public:		
		// constructor
		Constant();
		Constant(I32 n);
		Constant(const I32* pn, U32 count);
		Constant(F32 f);
		Constant(const F32* pf, U32 count);
		Constant(const Vector4& vec);
		Constant(const Vector4* pvec, U32 count);
		Constant(const Matrix& mat);
		Constant(const Matrix* pmat, U32 count);
		Constant(const TextureData* texture);
		
		// destructor
		virtual ~Constant();		

		// set/get the constant value		
		VOID SetInt(I32 n);
		I32 GetInt() const;
		VOID SetIntArray(const I32* pn, U32 count);
		const I32* GetIntArray(U32& count) const;
		VOID SetFloat(F32 f);
		F32 GetFloat() const;
		VOID SetFloatArray(const F32* pf, U32 count);
		const F32* GetFloatArray(U32& count) const;
		VOID SetVector(const Vector4& vec);
		const Vector4& GetVector() const;
		VOID SetVectorArray(const Vector4* pvec, U32 count);
		const Vector4* GetVectorArray(U32& count) const;
		VOID SetMatrix(const Matrix& mat);
		const Matrix& GetMatrix() const;
		VOID SetMatrixArray(const Matrix* pmat, U32 count);
		const Matrix* GetMatrixArray(U32& count) const;
		VOID SetTexture(const TextureData* texture);
		const TextureData* GetTexture() const;

	protected:
		U32 mType; U32 mCount; U8 *mData;
    TextureDataPtr mTexturePtr;		

	private: friend class Primitive;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<Constant>ConstantPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Constant__

//////////////////////////////////////////////////////////////////////////
