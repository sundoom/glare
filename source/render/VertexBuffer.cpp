/*
 *  VertexBuffer.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "VertexBuffer.h"
#include "State.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer()
	{
		mObject = 0;
		mCount = 0;
		mStride = 0;
    ::memset(mAttrib, 0, sizeof(mAttrib));
	}

	VertexBuffer::~VertexBuffer() 
	{
		if(mObject){glDeleteBuffers(1, &mObject);mObject = 0;}
	}

	//////////////////////////////////////////////////////////////////////////	

	/*
	====================
	Load
	====================
	*/
	VOID VertexBuffer::Load(U32 count, U32 stride, U8 attrib[16], const VOID* data)
	{
		CHECK(data);

    // store the info
    mCount = count;		
    mStride = stride;
    ::memcpy(mAttrib, attrib, sizeof(mAttrib));

    // process the buffer
    if(mObject == 0)
    {
      // generate the buffer object
      glGenBuffers(1, &mObject); CHECK(mObject);

      // bind the buffer object      
      BindVertexBuffer(mObject);

      // commit the data to the gpu memory
      glBufferData(GL_ARRAY_BUFFER, mCount*mStride, data, GL_STATIC_DRAW);
    }
    else
    {
      // bind the buffer object
      BindVertexBuffer(mObject);

      // commit the data to the gpu memory
      glBufferSubData(GL_ARRAY_BUFFER, 0, mCount*mStride, data);
    }
	}

  /*
	====================
	Bind
	====================
	*/
	VOID VertexBuffer::Bind()
	{
    // get the attribute count
    U8 attrib_count = mAttrib[0];

    // bind the vertex attrib array
    BindVertexAttribArray(attrib_count);    

    // bind the buffer
    BindVertexBuffer(mObject);

    // bind the attribute
    U8* attrib_ptr = NULL;
    for(U8 i = 1; i <= attrib_count; i++)
    {
      U32 component; U32 data_type; U32 data_size;
      switch(mAttrib[i])
      {
      case VT_1I:
        component = 1;
        data_type = GL_INT;
        data_size = component * sizeof(I32);
        break;
      case VT_2I:
        component = 2;
        data_type = GL_INT;
        data_size = component * sizeof(I32);
        break;
      case VT_3I:
        component = 3;
        data_type = GL_INT;
        data_size = component * sizeof(I32);
        break;
      case VT_4I:
        component = 4;
        data_type = GL_INT;
        data_size = component * sizeof(I32);
        break;
      case VT_1F:
        component = 1;
        data_type = GL_FLOAT;
        data_size = component * sizeof(F32);
        break;
      case VT_2F:
        component = 2;
        data_type = GL_FLOAT;
        data_size = component * sizeof(F32);
        break;
      case VT_3F:
        component = 3;
        data_type = GL_FLOAT;
        data_size = component * sizeof(F32);
        break;
      case VT_4F:
        component = 4;
        data_type = GL_FLOAT;
        data_size = component * sizeof(F32);
        break;
      default:
        GAssert("The type of attrib is error!\n");
        break;
      }
      glVertexAttribPointer(i-1, component, data_type, GL_FALSE, mStride, attrib_ptr);
      attrib_ptr += data_size;
    }
  }
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
