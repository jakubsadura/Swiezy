/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blSignalAnnotation.h"
#include "blSignalEvent.h"

blSignalAnnotation::blSignalAnnotation() 
{
	m_XPos = 0;
	m_ValueY = 0;
	m_Duration = 0;
	m_Type = 0;
	m_Channel = 0;
}

blSignalAnnotation::~blSignalAnnotation()
{
}

std::string blSignalAnnotation::GetName()
{
	if ( m_Name.empty() )
	{
		blSignalEvent event = blSignalEventTable::GetEvent( m_Type );
		m_Name = event.m_Description;
	}

	return m_Name;
}

std::size_t blSignalAnnotation::GetXPos() const
{
	return m_XPos;
}

void blSignalAnnotation::SetXPos( std::size_t val )
{
	if ( m_XPos != val )
	{
		m_XPos = val;
		Modified();
	}
}

float blSignalAnnotation::GetValueY() const
{
	return m_ValueY;
}

void blSignalAnnotation::SetValueY( float val )
{
	if ( m_ValueY != val )
	{
		m_ValueY = val;
		Modified();
	}
}

std::size_t blSignalAnnotation::GetDuration() const
{
	return m_Duration;
}

void blSignalAnnotation::SetDuration( std::size_t val )
{
	if ( m_Duration != val )
	{
		m_Duration = val;
		Modified();
	}
}

unsigned short blSignalAnnotation::GetType() const
{
	return m_Type;
}

void blSignalAnnotation::SetType( unsigned short val )
{
	if ( m_Type != val )
	{
		m_Name = "";
		m_Type = val;
		Modified();
	}
}

unsigned short blSignalAnnotation::GetChannel() const
{
	return m_Channel;
}

void blSignalAnnotation::SetChannel( unsigned short val )
{
	if ( m_Channel != val )
	{
		m_Channel = val;
		Modified();
	}
}

void blSignalAnnotation::Copy( Pointer input )
{
	SetXPos( input->GetXPos() );
	SetValueY( input->GetValueY() );
	SetDuration( input->GetDuration() );
	SetType( input->GetType() );
	SetChannel( input->GetChannel() );
}

void blSignalAnnotation::SetFreeName( const std::string &name )
{
	m_Name = name;
	m_Type = 5;
}
