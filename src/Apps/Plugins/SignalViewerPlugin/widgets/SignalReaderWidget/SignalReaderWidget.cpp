/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SignalReaderWidget.h"


SignalReaderWidget::SignalReaderWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    SignalReaderWidgetUI(parent, id, pos, size, style)
{

}

Core::BaseProcessor::Pointer SignalReaderWidget::GetProcessor()
{
	return NULL;
}

void SignalReaderWidget::OnSpinFactor( wxSpinEvent &event )
{
	UpdateWidget( );
}

void SignalReaderWidget::SetProperties( blTagMap::Pointer tagMap )
{
	//size_t totalSamples = 0;
	//size_t numberOfSignals = 0;
	//size_t numberOfDataRecords = 0;
	//size_t samplesPerRecord = 0;
	//float sampleRate = 0;
	//size_t sampleSize = 0;
	//
	//blTag::Pointer tag;
	//tag = tagMap->FindTagByName( "NumberOfSignals" );
	//if ( tag.IsNotNull() )
	//{
	//	tag->GetValue( numberOfSignals );
	//}

	//tag = tagMap->FindTagByName( "NumberOfDataRecords" );
	//if ( tag.IsNotNull() )
	//{
	//	tag->GetValue( numberOfDataRecords );
	//}

	//tag = tagMap->FindTagByName( "SamplesPerRecord" );
	//if ( tag.IsNotNull() )
	//{
	//	tag->GetValue( samplesPerRecord );
	//}

	//tag = tagMap->FindTagByName( "SampleRate" );
	//if ( tag.IsNotNull() )
	//{
	//	tag->GetValue( sampleRate );
	//}

	//tag = tagMap->FindTagByName( "SampleSize" );
	//if ( tag.IsNotNull() )
	//{
	//	tag->GetValue( sampleSize );
	//}

	//tag = tagMap->FindTagByName( "TotalSamples" );
	//if ( tag.IsNotNull() )
	//{
	//	tag->GetValue( totalSamples );
	//}

	m_TagMap = tagMap;
	std::vector<blTagMap::Pointer>  tagMapVector;
	tagMapVector.push_back( tagMap );
	m_PropertiesWidget->UpdateWidget( tagMapVector );

	Fit();

	UpdateWidget();
}

void SignalReaderWidget::GetProperties( blTagMap::Pointer tagMap )
{
	UpdateData( );

	tagMap->AddTags( m_TagMap );
}

void SignalReaderWidget::UpdateWidget()
{
	blTag::Pointer tag;
	float sampleRate = 0;
	tag = m_TagMap->FindTagByName( "SampleRate" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( sampleRate );
	}

	size_t fileSize = 0;
	tag = m_TagMap->FindTagByName( "FileSize (Mb)" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( fileSize );
	}

	float newSampleRate = sampleRate / m_SpinFactor->GetValue();
	float newSize = fileSize / m_SpinFactor->GetValue();
	m_txtOutputFreq->SetValue( wxString::Format( "%f", newSampleRate ) );
	m_txtOutputSize->SetValue( wxString::Format( "%f", newSize ) );
}

void SignalReaderWidget::UpdateData()
{
	m_TagMap->AddTag( "SampleRateFactor", int ( m_SpinFactor->GetValue() ) );
}
