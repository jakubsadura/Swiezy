/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blArithmeticFilter.h"
#include "blTextUtils.h"

#include <sstream>


blArithmeticFilter::blArithmeticFilter() 
{
	m_copyAnnotations = false;
	m_K = 1;
}

blArithmeticFilter::~blArithmeticFilter()
{
}

void blArithmeticFilter::Update()
{
	if ( m_Output.IsNull() )
	{
		// Set values to 0 and don't copy annotations
		m_Output = blSignal::Build( m_Input1, 0, m_copyAnnotations );
	}

	switch ( m_Operation )
	{
	case BLSIGNAL_ADD:ComputeAdd( );break;
	case BLSIGNAL_SUBSTRACT:ComputeSubstract( );break;
	case BLSIGNAL_DIVIDE_K:ComputeDivideK( );break;
	}

}

blSignal::Pointer blArithmeticFilter::GetOutput()
{
	return m_Output;
}

//!
void blArithmeticFilter::SetInput1( blSignal::Pointer signal )
{
	m_Input1 = signal;
}

//!
void blArithmeticFilter::SetInput2( blSignal::Pointer signal )
{
	m_Input2 = signal;
}

bool blArithmeticFilter::GetCopyAnnotations() const
{
	return m_copyAnnotations;
}

void blArithmeticFilter::SetCopyAnnotations( bool val )
{
	m_copyAnnotations = val;
}

void blArithmeticFilter::SetOutput( blSignal::Pointer signal )
{
	m_Output = signal;
}

void blArithmeticFilter::SetK( double k )
{
	m_K = k;
}

void blArithmeticFilter::ComputeSubstract()
{
	if ( m_Input1.IsNull( ) || m_Input2.IsNull( ) )
	{
		return;
	}

	std::ostringstream strStream;
	strStream << m_Input1->GetName( ) << " - " << m_Input2->GetName( );
	m_Output->SetName( strStream.str( ) );

	size_t numberOfValues = std::min( 
		m_Input1->GetNumberOfValues( ), 
		m_Input2->GetNumberOfValues( ) );
	for ( size_t i = 0 ; i < numberOfValues ; i++ )
	{
		m_Output->SetValue( i, m_Input1->GetValueY( i ) - m_Input2->GetValueY( i ) );
	}
}

void blArithmeticFilter::ComputeAdd()
{
	if ( m_Input1.IsNull( ) || m_Input2.IsNull( ) )
	{
		return;
	}

	std::ostringstream strStream;
	strStream << m_Input1->GetName( ) << " + " << m_Input2->GetName( );
	m_Output->SetName( strStream.str( ) );

	size_t numberOfValues = std::min( 
		m_Input1->GetNumberOfValues( ), 
		m_Input2->GetNumberOfValues( ) );
	for ( size_t i = 0 ; i < numberOfValues ; i++ )
	{
		m_Output->SetValue( i, m_Input1->GetValueY( i ) + m_Input2->GetValueY( i ) );
	}
}

void blArithmeticFilter::ComputeDivideK()
{
	if ( m_Input1.IsNull( ) || m_K == 0  )
	{
		return;
	}

	std::ostringstream strStream;
	strStream << m_Input1->GetName( ) << " / " << m_K;
	m_Output->SetName( strStream.str( ) );

	for ( size_t i = 0 ; i < m_Input1->GetNumberOfValues( ) ; i++ )
	{
		m_Output->SetValue( i, blSignal::SampleType( m_Input1->GetValueY( i ) / m_K ) );
	}
}
