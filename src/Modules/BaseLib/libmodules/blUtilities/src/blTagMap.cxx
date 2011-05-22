/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blTagMap.h"


blTagMap::blTagMap( )
{
}

blTagMap::~blTagMap(void)
{
}

void blTagMap::AddTag( blTag::Pointer tag )
{
	m_TagMap[ tag->GetId() ] = tag;
	
	// Send a signal to notify that the tag has changed
	m_OnChangedTagSignal( const_cast<blTagMap*>(this), tag->GetId() );
}

unsigned long blTagMap::AddTag( 
	const std::string &name, 
	boost::any value )
{
	unsigned long tagType = 0;
	blTag::Pointer tag = FindTagByName( name );
	if ( tag.IsNotNull() )
	{
		tag->SetValue( value );
		tagType = tag->GetId();
	}
	else
	{
		TagMapType::iterator it;
		it = m_TagMap.begin();
		while ( it != m_TagMap.end() )
		{
			if ( tagType < it->second->GetId() + 1 )
			{
				tagType = it->second->GetId() + 1;
			}
			it++;
		}
		AddTag( blTag::New( tagType, name, value ) );
	}
	return tagType;
}	

blTag::Pointer blTagMap::GetTag( 
	unsigned long type )
{
	return m_TagMap[ type ];
}

blTag::Pointer blTagMap::GetTag( Iterator it )
{
	return (*it).second;
}

blTag::Pointer 
blTagMap::FindTagByName( const std::string &name )
{
	blTag::Pointer foundTag;
	TagMapType::iterator it;
	it = m_TagMap.begin();
	while ( foundTag.IsNull() && it != m_TagMap.end() )
	{
		if ( it->second->GetName() == name )
		{
			foundTag = it->second;
		}
		it++;
	}

	return foundTag;
}

void blTagMap::NotifyObserversOnChangedTag(blTagMap* tagMap, unsigned long tagId)
{
	// Send a signal to notify that the tag has changed
	m_OnChangedTagSignal( tagMap, tagId );
}

size_t blTagMap::GetLength()
{
	return m_TagMap.size();
}

blTagMap::Iterator blTagMap::GetIteratorBegin()
{
	return m_TagMap.begin();
}

blTagMap::Iterator blTagMap::GetIteratorEnd()
{
	return m_TagMap.end();
}

void blTagMap::RemoveAll()
{
	m_TagMap.clear();
}

blTagMap::TagMapType blTagMap::GetTagMap() const
{
	return m_TagMap;
}

void blTagMap::SetTagMap( const blTagMap::TagMapType &val )
{
	m_TagMap = val;
}

void blTagMap::AddTags( blTagMap::Pointer tagMap )
{
	TagMapType::iterator it;
	for ( it = tagMap->GetIteratorBegin() ; it != tagMap->GetIteratorEnd() ; it++ )
	{
		AddTag( it->second->GetName( ), it->second->GetValue() );
	}

}

bool blTagMap::Compare(blTagMap::Pointer source)
{
	TagMapType::iterator it;
	for ( it = GetIteratorBegin() ; it != GetIteratorEnd() ; it++ )
	{
		blTag::Pointer tag = source->FindTagByName( it->second->GetName( ) );
		if ( tag.IsNull() )
		{
			return false;
		}

		if ( !it->second->Compare( tag ) )
		{
			return false;
		}
	}

	return true;
}
