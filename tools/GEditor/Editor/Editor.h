
// Editor.h : interface of the Editor class
//
//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>
#include <GMath.h>
NAMESPACE_USE(Math)
#include <Render.h>
NAMESPACE_USE(Render)
#include <Engine.h>
NAMESPACE_USE(Engine)

//////////////////////////////////////////////////////////////////////////

#include "Brush.h"
#include "EChunk.h"
#include "EMap.h"

//////////////////////////////////////////////////////////////////////////

#include "Brush2D.h"
#include "EChunk2D.h"
#include "EMap2D.h"

//////////////////////////////////////////////////////////////////////////

class Editor
{
private:
	Editor();
	virtual ~Editor();

public:
	// query the instance of the Editor
	static Editor& CDECL Instance();

public:
	VOID Init(HWND hWnd);
	VOID Run();
	VOID Exit();

	// get the graph	
	const Graph* GetGraph() const;

	// new/open/save the map
	VOID New(U32 w, U32 h, const CHAR* name);
	VOID Open(const GData* data);
	VOID Save(const CHAR* name);

	// set/get the eye
	VOID SetEye(const Vector3& eye);
	const Vector3& GetEye() const;

	// set/get the scale
	VOID SetScale(F32 s);
	F32 GetScale() const;	

	// projection du point (objx,objy,obz) sur l'ecran (winx,winy,winz)
	VOID project(const Vector3& obj, Vector3& win);

	// transformation du point ecran (winx,winy,winz) en point objet
	VOID unproject(const Vector3& win, Vector3& obj);

	// flush the map
	VOID Flush(const Vector3& pt);

	// move the mouse
	VOID DoMove(const Vector3& pt);	

	// drag the mouse
	VOID DoDrag(const Vector3& pt);

	VOID DrawGrid(BOOL b);

private:
	HWND mWnd;
	EGLDisplay mDisplay;
	EGLContext mContext;
	EGLSurface mSurface;

	GraphPtr mGraphPtr;
	Vector3 mEye;
	F32 mScale;
	// EMap2DPtr mMapPtr;
	// Brush2DPtr mBrushPtr;

	BOOL mDrawGrid;

	EMapPtr mMapPtr;
	BrushPtr mBrushPtr;
};

//////////////////////////////////////////////////////////////////////////

