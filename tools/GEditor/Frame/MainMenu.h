/*
 *  MainMenu.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __MainMenu__
#define __MainMenu__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

class MainMenu : public wxMenuBar
{
public:
	MainMenu();

	~MainMenu();	
	
private:
	wxMenu* mFileMenu;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __MainMenu__

//////////////////////////////////////////////////////////////////////////
