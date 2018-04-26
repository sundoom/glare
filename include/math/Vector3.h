/*
 *  Vector3.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Vector3__
#define __Vector3__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////

	class Vector3
	{
    public:
		// default constructor leaves data uninitialized.
        Vector3();

		// assigns scalar parameter to all elements. useful to initialize to zero or one.
		explicit Vector3(F32);

		// initializes from 3 scalar parameters.
        Vector3(F32 x,F32 y,F32 z);

		// initializes from 2 scalar parameters.
        Vector3(const Vector2& v2,F32 zz);

		// initializes from an array of scalar parameters.
		Vector3(const F32 v[]);
		
		// copy constructor.
		Vector3(const Vector3& rhs);

		const Vector3& operator=(const Vector3&);

		// returns TRUE if this and arg's elems are within epsilon of each other.
		BOOL equals(const Vector3 &, F32 epsilon) const;

		// returns TRUE if the two vectors are exactly equal.
        BOOL operator == (const Vector3& v) const;
        
		// returns FALSE if the two vectors are exactly equal.
        BOOL operator != (const Vector3& v) const;

		// TRUE if all the members are smaller.
        BOOL operator <  (const Vector3& v) const;

		// access the data as an array.
        F32* ptr();
        const F32* ptr() const;

		// set methods
        VOID set( F32 x, F32 y, F32 z);
        VOID set( const Vector3& rhs );

        F32& operator [] (U32 i);
        F32 operator [] (U32 i) const;

        F32& x();
        F32& y();
        F32& z();

        F32 x() const;
        F32 y() const;
        F32 z() const;

        // dot product.
        F32 operator * (const Vector3& rhs) const;

        // cross product.
        const Vector3 operator ^ (const Vector3& rhs) const;

        // multiply by scalar.
        const Vector3 operator * (F32 rhs) const;

        // unary multiply by scalar.
        Vector3& operator *= (F32 rhs);

        // divide by scalar.
        const Vector3 operator / (F32 rhs) const;

        // unary divide by scalar.
        Vector3& operator /= (F32 rhs);

        // binary vector add.
        const Vector3 operator + (const Vector3& rhs) const;

        // unary vector add. slightly more efficient because no temporary intermediate object.
        Vector3& operator += (const Vector3& rhs);

        // binary vector subtract.
        const Vector3 operator - (const Vector3& rhs) const;

        // unary vector subtract.
        Vector3& operator -= (const Vector3& rhs);

        // negation operator. returns the negative of the vector.
        const Vector3 operator - () const;

        // length of the vector = sqrt( vec . vec )
        F32 length() const;

        // length squared of the vector = vec . vec
        F32 length2() const;

        // normalize the vector so that it has length unity. returns the previous length of the vector.
        F32 normalize();

	protected: F32 _v[3];
			 friend class Matrix;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////