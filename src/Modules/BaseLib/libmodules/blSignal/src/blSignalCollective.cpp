/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blSignalCollective.h"



blSignalCollective::blSignalCollective()
{
	m_name = "";
	m_EventSampleRate = 1;
}

blSignalCollective::~blSignalCollective()
{
}

unsigned int blSignalCollective::GetNumberOfSignals() const
{
	return m_signals.size();
}


blSignal::Pointer blSignalCollective::GetSignal(unsigned int pos)
{
	return  m_signals.at(pos);
}

blSignal::Pointer blSignalCollective::GetSignal( const std::string &name )
{
	for ( unsigned i = 0 ; i < m_signals.size() ; i++ )
	{
		if ( m_signals.at( i )->GetName( ) == name )
		{
			return m_signals.at( i );
		}
	}

	return NULL;
}

void blSignalCollective::SetNumberOfSignals(unsigned int val) 
{
	m_signals.resize(val);
	for ( unsigned i = 0 ; i < val ; i++ )
	{
		if ( m_signals.at( i ).IsNull( ) )
		{
			m_signals[ i ] = blSignal::New( );
		}

	}
}


void blSignalCollective::SetSignal(unsigned int pos, blSignal::Pointer signal)
{
	if(pos > m_signals.size())
		m_signals.resize(pos);

	m_signals.at(pos) = signal;
}


void blSignalCollective::RemoveSignal (unsigned int nPos)
{
	if(nPos <= m_signals.size())
		m_signals.erase(m_signals.begin() + nPos);
}

void blSignalCollective::RemoveAllSignals ()
{
	m_signals.clear();
}

void blSignalCollective::AddSignal( blSignal::Pointer signal )
{
	m_signals.push_back( signal );
}

float blSignalCollective::GetEventSampleRate() const
{
	return m_EventSampleRate;
}

void blSignalCollective::SetEventSampleRate( float val )
{
	m_EventSampleRate = val;
}

std::string blSignalCollective::GetName() const
{
	return m_name;
}

void blSignalCollective::SetName( const std::string& val )
{
	m_name = val;
}

blSignalAnnotation::Pointer blSignalCollective::FindAnnotation( 
	unsigned short type, unsigned &signalPosition )
{
	blSignalAnnotation::Pointer annotation;

	signalPosition = 0 ; 
	while ( annotation.IsNull() &&  signalPosition < m_signals.size() )
	{
		annotation = m_signals[ signalPosition ]->FindAnnotation( type );
		if ( annotation.IsNull() )
		{
			signalPosition++;
		}
	}

	return annotation;
}

void blSignalCollective::AddAnnotation( blSignalAnnotation::Pointer annot )
{
	blSignalAnnotation::Pointer newAnnotation = blSignalAnnotation::New();
	newAnnotation->SetXPos( annot->GetXPos() );
	newAnnotation->SetDuration( annot->GetDuration() );
	newAnnotation->SetType( annot->GetType() );

	for ( unsigned i = 0 ; i < GetNumberOfSignals() ; i++ )
	{
		AddAnnotation( i, newAnnotation );
	}

}

void blSignalCollective::AddAnnotation( unsigned channel, blSignalAnnotation::Pointer annot )
{
	annot->SetChannel( channel );
	annot->SetValueY( GetSignal( channel )->GetValueY( annot->GetXPos( ) ) );

	GetSignal( channel )->AddAnnotation(annot);
}

void blSignalCollective::Copy( Pointer input )
{
	SetName( input->GetName() );
	SetEventSampleRate( GetEventSampleRate() );

	SetNumberOfSignals( input->GetNumberOfSignals() );
	// Reuse blSignal pointer
	for ( unsigned i = 0 ; i < input->GetNumberOfSignals() ;i++ )
	{
		GetSignal( i )->Copy( input->GetSignal( i ) );
	}
}
