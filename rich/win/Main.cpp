/*
 *  Main.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>
#include <Engine.h>
NAMESPACE_USE(Engine)

#include "Window.h"
#include "Graph.h"

#include "../game/Game.h"
NAMESPACE_USE(Rich)

//////////////////////////////////////////////////////////////////////////

static WindowPtr gWndPtr;
static U32 gWndWidth = 1024;
static U32 gWndHeight = 768;
static Graph* gGraphPtr = NULL;

//////////////////////////////////////////////////////////////////////////

static VOID appInit();
static VOID appRun();
static VOID appExit();

//////////////////////////////////////////////////////////////////////////

/*
====================
main
====================
*/ 
I32 main(I32 argc, CHAR** argv)
{
	// register the window class
	WNDCLASSEX wcex;	
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)Window::wndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= ::GetModuleHandle(NULL);
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)WHITE_BRUSH;
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= "GWindow";
	wcex.hIconSm		= 0;
	CHECK(::RegisterClassEx(&wcex));

	appInit();

	// the message loop
	MSG msg; ::memset(&msg,0,sizeof(msg));
	while(msg.message != WM_QUIT) 
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		} 
		else 
		{
			appRun();

			// sleep it
      GSleep(0.001f);
		}
	}

	appExit();	

	// unregister the window class
	::UnregisterClass("GWindow", ::GetModuleHandle(NULL));

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
====================
appInit
====================
*/
static VOID appInit()
{
  // the audio instance
  Audio::instance();

	// create the window
	gWndPtr = GNEW(Window); CHECK(gWndPtr);
	gWndPtr->SetText("Glare");
	gWndPtr->move(gWndWidth, gWndHeight);

  // create the grap
  gGraphPtr = GNEW(Graph); CHECK(gGraphPtr);
  gGraphPtr->Init((HANDLE)gWndPtr->handle());

  // init the game
	Game::instance().init();
	Game::instance().window(gWndWidth,gWndHeight);
}

/*
====================
appRun
====================
*/
static VOID appRun()
{
  // get the frame time
  static F64 system_time = GSeconds();
  F64 current_time = GSeconds();
  F64 frame_time = current_time - system_time;
  if(frame_time == 0) return;
  system_time = current_time;

  // run the game
	Game::instance().run(frame_time);

  // swap buffers
  gGraphPtr->Swap();

  // update the audio
  Audio::instance().Update((F32)frame_time);
}

/*
====================
appExit
====================
*/
static VOID appExit()
{
	// exit the game
	Game::instance().exit();

  // release the graph
  if(gGraphPtr)
  {
		gGraphPtr->Swap();
    gGraphPtr->Exit();
    GDELETE(gGraphPtr);
    gGraphPtr = NULL;
  }

	// release the window
	if(gWndPtr) gWndPtr.Release();
}

//////////////////////////////////////////////////////////////////////////

/*
====================
GLoad
====================
*/
const GData* GLoad(const CHAR* path)
{
	GFile file;
	file.Open(path);
	U32 pos = file.Seek(0,FS_CUR);
	U32 size = file.Seek(0,FS_END);	
	file.Seek(pos,FS_SET);
	GData* data = GNEW(GData(size+1,0)); CHECK(data);
	CHECK(file.Read(data->Ptr(),data->Size())==size);
	file.Close();
	return data;
}

//////////////////////////////////////////////////////////////////////////