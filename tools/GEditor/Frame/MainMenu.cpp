/*
 *  MainMenu.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#include "MainApp.h"

//////////////////////////////////////////////////////////////////////////

MainMenu::MainMenu()
{
	mFileMenu = new wxMenu();
	mFileMenu->Append(wxID_NEW, "&New", "Create a new map");
	mFileMenu->Append(wxID_OPEN, "&Open", "Open an existing map");
	mFileMenu->AppendSeparator();
	mFileMenu->Append(wxID_SAVE, "&Save", "Save current map" );
	mFileMenu->Append(wxID_SAVEAS, "Save &As", "Save the map with a new name");
	mFileMenu->AppendSeparator();
	mFileMenu->Append(wxID_EXIT, "E&xit", "Exit the editor");
	Append(mFileMenu, "File");
}

MainMenu::~MainMenu()
{
	
}

//////////////////////////////////////////////////////////////////////////