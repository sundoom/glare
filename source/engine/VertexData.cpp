/*
 *  VertexData.cpp
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

  VertexData::VertexData()
	{
    mHandle = NULL;
	}

	VertexData::~VertexData() 
	{
    if(mHandle)
    {
      RIDestroyVertexBuffer(mHandle);
      mHandle = NULL;
    }
	}

  //////////////////////////////////////////////////////////////////////////

  /*
	====================
	Load
	====================
	*/
	VOID VertexData::Load(U32 count, U32 stride, U8 attrib[16], const VOID* data)
	{
		CHECK(data);

    if(mHandle == NULL)
    {
      mHandle = RICreateVertexBuffer(count, stride, attrib, data);
    }
    else
    {
      RIUpdateVertexBuffer(mHandle, count, stride, attrib, data);
    }
  }

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
