/*
 *  Vector4.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Vector4__
#define __Vector4__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////

	class Vector4
	{
    public:
		// default constructor leaves data uninitialized.
        Vector4();

		// assigns scalar parameter to all elements. useful to initialize to zero or one.
		explicit Vector4(F32);
        
		// initializes from 4 scalar parameters.
        Vector4(F32 x, F32 y, F32 z, F32 w);
		
		// initializes from 2 scalar parameters.
        Vector4(const Vector3& v3,F32 w);

		// initializes from an array of scalar parameters.
		Vector4(const F32 v[]);
		
		// copy constructor.
		Vector4(const Vector4& rhs);

		const Vector4& operator=(const Vector4&);

		// returns TRUE if this and arg's elems are within epsilon of each other.
		BOOL equals(const Vector4 &, F32 epsilon) const;
		
		// returns TRUE if the two vectors are exactly equal.
        BOOL operator == (const Vector4& v) const;
		
		// returns FALSE if the two vectors are exactly equal.
        BOOL operator != (const Vector4& v) const;
		
		// TRUE if all the members are smaller.
        BOOL operator <  (const Vector4& v) const;
		
		// access the data as an array.
        F32* ptr();
        const F32* ptr() const;
		
		// set methods
        VOID set( F32 x, F32 y, F32 z, F32 w);
		VOID set( const Vector4& rhs);
		
        F32& operator [] (U32 i);
        F32  operator [] (U32 i) const;
		
        F32& x();
        F32& y();
        F32& z();
        F32& w();
		
        F32 x() const;
        F32 y() const;
        F32 z() const;
        F32 w() const;
		
        F32& r();
        F32& g();
        F32& b();
        F32& a();
		
        F32 r() const;
        F32 g() const;
        F32 b() const;
        F32 a() const;
		
        U32 abgr() const;
        U32 rgba() const;
		
        // dot product.
        F32 operator * (const Vector4& rhs) const;
		
        // multiply by scalar.
        Vector4 operator * (F32 rhs) const;
		
        // unary multiply by scalar.
        Vector4& operator *= (F32 rhs);
		
        // divide by scalar.
        Vector4 operator / (F32 rhs) const;
		
        // unary divide by scalar.
        Vector4& operator /= (F32 rhs);
		
        // binary vector add.
        Vector4 operator + (const Vector4& rhs) const;
		
        // unary vector add. slightly more efficient because no temporary intermediate object.
        Vector4& operator += (const Vector4& rhs);
		
        // binary vector subtract.
        Vector4 operator - (const Vector4& rhs) const;
		
        // unary vector subtract.
        Vector4& operator -= (const Vector4& rhs);
		
        // negation operator. returns the negative of the vector.
        const Vector4 operator - () const;
		
        // length of the vector = sqrt( vec . vec )
        F32 length() const;
		
        // length squared of the vector = vec . vec
        F32 length2() const;
		
        // normalize the vector so that it has length unity. returns the previous length of the vector.
        F32 normalize();

	protected: F32 _v[4];
			 friend class Matrix;
	};

	//////////////////////////////////////////////////////////////////////////

	// compute the dot product of a (vec3,1.0) and a vec4.
	F32 operator * (const Vector3& lhs,const Vector4& rhs);

	// compute the dot product of a vec4 and a (vec3,1.0).
	F32 operator * (const Vector4& lhs,const Vector3& rhs);

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////