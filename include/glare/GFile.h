/*
 *  GFile.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __GFile__
#define __GFile__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

typedef GPtr<class GFile>GFilePtr;

//////////////////////////////////////////////////////////////////////////

#pragma pack (push,1)

typedef enum _FILE_SEEK
{ 
	FS_SET	= 0,
	FS_CUR	= 1,
	FS_END	= 2,	
}FILE_SEEK;

#pragma pack (pop)

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented file interface.
class GFile : public GReference
{
public:
	// constructor
	GFile();

	// destructor
	virtual ~GFile();

	// open a file
	VOID Open(const CHAR *name, U32 option = 0);

	// read the data from the file
	U32 Read(VOID* data, U32 size);

	// write the data to the file
	U32 Write(const VOID* data, U32 size);

	// seek the offset in the file
	U32 Seek(I32 offset, U32 origin = 0);

	// close the file
	VOID Close();		

private:
#ifdef _WIN32
	HANDLE mFile;
#else
	I32 mFile;
#endif
	U32	mOption;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GFile__

//////////////////////////////////////////////////////////////////////////