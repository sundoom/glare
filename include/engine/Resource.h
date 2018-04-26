/*
 *  Resource.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Resource__
#define __Resource__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented Resource interface.  
	class Resource : public GReference
	{
  protected:
		// constructor
		Resource(const CHAR* name);
		
		// destructor
		virtual ~Resource();
  
		// load the data
		virtual VOID Load(const GData* data) = 0;

  protected:
    Str mName;

  public:
    template<typename T> 
    static T* Find(const CHAR* name)
    {
      CHECK(name);
      std::map<Str, Resource*>::iterator it = s_res_map.find(name);
      if(it != s_res_map.end()) return dynamic_cast<T*>(it->second);
      T*res = GNEW(T(name));
      res->Load((GData*)GLoad(name));
      return res;
    }
  private:		
    static VOID Add(const CHAR* name, Resource* res);
    static VOID Remove(const CHAR* name);
  private:
    static std::map<Str, Resource*>s_res_map;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Resource__

//////////////////////////////////////////////////////////////////////////