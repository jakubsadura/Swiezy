/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "coreBaseWorkflowIO.h"


Core::BaseWorkflowIO::BaseWorkflowIO() 
{
	m_Version = 0;
}

Core::BaseWorkflowIO::~BaseWorkflowIO()
{
}

void Core::BaseWorkflowIO::SetFilename( const char* filename )
{
	m_Filename = filename;
}

void Core::BaseWorkflowIO::SetInput( Workflow::Pointer workflow )
{
	m_Data = workflow;
}

void Core::BaseWorkflowIO::Update()
{
	InternalUpdate();
}

Core::Workflow::Pointer Core::BaseWorkflowIO::GetOutput()
{
	return m_Data;
}

