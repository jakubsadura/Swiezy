/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wflT2FlowReader.h"
#include "tinyxml.h"
#include "wflWorkflow.h"

wflT2FlowReader::wflT2FlowReader()
{

}

wflT2FlowReader::~wflT2FlowReader()
{

}

void wflT2FlowReader::InternalUpdate()
{
	m_Data = NULL;

	TiXmlDocument doc( m_Filename );
	if (!doc.LoadFile()) 
	{
		m_Data = NULL;
		return;
	}

	m_Data = wflWorkflow::New( );
	if ( doc.FirstChild( "workflow" ) == NULL )
	{
		m_Data = NULL;
		return;
	}

	m_Data->SetProperty( "filename", m_Filename );

	TiXmlElement* mainElem;
	mainElem = doc.FirstChild( "workflow" )->ToElement();
	if ( !mainElem )
	{
		m_Data = NULL;
		return;
	}

	TiXmlAttribute* attrib = mainElem->FirstAttribute();
	while ( attrib )
	{
		m_Data->SetProperty( attrib->Name( ), attrib->Value( ) );
		attrib = attrib->Next( );
	}

	if ( !mainElem->FirstChild( "dataflow" ) )
	{
		m_Data = NULL;
		return;
	}
	TiXmlElement* dataFlowElem = mainElem->FirstChild( "dataflow" )->ToElement();

	if ( !dataFlowElem->FirstChild( "name" ) )
	{
		m_Data = NULL;
		return;
	}
	TiXmlElement* nameElem = dataFlowElem->FirstChild( "name" )->ToElement();
	m_Data->SetProperty( nameElem->Value( ), nameElem->GetText( ) );

	// Input ports
	if ( !dataFlowElem->FirstChild( "inputPorts" ) )
	{
		m_Data = NULL;
		return;
	}
	TiXmlElement* inputPortsElem = dataFlowElem->FirstChild( "inputPorts" )->ToElement();

	TiXmlNode* child = inputPortsElem->FirstChild();
	while( child )
	{
		wflInputPort::Pointer inputPort = wflInputPort::New( );
		ReadPort( child->ToElement(), inputPort->GetProperties() );
		m_Data->GetInputPorts().push_back( inputPort );
		child = child->NextSibling();
	}


	// Output ports
	if ( !dataFlowElem->FirstChild( "outputPorts" ) )
	{
		m_Data = NULL;
		return;
	}
	TiXmlElement* outputPortsElem = dataFlowElem->FirstChild( "outputPorts" )->ToElement();

	child = outputPortsElem->FirstChild();
	while( child )
	{
		wflOutputPort::Pointer outputPort = wflOutputPort::New( );
		ReadPort( child->ToElement(), outputPort->GetProperties() );
		m_Data->GetOutputPorts().push_back( outputPort );
		child = child->NextSibling();
	}
}

void wflT2FlowReader::ReadPort( 
	TiXmlElement* portElem,
	std::map<std::string,std::string> &properties )
{
	TiXmlNode* child = portElem->FirstChild();
	while( child )
	{
		if ( strcmp( child->Value(), "annotations" ) == 0 )
		{
			TiXmlHandle handle(child);
			TiXmlElement* annotation = handle.FirstChild("annotation_chain").FirstChild().FirstChild("annotationAssertions")
				.FirstChild().FirstChild("annotationBean").FirstChild("text").ToElement();
			if ( annotation && annotation->GetText() )
			{
				properties[ "datatype" ] = annotation->GetText();
			}
		}
		else
		{
			std::string text;
			if ( child->ToElement()->GetText() )
			{
				text = child->ToElement()->GetText();
			}
			properties[ child->Value() ] = text;

		}

		child = child->NextSibling();
	}
}

