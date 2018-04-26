/*
 *  Primitive.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Primitive__
#define __Primitive__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented primitive interface.
	class Primitive: public GReference
	{
	public:
		enum TYPE
		{
			PT_POINTS			    = 0x0000,
			PT_LINES			    = 0x0001,
			PT_LINE_LOOP		  = 0x0002,
			PT_LINE_STRIP     = 0x0003,
			PT_TRIANGLES      = 0x0004,
			PT_TRIANGLE_STRIP = 0x0005,
			PT_TRIANGLE_FAN   = 0x0006,
			PT_QUADS          = 0x0007,
			PT_QUAD_STRIP     = 0x0008,
			PT_POLYGON        = 0x0009,
		};

	public:		
		// constructor
		Primitive();
		
		// destructor
		virtual ~Primitive();

		// set/get the element type
		VOID SetType(U32 t);
		U32 GetType() const;		

		// set/get the shader
		VOID SetShader(const ShaderData* s, const CHAR* pass);
		const ShaderData* GetShader() const;
		ShaderData* GetShader();

		// set/get the constant
		VOID SetConstant(const CHAR* name, const Constant* c);
		const Constant* GetConstant(const CHAR *name) const;
		Constant* GetConstant(const CHAR *name);

		// set/get the vertex data
		VOID SetVertexData(const VertexData* vd);
		const VertexData* GetVertexData() const;
		VertexData* GetVertexData();

		// set/get the index data
		VOID SetIndexData(const IndexData * id);
		const IndexData* GetIndexData() const;
		IndexData* GetIndexData();

		// set/get the bounding box
		VOID SetBox(const BoundingBox& box);
		const BoundingBox& GetBox() const;
		BoundingBox& GetBox();

  public:
		// implement the render operation
		VOID Draw();

	private:
		U32 mType;
		GPtr<ShaderData>mShaderPtr;
		Str mPassName;
		std::map<Str, ConstantPtr>mConstants;
		GPtr<VertexData>mVDPtr;
		GPtr<IndexData>mIDPtr;
		BoundingBox mBox;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<Primitive>PrimitivePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Primitive__

//////////////////////////////////////////////////////////////////////////