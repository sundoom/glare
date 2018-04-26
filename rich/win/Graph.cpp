/*
 *  Graph.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "Graph.h"
#include <Render.h>
NAMESPACE_USE(Render)

#include <EGL/egl.h>
#pragma comment(lib,"libEGL.lib")

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

Graph::Graph()
{
  
}

Graph::~Graph()
{

}

//////////////////////////////////////////////////////////////////////////

#ifdef USE_RENDER_THREAD

//////////////////////////////////////////////////////////////////////////

/*
====================
Init
====================
*/
VOID Graph::Init(HANDLE wnd)
{
  CHECK(wnd);
  mWnd = wnd;

  // start the graph thread
  GThread::Start();

  // reset the render state
  RIReset(0);
}

/*
====================
Exit
====================
*/
VOID Graph::Exit()
{
  // end the graph thread
  GThread::End();
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Swap
====================
*/
VOID Graph::Swap()
{
	AUTO_LOCK_MUTEX(&mMutex);
  RISwap();
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Run
====================
*/
VOID Graph::Run()
{
  // initialize egl
  EGLDisplay display = EGL_NO_DISPLAY; 
  EGLSurface surface = EGL_NO_SURFACE; 
  EGLContext context = EGL_NO_CONTEXT;
  {
    AUTO_LOCK_MUTEX(&mMutex);
    display = eglGetDisplay((EGLNativeDisplayType)::GetDC((HWND)mWnd));
    CHECK(display!=EGL_NO_DISPLAY);  
    EGLint major, minor;
    CHECK(eglInitialize(display, &major, &minor));  
    EGLint num_configs;
    CHECK(eglGetConfigs(display, NULL, 0, &num_configs));  
    EGLint attribs[] =
    {
      EGL_RED_SIZE,       5,
      EGL_GREEN_SIZE,     6,
      EGL_BLUE_SIZE,      5,
      EGL_ALPHA_SIZE,     EGL_DONT_CARE,
      EGL_DEPTH_SIZE,     EGL_DONT_CARE,
      EGL_STENCIL_SIZE,   EGL_DONT_CARE,
      EGL_SAMPLE_BUFFERS, 0,
      EGL_NONE
    };
    EGLConfig config;
    CHECK(eglChooseConfig(display, attribs, &config, 1, &num_configs));  
    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)mWnd, NULL);
    CHECK(surface!=EGL_NO_SURFACE);  
    EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs);
    CHECK(context!=EGL_NO_CONTEXT);
    CHECK(eglMakeCurrent(display, surface, surface, context));
    CHECK(eglSwapInterval(display,0));
  }

  // the thread running
  while(mRunning)
  {
		// exec the render command
		{
			AUTO_LOCK_MUTEX(&mMutex);
			RIExec();
		}

		// swap buffers
		CHECK(eglSwapBuffers(display, surface));
    GSleep(0.001f);
  }

  // clean all of the render command
  while(RIExec());
  CHECK(eglSwapBuffers(display, surface));
  
  // release the egl  
  if(context!=EGL_NO_CONTEXT)eglDestroyContext(display, context);		
  if(context!=EGL_NO_SURFACE)eglDestroySurface(display, surface);
  eglTerminate(display);
}

//////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////

/*
====================
Init
====================
*/
VOID Graph::Init(HANDLE wnd)
{
  CHECK(wnd);
  
  // Get Display
  mDisplay = eglGetDisplay(::GetDC((HWND)wnd));
  CHECK(mDisplay!=EGL_NO_DISPLAY);

  // Initialize EGL
  EGLint major, minor;
  CHECK(eglInitialize(mDisplay, &major, &minor));

  // Get configs
  EGLint num_configs;
  CHECK(eglGetConfigs(mDisplay, NULL, 0, &num_configs));

  // Choose config
  EGLint attribs[] =
  {
    EGL_RED_SIZE,       5,
    EGL_GREEN_SIZE,     6,
    EGL_BLUE_SIZE,      5,
    EGL_ALPHA_SIZE,     EGL_DONT_CARE,
    EGL_DEPTH_SIZE,     EGL_DONT_CARE,
    EGL_STENCIL_SIZE,   EGL_DONT_CARE,
    EGL_SAMPLE_BUFFERS, 0,
    EGL_NONE
  };
  EGLConfig config;
  CHECK(eglChooseConfig(mDisplay, attribs, &config, 1, &num_configs));

  // Create a surface
  mSurface = eglCreateWindowSurface(mDisplay, config, (EGLNativeWindowType)wnd, NULL);
  CHECK(mSurface!=EGL_NO_SURFACE);

  // Create a GL context
  EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
  mContext = eglCreateContext(mDisplay, config, EGL_NO_CONTEXT, context_attribs);
  CHECK(mContext!=EGL_NO_CONTEXT);

  // Make the mContext current
  CHECK(eglMakeCurrent(mDisplay, mSurface, mSurface, mContext));
  CHECK(eglSwapInterval(mDisplay,0));

  // reset the render state
  RIReset(0);
}

/*
====================
Exit
====================
*/
VOID Graph::Exit()
{
  // release the egl
  if(mDisplay!=EGL_NO_DISPLAY) 
  {
    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if(mContext!=EGL_NO_CONTEXT)eglDestroyContext(mDisplay, mContext);		
    if(mContext!=EGL_NO_SURFACE)eglDestroySurface(mDisplay, mSurface);
    eglTerminate(mDisplay);
  }
}

//////////////////////////////////////////////////////////////////////////
/*
====================
Swap
====================
*/
VOID Graph::Swap()
{
  CHECK(eglSwapBuffers(mDisplay, mSurface));
}

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////