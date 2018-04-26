/*
 *  Quaternion.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Quaternion__
#define __Quaternion__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////

	class Quaternion
	{
    public:
		// default constructor, does not do any initialization.
        Quaternion();

        Quaternion( F32 x, F32 y, F32 z, F32 w );

		// Copy constructor.
		Quaternion(const Quaternion&);

		// copies xyz elements from vector, and scalar from w (defaults to 0).
        Quaternion( const Vector4& v );

		// creates from angle-axis representation.
        Quaternion( F32 angle, const Vector3& axis);

        Quaternion( F32 angle1, const Vector3& axis1, 
                      F32 angle2, const Vector3& axis2,
                      F32 angle3, const Vector3& axis3);

        Quaternion& operator = (const Quaternion& v);

        BOOL operator == (const Quaternion& v) const;

        BOOL operator != (const Quaternion& v) const;

		// methods to access data members
        VOID set(F32 x, F32 y, F32 z, F32 w);
        
        VOID set(Vector4& v);
        
        VOID set(const class Matrix& m);        
        VOID get(class Matrix& m) const;
		
        F32& operator [] (U32 i);
        F32  operator [] (U32 i) const;

        F32 & x();
        F32 & y();
        F32 & z();
        F32 & w();

        F32 x() const;
        F32 y() const;
        F32 z() const;
        F32 w() const;

        // return TRUE if the Quaternion represents a zero rotation, and therefore can be ignored in computations.
        VOID identity();
		BOOL isIdentity() const;
        
        // Multiply by scalar 
        const Quaternion operator * (F32 rhs) const;

        // Unary multiply by scalar 
        Quaternion& operator *= (F32 rhs);

        // Binary multiply 
        const Quaternion operator*(const Quaternion& rhs) const;

        // Unary multiply 
        Quaternion& operator*=(const Quaternion& rhs);

        // Divide by scalar 
        Quaternion operator / (F32 rhs) const;

        // Unary divide by scalar 
        Quaternion& operator /= (F32 rhs);

        // Binary divide 
        const Quaternion operator/(const Quaternion& denom) const;

        // Unary divide 
        Quaternion& operator/=(const Quaternion& denom);

        // Binary addition 
        const Quaternion operator + (const Quaternion& rhs) const;

        // Unary addition
        Quaternion& operator += (const Quaternion& rhs);

        // Binary subtraction 
        const Quaternion operator - (const Quaternion& rhs) const;

        // Unary subtraction 
        Quaternion& operator -= (const Quaternion& rhs);

        // Negation operator - returns the negative of the quaternion.
        // Basically just calls operator - () on the Vec4
        const Quaternion operator - () const;

        /// Length of the quaternion = sqrt( vec . vec )
        F32 length() const;

        // Length of the quaternion = vec . vec
        F32 length2() const;

        // conjugate 
        Quaternion conjugate() const;

        /// multiplicative inverse method: q^(-1) = q^*/(q.q^*)
        Quaternion inverse() const;

		// Set a quaternion which will perform a rotation of an
        // angle around the axis given by the vector (x,y,z).
        // Should be written to also accept an angle and a Vec3?
        // Define Spherical Linear interpolation method also
        // Not inlined - see the Quaternion.cpp file for implementation
        VOID setRotate( F32 angle, F32 x, F32 y, F32 z );
        VOID setRotate( F32 angle, const Vector3& vec );
        VOID setRotate(	F32 angle1, const Vector3& axis1, 
								F32 angle2, const Vector3& axis2,
								F32 angle3, const Vector3& axis3);        

        // Make a rotation Quaternion which will rotate vec1 to vec2.
        // Generally take a dot product to get the angle between these
        // and then use a cross product to get the rotation axis
        // Watch out for the two special cases when the vectors
        // are co-incident or opposite in direction.
        VOID setRotate( const Vector3& vec1, const Vector3& vec2 );        

        // return the angle and vector components represented by the quaternion.
        VOID getRotate( F32 & angle, F32 & x, F32 & y, F32 & z ) const;

        // return the angle and vector represented by the quaternion.
        VOID getRotate( F32 & angle, Vector3& vec ) const;        

        // spherical linear interpolation. 
		// as t goes from 0 to 1, the gquat object goes from "from" to "to".
        VOID slerp( F32 t, const Quaternion& from, const Quaternion& to);
               
        // rotate a vector by this quaternion.
        Vector3 operator* (const Vector3& v) const;

    protected: F32 _v[4];
			 friend class Matrix;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////