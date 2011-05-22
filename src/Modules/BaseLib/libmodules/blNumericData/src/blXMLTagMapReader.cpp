/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blXMLTagMapReader.h"
#include "tinyxml.h"

blXMLTagMapReader::blXMLTagMapReader() 
{
}

blXMLTagMapReader::~blXMLTagMapReader()
{
}

void blXMLTagMapReader::InternalUpdate()
{
	TiXmlDocument doc( m_Filename );
	if (!doc.LoadFile()) return;

	m_Data = blTagMap::New( );
	LoadData( &doc, m_Data, "tagmap" );
}

void blXMLTagMapReader::LoadData( 
	TiXmlNode* node, blTagMap::Pointer tagMapInstance, const std::string &nodeName )
{
	TiXmlElement* mainElem;
	mainElem = node->FirstChild( nodeName )->ToElement();
	if ( !mainElem ) return;

	int count = 0;
	if ( !mainElem->FirstChild( "tag" ) )
	{
		return;
	}
	TiXmlElement* childElem = mainElem->FirstChild( "tag" )->ToElement();
	while( childElem )
	{
		std::string name = childElem->Attribute("name");
		std::string valueTypeName = childElem->Attribute("type");
		std::string valueAsString;
		if ( childElem->GetText( ) != NULL )
		{
			valueAsString = childElem->GetText( );
		}
		
		blTag::Pointer tag = blTag::New( count, name, NULL );
		tag->SetValueAsString( valueTypeName, valueAsString );

		if ( valueTypeName == blTag::GetTypeName( typeid( blTagMap::Pointer ) ) )
		{
			blTagMap::Pointer tagMapProperty = blTagMap::New();
			LoadData( childElem, tagMapProperty, "tagmap" );
			tag->SetValue( tagMapProperty );
		}

		tagMapInstance->AddTag( tag );

		TiXmlNode* nextNode = mainElem->IterateChildren( "tag", childElem);
		childElem = nextNode ? nextNode->ToElement() : NULL;
		count++;
	}
}
