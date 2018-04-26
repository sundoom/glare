/*
 *  IndexData.cpp
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

  IndexData::IndexData()
	{
    mHandle = NULL;
	}

	IndexData::~IndexData() 
	{
    if(mHandle)
    {
      RIDestroyIndexBuffer(mHandle);
      mHandle = NULL;
    }
	}

  //////////////////////////////////////////////////////////////////////////

  /*
	====================
	Load
	====================
	*/
	VOID IndexData::Load(U32 count, U32 stride, const VOID* data)
	{
    CHECK(data);
    
    if(mHandle == NULL)
    {
      mHandle = RICreateIndexBuffer(count, stride, data);
      mCount = count;
      mStride = stride;
    }
    else
    {
      RIUpdateIndexBuffer(mHandle, count, stride, data);
    }

    mCount = count;
    mStride = stride;
  }

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
