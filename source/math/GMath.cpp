/*
 *  GMath.cpp
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
	
	BOOL GMath::equals(F32 a,F32 b,F32 eps)
	{
		const F32 diff = GMath::abs(a - b);
		return (diff < eps ? TRUE : FALSE);
	}

	BOOL GMath::equals(F64 a,F64 b,F64 eps)
	{
		const F64 diff = GMath::abs(a - b);
		return (diff < eps ? TRUE : FALSE);
	}

	F32 GMath::floor(F32 a)
	{
		return ::floorf(a);
	}

	F64 GMath::floor(F64 a)
	{
		return ::floor(a);
	}

	F32 GMath::ceil(F32 a)
	{
		return ::ceilf(a);
	}

	F64 GMath::ceil(F64 a)
	{
		return ::ceil(a);
	}

	I32 GMath::trunc(F32 a)
	{
		return (I32) a;	// ### PT: this actually depends on FPU settings
	}

	I32 GMath::trunc(F64 a)
	{
		return (I32) a;	// ### PT: this actually depends on FPU settings
	}

	F32 GMath::abs(F32 a)
	{
		return ::fabsf(a);
	}

	F64 GMath::abs(F64 a)
	{
		return ::fabs(a);
	}

	I32 GMath::abs(I32 a)
	{
		return ::abs(a);
	}

	F32 GMath::sign(F32 a)
	{
		return (a >= 0.0f) ? 1.0f : -1.0f;
	}

	F64 GMath::sign(F64 a)
	{
		return (a >= 0.0) ? 1.0 : -1.0;
	}

	I32 GMath::sign(I32 a)
	{
		return (a >= 0) ? 1 : -1;
	}

	F32 GMath::max(F32 a,F32 b)
	{
		return (a < b) ? b : a;
	}

	F64 GMath::max(F64 a,F64 b)
	{
		return (a < b) ? b : a;
	}

	I32 GMath::max(I32 a,I32 b)
	{
		return (a < b) ? b : a;
	}

	U32 GMath::max(U32 a,U32 b)
	{
		return (a < b) ? b : a;
	}

	F32 GMath::min(F32 a,F32 b)
	{
		return (a < b) ? a : b;
	}

	F64 GMath::min(F64 a,F64 b)
	{
		return (a < b) ? a : b;
	}

	I32 GMath::min(I32 a,I32 b)
	{
		return (a < b) ? a : b;
	}

	U32 GMath::min(U32 a,U32 b)
	{
		return (a < b) ? a : b;
	}

	F32 GMath::mod(F32 x, F32 y)
	{
		return (F32)::fmod(x,y);
	}

	F64 GMath::mod(F64 x, F64 y)
	{
		return ::fmod(x,y);
	}

	F32 GMath::clamp(F32 v, F32 hi, F32 low)
	{
		if (v > hi) 
			return hi;
		else if (v < low) 
			return low;
		else
			return v;
	}

	F64 GMath::clamp(F64 v, F64 hi, F64 low)
	{
		if (v > hi) 
			return hi;
		else if (v < low) 
			return low;
		else
			return v;
	}

	U32 GMath::clamp(U32 v, U32 hi, U32 low)
	{
		if (v > hi) 
			return hi;
		else if (v < low) 
			return low;
		else
			return v;
	}

	I32 GMath::clamp(I32 v, I32 hi, I32 low)
	{
		if (v > hi) 
			return hi;
		else if (v < low) 
			return low;
		else
			return v;
	}

	F32 GMath::sqrt(F32 a)
	{
		return ::sqrtf(a);
	}

	F64 GMath::sqrt(F64 a)
	{
		return ::sqrt(a);
	}

	F32 GMath::recipSqrt(F32 a)
	{
		return 1.0f/::sqrtf(a);
	}

	F64 GMath::recipSqrt(F64 a)
	{
		return 1.0/::sqrt(a);
	}

	F32 GMath::pow(F32 x, F32 y)
	{
		return ::powf(x,y);
	}

	F64 GMath::pow(F64 x, F64 y)
	{
		return ::pow(x,y);
	}

	F32 GMath::exp(F32 a)
	{
		return ::expf(a);
	}

	F64 GMath::exp(F64 a)
	{
		return ::exp(a);
	}

	F32 GMath::logE(F32 a)
	{
		return ::logf(a);
	}

	F64 GMath::logE(F64 a)
	{
		return ::log(a);
	}

	F32 GMath::log2(F32 a)
	{
		const F32 ln2 = (F32)0.693147180559945309417;
		return ::logf(a) / ln2;
	}

	F64 GMath::log2(F64 a)
	{
		const F64 ln2 = (F64)0.693147180559945309417;
		return ::log(a) / ln2;
	}

	F32 GMath::log10(F32 a)
	{
		return (F32)::log10(a);
	}

	F64 GMath::log10(F64 a)
	{
		return ::log10(a);
	}

	F32 GMath::degToRad(F32 a)
	{
		return (F32)0.01745329251994329547 * a;
	}

	F64 GMath::degToRad(F64 a)
	{
		return (F64)0.01745329251994329547 * a;
	}

	F32 GMath::radToDeg(F32 a)
	{
		return (F32)57.29577951308232286465 * a;
	}

	F64 GMath::radToDeg(F64 a)
	{
		return (F64)57.29577951308232286465 * a;
	}

	F32 GMath::sin(F32 a)
	{
		return ::sinf(a);
	}

	F64 GMath::sin(F64 a)
	{
		return ::sin(a);
	}

	F32 GMath::cos(F32 a)
	{
		return ::cosf(a);
	}

	F64 GMath::cos(F64 a)
	{
		return ::cos(a);
	}

	VOID GMath::sinCos(F32 a, F32 & s, F32 & c)
	{
		s = ::sinf(a);
		c = ::cosf(a);
	}

	VOID GMath::sinCos(F64 a, F64 & s, F64 & c)
	{
		s = ::sin(a);
		c = ::cos(a);
	}

	F32 GMath::tan(F32 a)
	{
		return ::tanf(a);
	}

	F64 GMath::tan(F64 a)
	{
		return ::tan(a);
	}

	F32 GMath::asin(F32 f)
	{
		// Take care of FPU inaccuracies
		if(f>=1.0f)	return (F32)HALF_PI;
		if(f<=-1.0f)return -(F32)HALF_PI;
		return ::asinf(f);
	}

	F64 GMath::asin(F64 f)
	{
		// Take care of FPU inaccuracies
		if(f>=1.0)	return (F32)HALF_PI;
		if(f<=-1.0)	return -(F32)HALF_PI;
		return ::asin(f);
	}

	F32 GMath::acos(F32 f)
	{
		// Take care of FPU inaccuracies
		if(f>=1.0f)	return 0.0f;
		if(f<=-1.0f)return (F32)PI;
		return ::acosf(f);
	}

	F64 GMath::acos(F64 f)
	{
		// Take care of FPU inaccuracies
		if(f>=1.0)	return 0.0;
		if(f<=-1.0)	return (F64)PI;
		return ::acos(f);
	}

	F32 GMath::atan(F32 a)
	{
		return ::atanf(a);
	}

	F64 GMath::atan(F64 a)
	{
		return ::atan(a);
	}

	F32 GMath::atan2(F32 x, F32 y)
	{
		return ::atan2f(x,y);
	}

	F64 GMath::atan2(F64 x, F64 y)
	{
		return ::atan2(x,y);
	}

	F32 GMath::rand(F32 a,F32 b)
	{
		const F32 r = (F32)::rand()/((F32)RAND_MAX+1);
		return r*(b-a) + a;
	}

	I32 GMath::rand(I32 a,I32 b)
	{
		return a + ( ((I32)::rand() * (b - a)) /((I32)RAND_MAX) );
	}

	/*
	--------------------------------------------------------------------
	lookup2.c, by Bob Jenkins, December 1996, Public Domain.
	hash(), hash2(), hash3, and mix() are externally useful functions.
	Routines to test the hash are included if SELF_TEST is defined.
	You can use this free for any purpose.  It has no warranty.
	--------------------------------------------------------------------
	--------------------------------------------------------------------
	mix -- mix 3 32-bit values reversibly.
	For every delta with one or two bit set, and the deltas of all three
	high bits or all three low bits, whether the original value of a,b,c
	is almost all zero or is uniformly distributed,
	* If mix() is run forward or backward, at least 32 bits in a,b,c
	have at least 1/4 probability of changing.
	* If mix() is run forward, every bit of c will change between 1/3 and
	2/3 of the time.  (Well, 22/100 and 78/100 for some 2-bit deltas.)
	mix() was built out of 36 single-cycle latency instructions in a 
	structure that could supported 2x parallelism, like so:
	a -= b; 
	a -= c; x = (c>>13);
	b -= c; a ^= x;
	b -= a; x = (a<<8);
	c -= a; b ^= x;
	c -= b; x = (b>>13);
	...
	Unfortunately, superscalar Pentiums and Sparcs can't take advantage 
	of that parallelism.  They've also turned some of those single-cycle
	latency instructions into multi-cycle latency instructions.  Still,
	this is the fastest good hash I could find.  There were about 2^^68
	to choose from.  I only looked at a billion or so.
	--------------------------------------------------------------------
	*/
	#define HASH_MIX(a,b,c) \
	{ \
	a -= b; a -= c; a ^= (c>>13); \
	b -= c; b -= a; b ^= (a<<8); \
	c -= a; c -= b; c ^= (b>>13); \
	a -= b; a -= c; a ^= (c>>12);  \
	b -= c; b -= a; b ^= (a<<16); \
	c -= a; c -= b; c ^= (b>>5); \
	a -= b; a -= c; a ^= (c>>3);  \
	b -= c; b -= a; b ^= (a<<10); \
	c -= a; c -= b; c ^= (b>>15); \
	}

	/*
	--------------------------------------------------------------------
	This works on all machines.  hash2() is identical to hash() on 
	little-endian machines, except that the length has to be measured
	in ub4s instead of bytes.  It is much faster than hash().  It 
	requires
	-- that the key be an array of ub4's, and
	-- that all your machines have the same endianness, and
	-- that the length be the number of ub4's in the key
	--------------------------------------------------------------------
	*/
	U32 GMath::hash(  U32 *k, U32 length)
		//register ub4 *k;        /* the key */
		//register ub4  length;   /* the length of the key, in ub4s */
	{
		U32 a,b,c,len;

		/* Set up the internal state */
		len = length;
		a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
		c = 0;           /* the previous hash value */

		/*---------------------------------------- handle most of the key */
		while (len >= 3)
		{
			a += k[0];
			b += k[1];
			c += k[2];
			HASH_MIX(a,b,c);
			k += 3; len -= 3;
		}

		/*-------------------------------------- handle the last 2 ub4's */
		c += length;
		switch(len)              /* all the case statements fall through */
		{
			/* c is reserved for the length */
		case 2 : b+=k[1];
		case 1 : a+=k[0];
			/* case 0: nothing left to add */
		}
		HASH_MIX(a,b,c);
		/*-------------------------------------------- report the result */
		return c;
	}
	#undef HASH_MIX

	I32 GMath::hash32(I32 key)
	{
		key += ~(key << 15);
		key ^=  (key >> 10);
		key +=  (key << 3);
		key ^=  (key >> 6);
		key += ~(key << 11);
		key ^=  (key >> 16);
		return key;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
