/*
 *  GPtr.h
 *  glare
 *
 *  Created by sunjun on 11-6-10.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __GPtr__
#define __GPtr__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>

//////////////////////////////////////////////////////////////////////////
// smart pointer for handling referenced counted objects.
template<class T>
class GPtr
{
private: 
	typedef T* GPtr::*bool_type;
	T* mPtr;

public: 
	typedef T element_type;

	GPtr() : mPtr(0)
	{
		
	}

	GPtr(T* ptr) : mPtr(ptr) 
	{
		if(mPtr) mPtr->IncRef();
	}

	GPtr(const GPtr& rp) : mPtr(rp.mPtr) 
	{
		if(mPtr) mPtr->IncRef();
	}

	template<class O> 
	GPtr(const GPtr<O>& rp) : mPtr(rp.mPtr) 
	{ 
		if(mPtr) mPtr->IncRef();
	}

	~GPtr() 
	{
		if(mPtr) mPtr->DecRef();  
		mPtr = 0;
	}

	GPtr& operator = (const GPtr& rp)
	{
		if(mPtr == rp.mPtr) return *this;
		T* tmp_ptr = mPtr;
		mPtr = rp.mPtr;
		if(mPtr) mPtr->IncRef();
		if(tmp_ptr) tmp_ptr->DecRef();
		return *this;
	}

	template<class O> 
	GPtr& operator = (const GPtr<O>& rp)
	{
		if(mPtr == rp.mPtr) return *this;
		T* tmp_ptr = mPtr;
		mPtr = rp.mPtr;
		if(mPtr) mPtr->IncRef();
		if(tmp_ptr) tmp_ptr->DecRef();
		return *this;
	}

	GPtr& operator = (T* ptr)
	{
		if(mPtr == ptr) return *this;
		T* tmp_ptr = mPtr;
		mPtr = ptr;
		if(mPtr) mPtr->IncRef();
		if(tmp_ptr) tmp_ptr->DecRef();
		return *this;
	}

	BOOL operator == (const GPtr& rp) const 
	{
		return (mPtr == rp.mPtr) ? TRUE : FALSE;
	}

	BOOL operator == (const T* ptr) const 
	{
    return (mPtr == ptr) ? TRUE : FALSE;
	}

	friend BOOL operator == (const T* ptr, const GPtr& rp) 
	{
		return (ptr == rp.mPtr) ? TRUE : FALSE;
	}

	BOOL operator != (const GPtr& rp) const 
	{
		return (mPtr != rp.mPtr) ? TRUE : FALSE;
	}

	BOOL operator != (const T* ptr) const 
	{
		return (mPtr != ptr) ? TRUE : FALSE;
	}

	friend BOOL operator != (const T* ptr, const GPtr& rp) 
	{
		return ( ptr != rp.mPtr ) ? TRUE : FALSE;
	}

	T& operator*() const 
	{
    return *mPtr;
	}

	T* operator->() const 
	{
		return mPtr;
	}

	T* Ptr() const 
	{
		return mPtr;
	}

	BOOL operator!() const
	{
		return mPtr == 0 ? TRUE : FALSE;
	} 

	BOOL Valid() const       
	{
		return mPtr != 0 ? TRUE : FALSE;
	}

	VOID Release() 
	{
		if(mPtr) mPtr->DecRef(); 
		mPtr = 0;
	}

	operator bool_type() const 
	{ 
		return mPtr != 0 ? &GPtr::mPtr : 0; 
	}
};

//////////////////////////////////////////////////////////////////////////

template<class T, class Y> GPtr<T> static_ptr_cast(const GPtr<Y>& rp) 
{ 
	return static_cast<T*>(rp.Ptr());
}

template<class T, class Y> GPtr<T> dynamic_ptr_cast(const GPtr<Y>& rp) 
{ 
	return dynamic_cast<T*>(rp.Ptr());
}

template<class T, class Y> GPtr<T> const_ptr_cast(const GPtr<Y>& rp) 
{ 
	return const_cast<T*>(rp.Ptr()); 
}

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GPtr__

//////////////////////////////////////////////////////////////////////////