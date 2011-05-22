/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "coreWorkflowReader.h"
#include "coreXMLWorkflowReader.h"
#include "coreBoostWorkflowReader.h"

Core::WorkflowReader::WorkflowReader() 
{
}

Core::WorkflowReader::~WorkflowReader()
{
}

void Core::WorkflowReader::InternalUpdate()
{
	m_Data = NULL;

	XMLWorkflowReader::Pointer reader = XMLWorkflowReader::New( );
	reader->SetFilename( m_Filename.c_str() );
	reader->Update();
	m_Data = reader->GetOutput();

	if ( m_Data.IsNull() )
	{
		BoostWorkflowReader::Pointer boostReader = BoostWorkflowReader::New( );
		boostReader->SetFilename( m_Filename.c_str() );
		boostReader->Update();
		m_Data = boostReader->GetOutput();
	}
}
