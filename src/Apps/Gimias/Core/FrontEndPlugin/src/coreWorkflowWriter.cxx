/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "coreWorkflowWriter.h"
#include "coreXMLWorkflowWriter.h"

Core::WorkflowWriter::WorkflowWriter() 
{
	m_Version = 0;
}

Core::WorkflowWriter::~WorkflowWriter()
{
}

void Core::WorkflowWriter::InternalUpdate()
{
	XMLWorkflowWriter::Pointer writer = XMLWorkflowWriter::New( );
	writer->SetFilename( m_Filename.c_str() );
	writer->SetInput( m_Data );
	writer->Update();
}
