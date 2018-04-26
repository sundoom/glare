/*
 *  GConfig.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Glare.h>
#include "xml/tinyxml.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

GConfig ::GConfig ()
{

}

GConfig ::~GConfig ()
{

}

//////////////////////////////////////////////////////////////////////////

/*
====================
SetValue
====================
*/
VOID GConfig::SetValue(const CHAR* value)
{
	mValue = value;
}

/*
====================
GetValue
====================
*/
const CHAR* GConfig::GetValue() const
{
	return mValue.c_str();
}

/*
====================
SetText
====================
*/
VOID GConfig::SetText(const CHAR* text)
{
	mText = text;
}

/*
====================
GetText
====================
*/
const CHAR* GConfig::GetText() const
{
	return mText.c_str();
}

/*
====================
SetAttribute
====================
*/
VOID GConfig::SetAttribute(const CHAR* name, const CHAR* value)
{
	std::map<Str,Str>::iterator it = mAttributes.find(name);
	CHECK(it==mAttributes.end()&&"The attribute is existent already!\n");
	mAttributes.insert( std::map<Str, Str>::value_type(name, value) );
}	

/*
====================
GetAttribute
====================
*/
const CHAR* GConfig::GetAttribute(const CHAR* name) const
{
	std::map<Str,Str>::const_iterator it = mAttributes.find(name);
	if(it!=mAttributes.end()) return it->second.c_str();
	return "";
}

/*
====================
SetChild
====================
*/
VOID GConfig::SetChild(const GConfig* child)
{
	CHECK(child);
	mChildren.push_back((GConfig*)child);
}

/*
====================
GetChild
====================
*/
const GConfig* GConfig::GetChild(U32 i) const
{
	if(i >= mChildren.size()) return NULL;
	return mChildren[i].Ptr();
}

//////////////////////////////////////////////////////////////////////////

/*
====================
parse
====================
*/
static GConfig* parse(const TiXmlElement * element_ptr)
{
	CHECK(element_ptr);
	GConfig* config_ptr = GNEW(GConfig ); CHECK(config_ptr);

	// parse the value of the element
	config_ptr->SetValue(element_ptr->Value());

	// parse the text of the element
	const CHAR * text = element_ptr->GetText();
	if(text) config_ptr->SetText(text);

	// parse the attribute of the element
	for(const TiXmlAttribute* attribute_ptr = element_ptr->FirstAttribute(); attribute_ptr; attribute_ptr = attribute_ptr->Next())
	{
		config_ptr->SetAttribute(attribute_ptr->Name(), attribute_ptr->Value());
	}

	// parse the children of the element
	for(const TiXmlElement *child_element_ptr = element_ptr->FirstChildElement(); child_element_ptr; child_element_ptr = child_element_ptr->NextSiblingElement()) 
	{
		config_ptr->SetChild(parse(child_element_ptr));
	}

	return config_ptr;
}

/*
====================
load
====================
*/
GConfig* GConfig::Load(const CHAR* config)
{
	CHECK(config);

	// xml parse
	TiXmlDocument document;
	document.Parse(config, NULL, TIXML_ENCODING_UTF8);
	if(document.Error()){GAssert(VA("The config is error: %s(row=%d,col=%d)", document.ErrorDesc(),document.ErrorRow(),document.ErrorCol())); return NULL;}
	return parse( document.RootElement() );
}

//////////////////////////////////////////////////////////////////////////
