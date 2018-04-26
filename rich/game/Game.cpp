/*
 *  Game.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "TextureData.h"
#include "ShaderData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Rich)

//////////////////////////////////////////////////////////////////////////

	Game::Game()
	{    
		mFBO = 0;

    mWndWidth = 1024;
    mWndHeight = 768;	

    mFov = 90.0;
    mAspect = (F32)mWndWidth/(F32)mWndHeight;
    mZNear = 1.0;
    mZFar = 2000.0;

    mEye = Vector3(-3,0,3);
    mCenter = Vector3(0,0,0);
    mUp = Vector3(0,0,1);

		mRotation = 0.0f;
	}

	Game::~Game()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	instance
	====================
	*/ 
	Game& Game::instance()
	{
	  static Game s_game;
		return s_game;
	}

	//////////////////////////////////////////////////////////////////////////
  

	/*
	====================
	init
	====================
	*/
	VOID Game::init()
	{
    build();

    // play a sound
    mWaveDataPtr = Resource::Find<WaveData>("sound/music.wav");
    Wave* wave = GNEW(Wave); CHECK(wave);
    wave->Buffer(mWaveDataPtr->Buffer());
    wave->Duration(mWaveDataPtr->Duration());
    Audio::instance().Play(wave);
	}

	/*
	====================
	run
	====================
	*/
	VOID Game::run(F64 delta_time)
	{
    mRotation += 0.1f;

    // draw the render target
    {
      mRTPtr->Bind();

      // set the viewport
      RISetViewport(0, 0, mColorTexPtr->Width(), mColorTexPtr->Height());

      // clear the target and the zbuffer
      RIClear(CT_COLOR|CT_DEPTH, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

      // draw the mesh
      {
        // make the projection and the modelview
        Matrix proj_mat = Matrix::makePerspective(mFov,mAspect,mZNear,mZFar);    
        Matrix view_mat = Matrix::makeLookAt(mEye,mCenter,mUp);
        Quaternion quat; quat.setRotate(GMath::degToRad(mRotation),Vector3(0,0,1));
        Matrix world_mat = Matrix::makeRotate(quat);
        Matrix wvp_mat = world_mat * view_mat * proj_mat;
        mMeshPtr->Draw(wvp_mat);
      }
    }

    // draw the default render target
    {
      RIBindRenderTarget(NULL);

      // set the viewport
      RISetViewport(0, 0, mWndWidth, mWndHeight);

      // clear the target and the zbuffer
      RIClear(CT_COLOR|CT_DEPTH, 0.5f, 0.4f, 0.5f, 1.0f, 1.0f);

      // draw the mesh
      {
        // make the projection and the modelview
        Matrix proj_mat = Matrix::makePerspective(mFov,mAspect,mZNear,mZFar);    
        Matrix view_mat = Matrix::makeLookAt(mEye,mCenter,mUp);
        Quaternion quat; quat.setRotate(GMath::degToRad(mRotation),Vector3(0,0,1));
        Matrix world_mat = Matrix::makeRotate(quat);
        Matrix wvp_mat = world_mat * view_mat * proj_mat;
        mMeshPtr->Draw(wvp_mat);
      }

      // draw the quad
      {
        // compute the ortho matrix
        F32 left, right, bottom, top, nearval, farval;
        left = -(F32)mWndWidth / 2.0f;
        right = (F32)mWndWidth / 2.0f;
        bottom = -(F32)mWndHeight / 2.0f;
        top = (F32)mWndHeight / 2.0f;
        nearval = 0.0f;
        farval = 1.0f;

        Matrix proj_mat = Matrix::makeOrtho(left, right, bottom, top, nearval, farval);
        Matrix view_mat = Matrix::makeTranslate(Vector3(left,bottom,0.0));
        Matrix wvp_mat = view_mat * proj_mat;
        mQuadPtr->GetConstant("gWVP")->SetMatrix(wvp_mat);
        mQuadPtr->Draw();
      }
    }     
	}

	/*
	====================
	exit
	====================
	*/
	VOID Game::exit()
	{
    if(mMeshPtr) mMeshPtr.Release();
    if(mQuadPtr) mQuadPtr.Release();
    if(mColorTexPtr)mColorTexPtr.Release();
    if(mRTPtr)mRTPtr.Release();
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	window
	====================
	*/
	VOID Game::window(U32 width, U32 height)
	{
		// set the window
		mWndWidth = width;
		mWndHeight = height;
		mAspect = (F32)mWndWidth/(F32)mWndHeight;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	build
	====================
	*/
	VOID Game::build()
	{
    // load the mesh
    mMeshPtr = Resource::Find<StaticMesh>("model/mesh.xml");

    // build the color texture    
    mColorTexPtr = GNEW(TextureData("color_tex")); CHECK(mColorTexPtr);
    mColorTexPtr->Load(0, 256, 256, 0, PF_RGBA, DT_UNSIGNED_BYTE, NULL);

    // build the render target
    mRTPtr = GNEW(Target(mColorTexPtr.Ptr())); CHECK(mRTPtr);

    // build the quad
    {
      // build the primitive
      mQuadPtr = GNEW(Primitive); CHECK(mQuadPtr);
      mQuadPtr->SetType(Primitive::PT_TRIANGLES);

      // set the shader
      ShaderData* shader = Resource::Find<ShaderData>("shader/default.xml");
      CHECK(shader);
      mQuadPtr->SetShader(shader, "p0");

      // set the wvp
      Constant* wvp_constant_ptr = GNEW(Constant); CHECK(wvp_constant_ptr);
      wvp_constant_ptr->SetMatrix(Matrix());
      mQuadPtr->SetConstant("gWVP",wvp_constant_ptr);

      // set the color texture
      Constant* texture_constant_ptr = GNEW(Constant); CHECK(texture_constant_ptr);
      texture_constant_ptr->SetTexture(mColorTexPtr.Ptr());
      mQuadPtr->SetConstant("gBaseTex",texture_constant_ptr);      

      // build the vertex buffer
      {
        F32 x = 0.0f, y = 0.0f, w = 256, h = 256;
        DVT vertexes[] = 
        {
          {x,		y,		0,		0,		0},
          {x+w,	y,		0,		1,		0},
          {x+w,	y+h,	0,		1,		1},
          {x,		y+h,	0,		0,		1},
        };	
        VertexData* vd_ptr = GNEW(VertexData); CHECK(vd_ptr);
        U32 count = sizeof(vertexes)/sizeof(DVT);
        U32 stride = sizeof(DVT);
        U8 attrib[16];
        attrib[0] = 2;
        attrib[1] = VT_3F;
        attrib[2] = VT_2F;
        vd_ptr->Load(count, stride, attrib, vertexes);
        mQuadPtr->SetVertexData(vd_ptr);

        // build the bounding box
        BoundingBox box;
        box.set(MAX_F32,MAX_F32,MAX_F32,MIN_F32,MIN_F32,MIN_F32);
        for(U32 i = 0; i < sizeof(vertexes)/sizeof(DVT); i++)box.expand(vertexes[i].point);
        mQuadPtr->SetBox(box);
      }

      // build the index
      {
        const U16 indexes[] = { 3, 0, 2, 2, 0, 1 };
        U32 count = sizeof(indexes)/sizeof(U16);
        U32 stride = sizeof(U16);
        IndexData* id_ptr = GNEW(IndexData); CHECK(id_ptr);
        id_ptr->Load(count, stride, indexes);
        mQuadPtr->SetIndexData(id_ptr);	
      }
    }
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
