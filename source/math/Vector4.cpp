/*
 *  Vector4.cpp
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

	Vector4::Vector4() 
	{ 
		_v[0]=0.0f; _v[1]=0.0f; _v[2]=0.0f; _v[3]=0.0f;
	}

	Vector4::Vector4(F32 v)
	{
		_v[0]=v; _v[1]=v; _v[2]=v; _v[3]=v;
	}

	Vector4::Vector4(F32 x, F32 y, F32 z, F32 w)
	{
		_v[0]=x;
		_v[1]=y;
		_v[2]=z;
		_v[3]=w;
	}

	Vector4::Vector4(const Vector3& v3,F32 w)
	{
		_v[0]=v3[0];
		_v[1]=v3[1];
		_v[2]=v3[2];
		_v[3]=w;
	}

	Vector4::Vector4(const F32 v[])
	{
		_v[0] = v[0];
		_v[1] = v[1];
		_v[2] = v[2];
		_v[3] = v[3];
	}
	
	Vector4::Vector4(const Vector4& rhs)
	{
		_v[0] = rhs._v[0];
		_v[1] = rhs._v[1];
		_v[2] = rhs._v[2];
		_v[3] = rhs._v[3];
	}

	const Vector4& Vector4::operator=(const Vector4& rhs)
	{
		_v[0] = rhs._v[0];
		_v[1] = rhs._v[1];
		_v[2] = rhs._v[2];
		_v[3] = rhs._v[3];
		return *this;
	}

	BOOL Vector4::equals(const Vector4 & v, F32 epsilon) const
	{
		return
			(::fabsf(_v[0]-v._v[0]) < epsilon &&
			::fabsf(_v[1]-v._v[1]) < epsilon &&
			::fabsf(_v[2]-v._v[2]) < epsilon &&
			::fabsf(_v[3]-v._v[3]) < epsilon) ? TRUE : FALSE;
	}

	BOOL Vector4::operator == (const Vector4& v) const 
	{ 
		return (_v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2] && _v[3]==v._v[3]) ? TRUE : FALSE; 
	}

	BOOL Vector4::operator != (const Vector4& v) const 
	{ 
		return (_v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2] || _v[3]!=v._v[3]) ? TRUE : FALSE; 
	}

	BOOL Vector4::operator < (const Vector4& v) const
	{
		return ((_v[0] < v._v[0])&&(_v[1] < v._v[1])&&(_v[2] < v._v[2])&&(_v[3] < v._v[3])) ? TRUE : FALSE;
	}

	F32* Vector4::ptr() 
	{
		return _v; 
	}

	const F32* Vector4::ptr() const 
	{ 
		return _v; 
	}

	VOID Vector4::set( F32 x, F32 y, F32 z, F32 w)
	{
		_v[0]=x; _v[1]=y; _v[2]=z; _v[3]=w;
	}

	VOID Vector4::set( const Vector4& rhs)
	{
		_v[0]=rhs._v[0]; _v[1]=rhs._v[1]; _v[2]=rhs._v[2]; _v[3]=rhs._v[3];
	}

	F32& Vector4::operator [] (U32 i) 
	{ 
		return _v[i]; 
	}

	F32  Vector4::operator [] (U32 i) const 
	{ 
		return _v[i]; 
	}

	F32& Vector4::x() 
	{ 
		return _v[0]; 
	}

	F32& Vector4::y() 
	{ 
		return _v[1]; 
	}
	
	F32& Vector4::z() 
	{ 
		return _v[2]; 
	}

	F32& Vector4::w() 
	{ 
		return _v[3]; 
	}

	F32 Vector4::x() const 
	{ 
		return _v[0]; 
	}

	F32 Vector4::y() const 
	{ 
		return _v[1]; 
	}

	F32 Vector4::z() const 
	{ 
		return _v[2]; 
	}

	F32 Vector4::w() const 
	{ 
		return _v[3]; 
	}

	F32& Vector4::r() 
	{ 
		return _v[0]; 
	}

	F32& Vector4::g() 
	{ 
		return _v[1]; 
	}

	F32& Vector4::b() 
	{ 
		return _v[2]; 
	}

	F32& Vector4::a() 
	{ 
		return _v[3]; 
	}

	F32 Vector4::r() const 
	{ 
		return _v[0]; 
	}

	F32 Vector4::g() const 
	{ 
		return _v[1]; 
	}

	F32 Vector4::b() const 
	{ 
		return _v[2]; 
	}

	F32 Vector4::a() const 
	{ 
		return _v[3]; 
	}

	U32 Vector4::abgr() const
	{
		return 
			(U32)GMath::clamp((_v[0]*255.0f),255.0f,0.0f)<<24 |
			(U32)GMath::clamp((_v[1]*255.0f),255.0f,0.0f)<<16 |
			(U32)GMath::clamp((_v[2]*255.0f),255.0f,0.0f)<<8  |
			(U32)GMath::clamp((_v[3]*255.0f),255.0f,0.0f);
	}

	U32 Vector4::rgba() const
	{
		return 
			(U32)GMath::clamp((_v[3]*255.0f),255.0f,0.0f)<<24 |
			(U32)GMath::clamp((_v[2]*255.0f),255.0f,0.0f)<<16 |
			(U32)GMath::clamp((_v[1]*255.0f),255.0f,0.0f)<<8  |
			(U32)GMath::clamp((_v[0]*255.0f),255.0f,0.0f);
	}

	F32 Vector4::operator * (const Vector4& rhs) const
	{
		return 
			_v[0]*rhs._v[0]+
			_v[1]*rhs._v[1]+
			_v[2]*rhs._v[2]+
			_v[3]*rhs._v[3];
	}

	Vector4 Vector4::operator * (F32 rhs) const
	{
		return Vector4(_v[0]*rhs, _v[1]*rhs, _v[2]*rhs, _v[3]*rhs);
	}

	Vector4& Vector4::operator *= (F32 rhs)
	{
		_v[0]*=rhs;
		_v[1]*=rhs;
		_v[2]*=rhs;
		_v[3]*=rhs;
		return *this;
	}

	Vector4 Vector4::operator / (F32 rhs) const
	{
		return Vector4(_v[0]/rhs, _v[1]/rhs, _v[2]/rhs, _v[3]/rhs);
	}

	Vector4& Vector4::operator /= (F32 rhs)
	{
		_v[0]/=rhs;
		_v[1]/=rhs;
		_v[2]/=rhs;
		_v[3]/=rhs;
		return *this;
	}

	Vector4 Vector4::operator + (const Vector4& rhs) const
	{
		return Vector4(_v[0]+rhs._v[0], _v[1]+rhs._v[1],
			_v[2]+rhs._v[2], _v[3]+rhs._v[3]);
	}

	Vector4& Vector4::operator += (const Vector4& rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		_v[3] += rhs._v[3];
		return *this;
	}

	Vector4 Vector4::operator - (const Vector4& rhs) const
	{
		return Vector4(_v[0]-rhs._v[0], _v[1]-rhs._v[1],
			_v[2]-rhs._v[2], _v[3]-rhs._v[3] );
	}

	Vector4& Vector4::operator -= (const Vector4& rhs)
	{
		_v[0]-=rhs._v[0];
		_v[1]-=rhs._v[1];
		_v[2]-=rhs._v[2];
		_v[3]-=rhs._v[3];
		return *this;
	}

	const Vector4 Vector4::operator - () const
	{
		return Vector4 (-_v[0], -_v[1], -_v[2], -_v[3]);
	}

	F32 Vector4::length() const
	{
		return ::sqrtf( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3]);
	}

	F32 Vector4::length2() const
	{
		return _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3];
	}

	F32 Vector4::normalize()
	{
		F32 norm = Vector4::length();
		if (norm>0.0f)
		{
			F32 inv = 1.0f/norm;
			_v[0] *= inv;
			_v[1] *= inv;
			_v[2] *= inv;
			_v[3] *= inv;
		}
		return( norm );
	}

	//////////////////////////////////////////////////////////////////////////
	
	F32 operator * (const Vector3& lhs,const Vector4& rhs)
	{
		return lhs[0]*rhs[0]+lhs[1]*rhs[1]+lhs[2]*rhs[2]+rhs[3];
	}
	
	F32 operator * (const Vector4& lhs,const Vector3& rhs)
	{
		return lhs[0]*rhs[0]+lhs[1]*rhs[1]+lhs[2]*rhs[2]+lhs[3];
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
