/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blSignal.h"

#include <algorithm>
#include <limits.h>

const InternalTimeType MAX_VALUE = 4294967296;

blSignal::blSignal()
{
	m_name = "";
	m_xUnit = "";
	m_yUnit = "";
	m_recordDuration.numerator = 1;
	m_recordDuration.dominator = 1;
	m_numberOfRecords = 0;
	// Non - equidistant sampling
	m_SamplesPerRecord = 0;
	m_StartTime = 0;
}

blSignal::~blSignal()
{
}

blSignal::Pointer blSignal::Build(
 std::string name,
 std::string xUnit,
 std::string yUnit,
 std::vector< SampleType >& xVector,
 std::vector< SampleType >& yVector )
{
	blSignal::Pointer signal = blSignal::New();
	signal->SetName( name );
	signal->SetXUnit( xUnit );
	signal->SetYUnit( yUnit );
	signal->SetXVector( xVector );
	signal->SetYVector( yVector );
	// (non equidistant) sampling. 
	signal->SetSamplesPerRecord( 0 );
	return signal;
}

blSignal::Pointer blSignal::Build(
	std::string name,
	std::string yUnit,
	std::vector< SampleType >& yVector
	)
{
	blSignal::Pointer signal = blSignal::New();
	signal->SetName( name );
	signal->SetYUnit( yUnit );
	signal->SetYVector( yVector );
	// equidistant sampling. 
	signal->SetSamplesPerRecord( yVector.size() );
	signal->SetRecordDuration( Duration( yVector.size(), 1 ) );
	signal->SetNumberOfRecords( 1 );
	return signal;
}

blSignal::Pointer blSignal::Build( 
	std::string name,
	unsigned int numberOfRecords, 
	Duration recordDuration, 
	unsigned long samplesPerRecord )
{
	blSignal::Pointer signal = blSignal::New();
	signal->SetName( name );
	// equidistant sampling. 
	signal->SetSamplesPerRecord( samplesPerRecord );
	signal->SetRecordDuration( recordDuration );
	signal->SetNumberOfRecords( numberOfRecords );
	signal->Allocate();
	return signal;
}

blSignal::Pointer blSignal::Build( 
	blSignal::Pointer input, SampleType value, bool copyAnnotations )
{
	blSignal::Pointer signal;
	if ( input.IsNotNull() )
	{
		signal = blSignal::New();
		signal->SetName( input->GetName() );
		signal->SetXUnit( input->GetXUnit() );
		signal->SetYUnit( input->GetYUnit() );
		signal->SetNumberOfRecords( input->GetNumberOfRecords() );
		signal->SetRecordDuration( input->GetRecordDuration() );
		signal->SetSamplesPerRecord( input->GetSamplesPerRecord() );
		signal->SetInternalStartTime( input->GetInternalStartTime() );
		signal->Allocate();

		if ( copyAnnotations )
		{
			signal->CopyAnnotations( input );
		}
	}
	else
	{
		signal = blSignal::Build( "", 0, blSignal::Duration( ), 1 );
	}
	
	for ( size_t i = 0 ; i < signal->GetNumberOfValues() ; i++ )
	{
		signal->SetValue( i, value );
	}


	return signal;
}

blSignal::SampleType blSignal::GetMinXValue()
{
	if ( m_xVector.empty( ) )
	{
		return 0;
	}

	std::vector< SampleType >::iterator it;
	it = std::min_element( m_xVector.begin(), m_xVector.end( ) );
	return *it;
}

blSignal::SampleType blSignal::GetMaxXValue()
{
	blSignal::SampleType maxValue = 0;

	// If equidistant samples return max time
	if ( IsEquidistant() )
	{
		maxValue = SampleType( m_numberOfRecords ) * m_recordDuration.numerator / m_recordDuration.dominator;
	}

	if ( !m_xVector.empty( ) )
	{
		std::vector< SampleType >::iterator it;
		it = std::max_element( m_xVector.begin(), m_xVector.end( ) );
		maxValue = *it;
	}

	return maxValue;
}

blSignal::SampleType blSignal::GetMinYValue()
{
	if ( m_yVector.empty( ) )
	{
		return 0;
	}
	std::vector< SampleType >::iterator it;
	it = std::min_element( m_yVector.begin(), m_yVector.end( ) );
	return *it;
}

