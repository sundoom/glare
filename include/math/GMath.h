/*
 *  GMath.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __GMath__
#define __GMath__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////

	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

	#ifdef max
		#undef max
	#endif

	#ifdef min
		#undef min
	#endif

	// max value
	#undef FLT_MAX
	#define FLT_MAX		3.402823466e+38F

	// integer representation of 1.0
	#undef IEEE_1_0
	#define IEEE_1_0	0x3f800000

	//////////////////////////////////////////////////////////////////////////

	// Integer representation of a floating-point value.
	#define IR(x)		((U32&)(x))

	// Floating-point representation of a integer value.
	#define FR(x)		((F32&)(x))

	// Absolute integer representation of a floating-point value
	#define AIR(x)		(IR(x)&0x7fffffff)

	//////////////////////////////////////////////////////////////////////////

	// we can get bad range checks if we use F64 prec consts to check single prec results.
	#define PI			3.141592653589793f
	#define HALF_PI		1.57079632679489661923f
	#define TWO_PI		6.28318530717958647692f
	#define INV_PI		0.31830988618379067154f

	#define	INVSQRT2	0.707106781188f	//!< 1 / sqrt(2)
	#define	INVSQRT3	0.577350269189f	//!< 1 / sqrt(3)

	//////////////////////////////////////////////////////////////////////////
	// axis
	//	z
	//	|		y	
	//	|     /
	//	|   /
	//	| /
	//	0---------x

	//////////////////////////////////////////////////////////////////////////
	// GMath
	//////////////////////////////////////////////////////////////////////////

	// static class with stateless scalar math routines.
	class GMath
	{
	public:
		// returns TRUE if the two numbers are within eps of each other.
		static BOOL equals(F32,F32,F32 eps);
		static BOOL equals(F64,F64,F64 eps);

		// The floor function returns a floating-point value representing the largest integer that is less than or equal to x.
		static F32 floor(F32);
		static F64 floor(F64);

		// The ceil function returns a F64 value representing the smallest integer that is greater than or equal to x. 
		static F32 ceil(F32);
		static F64 ceil(F64);

		// Truncates the F32 to an integer.
		static I32 trunc(F32);
		static I32 trunc(F64);

		// abs returns the absolute value of its argument. 
		static F32 abs(F32);
		static F64 abs(F64);
		static I32 abs(I32);

		// sign returns the sign of its argument. The sign of zero is undefined.
		static F32 sign(F32);
		static F64 sign(F64);
		static I32 sign(I32);

		// The return value is the greater of the two specified values. 
		static F32 max(F32,F32);
		static F64 max(F64,F64);
		static I32 max(I32,I32);
		static U32 max(U32,U32);

		// The return value is the lesser of the two specified values. 
		static F32 min(F32,F32);
		static F64 min(F64,F64);
		static I32 min(I32,I32);
		static U32 min(U32,U32);

		// mod returns the floating-point remainder of x / y. If the value of y is 0.0, mod returns a quiet NaN.
		static F32 mod(F32 x, F32 y);
		static F64 mod(F64 x, F64 y);

		// Clamps v to the range [hi,lo]
		static F32 clamp(F32 v, F32 hi, F32 low);
		static F64 clamp(F64 v, F64 hi, F64 low);
		static U32 clamp(U32 v, U32 hi, U32 low);
		static I32 clamp(I32 v, I32 hi, I32 low);

		// Square root.
		static F32 sqrt(F32);
		static F64 sqrt(F64);

		// reciprocal square root.
		static F32 recipSqrt(F32);
		static F64 recipSqrt(F64);

		// Calculates x raised to the power of y.
		static F32 pow(F32 x, F32 y);
		static F64 pow(F64 x, F64 y);

		// Calculates e^n
		static F32 exp(F32);
		static F64 exp(F64);

		// Calculates logarithms.
		static F32 logE(F32);
		static F64 logE(F64);
		static F32 log2(F32);
		static F64 log2(F64);
		static F32 log10(F32);
		static F64 log10(F64);

		// Converts degrees to radians.
		static F32 degToRad(F32);
		static F64 degToRad(F64);

		// Converts radians to degrees.
		static F32 radToDeg(F32);
		static F64 radToDeg(F64);

		// Sine of an angle.
		static F32 sin(F32);
		static F64 sin(F64);

		// Cosine of an angle.
		static F32 cos(F32);
		static F64 cos(F64);

		// Computes both the sin and cos.
		static VOID sinCos(F32, F32 & sin, F32 & cos);
		static VOID sinCos(F64, F64 & sin, F64 & cos);

		// Tangent of an angle.
		static F32 tan(F32);
		static F64 tan(F64);

		// Arcsine : Returns angle between -PI/2 and PI/2 in radians
		static F32 asin(F32);
		static F64 asin(F64);

		// Arccosine : Returns angle between 0 and PI in radians
		static F32 acos(F32);
		static F64 acos(F64);

		// ArcTangent: Returns angle between -PI/2 and PI/2 in radians
		static F32 atan(F32);
		static F64 atan(F64);

		// Arctangent of (x/y) with correct sign. Returns angle between -PI and PI in radians
		static F32 atan2(F32 x, F32 y);
		static F64 atan2(F64 x, F64 y);

		// random numbers
		// uniform random number in [a,b]
		static F32 rand(F32 a,F32 b);
		static I32 rand(I32 a,I32 b);

		// hashing: hashes an array of n 32 bit values
		// to a 32 bit value.  Because the output bits
		// are uniformly distributed, the caller may mask
		// off some of the bits to index into a hash table
		// smaller than 2^32.
		static U32 hash(U32 * array, U32 n);
		static I32 hash32(I32);		
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>
#include <Quaternion.h>
#include <Matrix.h>
#include <Plane.h>
#include <Frustum.h>
#include <BoundingBox.h>

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////