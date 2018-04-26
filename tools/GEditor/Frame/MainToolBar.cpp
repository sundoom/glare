/*
 *  MainToolBar.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#include "MainApp.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MainToolBar, wxToolBar)
	EVT_MENU(IDM_GRID, MainToolBar::OnGrid)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////

MainToolBar::MainToolBar(wxWindow* parent) : 
	wxToolBar(parent, wxID_ANY,wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER|wxTB_FLAT)
{
	this->AddTool(wxID_NEW, _T("New"), wxBITMAP(new), _T("Create a new map"), wxITEM_NORMAL);
	this->AddTool(wxID_OPEN, _T("Open"), wxBITMAP(open), _T("Open an existing map"), wxITEM_NORMAL);
	this->AddTool(wxID_SAVE, _T("Save"), wxBITMAP(Save), _T("Save the current map"), wxITEM_NORMAL);

	this->AddSeparator();

	this->AddTool(wxID_COPY, _T("Copy"), wxBITMAP(copy), _T("Copy a object"), wxITEM_NORMAL);
	this->AddTool(wxID_CUT, _T("Cut"), wxBITMAP(cut), _T("Cut a object"), wxITEM_NORMAL);
	this->AddTool(wxID_PASTE, _T("Paste"), wxBITMAP(paste), _T("Paste a object"), wxITEM_NORMAL);

	this->AddSeparator();

	this->AddTool(wxID_UNDO, _T("Undo"), wxBITMAP(undo), _T("Undo a object"), wxITEM_NORMAL);
	this->AddTool(wxID_REDO, _T("Redo"), wxBITMAP(redo), _T("Redo a object"), wxITEM_NORMAL);

	this->AddSeparator();
	
	this->AddTool(IDM_GRID, _T("Grid"), wxBITMAP(grid), _T("Draw the grid"), wxITEM_CHECK);

	this->Realize();
}

MainToolBar::~MainToolBar()
{

}

//////////////////////////////////////////////////////////////////////////

void MainToolBar::OnGrid(wxCommandEvent& event)
{
	if ( event.IsChecked() )
	{
		Editor::Instance().DrawGrid(TRUE);
	}
	else
	{
		Editor::Instance().DrawGrid(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////