blSignal::SampleType blSignal::GetMaxYValue()
{
	if ( m_yVector.empty( ) )
	{
		return 0;
	}
	std::vector< SampleType >::iterator it;
	it = std::max_element( m_yVector.begin(), m_yVector.end( ) );
	return *it;
}

blSignal::Pointer blSignal::BuildSignalFromEcgData( 
	std::vector< float > &ecgSamples, 
	double ecgStartTime, 
	double ecgIncrementTime, 
	int scaleEcgSamplesByValue )
{
	blSignal::Pointer signal = blSignal::New();

	signal->SetStartTime( SampleType( ecgStartTime ) );

	signal->SetSamplesPerRecord( ecgSamples.size( ) );
	Duration dur;
	dur.numerator = (unsigned int) ( ecgSamples.size( ) );
	dur.dominator = (unsigned int) ( 1 / ecgIncrementTime );
	signal->SetRecordDuration( dur );
	signal->SetNumberOfRecords( 1 );

	for(unsigned i = 0; i < ecgSamples.size(); i++)
	{
		if(scaleEcgSamplesByValue != 1)
		{
			ecgSamples.at(i) = ecgSamples.at(i)/scaleEcgSamplesByValue;
		}
	}
	signal->SetName("ECG");

	signal->SetXUnit("time [sec]");

	signal->SetYVector(ecgSamples);
	signal->SetYUnit("voltage [mV]");

	return signal;

}

void blSignal::AddAnnotation( blSignalAnnotation::Pointer annotation )
{
	m_AnnotationsVector.push_back( annotation );
}

std::vector< blSignalAnnotation::Pointer > blSignal::GetAnnotationsVector() const
{
	return m_AnnotationsVector;
}

bool blSignal::FindXValuePosition(SampleType x, unsigned& pos)
{
	bool found = false;
	pos = 0;

	if ( IsEquidistant() )
	{
		double position = ( x - GetStartTime() ) * GetFrameRate( );
		if ( position < 0 )
		{
			pos = 0;
		}
		else if ( position >= m_yVector.size() )
		{
			pos = m_yVector.size() - 1;
		}
		else
		{
			found = true;
			pos = unsigned ( ( x - GetStartTime() ) * GetFrameRate( ) );
		}
	}
	else {
		// Get the position in the xVector of the x value
		for (int i = 0; i < long(m_xVector.size())-1; i++)
		{
			if ( x >= m_xVector.at(i) && x <= m_xVector.at(i+1) )
			{
				pos = i;
				found = true;
			}
		}
	}

	return found;
}

void blSignal::SetValue( long pos, SampleType x, SampleType y )
{
	m_xVector[ pos ] = x;
	m_yVector[ pos ] = y;
}

void blSignal::SetValue( long pos, SampleType y )
{
	m_yVector[ pos ] = y;
}

void blSignal::RemoveAnnotation( int id )
{
	std::vector< blSignalAnnotation::Pointer >::iterator it = 
		m_AnnotationsVector.begin();
	m_AnnotationsVector.erase(it + id);

}

void blSignal::UpdateAnnotation( int id, int type, std::size_t duration )
{
	m_AnnotationsVector[ id ]->SetType( type );
	m_AnnotationsVector[ id ]->SetDuration(duration);
}

blSignalAnnotation::Pointer blSignal::GetAnnotation( int id )
{
	if ( unsigned( id ) < m_AnnotationsVector.size() )
	{
		return m_AnnotationsVector[ id ];
	}

	return NULL;
}

blSignalAnnotation::Pointer blSignal::FindAnnotation( unsigned short type )
{
	blSignalAnnotation::Pointer annotation;
	std::vector< blSignalAnnotation::Pointer >::iterator it;
	it = m_AnnotationsVector.begin(); 
	while ( annotation.IsNull( ) && it != m_AnnotationsVector.end() )
	{
		if ( type == (*it)->GetType() )
		{
			annotation = *it;
		}

		it++;
	}

	return annotation;
}

blSignal::SampleType blSignal::GetValueX( unsigned xPos )
{
	SampleType time = 0;

	if ( IsEquidistant() )
	{
		time = SampleType( xPos / GetFrameRate() + GetStartTime() );
	}
	else
	{
		if ( xPos < m_xVector.size() )
		{
			time = m_xVector.at( xPos );
		}
	}

	return time;
}

