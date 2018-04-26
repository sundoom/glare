
// Editor.cpp : implementation of the Editor class
//
//////////////////////////////////////////////////////////////////////////

#include <EGL/egl.h>
#include "Editor.h"

//////////////////////////////////////////////////////////////////////////

/*
====================
GLoad
====================
*/
const GData* GLoad(const CHAR* path)
{
	GFile file;
	CHECK(file.Open(path));
	U32 pos = file.Tell();
	file.Seek(0,2);
	U32 size = file.Tell();
	file.Seek(pos,0);
	GData* data = GNEW(GData(size+1,0)); CHECK(data);
	CHECK(file.Read(data->Ptr(),data->Size())==size);
	file.Close();
	return data;
}

//////////////////////////////////////////////////////////////////////////
// Editor
//////////////////////////////////////////////////////////////////////////

Editor::Editor()
{	
	mDisplay = NULL;
	mContext = NULL;
	mSurface = NULL;

	mScale = 1.0;
	mEye.set(0,0,0);

	mDrawGrid = FALSE;
}

Editor::~Editor()
{
	
}

//////////////////////////////////////////////////////////////////////////

/*
====================
instance
====================
*/ 
Editor& CDECL Editor::Instance()
{
	GUARD(Editor::Instance);

	static Editor s_world_editor;
	return s_world_editor;

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

VOID Editor::Init(HWND hWnd)
{
	CHECK(hWnd);
	mWnd = hWnd;

	// Get Display
	mDisplay = eglGetDisplay(::GetDC(hWnd));
	CHECK(mDisplay!=EGL_NO_DISPLAY);

	// Initialize EGL
	EGLint major, minor;
	CHECK(eglInitialize(mDisplay, &major, &minor));

	// Get configs
	EGLint num_configs;
	CHECK(eglGetConfigs(mDisplay, NULL, 0, &num_configs));

	// Choose config
	EGLint attribs[] =
	{
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_NONE
	};
	EGLConfig config;
	CHECK(eglChooseConfig(mDisplay, attribs, &config, 1, &num_configs));

	// Create a surface
	mSurface = eglCreateWindowSurface(mDisplay, config, (EGLNativeWindowType)hWnd, NULL);
	CHECK(mSurface!=EGL_NO_SURFACE);

	// Create a GL context
	EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
	mContext = eglCreateContext(mDisplay, config, EGL_NO_CONTEXT, context_attribs);
	CHECK(mContext!=EGL_NO_CONTEXT)

	// Make the gContext current
	CHECK(eglMakeCurrent(mDisplay, mSurface, mSurface, mContext));

	// init the time
	Timer::instance().time();

	// create the graph context
	mGraphPtr = GNEW(Graph); CHECK(mGraphPtr);
}

VOID Editor::Run()
{
	// check the window rect
	RECT wnd_rc; ::GetClientRect(mWnd, &wnd_rc);
	I32 wnd_width = wnd_rc.right-wnd_rc.left;
	I32 wnd_height = wnd_rc.bottom-wnd_rc.top;
	if(wnd_width<=0||wnd_height<=0) return;

	// get the frame time
	static U32 system_time = Timer::instance().time();
	U32 current_time = Timer::instance().time();
	U32 frame_time = current_time - system_time;
	if(frame_time == 0) return;
	system_time = current_time;

	if(mGraphPtr==NULL) return;

	// reset the render context
	mGraphPtr->Reset();

	// get the window rect	
	Vector4 rect = Vector4(wnd_rc.left, wnd_rc.top, wnd_rc.right, wnd_rc.bottom);

	// set the viewport
	mGraphPtr->viewport(rect);
	ViewportPtr viewport = GNEW(Viewport(rect[0],rect[1],rect[2],rect[3])); CHECK(viewport);
	mGraphPtr->draw(static_cast<Operation*>(viewport.Ptr()));

	// set the scissor
	ScissorPtr scissor = GNEW(Scissor(rect[0],rect[1],rect[2],rect[3]));
	mGraphPtr->draw(static_cast<Operation*>(scissor.Ptr()));

	// clear the target and the zbuffer
	ClearPtr clear = GNEW(Clear(Clear::CT_COLOR|Clear::CT_DEPTH,Vector4(0.5,0.5,0.5,1.0),1.0f)); CHECK(clear);
	mGraphPtr->draw(static_cast<Operation*>(clear.Ptr()));

	// make the projection and the view
	F32 left	= -P2U(wnd_width/2)*mScale;
	F32 right	= P2U(wnd_width/2)*mScale;
	F32 bottom	= -P2U(wnd_height/2)*mScale;
	F32 top		= P2U(wnd_height/2)*mScale;
	F32 znear	= -512.0;
	F32 zfar	= 512.0;
	mGraphPtr->projection(Matrix::makeOrtho(left,right,bottom,top,znear,zfar));
	mGraphPtr->view(Matrix::makeTranslate(-mEye));

	// draw the map
	if(mMapPtr)
	{
		// update the map
		Vector3 start(rect[0], rect[1], 0);
		mGraphPtr->unproject(start, start);
		Vector3 end(rect[2], rect[3], 0);
		mGraphPtr->unproject(end, end);
		mMapPtr->Update(Vector4(start[0], start[1], end[0], end[1]));

		// draw the map
		mMapPtr->Draw(mGraphPtr.Ptr());

		// draw the reference line
		if(mDrawGrid)
		{
			mMapPtr->DrawReferenceLine(mGraphPtr.Ptr());
		}		
	}	

	// draw the brush
	if(mBrushPtr) mBrushPtr->Draw(mGraphPtr.Ptr());

	// swap the graph buffer
	mGraphPtr->swap();

	// swap buffers
	eglSwapBuffers(mDisplay, mSurface);
}

VOID Editor::Exit()
{
	if(mGraphPtr) mGraphPtr.Release();

	// release the egl
	if(mDisplay!=EGL_NO_DISPLAY) 
	{
		eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if(mContext!=EGL_NO_CONTEXT)eglDestroyContext(mDisplay, mContext);		
		if(mContext!=EGL_NO_SURFACE)eglDestroySurface(mDisplay, mSurface);
		eglTerminate(mDisplay);
	}
}

//////////////////////////////////////////////////////////////////////////

const Graph* Editor::GetGraph() const
{
	return mGraphPtr.Ptr();
}

//////////////////////////////////////////////////////////////////////////

VOID Editor::New(U32 w, U32 h, const CHAR* name)
{
	CHECK(name);
	w = GMath::clamp(w, w, (U32)1);
	h = GMath::clamp(h, h, (U32)1);

	// calculate the size of the map
	U32 size = 0;
	size += sizeof(U32);	// map header
	size += sizeof(w);		// map width
	size += sizeof(h);		// map height
	for(U32 j = 0; j < h; j++)
	{
		for(U32 i = 0; i < w; i++)
		{
			size += sizeof(U32);	// chunk header
			size += sizeof(U32);	// number of the layer

			size += sizeof(U32);	// the name length
			size += sizeof(CHAR)*::strlen(name);// the name 
			size += sizeof(F32);	// s
			size += sizeof(F32);	// t
			size += sizeof(U32);	// width
			size += sizeof(U32);	// height
			size += sizeof(U8);		// alpha
		}
	}

	// set the map data
	GDataPtr data = GNEW(GData); CHECK(data);
	data->Size(size);
	U8* data_ptr = (U8*)data->Ptr();
	*(U32*)data_ptr = MAKEFOURCC('G','M','A','P');
	data_ptr += sizeof(U32);
	*(U32*)data_ptr = w;
	data_ptr += sizeof(U32);
	*(U32*)data_ptr = h;
	data_ptr += sizeof(U32);
	for(U32 j = 0; j < h; j++)
	{
		for(U32 i = 0; i < w; i++)
		{
			*(U32*)data_ptr = MAKEFOURCC('G','C','H','K');
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = 1;
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = ::strlen(name);
			data_ptr += sizeof(U32);
			::memcpy(data_ptr, name, sizeof(CHAR)*::strlen(name));
			data_ptr += sizeof(CHAR)*::strlen(name);
			*(F32*)data_ptr = 1.0f;
			data_ptr += sizeof(F32);
			*(F32*)data_ptr = 1.0f;
			data_ptr += sizeof(F32);
			*(U32*)data_ptr = 1;
			data_ptr += sizeof(U32);
			*(U32*)data_ptr = 1;
			data_ptr += sizeof(U32);
			*(U8*)data_ptr = 127;
			data_ptr += sizeof(U8);
		}
	}

	// open the map
	Open(data.Ptr());
}

VOID Editor::Open(const GData* data)
{
	if(mMapPtr) mMapPtr.Release();
	mMapPtr = GNEW(EMap); CHECK(mMapPtr);
	mMapPtr->Load(data);

	// set the brush
	mBrushPtr = GNEW(BrushLozenge(mMapPtr.Ptr()));
	// mBrushPtr = GNEW(BrushCircle(mMapPtr.Ptr()));
	CHECK(mBrushPtr);
	mBrushPtr->Name("map/brick.tga");
	mBrushPtr->Radius(1.0f);
	mBrushPtr->Strength(0.5f);
	mBrushPtr->Softness(1.0f);
	
}

VOID Editor::Save(const CHAR* name)
{

}

//////////////////////////////////////////////////////////////////////////

// set/get the eye
VOID Editor::SetEye(const Vector3& eye)
{
	mEye = eye;
}

const Vector3& Editor::GetEye() const
{
	return mEye;
}

// set/get the scale
VOID Editor::SetScale(F32 s)
{
	mScale = s;
}

F32 Editor::GetScale() const
{
	return mScale;
}

//////////////////////////////////////////////////////////////////////////

// projection du point (objx,objy,obz) sur l'ecran (winx,winy,winz)
VOID Editor::project(const Vector3& obj, Vector3& win)
{
	if(mGraphPtr)mGraphPtr->project(obj,win);
}

// transformation du point ecran (winx,winy,winz) en point objet
VOID Editor::unproject(const Vector3& win, Vector3& obj)
{
	if(mGraphPtr)mGraphPtr->unproject(win,obj);
}

//////////////////////////////////////////////////////////////////////////

// move the mouse
VOID Editor::DoMove(const Vector3& pt)
{
	if(mBrushPtr)mBrushPtr->DoMove(pt);
}

// drag the mouse
VOID Editor::DoDrag(const Vector3& pt)
{
	if(mBrushPtr)mBrushPtr->DoDrag(pt);
}

//////////////////////////////////////////////////////////////////////////

VOID Editor::DrawGrid(BOOL b)
{
	mDrawGrid = b;
}

//////////////////////////////////////////////////////////////////////////

#if 0
void MIToMD(int Dx,int Dy,int &Ix,int &Iy)
{
	Ix=(TileWidth>>1)*(Dx-Dy);//转换为绝对坐标x
	Iy=(TileHeight>>1)*(Dx+Dy);//转换为绝对坐标y      大菱形
}
//屏幕坐标转换成斜45度的坐标
void MDToMI(int Ix,int Iy,int &Dx,int &Dy)
{
	Dx=int(0.5*((Iy<<1)+Ix)/(TileWidth>>1));
	Dy=int(0.5*((Iy<<1)-Ix)/(TileWidth>>1));
}
#endif
//////////////////////////////////////////////////////////////////////////



