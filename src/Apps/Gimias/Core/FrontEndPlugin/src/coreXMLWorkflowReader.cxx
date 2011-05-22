/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "coreXMLWorkflowReader.h"
#include "tinyxml.h"
#include "blXMLTagMapReader.h"

void LoadXMLList( TiXmlElement * mainElm, const std::string &name, std::list<std::string> &list )
{
	TiXmlHandle childHandle( mainElm );
	int count = 0;
	TiXmlElement* pElem = childHandle.FirstChild( name ).FirstChild( "Item" ).ToElement();
	while( pElem )
	{
		list.push_back( pElem->Attribute( "name" ) );
		count++;
		pElem = childHandle.FirstChild( name ).Child( "Item", count).ToElement();
	}

}

Core::XMLWorkflowReader::XMLWorkflowReader() 
{
}

Core::XMLWorkflowReader::~XMLWorkflowReader()
{
}

void Core::XMLWorkflowReader::InternalUpdate()
{
	TiXmlDocument doc( m_Filename );
	if (!doc.LoadFile()) return;

	m_Data = Core::Workflow::New( "" );
	LoadData( &doc, m_Data );
}

void Core::XMLWorkflowReader::LoadData( TiXmlNode* node, Workflow::Pointer workflow )
{
	if ( node->FirstChild( "Workflow" ) == NULL )
	{
		m_Data = NULL;
		return;
	}

	TiXmlElement* mainElem;
	mainElem = node->FirstChild( "Workflow" )->ToElement();
	if ( !mainElem ) 
	{
		m_Data = NULL;
		return;
	}
	m_Data->SetName( mainElem->Attribute("name") );
	m_Data->SetVersion( mainElem->Attribute("version") );
	std::list<std::string> pluginNames;
	LoadXMLList( mainElem, "RequiredPlugins", pluginNames );
	workflow->SetPluginNamesList( pluginNames );

	if ( !mainElem->FirstChild( "Step" ) )
	{
		m_Data = NULL;
		return;
	}
	TiXmlElement* childElem = mainElem->FirstChild( "Step" )->ToElement();
	while( childElem )
	{
		Core::WorkflowStep::Pointer workflowStep;
		workflowStep = Core::WorkflowStep::New( "" );
		workflowStep->SetName( childElem->Attribute("name") );
		LoadXMLList( childElem, "Windows", workflowStep->GetWindowList() );
		blXMLTagMapReader::LoadData( childElem, workflowStep->GetProperties(), "Properties" );

		int count = 0;
		TiXmlElement *secondChild = TiXmlHandle( childElem ).FirstChild( "SubStep" ).ToElement();
		while( secondChild )
		{
			Core::WorkflowSubStep::Pointer workflowSubStep;
			workflowSubStep = Core::WorkflowSubStep::New( secondChild->Attribute( "name" ) );

			std::list<std::string> alternatives;
			LoadXMLList( secondChild, "Alternatives", alternatives );
			std::list<std::string>::iterator it;
			for ( it = alternatives.begin() ; it != alternatives.end() ; it++)
			{
				workflowSubStep->AddAlternative( *it );
			}

			workflowStep->AddSubStep( workflowSubStep );

			count++;
			secondChild = TiXmlHandle( childElem ).Child( "SubStep", count ).ToElement();
		}

		m_Data->AddStep( workflowStep );

		TiXmlNode* nextNode = mainElem->IterateChildren( "Step", childElem);
		childElem = nextNode ? nextNode->ToElement() : NULL;
	}
}
