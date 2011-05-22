/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "coreXMLWorkflowWriter.h"
#include "tinyxml.h"
#include <boost/foreach.hpp>
#include "blXMLTagMapWriter.h"

void SaveXMLList( TiXmlElement * mainElm, const std::string &name, std::list<std::string> list )
{
	TiXmlElement *childElem = new TiXmlElement( name );
	mainElm->LinkEndChild( childElem );
	BOOST_FOREACH(std::string text, list) 
	{
		TiXmlElement *itemElm = new TiXmlElement( "Item" );
		itemElm->SetAttribute( "name", text );
		childElem->LinkEndChild( itemElm );
	}
}

Core::XMLWorkflowWriter::XMLWorkflowWriter()
{
	m_Version = 0;
}

Core::XMLWorkflowWriter::~XMLWorkflowWriter()
{
}

void Core::XMLWorkflowWriter::InternalUpdate()
{
	std::ostringstream strVersion;
	strVersion << m_Version;
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( strVersion.str().c_str(), "", "" );
	doc.LinkEndChild( decl );

	SaveData( &doc, m_Data );

	doc.SaveFile( m_Filename );
}

void Core::XMLWorkflowWriter::SaveData( TiXmlNode* node, Workflow::Pointer workflow )
{
	TiXmlElement * mainElm = new TiXmlElement( "Workflow" );
	node->LinkEndChild( mainElm );
	mainElm->SetAttribute("name", workflow->GetName() );
	mainElm->SetAttribute("version", workflow->GetVersion() );
	SaveXMLList( mainElm, "RequiredPlugins", workflow->GetPluginNamesList() );

	Core::WorkflowStep::Pointer workflowStep;
	BOOST_FOREACH(workflowStep, workflow->GetStepVector()) 
	{
		TiXmlElement *firstChild = new TiXmlElement( "Step" );
		firstChild->SetAttribute("name", workflowStep->GetName( ) );
		SaveXMLList( firstChild, "Windows", workflowStep->GetWindowList() );
		blXMLTagMapWriter::SaveData( firstChild, workflowStep->GetProperties(), "Properties" );

		Core::WorkflowSubStep::Pointer workflowSubStep;
		BOOST_FOREACH(workflowSubStep, workflowStep->GetSubStepVector()) 
		{
			TiXmlElement *secondChild = new TiXmlElement( "SubStep" );
			secondChild->SetAttribute("name", workflowSubStep->GetName() );
			SaveXMLList( secondChild, "Alternatives", workflowSubStep->GetAlternatives() );
			firstChild->LinkEndChild( secondChild );
		}

		mainElm->LinkEndChild( firstChild );
	}

}

