/*
 *  IndexBuffer.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "IndexBuffer.h"
#include "State.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////		

	IndexBuffer::IndexBuffer()
	{
		mObject = 0;
		mCount = 0;
		mStride = 0;
	}

	IndexBuffer::~IndexBuffer() 
	{
    if(mObject){glDeleteBuffers(1, &mObject);mObject = 0;}
	}	

	//////////////////////////////////////////////////////////////////////////
	
	/*
	====================
	Load
	====================
	*/
	VOID IndexBuffer::Load(U32 count, U32 stride, const VOID* data)
	{
    CHECK(data);

    // store the info
    mCount = count;		
    mStride = stride;

    if(mObject == 0)
    {
      // generate the buffer object
      glGenBuffers(1, &mObject); CHECK(mObject);

      // bind the buffer object
      BindIndexBuffer(mObject);

      // commit the data to the gpu memory
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount*mStride, data, GL_STATIC_DRAW);
    }
    else
    {
      // bind the buffer object
      BindIndexBuffer(mObject);

      // commit the data to the gpu memory
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mCount*mStride, data);
    }
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Bind
	====================
	*/
	VOID IndexBuffer::Bind()
	{
    // bind the buffer object
    BindIndexBuffer(mObject);
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
