/*
 *  MainView.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __MainView__
#define __MainView__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

class MainView : public wxWindow
{
public:
	MainView(wxWindow* parent);
	~MainView();	

private:
	// event handlers
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseRightDown(wxMouseEvent& event);
	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	DECLARE_EVENT_TABLE()
	
private:
	POINT mPtMouse, mOffset;
	BOOL mLButtonPress, mRButtonPress;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __MainView__

//////////////////////////////////////////////////////////////////////////
