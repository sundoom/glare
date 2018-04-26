/*
 *  Engine.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Engine__
#define __Engine__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>
#include <GMath.h>
NAMESPACE_USE(Math)
#include <Render.h>
NAMESPACE_USE(Render)
#include <Audio.h>
NAMESPACE_USE(Audio)

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	#pragma pack(1)
	struct DVT
	{
		F32 point[3];		// coordinate
		F32 texcoord[2];	// texture coordinate
	};
	struct DVTN
	{
		F32 point[3];		// coordinate
		F32 texcoord[2];	// texture coordinate
		F32 normal[3];		// normal
	};	
	struct DVC
	{
		F32 point[3];		// coordinate
		F32 color[4];		// color : r g b a
	};
	
	struct DVCT
	{
		F32 point[3];		// coordinate
		F32 color[4];		// color : r g b a
		F32 texcoord[2];	// texture coordinate
	};	
	struct DVN
	{
		F32 point[3];		// coordinate
		F32 normal[3];		// normal
	};
	struct DVS
	{
		F32 point[3];		// coordinate
		F32 normal[3];		// normal
		F32 tangent[3];		// tangent
		F32 binormal[3];	// binormal
	};
	struct DVTS
	{
		F32 point[3];		// coordinate
		F32 texcoord[2];	// texture coordinate
		F32 normal[3];		// normal
		F32 tangent[3];		// tangent
		F32 binormal[3];	// binormal
	};
	#pragma pack()

	//////////////////////////////////////////////////////////////////////////

	// unit transformation
	#define U2P(x) ((I32)((x)*64.0f))
	#define P2U(x) ((F32)((x)/64.0f))

	//////////////////////////////////////////////////////////////////////////

	#define TILE_WIDTH	128
	#define TILE_HEIGHT	64
	extern VOID T2P(I32& px, I32& py, I32 tx, I32 ty);
	extern VOID P2T(I32& tx, I32& ty, I32 px, I32 py);
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#include <Resource.h>
#include <VertexData.h>
#include <IndexData.h>
#include <TextureData.h>
#include <ShaderData.h>
#include <WaveData.h>

#include <Image.h>
#include <Target.h>
#include <Constant.h>
#include <Primitive.h>

#include <StaticMesh.h>

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////