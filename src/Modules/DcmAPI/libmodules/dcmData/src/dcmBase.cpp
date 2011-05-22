/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmBase.h"
using namespace dcmAPI;

Base::Base()
{
	m_tags = new TagVector();
	m_tagsMap = new TagsMap();
}

Base::~Base()
{
	delete m_tags;
	delete m_tagsMap;
}

void Base::AddTag(const TagId& _id, std::string _value)
{
	(*m_tagsMap)[ _id ] = _value;
}

void Base::AddTag(const Tag& tag )
{
	(*m_tagsMap)[ tag.m_tagId ] = tag.m_value;
}

std::string Base::GetTagAsText(const TagId& _id)
{
	std::string tagAsText = "";
	TagsMap::iterator iter = m_tagsMap->find(_id);
	if( iter != m_tagsMap->end() )
	{
		tagAsText = iter->second;
	}
	return tagAsText;
}

double Base::GetTagAsNumber(const TagId& _id)
{
	double tagAsNumber = atof( GetTagAsText( _id ).c_str( ) ); 
	return tagAsNumber;
}

TagsMap * Base::GetTagsMap() const
{
	return m_tagsMap;
}

void Base::SetTagsMap( TagsMap * val )
{
	m_tagsMap = val;
}

void Base::GetAllTags(TagsMap& tagsMap)
{

}


