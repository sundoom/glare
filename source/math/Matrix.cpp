/*
 *  Matrix.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <GMath.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////

	#define SET_ROW(row, v1, v2, v3, v4 )    \
		_mat[(row)][0] = (v1); \
		_mat[(row)][1] = (v2); \
		_mat[(row)][2] = (v3); \
		_mat[(row)][3] = (v4);

	#define INNER_PRODUCT(a,b,r,c) \
		((a)._mat[r][0] * (b)._mat[0][c]) + \
		((a)._mat[r][1] * (b)._mat[1][c]) + \
		((a)._mat[r][2] * (b)._mat[2][c]) + \
		((a)._mat[r][3] * (b)._mat[3][c])

	//////////////////////////////////////////////////////////////////////////

	Matrix::Matrix() 
	{ 
		identity();
	}

	Matrix::Matrix( const Matrix& mat) 
	{ 
		set(mat.ptr()); 
	}

	Matrix::Matrix( F32 const * const ptr ) 
	{ 
		set(ptr); 
	}

	Matrix::Matrix( const Quaternion& quat ) 
	{
		setRotate(quat);
	}

	Matrix::Matrix( F32 a00, F32 a01, F32 a02, F32 a03,
				  F32 a10, F32 a11, F32 a12, F32 a13,
				  F32 a20, F32 a21, F32 a22, F32 a23,
				  F32 a30, F32 a31, F32 a32, F32 a33)
	{
		SET_ROW( 0, a00, a01, a02, a03 )
		SET_ROW( 1, a10, a11, a12, a13 )
		SET_ROW( 2, a20, a21, a22, a23 )
		SET_ROW( 3, a30, a31, a32, a33 )
	}

	BOOL Matrix::operator == (const Matrix& m) const 
	{ 
		return memcmp( _mat, m._mat, sizeof(_mat) )==0 ? TRUE : FALSE;
	}

	BOOL Matrix::operator != (const Matrix& m) const 
	{ 
		return memcmp( _mat, m._mat, sizeof(_mat) )!=0 ? TRUE : FALSE;
	}

	F32& Matrix::operator()(I32 row, I32 col) 
	{ 
		return _mat[row][col]; 
	}

	F32 Matrix::operator()(I32 row, I32 col) const 
	{ 
		return _mat[row][col]; 
	}

	Matrix& Matrix::operator = (const Matrix& rhs)
	{
		if( &rhs == this ) return *this;
		set(rhs.ptr());
		return *this;
	}

	VOID Matrix::set(const Matrix& rhs) 
	{ 
		set(rhs.ptr()); 
	}

	VOID Matrix::set(F32 const * const ptr)
	{
		memcpy(_mat, ptr, sizeof(_mat));
	}       

	VOID Matrix::set(F32 a00, F32 a01, F32 a02,F32 a03,
					F32 a10, F32 a11, F32 a12,F32 a13,
					F32 a20, F32 a21, F32 a22,F32 a23,
					F32 a30, F32 a31, F32 a32,F32 a33)
	{
		SET_ROW( 0, a00, a01, a02, a03 )
		SET_ROW( 1, a10, a11, a12, a13 )
		SET_ROW( 2, a20, a21, a22, a23 )
		SET_ROW( 3, a30, a31, a32, a33 )
	}

	F32 * Matrix::ptr() 
	{ 
		return (F32*)_mat; 
	}

	const F32 * Matrix::ptr() const 
	{ 
		return (const F32 *)_mat; 
	}

	BOOL Matrix::isIdentity() const
	{
		return(	_mat[0][0]==1.0f && _mat[0][1]==0.0f && _mat[0][2]==0.0f &&  _mat[0][3]==0.0f &&
				_mat[1][0]==0.0f && _mat[1][1]==1.0f && _mat[1][2]==0.0f &&  _mat[1][3]==0.0f &&
				_mat[2][0]==0.0f && _mat[2][1]==0.0f && _mat[2][2]==1.0f &&  _mat[2][3]==0.0f &&
				_mat[3][0]==0.0f && _mat[3][1]==0.0f && _mat[3][2]==0.0f &&  _mat[3][3]==1.0f ) ? TRUE : FALSE;
	}

	VOID Matrix::identity()
	{
		SET_ROW(0,	1, 0, 0, 0 )
		SET_ROW(1,	0, 1, 0, 0 )
		SET_ROW(2,	0, 0, 1, 0 )
		SET_ROW(3,	0, 0, 0, 1 )
	}

	VOID Matrix::setScale( const Vector3& v )
	{
		setScale(v[0], v[1], v[2] );
	}

	VOID Matrix::setScale( F32 x, F32 y, F32 z )
	{
		SET_ROW(0,    x, 0, 0, 0 )
		SET_ROW(1,    0, y, 0, 0 )
		SET_ROW(2,    0, 0, z, 0 )
		SET_ROW(3,    0, 0, 0, 1 )
	}

	Vector3 Matrix::getScale() const
	{
		Vector3 x_vec(_mat[0][0],_mat[1][0],_mat[2][0]); 
		Vector3 y_vec(_mat[0][1],_mat[1][1],_mat[2][1]); 
		Vector3 z_vec(_mat[0][2],_mat[1][2],_mat[2][2]); 
		return Vector3(x_vec.length(), y_vec.length(), z_vec.length()); 
	}
	
	VOID Matrix::setTranslate( const Vector3& v )
	{
		setTranslate( v[0], v[1], v[2] );
	}

	VOID Matrix::setTranslate( F32 x, F32 y, F32 z )
	{
		SET_ROW(0,    1, 0, 0, 0 )
		SET_ROW(1,    0, 1, 0, 0 )
		SET_ROW(2,    0, 0, 1, 0 )
		SET_ROW(3,    x, y, z, 1 )
	}

	Vector3 Matrix::getTranslate() const
	{
		return Vector3(_mat[3][0],_mat[3][1],_mat[3][2]);
	}
	
	VOID Matrix::setRotate( const Vector3& from, const Vector3& to )
	{
		identity();
		
		Quaternion quat;
		quat.setRotate(from,to);
		setRotate(quat);
	}

	VOID Matrix::setRotate( F32 angle, const Vector3& axis )
	{
		identity();
		
		Quaternion quat;
		quat.setRotate( angle, axis);
		setRotate(quat);
	}

	VOID Matrix::setRotate( F32 angle, F32 x, F32 y, F32 z )
	{
		identity();
		
		Quaternion quat;
		quat.setRotate( angle, x, y, z);
		setRotate(quat);
	}

	VOID Matrix::setRotate( F32 angle1, const Vector3& axis1, F32 angle2, const Vector3& axis2, F32 angle3, const Vector3& axis3 )
	{
		identity();
		
		Quaternion quat;
		quat.setRotate( angle1, axis1, angle2, axis2, angle3, axis3 );
		setRotate(quat);
	}

	VOID Matrix::setRotate( const Quaternion& q )
	{
		identity();
		
		F32 length2 = q.length2();
		if( ::fabs(length2) <= EPS_F32 )
		{
			_mat[0][0] = 0.0; _mat[1][0] = 0.0; _mat[2][0] = 0.0;
			_mat[0][1] = 0.0; _mat[1][1] = 0.0; _mat[2][1] = 0.0;
			_mat[0][2] = 0.0; _mat[1][2] = 0.0; _mat[2][2] = 0.0;
		}
		else
		{
			F32 rlength2;
			// normalize quat if required.
			// We can avoid the expensive sqrt in this case since all 'coefficients' below are products of two q components.
			// That is a square of a square root, so it is possible to avoid that
			if( length2 != 1.0f )
			{
				rlength2 = 2.0f/length2;
			}
			else
			{
				rlength2 = 2.0f;
			}
			
			// Source: Gamasutra, Rotating Objects Using Quaternions
			//
			//http://www.gamasutra.com/features/19980703/quaternions_01.htm
			
			F32 wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
			
			// calculate coefficients
			x2 = rlength2*q._v[0];
			y2 = rlength2*q._v[1];
			z2 = rlength2*q._v[2];
			
			xx = q._v[0] * x2;
			xy = q._v[0] * y2;
			xz = q._v[0] * z2;
			
			yy = q._v[1] * y2;
			yz = q._v[1] * z2;
			zz = q._v[2] * z2;
			
			wx = q._v[3] * x2;
			wy = q._v[3] * y2;
			wz = q._v[3] * z2;
			
			// Note.  Gamasutra gets the matrix assignments inverted, resulting
			// in left-handed rotations, which is contrary to OpenGL and OSG's 
			// methodology.  The matrix assignment has been altered in the next
			// few lines of code to do the right thing.
			// Don Burns - Oct 13, 2001
			_mat[0][0] = 1.0f - (yy + zz);
			_mat[1][0] = xy - wz;
			_mat[2][0] = xz + wy;
			
			
			_mat[0][1] = xy + wz;
			_mat[1][1] = 1.0f - (xx + zz);
			_mat[2][1] = yz - wx;
			
			_mat[0][2] = xz - wy;
			_mat[1][2] = yz + wx;
			_mat[2][2] = 1.0f - (xx + yy);
		}
	}

	Quaternion Matrix::getRotate() const
	{
		Quaternion q;
		
		F32 s;
		F32 tq[4];
		I32 i, j;
		
		// Use tq to store the largest trace
		tq[0] = 1 + _mat[0][0]+_mat[1][1]+_mat[2][2];
		tq[1] = 1 + _mat[0][0]-_mat[1][1]-_mat[2][2];
		tq[2] = 1 - _mat[0][0]+_mat[1][1]-_mat[2][2];
		tq[3] = 1 - _mat[0][0]-_mat[1][1]+_mat[2][2];
		
		// Find the maximum (could also use stacked if's later)
		j = 0;
		for(i=1;i<4;i++) j = (tq[i]>tq[j]) ? i : j;
		
		// check the diagonal
		if (j==0)
		{
			/* perform instant calculation */
			q._v[3] = tq[0];
			q._v[0] = _mat[1][2]-_mat[2][1]; 
			q._v[1] = _mat[2][0]-_mat[0][2]; 
			q._v[2] = _mat[0][1]-_mat[1][0]; 
		}
		else if (j==1)
		{
			q._v[3] = _mat[1][2]-_mat[2][1]; 
			q._v[0] = tq[1];
			q._v[1] = _mat[0][1]+_mat[1][0]; 
			q._v[2] = _mat[2][0]+_mat[0][2]; 
		}
		else if (j==2)
		{
			q._v[3] = _mat[2][0]-_mat[0][2]; 
			q._v[0] = _mat[0][1]+_mat[1][0]; 
			q._v[1] = tq[2];
			q._v[2] = _mat[1][2]+_mat[2][1]; 
		}
		else /* if (j==3) */
		{
			q._v[3] = _mat[0][1]-_mat[1][0]; 
			q._v[0] = _mat[2][0]+_mat[0][2]; 
			q._v[1] = _mat[1][2]+_mat[2][1]; 
			q._v[2] = tq[3];
		}
		
		s = ::sqrtf(0.25f/tq[j]);
		q._v[3] *= s;
		q._v[0] *= s;
		q._v[1] *= s;
		q._v[2] *= s;
		
		return q;
	}

	VOID Matrix::mult( const Matrix& lhs, const Matrix& rhs )
	{
		if (&lhs==this)
		{
			postMult(rhs);
			return;
		}
		if (&rhs==this)
		{
			preMult(lhs);
			return;
		}	   
		
		// PRECONDITION: We assume neither &lhs nor &rhs == this
		// if it did, use preMult or postMult instead
		_mat[0][0] = INNER_PRODUCT(lhs, rhs, 0, 0);
		_mat[0][1] = INNER_PRODUCT(lhs, rhs, 0, 1);
		_mat[0][2] = INNER_PRODUCT(lhs, rhs, 0, 2);
		_mat[0][3] = INNER_PRODUCT(lhs, rhs, 0, 3);
		_mat[1][0] = INNER_PRODUCT(lhs, rhs, 1, 0);
		_mat[1][1] = INNER_PRODUCT(lhs, rhs, 1, 1);
		_mat[1][2] = INNER_PRODUCT(lhs, rhs, 1, 2);
		_mat[1][3] = INNER_PRODUCT(lhs, rhs, 1, 3);
		_mat[2][0] = INNER_PRODUCT(lhs, rhs, 2, 0);
		_mat[2][1] = INNER_PRODUCT(lhs, rhs, 2, 1);
		_mat[2][2] = INNER_PRODUCT(lhs, rhs, 2, 2);
		_mat[2][3] = INNER_PRODUCT(lhs, rhs, 2, 3);
		_mat[3][0] = INNER_PRODUCT(lhs, rhs, 3, 0);
		_mat[3][1] = INNER_PRODUCT(lhs, rhs, 3, 1);
		_mat[3][2] = INNER_PRODUCT(lhs, rhs, 3, 2);
		_mat[3][3] = INNER_PRODUCT(lhs, rhs, 3, 3);
	}

	VOID Matrix::preMult( const Matrix& other )
	{
		// brute force method requiring a copy
		// Matrix_implementation tmp(other* *this);
		// *this = tmp;
		
		// more efficient method just use a value_type[4] for temporary storage.
		F32 t[4];
		for(I32 col=0; col<4; ++col) 
		{
			t[0] = INNER_PRODUCT( other, *this, 0, col );
			t[1] = INNER_PRODUCT( other, *this, 1, col );
			t[2] = INNER_PRODUCT( other, *this, 2, col );
			t[3] = INNER_PRODUCT( other, *this, 3, col );
			_mat[0][col] = t[0];
			_mat[1][col] = t[1];
			_mat[2][col] = t[2];
			_mat[3][col] = t[3];
		}
	}

	VOID Matrix::postMult( const Matrix& other )
	{
		// brute force method requiring a copy
		//Matrix_implementation tmp(*this * other);
		// *this = tmp;
		
		// more efficient method just use a value_type[4] for temporary storage.
		F32 t[4];
		for(I32 row=0; row<4; ++row)
		{
			t[0] = INNER_PRODUCT( *this, other, row, 0 );
			t[1] = INNER_PRODUCT( *this, other, row, 1 );
			t[2] = INNER_PRODUCT( *this, other, row, 2 );
			t[3] = INNER_PRODUCT( *this, other, row, 3 );
			SET_ROW(row, t[0], t[1], t[2], t[3] )
		}
	}	

	VOID Matrix::preMultTranslate( const Vector3& v )
	{
		for( U32 i = 0; i < 3; ++i )
		{
			F32 tmp = v[i];
			if( tmp == 0 ) continue;
			_mat[3][0] += tmp*_mat[i][0];
			_mat[3][1] += tmp*_mat[i][1];
			_mat[3][2] += tmp*_mat[i][2];
			_mat[3][3] += tmp*_mat[i][3];
		}
	}

	VOID Matrix::postMultTranslate( const Vector3& v )
	{
		for( U32 i = 0; i < 3; ++i )
		{
			F32 tmp = v[i];
			if (tmp == 0) continue;
			_mat[0][i] += tmp*_mat[0][3];
			_mat[1][i] += tmp*_mat[1][3];
			_mat[2][i] += tmp*_mat[2][3];
			_mat[3][i] += tmp*_mat[3][3];
		}
	}

	VOID Matrix::preMultScale( const Vector3& v )
	{
		_mat[0][0] *= v[0]; _mat[0][1] *= v[0]; _mat[0][2] *= v[0]; _mat[0][3] *= v[0];
		_mat[1][0] *= v[1]; _mat[1][1] *= v[1]; _mat[1][2] *= v[1]; _mat[1][3] *= v[1];
		_mat[2][0] *= v[2]; _mat[2][1] *= v[2]; _mat[2][2] *= v[2]; _mat[2][3] *= v[2];
	}

	VOID Matrix::postMultScale( const Vector3& v )
	{
		_mat[0][0] *= v[0]; _mat[1][0] *= v[0]; _mat[2][0] *= v[0]; _mat[3][0] *= v[0];
		_mat[0][1] *= v[1]; _mat[1][1] *= v[1]; _mat[2][1] *= v[1]; _mat[3][1] *= v[1];
		_mat[0][2] *= v[2]; _mat[1][2] *= v[2]; _mat[2][2] *= v[2]; _mat[3][2] *= v[2];
	}

	VOID Matrix::preMultRotate( const Quaternion& q )
	{
		if( q.isIdentity() ) return;
		Matrix r;
		r.setRotate(q);
		preMult(r);
	}

	VOID Matrix::postMultRotate( const Quaternion& q )
	{
		if( q.isIdentity() ) return;
		Matrix r;
		r.setRotate(q);
		postMult(r);
	}

	VOID Matrix::operator *= ( const Matrix& other ) 
	{    
		if( this == &other ) 
		{
			Matrix temp(other);
			postMult( temp );
		}
		else 
			postMult( other ); 
	}

	Matrix Matrix::operator * ( const Matrix &m ) const
	{
		Matrix r;
		r.mult(*this,m);
		return  r;
	}

	BOOL Matrix::invert( const Matrix& mat )
	{
		if( &mat == this )
		{
			Matrix tm(mat);
			return invert(tm);
		}

		BOOL is_4x3 = (mat._mat[0][3]==0.0f && mat._mat[1][3]==0.0f && mat._mat[2][3]==0.0f && mat._mat[3][3]==1.0f) ? TRUE : FALSE;
		if( is_4x3 )
		{
			F32 r00, r01, r02,
				r10, r11, r12,
				r20, r21, r22;
			// Copy rotation components directly into registers for speed
			r00 = mat._mat[0][0]; r01 = mat._mat[0][1]; r02 = mat._mat[0][2];
			r10 = mat._mat[1][0]; r11 = mat._mat[1][1]; r12 = mat._mat[1][2];
			r20 = mat._mat[2][0]; r21 = mat._mat[2][1]; r22 = mat._mat[2][2];
			
			// Partially compute inverse of rot
			_mat[0][0] = r11*r22 - r12*r21;
			_mat[0][1] = r02*r21 - r01*r22;
			_mat[0][2] = r01*r12 - r02*r11;
			
			// Compute determinant of rot from 3 elements just computed
			F32 one_over_det = 1.0f/(r00*_mat[0][0] + r10*_mat[0][1] + r20*_mat[0][2]);
			r00 *= one_over_det; r10 *= one_over_det; r20 *= one_over_det;  // Saves on later computations
			
			// Finish computing inverse of rot
			_mat[0][0] *= one_over_det;
			_mat[0][1] *= one_over_det;
			_mat[0][2] *= one_over_det;
			_mat[0][3] = 0.0;
			_mat[1][0] = r12*r20 - r10*r22; // Have already been divided by det
			_mat[1][1] = r00*r22 - r02*r20; // same
			_mat[1][2] = r02*r10 - r00*r12; // same
			_mat[1][3] = 0.0;
			_mat[2][0] = r10*r21 - r11*r20; // Have already been divided by det
			_mat[2][1] = r01*r20 - r00*r21; // same
			_mat[2][2] = r00*r11 - r01*r10; // same
			_mat[2][3] = 0.0;
			_mat[3][3] = 1.0;
			
			// We no longer need the rxx or det variables anymore, so we can reuse them for whatever we want.  But we will still rename them for the sake of clarity.
			
#define d r22
			d  = mat._mat[3][3];
			
			if( (d-1.0)*(d-1.0) > 1.0e-6 )  // Involves perspective, so we must
			{                       // compute the full inverse
				
				Matrix TPinv;
				_mat[3][0] = _mat[3][1] = _mat[3][2] = 0.0;
				
#define px r00
#define py r01
#define pz r02
#define one_over_s  one_over_det
#define a  r10
#define b  r11
#define c  r12
				
				a  = mat._mat[0][3]; b  = mat._mat[1][3]; c  = mat._mat[2][3];
				px = _mat[0][0]*a + _mat[0][1]*b + _mat[0][2]*c;
				py = _mat[1][0]*a + _mat[1][1]*b + _mat[1][2]*c;
				pz = _mat[2][0]*a + _mat[2][1]*b + _mat[2][2]*c;
				
#undef a
#undef b
#undef c
#define tx r10
#define ty r11
#define tz r12
				
				tx = mat._mat[3][0]; ty = mat._mat[3][1]; tz = mat._mat[3][2];
				one_over_s  = 1.0f/(d - (tx*px + ty*py + tz*pz));
				
				tx *= one_over_s; ty *= one_over_s; tz *= one_over_s;  // Reduces number of calculations later on
				
				// Compute inverse of trans*corr
				TPinv._mat[0][0] = tx*px + 1.0f;
				TPinv._mat[0][1] = ty*px;
				TPinv._mat[0][2] = tz*px;
				TPinv._mat[0][3] = -px * one_over_s;
				TPinv._mat[1][0] = tx*py;
				TPinv._mat[1][1] = ty*py + 1.0f;
				TPinv._mat[1][2] = tz*py;
				TPinv._mat[1][3] = -py * one_over_s;
				TPinv._mat[2][0] = tx*pz;
				TPinv._mat[2][1] = ty*pz;
				TPinv._mat[2][2] = tz*pz + 1.0f;
				TPinv._mat[2][3] = -pz * one_over_s;
				TPinv._mat[3][0] = -tx;
				TPinv._mat[3][1] = -ty;
				TPinv._mat[3][2] = -tz;
				TPinv._mat[3][3] = one_over_s;
				
				preMult(TPinv); // Finish computing full inverse of mat
				
#undef px
#undef py
#undef pz
#undef one_over_s
#undef d
			}
			else // Rightmost column is [0; 0; 0; 1] so it can be ignored
			{
				tx = mat._mat[3][0]; ty = mat._mat[3][1]; tz = mat._mat[3][2];
				
				// Compute translation components of mat'
				_mat[3][0] = -(tx*_mat[0][0] + ty*_mat[1][0] + tz*_mat[2][0]);
				_mat[3][1] = -(tx*_mat[0][1] + ty*_mat[1][1] + tz*_mat[2][1]);
				_mat[3][2] = -(tx*_mat[0][2] + ty*_mat[1][2] + tz*_mat[2][2]);
				
#undef tx
#undef ty
#undef tz
			}
			
			return TRUE;
		}
		else
		{
			U32 indxc[4], indxr[4], ipiv[4];
			U32 i,j,k,l,ll;
			U32 icol = 0;
			U32 irow = 0;
			F32 temp, pivinv, dum, big;
			
			// copy in place this may be unnecessary
			*this = mat;
			
			for (j=0; j<4; j++) ipiv[j]=0;
			
			for(i=0;i<4;i++)
			{
				big=0.0;
				for (j=0; j<4; j++)
					if (ipiv[j] != 1)
						for (k=0; k<4; k++)
						{
							if (ipiv[k] == 0)
							{
								if (GMath::abs(operator()(j,k)) >= big)
								{
									big = GMath::abs(operator()(j,k));
									irow=j;
									icol=k;
								}
							}
							else if (ipiv[k] > 1)
								return FALSE;
						}
						++(ipiv[icol]);
						if (irow != icol)
							for (l=0; l<4; l++) 
							{
								((temp)=(operator()(irow,l)),(operator()(irow,l))=(operator()(icol,l)),(operator()(icol,l))=(temp));
							}
							
							indxr[i]=irow;
							indxc[i]=icol;
							if (operator()(icol,icol) == 0)
								return FALSE;
							
							pivinv = 1.0f/operator()(icol,icol);
							operator()(icol,icol) = 1;
							for (l=0; l<4; l++) operator()(icol,l) *= pivinv;
							for (ll=0; ll<4; ll++)
								if (ll != icol)
								{
									dum=operator()(ll,icol);
									operator()(ll,icol) = 0;
									for (l=0; l<4; l++) operator()(ll,l) -= operator()(icol,l)*dum;
								}
			}
			for (I32 lx=4; lx>0; --lx)
			{
				if (indxr[lx-1] != indxc[lx-1])
					for (k=0; k<4; k++) 
					{
						((temp)=(operator()(k,indxr[lx-1])),(operator()(k,indxr[lx-1]))=(operator()(k,indxc[lx-1])),(operator()(k,indxc[lx-1]))=(temp));
					}
			}
			
			return TRUE;
		}
	}

	VOID Matrix::transpose( const Matrix& mat )
	{
		for(U32 i = 0; i < 4; i++)
		{
			for(U32 j = 0; j < 4; j++)
			{
				_mat[i][j] = mat._mat[j][i];
			}
		}
	}

	Vector3 Matrix::preMult( const Vector3& v ) const
	{
		F32 d = 1.0f/(_mat[0][3]*v._v[0]+_mat[1][3]*v._v[1]+_mat[2][3]*v._v[2]+_mat[3][3]);
		return Vector3( 
			(_mat[0][0]*v._v[0] + _mat[1][0]*v._v[1] + _mat[2][0]*v._v[2] + _mat[3][0])*d,
			(_mat[0][1]*v._v[0] + _mat[1][1]*v._v[1] + _mat[2][1]*v._v[2] + _mat[3][1])*d,
			(_mat[0][2]*v._v[0] + _mat[1][2]*v._v[1] + _mat[2][2]*v._v[2] + _mat[3][2])*d );
	}

	Vector3 Matrix::postMult( const Vector3& v ) const
	{
		F32 d = 1.0f/(_mat[3][0]*v._v[0]+_mat[3][1]*v._v[1]+_mat[3][2]*v._v[2]+_mat[3][3]);
		return Vector3( 
			(_mat[0][0]*v._v[0] + _mat[0][1]*v._v[1] + _mat[0][2]*v._v[2] + _mat[0][3])*d,
			(_mat[1][0]*v._v[0] + _mat[1][1]*v._v[1] + _mat[1][2]*v._v[2] + _mat[1][3])*d,
			(_mat[2][0]*v._v[0] + _mat[2][1]*v._v[1] + _mat[2][2]*v._v[2] + _mat[2][3])*d );
	}

	Vector3 Matrix::operator* ( const Vector3& v ) const
	{
		return postMult(v);
	}

	Vector4 Matrix::preMult( const Vector4& v ) const
	{
		return Vector4( 
			(_mat[0][0]*v._v[0] + _mat[1][0]*v._v[1] + _mat[2][0]*v._v[2] + _mat[3][0]*v._v[3]),
			(_mat[0][1]*v._v[0] + _mat[1][1]*v._v[1] + _mat[2][1]*v._v[2] + _mat[3][1]*v._v[3]),
			(_mat[0][2]*v._v[0] + _mat[1][2]*v._v[1] + _mat[2][2]*v._v[2] + _mat[3][2]*v._v[3]),
			(_mat[0][3]*v._v[0] + _mat[1][3]*v._v[1] + _mat[2][3]*v._v[2] + _mat[3][3]*v._v[3]) );
	}

	Vector4 Matrix::postMult( const Vector4& v ) const
	{
		return Vector4( 
			(_mat[0][0]*v._v[0] + _mat[0][1]*v._v[1] + _mat[0][2]*v._v[2] + _mat[0][3]*v._v[3]),
			(_mat[1][0]*v._v[0] + _mat[1][1]*v._v[1] + _mat[1][2]*v._v[2] + _mat[1][3]*v._v[3]),
			(_mat[2][0]*v._v[0] + _mat[2][1]*v._v[1] + _mat[2][2]*v._v[2] + _mat[2][3]*v._v[3]),
			(_mat[3][0]*v._v[0] + _mat[3][1]*v._v[1] + _mat[3][2]*v._v[2] + _mat[3][3]*v._v[3]) );
	}

	Vector4 Matrix::operator* ( const Vector4& v ) const
	{
		return postMult(v);
	}

	//////////////////////////////////////////////////////////////////////////

	Matrix Matrix::makeScale( const Vector3& sv )
	{
		return makeScale(sv._v[0], sv._v[1], sv._v[2] );
	}

	Matrix Matrix::makeScale( F32 sx, F32 sy, F32 sz )
	{
		Matrix m;
		m.setScale(sx,sy,sz);
		return m;
	}
	
	Matrix Matrix::makeTranslate( const Vector3& tv )
	{
		return makeTranslate(tv._v[0], tv._v[1], tv._v[2] );
	}

	Matrix Matrix::makeTranslate( F32 tx, F32 ty, F32 tz )
	{
		Matrix m;
		m.setTranslate(tx,ty,tz);
		return m;
	}

	Matrix Matrix::makeRotate( const Vector3& from, const Vector3& to )
	{
		Matrix m;
		m.setRotate(from,to);
		return m;
	}

	Matrix Matrix::makeRotate( F32 angle, F32 x, F32 y, F32 z )
	{
		Matrix m;
		m.setRotate(angle,x,y,z);
		return m;
	}

	Matrix Matrix::makeRotate( F32 angle, const Vector3& axis )
	{
		Matrix m;
		m.setRotate(angle,axis);
		return m;
	}

	Matrix Matrix::makeRotate( F32 angle1, const Vector3& axis1, F32 angle2, const Vector3& axis2, F32 angle3, const Vector3& axis3 )
	{
		Matrix m;
		m.setRotate(angle1,axis1,angle2,axis2,angle3,axis3);
		return m;
	}

	Matrix Matrix::makeRotate( const Quaternion& q )
	{
		return Matrix(q);
	}

	Matrix Matrix::makeInverse( const Matrix& matrix )
	{
		Matrix m;
		m.invert(matrix);
		return m;
	}

	Matrix Matrix::makeTranspose( const Matrix& matrix )
	{
		Matrix m;
		m.transpose(matrix);
		return m;
	}
	
	Matrix Matrix::makeOrthonormal( const Matrix& rhs )
	{
		Matrix mat;
		F32 x_colMag = (rhs._mat[0][0] * rhs._mat[0][0]) + (rhs._mat[1][0] * rhs._mat[1][0]) + (rhs._mat[2][0] * rhs._mat[2][0]);
		F32 y_colMag = (rhs._mat[0][1] * rhs._mat[0][1]) + (rhs._mat[1][1] * rhs._mat[1][1]) + (rhs._mat[2][1] * rhs._mat[2][1]);
		F32 z_colMag = (rhs._mat[0][2] * rhs._mat[0][2]) + (rhs._mat[1][2] * rhs._mat[1][2]) + (rhs._mat[2][2] * rhs._mat[2][2]);
		
		if(!GMath::equals(1.0f,x_colMag,1e-6f) && !GMath::equals(0.0f,x_colMag,1e-6f))
		{
			x_colMag = ::sqrtf(x_colMag);
			mat._mat[0][0] = rhs._mat[0][0] / x_colMag;
			mat._mat[1][0] = rhs._mat[1][0] / x_colMag;
			mat._mat[2][0] = rhs._mat[2][0] / x_colMag;
		}
		else
		{
			mat._mat[0][0] = rhs._mat[0][0];
			mat._mat[1][0] = rhs._mat[1][0];
			mat._mat[2][0] = rhs._mat[2][0];
		}
		
		if(!GMath::equals(1.0f,y_colMag,1e-6f) && !GMath::equals(0.0f,y_colMag,1e-6f))
		{
			y_colMag = ::sqrtf(y_colMag);
			mat._mat[0][1] = rhs._mat[0][1] / y_colMag;
			mat._mat[1][1] = rhs._mat[1][1] / y_colMag;
			mat._mat[2][1] = rhs._mat[2][1] / y_colMag;
		}
		else
		{
			mat._mat[0][1] = rhs._mat[0][1];
			mat._mat[1][1] = rhs._mat[1][1];
			mat._mat[2][1] = rhs._mat[2][1];
		}
		
		if(!GMath::equals(1.0f,z_colMag,1e-6f) && !GMath::equals(0.0f,z_colMag,1e-6f))
		{
			z_colMag = ::sqrtf(z_colMag);
			mat._mat[0][2] = rhs._mat[0][2] / z_colMag;
			mat._mat[1][2] = rhs._mat[1][2] / z_colMag;
			mat._mat[2][2] = rhs._mat[2][2] / z_colMag;
		}
		else
		{
			mat._mat[0][2] = rhs._mat[0][2];
			mat._mat[1][2] = rhs._mat[1][2];
			mat._mat[2][2] = rhs._mat[2][2];
		}
		
		mat._mat[3][0] = rhs._mat[3][0];
		mat._mat[3][1] = rhs._mat[3][1];
		mat._mat[3][2] = rhs._mat[3][2];
		
		mat._mat[0][3] = rhs._mat[0][3];
		mat._mat[1][3] = rhs._mat[1][3];
		mat._mat[2][3] = rhs._mat[2][3];
		mat._mat[3][3] = rhs._mat[3][3];
		return mat;
	}
	
	Matrix Matrix::makeOrtho( F32 left, F32 right, F32 bottom, F32 top, F32 zNear, F32 zFar )
	{
		Matrix mat;
		F32 tx = -(right+left)/(right-left);
		F32 ty = -(top+bottom)/(top-bottom);
		F32 tz = -(zFar+zNear)/(zFar-zNear);
		mat._mat[0][0] = 2.0f/(right-left);	mat._mat[0][1] = 0.0f;				mat._mat[0][2] = 0.0f;				mat._mat[0][3] = 0.0f;
		mat._mat[1][0] = 0.0f;				mat._mat[1][1] = 2.0f/(top-bottom);	mat._mat[1][2] = 0.0f;				mat._mat[1][3] = 0.0f;
		mat._mat[2][0] = 0.0f;				mat._mat[2][1] = 0.0f;				mat._mat[2][2] = -2.0f/(zFar-zNear);mat._mat[2][3] = 0.0f;
		mat._mat[3][0] = tx;				mat._mat[3][1] = ty;				mat._mat[3][2] = tz;				mat._mat[3][3] = 1.0f;
		return mat;
	}
	
	BOOL Matrix::getOrtho(const Matrix& matrix, F32& left, F32& right, F32& bottom, F32& top, F32& zNear, F32& zFar)
	{
		if (matrix._mat[0][3]!=0.0 || matrix._mat[1][3]!=0.0 || matrix._mat[2][3]!=0.0 || matrix._mat[3][3]!=1.0) return FALSE;
		
		zNear = (matrix._mat[3][2]+1.0f) / matrix._mat[2][2];
		zFar = (matrix._mat[3][2]-1.0f) / matrix._mat[2][2];
		
		left = -(1.0f+matrix._mat[3][0]) / matrix._mat[0][0];
		right = (1.0f-matrix._mat[3][0]) / matrix._mat[0][0];
		
		bottom = -(1.0f+matrix._mat[3][1]) / matrix._mat[1][1];
		top = (1.0f-matrix._mat[3][1]) / matrix._mat[1][1];

		return TRUE;
	}
	
	Matrix Matrix::makeFrustum( F32 left, F32 right, F32 bottom, F32 top, F32 zNear,  F32 zFar )
	{
		F32 A = (right+left)/(right-left);
		F32 B = (top+bottom)/(top-bottom);
		F32 C = -(zFar+zNear)/(zFar-zNear);
		F32 D = -2.0f*zFar*zNear/(zFar-zNear);

		Matrix mat;
		mat._mat[0][0] = 2.0f*zNear/(right-left);	mat._mat[0][1] = 0.0f;						mat._mat[0][2] = 0.0f;	mat._mat[0][3] = 0.0f;
		mat._mat[1][0] = 0.0f;						mat._mat[1][1] = 2.0f*zNear/(top-bottom);	mat._mat[1][2] = 0.0f;	mat._mat[1][3] = 0.0f;
		mat._mat[2][0] = A;							mat._mat[2][1] = B;							mat._mat[2][2] = C;		mat._mat[2][3] = -1.0f;
		mat._mat[3][0] = 0.0f;						mat._mat[3][1] = 0.0f;						mat._mat[3][2] = D;		mat._mat[3][3] = 0.0f;
		return mat;
	}
	
	BOOL Matrix::getFrustum( const Matrix& mat, F32& left, F32& right, F32& bottom, F32& top, F32& zNear,  F32& zFar )
	{
		if( mat._mat[0][3]!=0.0f || mat._mat[1][3]!=0.0f || mat._mat[2][3]!=-1.0f || mat._mat[3][3]!=0.0f ) return FALSE;
		
		zNear = mat._mat[3][2] / (mat._mat[2][2]-1.0f);
		zFar = mat._mat[3][2] / (1.0f+mat._mat[2][2]);
		
		left = zNear * (mat._mat[2][0]-1.0f) / mat._mat[0][0];
		right = zNear * (1.0f+mat._mat[2][0]) / mat._mat[0][0];
		
		top = zNear * (1.0f+mat._mat[2][1]) / mat._mat[1][1];
		bottom = zNear * (mat._mat[2][1]-1.0f) / mat._mat[1][1];

		return TRUE;
	}
	
	Matrix Matrix::makePerspective( F32 fovy, F32 aspectRatio, F32 zNear, F32 zFar )
	{
		// calculate the appropriate left, right etc.
		F32 tan_fovy = ::tan( GMath::degToRad(fovy*0.5f) );
		F32 right  =  tan_fovy * aspectRatio * zNear;
		F32 left   = -right;
		F32 top    =  tan_fovy * zNear;
		F32 bottom =  -top;
		return makeFrustum(left,right,bottom,top,zNear,zFar);
	}
	
	BOOL Matrix::getPerspective( const Matrix& mat, F32& fovy,  F32& aspectRatio, F32& zNear, F32& zFar )
	{
		F32 right  =  0.0;
		F32 left   =  0.0;
		F32 top    =  0.0;
		F32 bottom =  0.0;
		if (getFrustum(mat, left,right,bottom,top,zNear,zFar))
		{
			fovy = GMath::radToDeg(::atan(top/zNear)-::atan(bottom/zNear));
			aspectRatio = (right-left)/(top-bottom);
			return TRUE;
		}
		return FALSE;
	}
	
	Matrix Matrix::makeLookAt( const Vector3& eye, const Vector3& center, const Vector3& up )
	{
		Vector3 f(center-eye);
		f.normalize();
		Vector3 s(f^up);
		s.normalize();
		Vector3 u(s^f);
		u.normalize();
		
		Matrix mat;
		mat.set(
			s[0],	u[0],   -f[0],  0.0,
			s[1],	u[1],   -f[1],  0.0,
			s[2],	u[2],	-f[2],  0.0,
			0.0,	0.0,    0.0,	1.0);
		
		mat.preMultTranslate(-eye);
		return mat;
	}
	
	VOID Matrix::getLookAt( const Matrix& mat, Vector3& eye,Vector3& center,Vector3& up, F32 lookDistance )
	{
		Matrix inv;
		inv.invert(mat);
		eye = Vector3(0.0,0.0,0.0)*inv;
		up = transform3x3(mat,Vector3(0.0,1.0,0.0));
		center = transform3x3(mat,Vector3(0.0,0.0,-1));
		center.normalize();
		center = eye + center*lookDistance;
	}
	
	Vector3 Matrix::transform3x3( const Vector3& v,const Matrix& m )
	{
		return Vector3( 
			(m._mat[0][0]*v._v[0] + m._mat[1][0]*v._v[1] + m._mat[2][0]*v._v[2]),
			(m._mat[0][1]*v._v[0] + m._mat[1][1]*v._v[1] + m._mat[2][1]*v._v[2]),
            (m._mat[0][2]*v._v[0] + m._mat[1][2]*v._v[1] + m._mat[2][2]*v._v[2]));
	}
	
	Vector3 Matrix::transform3x3( const Matrix& m,const Vector3& v )
	{
		return Vector3( 
			(m._mat[0][0]*v._v[0] + m._mat[0][1]*v._v[1] + m._mat[0][2]*v._v[2]),
			(m._mat[1][0]*v._v[0] + m._mat[1][1]*v._v[1] + m._mat[1][2]*v._v[2]),
			(m._mat[2][0]*v._v[0] + m._mat[2][1]*v._v[1] + m._mat[2][2]*v._v[2])) ;
	}

	//////////////////////////////////////////////////////////////////////////

	Vector3 operator* (const Vector3& v, const Matrix& m )
	{
		return m.preMult(v);
	}

	Vector4 operator* (const Vector4& v, const Matrix& m )
	{
		return m.preMult(v);
	}

	//////////////////////////////////////////////////////////////////////////

	#undef INNER_PRODUCT
	#undef SET_ROW

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
