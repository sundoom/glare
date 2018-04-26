/*
 *  Resource.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////  
  
	Resource::Resource(const CHAR* name)
	{
    CHECK(name);
    mName = name;
    Add(name, this);
	}

  Resource::~Resource()
	{
    Remove(mName.c_str());
	}

  //////////////////////////////////////////////////////////////////////////

  std::map<Str, Resource*> Resource::s_res_map;

  //////////////////////////////////////////////////////////////////////////  
 
	/*
	====================
	add
	====================
	*/  
	VOID Resource::Add(const CHAR* name, Resource* res)
	{
    CHECK(name);
    if(s_res_map.find(name) != s_res_map.end()) GAssert(VA("The resource(%s) is existent already!", name));
    s_res_map.insert(std::make_pair(name, res));
	}

	/*
	====================
	remove
	====================
	*/
	VOID Resource::Remove(const CHAR* name)
	{
		CHECK(name);
    std::map<Str, Resource*>::iterator it = s_res_map.find(name);
    if(it == s_res_map.end()) GAssert(VA("Can`t find the resource : %s.", name));
    s_res_map.erase(it);
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
