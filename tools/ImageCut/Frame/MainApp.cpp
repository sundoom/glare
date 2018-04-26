/*
 *  MainApp.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#include "MainApp.h"

//////////////////////////////////////////////////////////////////////////

class MainApp : public wxApp
{
public:
	bool OnInit()
	{
/*		wxFrame* frame = new MainFrame(NULL, wxID_ANY, wxT("Glow Editor"), wxDefaultPosition, wxSize(800, 600));
		SetTopWindow(frame);
		frame->Center();
		frame->Show();
    */
		return true;
	}

	int OnExit() 
	{ 
		return 0; 
	}
};

//////////////////////////////////////////////////////////////////////////

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)

//////////////////////////////////////////////////////////////////////////