/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseFilter.h"

Core::BaseFilter::BaseFilter(void)
{
	m_iTimeStep = 0;
	m_ProgressTicketHolder = ProgressTicketHolderType::New( );
	m_ProgressTicketHolder->SetSubject( ProgressTicket::New() );
}

Core::BaseFilter::~BaseFilter(void)
{
}


void Core::BaseFilter::SetNumberOfInputs( unsigned iNum )
{
	m_InputPortVector.resize( iNum );
	for ( unsigned i = 0 ; i < iNum ; i++ )
	{
		m_InputPortVector[ i ] = Core::BaseFilterInputPort::New();
		m_InputPortVector[ i ]->SetProcessorName( GetName() );
	}
}

void Core::BaseFilter::SetNumberOfOutputs( unsigned iNum )
{
	m_OutputPortVector.resize( iNum );
	for ( unsigned i = 0 ; i < iNum ; i++ )
	{
		m_OutputPortVector[ i ] = Core::BaseFilterOutputPort::New();
		m_OutputPortVector[ i ]->SetProcessorName( GetName() );
	}
}


/**
 */
Core::DataEntityHolder::Pointer 
Core::BaseFilter::GetInputDataEntityHolder( int iIndex )
{
	return m_InputPortVector.at( iIndex )->GetDataEntityHolder();
}

/**
*/
Core::DataEntityHolder::Pointer 
Core::BaseFilter::GetOutputDataEntityHolder( int iIndex )
{
	return m_OutputPortVector.at( iIndex )->GetDataEntityHolder( );
}


std::string Core::BaseFilter::GetInputDataName( 
	int iIndex )
{
	return m_InputPortVector[ iIndex ]->GetName( );
}

void Core::BaseFilter::SetInputDataName( 
	int iIndex, 
	const std::string strName )
{
	m_InputPortVector[ iIndex ]->SetName( strName );
}

void Core::BaseFilter::SetOutputDataName( 
	int iIndex, 
	const std::string strName )
{
	m_OutputPortVector[ iIndex ]->SetName( strName );
}

int Core::BaseFilter::GetTimeStep() const
{
	return m_iTimeStep;
}

void Core::BaseFilter::SetTimeStep( int val )
{
	m_iTimeStep = val;
}

unsigned Core::BaseFilter::GetInputDataNumberOfTimeSteps( int iIndex )
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = GetInputDataEntityHolder( iIndex )->GetSubject( );
	int iTimeSteps = 0;
	if ( dataEntity.IsNotNull() )
	{
		iTimeSteps = dataEntity->GetNumberOfTimeSteps( );
	}
	return iTimeSteps;
}

Core::ProgressTicketHolderType::Pointer 
Core::BaseFilter::GetProgressTicketHolder() const
{
	return m_ProgressTicketHolder;
}

void Core::BaseFilter::Abort()
{
	
}

Core::ProgressTicket::Pointer Core::BaseFilter::GetProgressTicket() const
{
	return m_ProgressTicketHolder->GetSubject( );
}

void Core::BaseFilter::SetProgress( float fProgress, const char* strMessage )
{
	GetProgressTicket( )->SetProgress( fProgress );
	GetProgressTicket( )->SetStatus( strMessage );
	GetProgressTicketHolder( )->NotifyObservers( );
}

void Core::BaseFilter::ResetProgress( 
	const char* processorName,
	const char* strMessage /*= NULL */ )
{
	GetProgressTicket( )->SetProcessorName( processorName );
	GetProgressTicket( )->SetProgress( 0 );
	GetProgressTicket( )->SetStatus( strMessage );
	GetProgressTicket( )->SetAbortProcessing ( false );
	GetProgressTicketHolder( )->NotifyObservers( );
}

Core::DataEntity::Pointer 
Core::BaseFilter::GetInputDataEntity( int iIndex )
{
	return GetInputDataEntityHolder( iIndex )->GetSubject( );
}

Core::DataEntity::Pointer 
Core::BaseFilter::GetOutputDataEntity( int iIndex )
{
	return GetOutputDataEntityHolder( iIndex )->GetSubject( );
}

void Core::BaseFilter::SetInputDataEntity( int iIndex, Core::DataEntity::Pointer dataEntity )
{
	GetInputDataEntityHolder( iIndex )->SetSubject( dataEntity );
}

void Core::BaseFilter::SetOutputDataEntity( int iIndex, Core::DataEntity::Pointer dataEntity )
{
	GetOutputDataEntityHolder( iIndex )->SetSubject( dataEntity );
}

void Core::BaseFilter::AddOutputDataEntity( Core::DataEntity::Pointer dataEntity )
{
	Core::BaseFilterOutputPort::Pointer port = Core::BaseFilterOutputPort::New();
	port->SetProcessorName( GetName() );
	port->GetDataEntityHolder( )->SetSubject( dataEntity );
	m_OutputPortVector.push_back( port );
}

unsigned Core::BaseFilter::GetNumberOfInputs()
{
	return m_InputPortVector.size();
}

unsigned Core::BaseFilter::GetNumberOfOutputs()
{
	return m_OutputPortVector.size();
}

std::string Core::BaseFilter::GetName() const
{
	return m_Name;
}

void Core::BaseFilter::SetName( std::string val )
{
	m_Name = val;

	for ( unsigned i = 0 ; i < GetNumberOfInputs() ; i++ )
	{
		m_InputPortVector[ i ]->SetProcessorName( GetName() );
	}

	for ( unsigned i = 0 ; i < GetNumberOfOutputs() ; i++ )
	{
		m_OutputPortVector[ i ]->SetProcessorName( GetName() );
	}
}

Core::BaseFilterInputPort::Pointer 
Core::BaseFilter::GetInputPort( int num )
{
	return m_InputPortVector.at( num );
}

Core::BaseFilterOutputPort::Pointer 
Core::BaseFilter::GetOutputPort( int num )
{
	return m_OutputPortVector.at( num );
}

void Core::BaseFilter::Update()
{

}

/**
Compilation test
\ingroup gmDataHandling
\author Xavi Planes
\date 09 11 09
*/
class BaseFilterTest : Core::BaseFilter
{
void Update( )
{
	int i;
	GetProcessingData( 0, i );

	std::vector<int> v;
	GetProcessingData( 0, v );
}
};

