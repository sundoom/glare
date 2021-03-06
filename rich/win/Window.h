/*
 *  Window.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Window__
#define __Window__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>

//////////////////////////////////////////////////////////////////////////

typedef GPtr<class Window>WindowPtr;

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented window interface.
class Window : public GReference
{
public:
	// constructor
	Window();

	// destructor
	virtual ~Window();

	// get the handle
	HWND handle() const;

	// set the window text
	VOID SetText(const CHAR* text);

	// move the window 
	VOID move(U32 w, U32 h, BOOL fullscreen = FALSE);

public:
	// the window`s proc
	static LONG CALLBACK wndProc( HWND, UINT, WPARAM, LPARAM );

private:
	HWND mWnd;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Window__

//////////////////////////////////////////////////////////////////////////