/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wflIOBase.h"


wflIOBase::wflIOBase() 
{
	m_Version = 0;
}

wflIOBase::~wflIOBase()
{
}

void wflIOBase::SetFilename( const char* filename )
{
	m_Filename = filename;
}

void wflIOBase::SetInput( wflWorkflow::Pointer workflow )
{
	m_Data = workflow;
}

void wflIOBase::Update()
{
	InternalUpdate();
}

wflWorkflow::Pointer wflIOBase::GetOutput()
{
	return m_Data;
}

