/*
 *  Window.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "Window.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

Window::Window()
{
	U32 ex_style = 0;		
	U32 style = WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_VISIBLE|WS_SYSMENU|WS_MINIMIZEBOX;

	// adjust the window
	I32 screen_width = ::GetSystemMetrics(SM_CXSCREEN);
	I32 screen_height = ::GetSystemMetrics(SM_CYSCREEN);

	// calculate overall dimensions for requested client area
	RECT rc = { 0, 0, 800, 600 };
	::AdjustWindowRect( &rc, style, FALSE );

	// clamp window dimensions to screen size
	U32 wnd_width = (rc.right-rc.left < screen_width)? rc.right-rc.left : screen_width;
	U32 wnd_height = (rc.bottom-rc.top < screen_height)? rc.bottom-rc.top : screen_height;

	// keep window contained in visible screen area
	U32 x = (screen_width - wnd_width) / 2;
	U32 y = (screen_height - wnd_height) / 2;
	if(x > screen_width - wnd_width) x = screen_width - wnd_width;
	if(y > screen_height - wnd_height) y = screen_height - wnd_height;	

	// create the window
	mWnd = ::CreateWindowEx(ex_style, "GWindow", "Window",style,
		x,y, wnd_width,wnd_height,NULL,NULL,::GetModuleHandle(NULL), NULL);
	CHECK(mWnd);
	::ShowWindow(mWnd, SW_SHOW);
	::UpdateWindow(mWnd);
	::SetForegroundWindow(mWnd);
	::SetFocus(mWnd);
}

Window::~Window()
{
	if(mWnd){::DestroyWindow(mWnd); mWnd = NULL;}
}

//////////////////////////////////////////////////////////////////////////

/*
====================
wndProc
====================
*/
LONG CALLBACK Window::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;		
	case WM_KEYDOWN:
		{
			if( wParam == VK_ESCAPE ) { PostQuitMessage(0); break; }
		//	Application::instance().onKeyEvent((INPUT_KEY)wParam, INPUT_ACTION::IA_Press);
		}
		break;
	case WM_KEYUP:
		{
		//	Application::instance().onKeyEvent((INPUT_KEY)wParam, INPUT_ACTION::IA_Release);
		}
		break;
	case WM_MOUSEMOVE:
		{
			I32 mx = ((I32)(I16)LOWORD(lParam));
			I32 my = ((I32)(I16)HIWORD(lParam));
		//	Application::instance().onMouseEvent(0,mx,my,0);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			I32 mx = ((I32)(I16)LOWORD(lParam));
			I32 my = ((I32)(I16)HIWORD(lParam));
		//	Application::instance().onMouseEvent(1,mx,my,0);
		}
		break;
	case WM_LBUTTONUP:
		{
			I32 mx = ((I32)(I16)LOWORD(lParam));
			I32 my = ((I32)(I16)HIWORD(lParam));
		//	Application::instance().onMouseEvent(2,mx,my,0);
		}
		break;
	case WM_MBUTTONDOWN:
		{
			I32 mx = ((I32)(I16)LOWORD(lParam));
			I32 my = ((I32)(I16)HIWORD(lParam));
		//	Application::instance().onMouseEvent(3,mx,my,0);
		}
		break;
	case WM_MBUTTONUP:
		{
			I32 mx = ((I32)(I16)LOWORD(lParam));
			I32 my = ((I32)(I16)HIWORD(lParam));
		//	Application::instance().onMouseEvent(4,mx,my,0);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			I32 mx = ((I32)(I16)LOWORD(lParam));
			I32 my = ((I32)(I16)HIWORD(lParam));
		//	Application::instance().onMouseEvent(5,mx,my,0);
		}
		break;
	case WM_RBUTTONUP:
		{
			I32 mx = ((I32)(I16)LOWORD(lParam));
			I32 my = ((I32)(I16)HIWORD(lParam));
		//	Application::instance().onMouseEvent(6,mx,my,0);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			I16 z_delta = GET_WHEEL_DELTA_WPARAM(wParam);
			I32 mx = ((I32)(I16)LOWORD(lParam));
			I32 my = ((I32)(I16)HIWORD(lParam));
		//	Application::instance().onMouseEvent(7,mx,my,z_delta);
		}
		break;
	case WM_CHAR:
		{
		//	Application::instance().onCharEvent(wParam);
		}
		break;
	default:
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
handle
====================
*/
HWND Window::handle() const
{
	return mWnd;
}

/*
====================
Text
====================
*/
VOID Window::SetText( const CHAR* name )
{
	CHECK(mWnd);
	CHECK(::SetWindowText(mWnd,name));
}

/*
====================
move
====================
*/
VOID Window::move( U32 w, U32 h, BOOL fullscreen )
{
	CHECK(mWnd);

	if( fullscreen )
	{
		U32 ex_style = WS_EX_TOPMOST;
		U32 style = WS_POPUP | WS_VISIBLE;
		U32 x = 0, y = 0;
		U32 wnd_width = w;
		U32 wnd_height = h;
		::SetWindowLong(mWnd,GWL_STYLE,style);
		::SetWindowLong(mWnd,GWL_EXSTYLE,ex_style);
		::SetWindowPos(mWnd,HWND_TOPMOST,x,y,wnd_width,wnd_height,SWP_NOACTIVATE);
	}
	else
	{
		U32 ex_style = 0;		
		U32 style = WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_VISIBLE|WS_SYSMENU|WS_MINIMIZEBOX;
		// U32 style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE;

		I32 screen_width = ::GetSystemMetrics(SM_CXSCREEN);
		I32 screen_height = ::GetSystemMetrics(SM_CYSCREEN);

		// calculate overall dimensions for requested client area
		RECT rc = { 0, 0, w, h };
		::AdjustWindowRect(&rc, style, FALSE);

		// clamp window dimensions to screen size
		U32 wnd_width = (rc.right-rc.left < screen_width)? rc.right-rc.left : screen_width;
		U32 wnd_height = (rc.bottom-rc.top < screen_height)? rc.bottom-rc.top : screen_height;

		// center window if given negative coordinates
		U32 x = (screen_width - wnd_width) / 2;
		U32 y = (screen_height - wnd_height) / 2;

		// keep window contained in visible screen area
		if(x > screen_width - wnd_width) x = screen_width - wnd_width;
		if(y > screen_height - wnd_height) y = screen_height - wnd_height;

		::SetWindowLong(mWnd,GWL_STYLE,style);
		::SetWindowLong(mWnd,GWL_EXSTYLE,ex_style);
		::SetWindowPos(mWnd,HWND_NOTOPMOST,x,y,wnd_width,wnd_height,SWP_DRAWFRAME|SWP_FRAMECHANGED|SWP_NOACTIVATE);
	}
}

//////////////////////////////////////////////////////////////////////////
