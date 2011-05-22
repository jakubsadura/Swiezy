/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blXMLTagMapWriter.h"
#include "tinyxml.h"

blXMLTagMapWriter::blXMLTagMapWriter()
{
	m_Version = 0;
}

blXMLTagMapWriter::~blXMLTagMapWriter()
{
}

void blXMLTagMapWriter::InternalUpdate()
{
	std::ostringstream strVersion;
	strVersion << m_Version;
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( strVersion.str().c_str(), "", "" );
	doc.LinkEndChild( decl );

	SaveData( &doc, m_Data, "tagmap" );

	doc.SaveFile( m_Filename );
}

void blXMLTagMapWriter::SaveData( 
	TiXmlNode* node, blTagMap::Pointer tagMapInstance, const std::string &nodeName )
{
	blTagMap::TagMapType tagMap = tagMapInstance->GetTagMap();
	TiXmlElement * tagMapElm = new TiXmlElement( nodeName );
	tagMapElm->SetAttribute("size", tagMap.size() );
	node->LinkEndChild( tagMapElm );

	blTagMap::TagMapType::iterator it;
	for ( it = tagMap.begin() ; it != tagMap.end() ; it++ )
	{
		blTag::Pointer tag = it->second;
		TiXmlElement *tagElm = new TiXmlElement( "tag" );
		tagElm->SetAttribute( "name", tag->GetName() );
		tagElm->SetAttribute( "type", tag->GetTypeName() );
		tagElm->InsertEndChild(TiXmlText( tag->GetValueAsString() ));
		//tagElm->SetValue( tag->GetValueAsString() );
		tagMapElm->LinkEndChild( tagElm );

		if ( tag->GetValue().type() == typeid( blTagMap::Pointer ) )
		{
			blTagMap::Pointer tagMapProperty;
			tag->GetValue<blTagMap::Pointer>( tagMapProperty );
			SaveData( tagElm, tagMapProperty, "tagmap" );
		}
	}
}

