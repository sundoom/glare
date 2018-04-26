/*
 *  Quaternion.cpp
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

	Quaternion::Quaternion() 
	{
		_v[0]=0.0; _v[1]=0.0; _v[2]=0.0; _v[3]=1.0; 
	}

    Quaternion::Quaternion( F32 x, F32 y, F32 z, F32 w )
    {
        _v[0]=x;
        _v[1]=y;
        _v[2]=z;
        _v[3]=w;
    }

	Quaternion::Quaternion(const Quaternion& q)
	{
		_v[0]=q._v[0];
        _v[1]=q._v[1];
        _v[2]=q._v[2];
        _v[3]=q._v[3];
	}

    Quaternion::Quaternion( const Vector4& v )
    {
        _v[0]=v.x();
        _v[1]=v.y();
        _v[2]=v.z();
        _v[3]=v.w();
    }

    Quaternion::Quaternion( F32 angle, const Vector3& axis)
    {
        setRotate(angle,axis);
    }        

    Quaternion::Quaternion( F32 angle1, const Vector3& axis1, 
                 F32 angle2, const Vector3& axis2,
                 F32 angle3, const Vector3& axis3)
    {
        setRotate(angle1,axis1,angle2,axis2,angle3,axis3);
    }

    Quaternion& Quaternion::operator = (const Quaternion& v) 
	{ 
		_v[0]=v._v[0];  _v[1]=v._v[1]; _v[2]=v._v[2]; _v[3]=v._v[3]; 
		return *this; 
	}

    BOOL Quaternion::operator == (const Quaternion& v) const 
	{ 
		return (_v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2] && _v[3]==v._v[3]) ? TRUE : FALSE; 
	}

    BOOL Quaternion::operator != (const Quaternion& v) const 
	{ 
		return (_v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2] || _v[3]!=v._v[3]) ? TRUE : FALSE; 
	}

    VOID Quaternion::set(F32 x, F32 y, F32 z, F32 w)
    {
        _v[0]=x;
        _v[1]=y;
        _v[2]=z;
        _v[3]=w;
    }
    
    VOID Quaternion::set(Vector4& v)
    {
        _v[0]=v.x();
        _v[1]=v.y();
        _v[2]=v.z();
        _v[3]=v.w();
    }       
    
    VOID Quaternion::set(const class Matrix& m)
	{
		*this = m.getRotate();
	}

    VOID Quaternion::get(class Matrix& m) const
	{
		m.makeRotate(*this);
	}

    F32 & Quaternion::operator [] (U32 i) 
	{ 
		return _v[i]; 
	}

    F32 Quaternion::operator [] (U32 i) const 
	{ 
		return _v[i]; 
	}

    F32 & Quaternion::x() 
	{ 
		return _v[0]; 
	}

    F32 & Quaternion::y() 
	{ 
		return _v[1]; 
	}

    F32 & Quaternion::z() 
	{ 
		return _v[2]; 
	}

    F32 & Quaternion::w() 
	{ 
		return _v[3]; 
	}

    F32 Quaternion::x() const 
	{ 
		return _v[0]; 
	}

    F32 Quaternion::y() const 
	{ 
		return _v[1]; 
	}

    F32 Quaternion::z() const 
	{ 
		return _v[2]; 
	}

    F32 Quaternion::w() const 
	{ 
		return _v[3]; 
	}

    VOID Quaternion::identity()
	{
		_v[0]=0; _v[1]=0; _v[2]=0; _v[3]=1;
	}

	BOOL Quaternion::isIdentity() const
	{
		return (_v[0]==0 && _v[1]==0 && _v[2]==0 && GMath::abs(_v[3])==1) ? TRUE : FALSE; 
	}

	const Quaternion Quaternion::operator * (F32 rhs) const
    {
        return Quaternion(_v[0]*rhs, _v[1]*rhs, _v[2]*rhs, _v[3]*rhs);
    }

    Quaternion& Quaternion::operator *= (F32 rhs)
    {
        _v[0]*=rhs;
        _v[1]*=rhs;
        _v[2]*=rhs;
        _v[3]*=rhs;
        return *this;
    }

    const Quaternion Quaternion::operator*(const Quaternion& rhs) const
    {
        return Quaternion( 
			rhs._v[3]*_v[0] + rhs._v[0]*_v[3] + rhs._v[1]*_v[2] - rhs._v[2]*_v[1],
            rhs._v[3]*_v[1] - rhs._v[0]*_v[2] + rhs._v[1]*_v[3] + rhs._v[2]*_v[0],
            rhs._v[3]*_v[2] + rhs._v[0]*_v[1] - rhs._v[1]*_v[0] + rhs._v[2]*_v[3],
            rhs._v[3]*_v[3] - rhs._v[0]*_v[0] - rhs._v[1]*_v[1] - rhs._v[2]*_v[2] );
    }

    Quaternion& Quaternion::operator*=(const Quaternion& rhs)
    {
        F32 x = rhs._v[3]*_v[0] + rhs._v[0]*_v[3] + rhs._v[1]*_v[2] - rhs._v[2]*_v[1];
        F32 y = rhs._v[3]*_v[1] - rhs._v[0]*_v[2] + rhs._v[1]*_v[3] + rhs._v[2]*_v[0];
        F32 z = rhs._v[3]*_v[2] + rhs._v[0]*_v[1] - rhs._v[1]*_v[0] + rhs._v[2]*_v[3];
        _v[3] = rhs._v[3]*_v[3] - rhs._v[0]*_v[0] - rhs._v[1]*_v[1] - rhs._v[2]*_v[2];
        _v[2] = z;
        _v[1] = y;
        _v[0] = x;
        return (*this);
    }

    Quaternion Quaternion::operator / (F32 rhs) const
    {
        F32 div = 1.0f/rhs;
        return Quaternion(_v[0]*div, _v[1]*div, _v[2]*div, _v[3]*div);
    }

    Quaternion& Quaternion::operator /= (F32 rhs)
    {
        F32 div = 1.0f/rhs;
        _v[0]*=div;
        _v[1]*=div;
        _v[2]*=div;
        _v[3]*=div;
        return *this;
    }

    const Quaternion Quaternion::operator/(const Quaternion& denom) const
    {
        return ( (*this) * denom.inverse() );
    }

    Quaternion& Quaternion::operator/=(const Quaternion& denom)
    {
        (*this) = (*this) * denom.inverse();
        return (*this);
    }

    const Quaternion Quaternion::operator + (const Quaternion& rhs) const
    {
        return Quaternion(_v[0]+rhs._v[0], _v[1]+rhs._v[1],
            _v[2]+rhs._v[2], _v[3]+rhs._v[3]);
    }

    Quaternion& Quaternion::operator += (const Quaternion& rhs)
    {
        _v[0] += rhs._v[0];
        _v[1] += rhs._v[1];
        _v[2] += rhs._v[2];
        _v[3] += rhs._v[3];
        return *this;
    }

    const Quaternion Quaternion::operator - (const Quaternion& rhs) const
    {
        return Quaternion(_v[0]-rhs._v[0], _v[1]-rhs._v[1],
            _v[2]-rhs._v[2], _v[3]-rhs._v[3] );
    }

    Quaternion& Quaternion::operator -= (const Quaternion& rhs)
    {
        _v[0]-=rhs._v[0];
        _v[1]-=rhs._v[1];
        _v[2]-=rhs._v[2];
        _v[3]-=rhs._v[3];
        return *this;
    }

    const Quaternion Quaternion::operator - () const
    {
        return Quaternion (-_v[0], -_v[1], -_v[2], -_v[3]);
    }

    F32 Quaternion::length() const
    {
		return ::sqrtf( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3]);
    }

    F32 Quaternion::length2() const
    {
        return _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3];
    }
    
    Quaternion Quaternion::conjugate() const
    { 
         return Quaternion( -_v[0], -_v[1], -_v[2], _v[3] );
    }
    
    Quaternion Quaternion::inverse() const
    {
         return conjugate() / length2();
	}

	VOID Quaternion::setRotate( F32 angle, F32  x, F32  y, F32  z )
	{
		const F32 epsilon = 0.0000001f;
		
		F32 length = ::sqrtf( x*x + y*y + z*z );
		if (length < epsilon)
		{
			// ~zero length axis, so reset rotation to zero.
			identity();
			return;
		}
		
		F32 inversenorm  = 1.0f/length;
		F32 coshalfangle = ::cos( 0.5f*angle );
		F32 sinhalfangle = ::sin( 0.5f*angle );
		
		_v[0] = x * sinhalfangle * inversenorm;
		_v[1] = y * sinhalfangle * inversenorm;
		_v[2] = z * sinhalfangle * inversenorm;
		_v[3] = coshalfangle;
	}

    VOID Quaternion::setRotate( F32 angle, const Vector3& vec )
	{
		setRotate( angle, vec[0], vec[1], vec[2] );
	}

    VOID Quaternion::setRotate( F32 angle1, const Vector3& axis1, F32 angle2, const Vector3& axis2, F32 angle3, const Vector3& axis3)
	{
		Quaternion q1; q1.setRotate(angle1,axis1);
		Quaternion q2; q2.setRotate(angle2,axis2);
		Quaternion q3; q3.setRotate(angle3,axis3);
		*this = q1*q2*q3;
	}
    
    VOID Quaternion::setRotate( const Vector3& from, const Vector3& to )
	{
		// This routine takes any vector as argument but normalized 
		// vectors are necessary, if only for computing the dot product.
		// Too bad the API is that generic, it leads to performance loss.
		// Even in the case the 2 vectors are not normalized but same length,
		// the sqrt could be shared, but we have no way to know beforehand
		// at this point, while the caller may know.
		// So, we have to test... in the hope of saving at least a sqrt
		Vector3 sourceVector = from;
		Vector3 targetVector = to;
		
		F32 fromLen2 = from.length2();
		F32 fromLen;
		// normalize only when necessary, epsilon test
		if ((fromLen2 < 1.0-1e-7) || (fromLen2 > 1.0+1e-7)) 
		{
			fromLen = ::sqrtf(fromLen2);
			sourceVector /= fromLen;
		} 
		else 
			fromLen = 1.0;
		
		F32 toLen2 = to.length2();
		// normalize only when necessary, epsilon test
		if ((toLen2 < 1.0-1e-7) || (toLen2 > 1.0+1e-7)) 
		{
			F32 toLen;
			// re-use fromLen for case of mapping 2 vectors of the same length
			if ((toLen2 > fromLen2-1e-7) && (toLen2 < fromLen2+1e-7)) 
			{
				toLen = fromLen;
			} 
			else toLen = ::sqrtf(toLen2);
			targetVector /= toLen;
		}
		
		
		// Now let's get into the real stuff
		// Use "dot product plus one" as test as it can be re-used later on
		F32 dotProdPlus1 = 1.0f + sourceVector * targetVector;
		
		// Check for degenerate case of full u-turn. Use epsilon for detection
		if (dotProdPlus1 < 1e-7) 
		{
			// Get an orthogonal vector of the given vector
			// in a plane with maximum vector coordinates.
			// Then use it as quaternion axis with pi angle
			// Trick is to realize one value at least is >0.6 for a normalized vector.
			if (::fabs(sourceVector.x()) < 0.6f) 
			{
				const F32 norm = ::sqrtf(1.0f - sourceVector.x() * sourceVector.x());
				_v[0] = 0.0f; 
				_v[1] = sourceVector.z() / norm;
				_v[2] = -sourceVector.y() / norm;
				_v[3] = 0.0f;
			} 
			else if (::fabs(sourceVector.y()) < 0.6f) 
			{
				const F32 norm = ::sqrtf(1.0f - sourceVector.y() * sourceVector.y());
				_v[0] = -sourceVector.z() / norm;
				_v[1] = 0.0f;
				_v[2] = sourceVector.x() / norm;
				_v[3] = 0.0f;
			} 
			else 
			{
				const F32 norm = ::sqrtf(1.0f - sourceVector.z() * sourceVector.z());
				_v[0] = sourceVector.y() / norm;
				_v[1] = -sourceVector.x() / norm;
				_v[2] = 0.0f;
				_v[3] = 0.0f;
			}
		}		
		else 
		{
			// Find the shortest angle quaternion that transforms normalized vectors
			// into one other. Formula is still valid when vectors are colinear
			const F32 s = ::sqrtf(0.5f * dotProdPlus1);
			const Vector3 tmp = sourceVector ^ targetVector / (2.0f*s);
			_v[0] = tmp.x();
			_v[1] = tmp.y();
			_v[2] = tmp.z();
			_v[3] = s;
		}
	}
    
    VOID Quaternion::getRotate( F32 & angle, F32 & x, F32 & y, F32 & z ) const
	{
		F32 sinhalfangle = ::sqrtf( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] );
		
		angle = 2.0f * ::atan2( sinhalfangle, _v[3] );
		if(sinhalfangle)
		{
			x = _v[0] / sinhalfangle;
			y = _v[1] / sinhalfangle;
			z = _v[2] / sinhalfangle;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
			z = 1.0f;
		}
	}
    
    VOID Quaternion::getRotate( F32 & angle, Vector3& vec ) const
	{
		F32 x,y,z;
		getRotate(angle,x,y,z);
		vec[0]=x;
		vec[1]=y;
		vec[2]=z;
	}
    
    VOID Quaternion::slerp( F32 t, const Quaternion& from, const Quaternion& to)
	{
		const F32 epsilon = 0.00001f;
		F32 omega, cosomega, sinomega, scale_from, scale_to ;
		
		Quaternion quatTo(to);

		// this is a dot product		
		cosomega = from._v[0]*to._v[0] + from._v[1]*to._v[1] + from._v[2]*to._v[2] + from._v[3]*to._v[3];
		if ( cosomega < 0.0f )
		{ 
			cosomega = -cosomega; 
			quatTo = -to;
		}
		
		if( (1.0f - cosomega) > epsilon )
		{
			omega= ::acos(cosomega) ;  // 0 <= omega <= Pi (see man acos)
			sinomega = ::sin(omega) ;  // this sinomega should always be +ve so
			// could try sinomega=sqrtf(1-cosomega*cosomega) to avoid a sin()?
			scale_from = ::sin((1.0f-t)*omega)/sinomega ;
			scale_to = ::sin(t*omega)/sinomega ;
		}
		else
		{
			
			// The ends of the vectors are very close
			// we can use simple linear interpolation - no need
			// to worry about the "spherical" interpolation
			scale_from = 1.0f - t ;
			scale_to = t ;
		}
		
		*this = (from*scale_from) + (quatTo*scale_to);
	}           
    
    Vector3 Quaternion::operator* (const Vector3& v) const
    {
        // nVidia SDK implementation
        Vector3 uv, uuv; 
        Vector3 qvec(_v[0], _v[1], _v[2]);
        uv = qvec ^ v;
        uuv = qvec ^ uv; 
        uv *= ( 2.0f * _v[3] ); 
        uuv *= 2.0f; 
        return v + uv + uuv;
    }

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
