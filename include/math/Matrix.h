/*
 *  Matrix.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Matrix__
#define __Matrix__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////

	class Matrix
	{
    public:
        Matrix();

        Matrix( const Matrix& mat);

        explicit Matrix( F32 const * const ptr );

        explicit Matrix( const class Quaternion& quat );

        Matrix( F32 a00, F32 a01, F32 a02, F32 a03,
					   F32 a10, F32 a11, F32 a12, F32 a13,
					   F32 a20, F32 a21, F32 a22, F32 a23,
					   F32 a30, F32 a31, F32 a32, F32 a33);
        
        BOOL operator == (const Matrix& m) const;

        BOOL operator != (const Matrix& m) const;

        F32& operator()(I32 row, I32 col);

        F32 operator()(I32 row, I32 col) const;

        Matrix& operator = (const Matrix& rhs);

        VOID set(const Matrix& rhs);
        VOID set(F32 const * const ptr);
        VOID set(F32 a00, F32 a01, F32 a02,F32 a03,
						F32 a10, F32 a11, F32 a12,F32 a13,
						F32 a20, F32 a21, F32 a22,F32 a23,
						F32 a30, F32 a31, F32 a32,F32 a33);
                  
        F32 * ptr();
        const F32 * ptr() const;

		// identity
        BOOL isIdentity() const;
        VOID identity();

		// scale
		VOID setScale( const Vector3& );
        VOID setScale( F32, F32, F32 );
		Vector3 getScale() const;
        
		// translate 
        VOID setTranslate( const Vector3& );
        VOID setTranslate( F32, F32, F32 );
		Vector3 getTranslate() const;

        // rotate 
        VOID setRotate( const Vector3& from, const Vector3& to );
        VOID setRotate( F32 angle, const Vector3& axis );
        VOID setRotate( F32 angle, F32 x, F32 y, F32 z );        
        VOID setRotate( F32 angle1, const Vector3& axis1, F32 angle2, const Vector3& axis2, F32 angle3, const Vector3& axis3 );
		VOID setRotate( const Quaternion& );
		Quaternion getRotate() const;

		// basic matrix multiplication, our workhorse methods.
        VOID mult( const Matrix&, const Matrix& );
        VOID preMult( const Matrix& );
        VOID postMult( const Matrix& );	
		
        // Optimized version of preMult(translate(v));
        VOID preMultTranslate( const Vector3& v );
        // Optimized version of postMult(translate(v));
        VOID postMultTranslate( const Vector3& v );
		
        // Optimized version of preMult(scale(v));
        VOID preMultScale( const Vector3& v );
        // Optimized version of postMult(scale(v));
        VOID postMultScale( const Vector3& v );
		
        // Optimized version of preMult(rotate(q));
        VOID preMultRotate( const Quaternion& q );
        // Optimized version of postMult(rotate(q));
        VOID postMultRotate( const Quaternion& q );
		
		VOID operator *= ( const Matrix& other );
		
        Matrix operator * ( const Matrix &m ) const;

		// invert the matrix rhs
        BOOL invert( const Matrix& rhs );

		// transpose the matrix rhs
		VOID transpose( const Matrix& rhs );

		// ret = v * matrix
		Vector3 preMult( const Vector3& v ) const;
		// ret = matrix * v
        Vector3 postMult( const Vector3& v ) const;
		// ret = matrix * v
        Vector3 operator* ( const Vector3& v ) const;
		// ret = v * matrix
        Vector4 preMult( const Vector4& v ) const;
		// ret = matrix * v
        Vector4 postMult( const Vector4& v ) const;
		// ret = matrix * v
        Vector4 operator* ( const Vector4& v ) const;

	public:
        // the scale matrix
        static Matrix makeScale( const Vector3& sv );
        static Matrix makeScale( F32 sx, F32 sy, F32 sz );		

		// the translate matrix
        static Matrix makeTranslate( const Vector3& dv );
        static Matrix makeTranslate( F32 x, F32 y, F32 z );		

		// the rotate matrix
        static Matrix makeRotate( const Vector3& from, const Vector3& to );
        static Matrix makeRotate( F32 angle, F32 x, F32 y, F32 z );
        static Matrix makeRotate( F32 angle, const Vector3& axis );
        static Matrix makeRotate( F32 angle1, const Vector3& axis1, 
										 F32 angle2, const Vector3& axis2,
										 F32 angle3, const Vector3& axis3 );
        static Matrix makeRotate( const Quaternion& quat );

		// make the inverse matrix
        static Matrix makeInverse( const Matrix& matrix );

		// make the transpose matrix
		static Matrix makeTranspose( const Matrix& matrix );

		// make the orthonormal matrix
        static Matrix makeOrthonormal( const Matrix& matrix );
        
        // create an orthographic projection matrix. 
        static Matrix makeOrtho( F32 left, F32 right, F32 bottom, F32 top, F32 zNear, F32 zFar );

		// get the orthographic settings of the orthographic projection matrix. 		
        static BOOL getOrtho(const Matrix& matrix, F32& left, F32& right, F32& bottom, F32& top, F32& zNear, F32& zFar);

        // create a perspective projection. see glfrustum for further details.
        static Matrix makeFrustum( F32 left, F32 right, F32 bottom, F32 top, F32 zNear,  F32 zFar );

		// get the frustum settings of a perspective projection matrix.        
        static BOOL getFrustum( const Matrix& matrix, F32& left, F32& right, F32& bottom, F32& top, F32& zNear,  F32& zFar );

        // create a symmetrical perspective projection.
        static Matrix makePerspective( F32 fovy, F32 aspectRatio, F32 zNear, F32 zFar );

		// Get the frustum settings of a symmetric perspective projection
        //	matrix.
        //	Return FALSE if matrix is not a perspective matrix,
        //	where parameter values are undefined. 
		//	Note, if matrix is not a symmetric perspective matrix then the
		//	shear will be lost.
		//	Asymmetric matrices occur when stereo, power walls, caves and
		//	reality center display are used.
		//	In these configuration one should use the AsFrustum method instead.
        static BOOL getPerspective( const Matrix& matrix, F32& fovy,  F32& aspectRatio, F32& zNear, F32& zFar );

        //	Create the position and orientation as per a camera
        static Matrix makeLookAt( const Vector3& eye, const Vector3& center, const Vector3& up );

		//	Get to the position and orientation of a modelview matrix
        static VOID getLookAt( const Matrix& matrix, Vector3& eye,Vector3& center,Vector3& up, F32 lookDistance=1.0f );
        
        // apply a 3x3 transform of v*M[0..2,0..2].
        static Vector3 transform3x3( const Vector3& v,const Matrix& m );

        // apply a 3x3 transform of M[0..2,0..2]*v.
        static Vector3 transform3x3( const Matrix& m,const Vector3& v );

    protected:
        F32 _mat[4][4];
	};

	Vector3 operator* (const Vector3& v, const Matrix& m );
	Vector4 operator* (const Vector4& v, const Matrix& m );

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////