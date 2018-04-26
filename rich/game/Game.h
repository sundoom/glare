/*
 *  Game.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __Game__
#define __Game__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>
#include <Engine.h>
NAMESPACE_USE(Engine)

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Rich)

//////////////////////////////////////////////////////////////////////////

	class Game
	{
	public:
		// query the instance of the application
		static Game& instance();

		// the application's run method.
		VOID init();
		VOID run(F64 delta_time);
		VOID exit();

		// set the window
		VOID window(U32 width, U32 height);

	private:
		// constructor
		Game();

		// destructor
		~Game();

		// build the resource
		VOID build();

	private:
		U32 mFBO;
		U32 mWndWidth, mWndHeight;
		F32 mFov, mAspect, mZNear, mZFar;
		Vector3 mEye, mCenter, mUp;
    F32 mRotation;
    StaticMeshPtr mMeshPtr;
    
    TextureDataPtr mColorTexPtr;
    TargetPtr mRTPtr;
    PrimitivePtr mQuadPtr;

    WaveDataPtr mWaveDataPtr;  
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////