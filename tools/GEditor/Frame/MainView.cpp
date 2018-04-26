/*
 *  MainView.cpp
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#include "MainApp.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MainView, wxWindow)
	EVT_LEFT_DOWN(MainView::OnMouseLeftDown)
	EVT_LEFT_UP(MainView::OnMouseLeftUp)
	EVT_RIGHT_DOWN(MainView::OnMouseRightDown)	
	EVT_RIGHT_UP(MainView::OnMouseRightUp)
	EVT_MOUSEWHEEL(MainView::OnMouseWheel)
	EVT_MOTION(MainView::OnMouseMove)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////

MainView::MainView(wxWindow* parent) : 
	wxWindow(parent, wxID_ANY,wxDefaultPosition, wxDefaultSize, wxBORDER|wxFULL_REPAINT_ON_RESIZE)
{
	mLButtonPress = FALSE;
	mRButtonPress = FALSE;
	mPtMouse.x = mPtMouse.y = 0;
	mOffset.x = mOffset.y = 0;
}

MainView::~MainView()
{

}

//////////////////////////////////////////////////////////////////////////

void MainView::OnMouseLeftDown(wxMouseEvent& event)
{
	mLButtonPress = TRUE;
}

void MainView::OnMouseLeftUp(wxMouseEvent& event)
{
	mLButtonPress = FALSE;
}

void MainView::OnMouseRightDown(wxMouseEvent& event)
{
	mRButtonPress = TRUE;

	::GetCursorPos(&mPtMouse);
	::SetCapture((HWND)this->GetHWND());
	while(::ShowCursor(FALSE) >= 0);		

	Vector3 pt(0, 0, 1.0f);
	Editor::Instance().project(pt,pt);
	mOffset.x = event.GetPosition().x - (I32)(pt[0]);
	mOffset.y = (this->GetClientRect().GetHeight() - event.GetPosition().y) - (I32)(pt[1]);
}

void MainView::OnMouseRightUp(wxMouseEvent& event)
{
	if(mRButtonPress)
	{
		::SetCursorPos(mPtMouse.x, mPtMouse.y);
		::ReleaseCapture();
		while(::ShowCursor(TRUE) < 0);
		mRButtonPress = FALSE;
		mOffset.x = 0;
		mOffset.y = 0;
	}
}

void MainView::OnMouseWheel(wxMouseEvent& event)
{

}

void MainView::OnMouseMove(wxMouseEvent& event)
{
	if(mLButtonPress)
	{
		Vector3 pt(event.GetPosition().x, this->GetClientRect().GetHeight()-event.GetPosition().y, 0.0f);
		Editor::Instance().unproject(pt,pt);
		Editor::Instance().DoDrag(Vector3(pt[0], pt[1], 0.0f));
	}
	else
	{
		Vector3 pt(event.GetPosition().x, this->GetClientRect().GetHeight()-event.GetPosition().y, 0.0f);
		Editor::Instance().unproject(pt,pt);
		Editor::Instance().DoMove(Vector3(pt[0], pt[1], 0.0f));			
	}

	if(mRButtonPress)
	{
/*		Vector3 pt((point.x-mOffset.x), (mOffset.y-point.y), 0);
		Editor::Instance().unproject(pt,pt);
		Vector3 eye = Editor::Instance().GetEye();		
		eye -= pt;
		Editor::Instance().SetEye(eye);
*/
		Vector3 pt(event.GetPosition().x - mOffset.x, (this->GetClientRect().GetHeight()-event.GetPosition().y) - mOffset.y, 1.0f);
		Editor::Instance().unproject(pt,pt);
		Vector3 eye = Editor::Instance().GetEye();
		eye[0] = eye[0]-pt[0];
		eye[1] = eye[1]-pt[1];
		Editor::Instance().SetEye(eye);
	}	
}

//////////////////////////////////////////////////////////////////////////