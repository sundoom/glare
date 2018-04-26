/*
 *  MainFrame.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __MainFrame__
#define __MainFrame__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

// Define a new frame type: this is going to be our main frame
class MainFrame : public wxFrame
{
public:
	MainFrame(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

	~MainFrame();

	// event handlers
	void OnIdle(wxIdleEvent& event);
	void FileNew(wxCommandEvent& event);
	void FileOpen(wxCommandEvent& event);
	void FileSave(wxCommandEvent& event);
	void FileSaveAs(wxCommandEvent& event);
	void FileExit(wxCommandEvent& event);

	void OnAbout(wxCommandEvent& event);

private:
	DECLARE_EVENT_TABLE()

private:
	MainMenu* mMainMenu;	
	MainToolBar* mMainToolBar;
	MainStatusBar*mMainStatusBar;
	MainView* mMainView;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __MainFrame__

//////////////////////////////////////////////////////////////////////////
