/*
 *  Vector2.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Vector2__
#define __Vector2__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////

	class Vector2
	{
    public:		
		// default constructor leaves data uninitialized.
        Vector2();

		// assigns scalar parameter to all elements. useful to initialize to zero or one.
		explicit Vector2(F32);

		// initializes from 2 scalar parameters.
        Vector2(F32 x,F32 y);

		// initializes from an array of scalar parameters.
		Vector2(const F32 v[]);

		// copy constructor.
		Vector2(const Vector2& rhs);

		const Vector2& operator=(const Vector2&);

		// returns TRUE if this and arg's elems are within epsilon of each other.
		BOOL equals(const Vector2 &, F32 epsilon) const;
		
		// returns TRUE if the two vectors are exactly equal.
        BOOL operator == (const Vector2& v) const;

		// returns FALSE if the two vectors are exactly equal.
        BOOL operator != (const Vector2& v) const;
		
		// TRUE if all the members are smaller.
        BOOL operator <  (const Vector2& v) const;
		
		// access the data as an array.
        F32 * ptr();
        const F32 * ptr() const;
		
		// set methods
        VOID set( F32 x, F32 y );
		VOID set( const Vector2& rhs);
		
        F32 & operator [] (U32 i);
        F32 operator [] (U32 i) const;
		
        F32 & x();
        F32 & y();
		
        F32 x() const;
        F32 y() const;        
		
        // dot product.
        F32 operator * (const Vector2& rhs) const;
		
        // multiply by scalar.
        const Vector2 operator * (F32 rhs) const;
		
        // unary multiply by scalar.
        Vector2& operator *= (F32 rhs);
		
        // divide by scalar.
        const Vector2 operator / (F32 rhs) const;
		
        // unary divide by scalar.
        Vector2& operator /= (F32 rhs);
		
        // binary vector add.
        const Vector2 operator + (const Vector2& rhs) const;
		
        // unary vector add. slightly more efficient because no temporary intermediate object.
        Vector2& operator += (const Vector2& rhs);
		
        // binary vector subtract.
        const Vector2 operator - (const Vector2& rhs) const;
		
        // unary vector subtract.
        Vector2& operator -= (const Vector2& rhs);
		
        // negation operator. returns the negative of the vector.
        const Vector2 operator - () const;
		
        // length of the vector = sqrt( vec . vec )
        F32 length() const;
		
        // length squared of the vector = vec . vec
        F32 length2() const;
		
        // normalize the vector so that it has length unity. * returns the previous length of the vector.
        F32 normalize();

	protected: F32 _v[2];		
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////