/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blClockInfo.h"

/**
\brief   Text utilities
\ingroup 
\author  Xavier Planes
\date    2007-12-14
*/
blClockInfo::blClockInfo ( )
{
	m_ClockStart = 0;
	m_ClockStop = 0;
	m_bIsRunning = false;
}

void blClockInfo::StartTime( )
{
	m_ClockStart = clock( );
	m_bIsRunning = true;
}

void blClockInfo::StopTime( )
{
	m_ClockStop = clock( );
	m_bIsRunning = false;
}

void blClockInfo::RemoveDiskIOTime( blClockInfo &clockInfo )
{
	if ( m_bIsRunning )
	{
		m_ClockStart -= (clockInfo.m_ClockStop - clockInfo.m_ClockStart);
	}
}

double blClockInfo::GetTime( )
{
	clock_t endTimer;
	if ( m_bIsRunning )
	{
		endTimer = clock();
	}
	else
	{
		endTimer = m_ClockStop;
	}
	return double(endTimer - m_ClockStart) / CLOCKS_PER_SEC;
}
