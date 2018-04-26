/*
 *  MainFrame.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#include "MainApp.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_IDLE(MainFrame::OnIdle)
	EVT_MENU(wxID_NEW, MainFrame::FileNew )
	EVT_MENU(wxID_OPEN, MainFrame::FileOpen )
	EVT_MENU(wxID_SAVE, MainFrame::FileSave )
	EVT_MENU(wxID_SAVEAS, MainFrame::FileSaveAs )
	EVT_MENU(wxID_EXIT, MainFrame::FileExit )
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame(wxWindow* parent, 
					 wxWindowID id, 
					 const wxString& title, 
					 const wxPoint& pos, 
					 const wxSize& size, 
					 long style) 
					 : wxFrame(parent, id, title, pos, size, style)
{
	// create the main menu
	mMainMenu = new MainMenu;
	SetMenuBar(mMainMenu);

	// create the main tool bar
	mMainToolBar = new MainToolBar(this);
	SetToolBar(mMainToolBar);

	// create the main status bar
	mMainStatusBar = new MainStatusBar(this); 
	SetStatusBar(mMainStatusBar);

	// create the main view
	mMainView = new MainView(this);

	// init the world editor
	Editor::Instance().Init((HWND)mMainView->GetHWND());
}

MainFrame::~MainFrame()
{
	// exit the world editor
	Editor::Instance().Exit();
}

//////////////////////////////////////////////////////////////////////////

void MainFrame::OnIdle(wxIdleEvent& event)
{
	Editor::Instance().Run();
}

//////////////////////////////////////////////////////////////////////////

void MainFrame::FileNew(wxCommandEvent& event)
{
	Editor::Instance().New(10,5,"map/grass.tga");
}

void MainFrame::FileOpen(wxCommandEvent& event)
{

}

void MainFrame::FileSave(wxCommandEvent& event)
{

}

void MainFrame::FileSaveAs(wxCommandEvent& event)
{

}

void MainFrame::FileExit(wxCommandEvent& event)
{
	// true is to force the frame to close
	Close(true);
}

//////////////////////////////////////////////////////////////////////////

// used as title for several dialog boxes
static const wxChar SAMPLE_TITLE[] = _T("wxWidgets Font Sample");

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
	wxMessageBox(wxT("wxWidgets font sample\n")
		wxT("(c) 1999-2006 Vadim Zeitlin"),
		wxString(wxT("About ")) + SAMPLE_TITLE,
		wxOK | wxICON_INFORMATION, this);
}

//////////////////////////////////////////////////////////////////////////