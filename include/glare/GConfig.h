/*
 *  GConfig.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __GConfig__
#define __GConfig__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

typedef GPtr<class GConfig>GConfigPtr;

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented config interface.
class GConfig : public GReference
{
public:
	// constructor
	GConfig();

	// destructor
	virtual ~GConfig();

	// set the value
	VOID SetValue(const CHAR* value);

	// get the value
	const CHAR* GetValue() const;

	// set the text
	VOID SetText(const CHAR* value);

	// get the text
	const CHAR* GetText() const;

	// set the attribute
	VOID SetAttribute(const CHAR* name, const CHAR* value);

	// get the attribute
	const CHAR* GetAttribute(const CHAR* name) const;

	// set a the child to the table
	VOID SetChild(const GConfig* child);

	// get the the child
	const GConfig* GetChild(U32 i) const;		

public:
	// load a config file
	static GConfig* Load(const CHAR* config);

private:
	Str mValue, mText;
	std::map<Str,Str>mAttributes;
	std::vector<GConfigPtr>mChildren;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GConfig__

//////////////////////////////////////////////////////////////////////////