std::string blSignal::GetName() const
{
	return m_name;
}

void blSignal::SetName( const std::string val )
{
	m_name = val;
}

std::string blSignal::GetXUnit() const
{
	return m_xUnit;
}

void blSignal::SetXUnit( const std::string val )
{
	m_xUnit = val;
}

std::string blSignal::GetYUnit() const
{
	return m_yUnit;
}

void blSignal::SetYUnit( std::string val )
{
	m_yUnit = val;
}

std::vector< blSignal::SampleType > blSignal::GetXVector() const
{
	return m_xVector;
}

void blSignal::SetXVector( const std::vector< blSignal::SampleType >& val )
{
	m_xVector = val;
}

std::vector< blSignal::SampleType > blSignal::GetYVector() const
{
	return m_yVector;
}

void blSignal::SetYVector( const std::vector< blSignal::SampleType >& val )
{
	m_yVector = val;
}

blSignal::SampleType blSignal::GetValueY( unsigned yPos )
{
	if ( yPos < m_yVector.size() )
	{
		return m_yVector[ yPos ];
	}

	return 0;
}

size_t blSignal::GetNumberOfValues()
{
	return m_numberOfRecords * m_SamplesPerRecord;
}

double blSignal::GetFrameRate()
{
	if ( m_SamplesPerRecord == 0 )
	{
		return 0;
	}
	return m_SamplesPerRecord * m_recordDuration.dominator / m_recordDuration.numerator;
}

unsigned long blSignal::GetSamplesPerRecord() const
{
	return m_SamplesPerRecord;
}

void blSignal::SetSamplesPerRecord( unsigned long val )
{
	m_SamplesPerRecord = val;
}


unsigned int blSignal::GetNumberOfRecords() const
{
	return m_numberOfRecords;
}

void blSignal::SetNumberOfRecords( unsigned int val )
{
	m_numberOfRecords = val;
}

blSignal::Duration blSignal::GetRecordDuration() const
{
	return m_recordDuration;
}

void blSignal::SetRecordDuration( Duration val )
{
	m_recordDuration = val;
}

void blSignal::Allocate()
{
	if ( !IsEquidistant() )
	{
		m_xVector.resize( GetNumberOfValues( ) );
	}
	m_yVector.resize( GetNumberOfValues( ) );
}

bool blSignal::IsEquidistant()
{
	return (m_SamplesPerRecord != 0);
}

blSignal::SampleType blSignal::GetStartTime()
{
	return SampleType( m_StartTime & ULONG_MAX ) * 86400 / MAX_VALUE;
}

void blSignal::SetStartTime( SampleType startTime )
{
	// 2^32 == 86400 sec
	m_StartTime = InternalTimeType( startTime / 86400 * MAX_VALUE );
}

InternalTimeType blSignal::GetInternalStartTime()
{
	return m_StartTime;
}

void blSignal::SetInternalStartTime( InternalTimeType startTime )
{
	m_StartTime = startTime;
}

void blSignal::Copy( blSignal::Pointer input )
{
	if ( input.IsNull() )
	{
		return;
	}

	SetName( input->GetName() );
	SetXUnit( input->GetXUnit() );
	SetYUnit( input->GetYUnit() );
	SetXVector( input->GetXVector() );
	SetYVector( input->GetYVector() );
	SetNumberOfRecords( input->GetNumberOfRecords() );
	SetRecordDuration( input->GetRecordDuration() );
	SetSamplesPerRecord( input->GetSamplesPerRecord() );
	SetInternalStartTime( input->GetInternalStartTime() );

	CopyAnnotations( input );
}

void blSignal::CopyAnnotations( Pointer input )
{
	std::vector< blSignalAnnotation::Pointer > annotationVector;
	annotationVector = input->GetAnnotationsVector( );
	for ( unsigned i = 0 ; i < annotationVector.size() ; i++ )
	{
		blSignalAnnotation::Pointer annotation = blSignalAnnotation::New();
		annotation->Copy( annotationVector[ i ] );
		m_AnnotationsVector.push_back( annotation );
	}
}
