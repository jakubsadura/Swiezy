/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreProgressTicket.h"

using namespace Core;

Core::ProgressTicket::ProgressTicket()
{
	m_fProgress = 0;
	m_bAbortProcessing = false;
}

Core::ProgressTicket::~ProgressTicket()
{

}

float Core::ProgressTicket::GetProgress() const
{
	return m_fProgress;
}

void Core::ProgressTicket::SetProgress( float val )
{
	m_fProgress = val;
}

std::string Core::ProgressTicket::GetStatus() const
{
	return m_strStatus;
}

void Core::ProgressTicket::SetStatus( const char* status )
{
	if ( status == NULL )
	{
		m_strStatus = "";
	}
	else
	{
		m_strStatus = status;
	}
}

bool Core::ProgressTicket::GetAbortProcessing() const
{
	return m_bAbortProcessing;
}

void Core::ProgressTicket::SetAbortProcessing( bool val )
{
	m_bAbortProcessing = val;
}

std::string Core::ProgressTicket::GetProcessorName() const
{
	return m_strProcessorName;
}

void Core::ProgressTicket::SetProcessorName( std::string val )
{
	m_strProcessorName = val;
}

