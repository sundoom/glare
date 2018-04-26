/*
 *  GFile.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Glare.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

GFile::GFile()
{
#ifdef _WIN32
	mFile = NULL;
#else
	mFile = -1;
#endif
	mOption = 0;
}

GFile::~GFile()
{
#ifdef _WIN32
	if(mFile) Close();
#else
	if(mFile!=-1) Close();
#endif
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Open
====================
*/
VOID GFile::Open(const CHAR *name, U32 option)
{
#ifdef _WIN32
	// check the file handle
	CHECK(mFile == NULL && "The file is opened and we need to close it!");	

	// store the option of the file
	mOption = option;
	switch(mOption)
	{
	case 0:	// open the file for reading
		{
			FILE *file = fopen( name, "rb" );
			if(file == NULL) GAssert(VA("Fail to open the file : %s", name));
			mFile = (HANDLE)file;			
		}
		break;
	case 1:	// open the file for writing
		{
			FILE *file = ::fopen(name, "wb");
			if(file == NULL) GAssert(VA("Fail to open the file : %s", name));
			mFile = (HANDLE)file;
		}
		break;
	case 2:	// open the file for reading and writing
		{
			FILE *file = ::fopen(name, "w+");
			if(file == NULL) GAssert(VA("Fail to open the file : %s", name));
			mFile = (HANDLE)file;
		}
		break;
	default:
		GAssert(VA("The file option is error : %s", name));
		break;
	}
#else
	// check the file handle
	CHECK(mFile == -1 && "The file is opened and we need to close it!");	

	// store the option of the file
	mOption = option;
	switch(mOption)
	{
	case 0:	// open the file for reading
		{
			I32 file = ::open(name, O_RDONLY, 00644);
			if(file == -1) GAssert(VA("Fail to open the file : %s", name));
			mFile = file;
		}
		break;
	case 1:	// open the file for writing
		{
			I32 file = ::open(name, O_WRONLY|O_CREAT, 00644);
			if(file == -1) GAssert(VA("Fail to open the file : %s", name));
			mFile = file;
		}
		break;
	case 2:	// open the file for reading and writing
		{
			I32 file = ::open(name, O_RDWR|O_CREAT, 00644);
			if(file == -1) GAssert(VA("Fail to open the file : %s", name));
			mFile = file;
		}
		break;
	default:
		GAssert(VA("The file option is error : %s", name));
		break;
	}
#endif
}

/*
====================
Read
====================
*/
U32 GFile::Read(VOID* data, U32 size)
{	
	CHECK(data);
	CHECK(mOption==0||mOption==2);
#ifdef _WIN32
	CHECK(mFile);
	return ::fread(data, 1, size, (FILE*)mFile);
#else
	CHECK(mFile!=-1);
	I32 result = ::read(mFile, data, size);
	CHECK(result != -1);
	return result;
#endif
}

/*
====================
Write
====================
*/
U32 GFile::Write(const VOID* data, U32 size)
{	
	CHECK(data);
	CHECK(mOption==1||mOption==2);
#ifdef _WIN32
	CHECK(mFile);
	return ::fwrite(data, 1, size, (FILE*)mFile);
#else
	CHECK(mFile!=-1);
	I32 result = ::write (mFile, data, size);
	CHECK(result != -1);
	return result;
#endif
}

/*
====================
Seek
====================
*/
U32 GFile::Seek(I32 offset, U32 origin)
{
#ifdef _WIN32
	CHECK(mFile);
	CHECK(::fseek((FILE*)mFile, offset, origin) == 0);
	return ::ftell((FILE*)mFile);
#else
	CHECK(mFile!=-1);
	I32 result = ::lseek (mFile, offset, origin);
	CHECK(result != -1);
	return result;
#endif
}

/*
====================
Close
====================
*/
VOID GFile::Close()
{
#ifdef _WIN32
	CHECK(mFile);
	::fclose((FILE*)mFile);
	mFile = NULL;	
#else
	CHECK(mFile!=-1);
	CHECK(::close(mFile) != -1);
	mFile = -1;
#endif
	mOption = 0;
}

//////////////////////////////////////////////////////////////////////////