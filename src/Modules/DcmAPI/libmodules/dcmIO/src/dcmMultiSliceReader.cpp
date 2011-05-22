/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmMultiSliceReader.h"
#include "dcmTypes.h"
#include "dcmImageUtilities.h"

// BaseLib
#include <blImageUtils.h>

using namespace dcmAPI;

MultiSliceReader::MultiSliceReader()
{
	m_DataSet = dcmAPI::DataSet::New( );
	m_SignalCollectiveECG = blSignalCollective::New( );
}

MultiSliceReader::~MultiSliceReader()
{
}

void MultiSliceReader::Update()
{
    for( unsigned int i = 0; i < m_Readers.size(); ++i )
    {
        if( m_Readers[i]->CanRead(m_DataSet->GetDataType()) )
        {
            m_Readers[i]->SetFilename(m_Path.c_str());
            m_Readers[i]->Update();
            m_Readers[i]->GetVolumes(m_vtkVolumesVector, m_VolumesTimestamps);
            // possible ECG data
            if( m_Readers[i]->HasEcg() )
            {
			    EcgData ecgData;
			    
			    m_Readers[i]->GetEcgData(ecgData);
			    
			    std::vector<float> data;
			    ecgData.GetData(data);

			    blSignal::Pointer signal;
			    signal = blSignal::BuildSignalFromEcgData( 
				    data, 
				    ecgData.GetStartTime(), 
				    ecgData.GetTimeIncrement(), 
				    100 );
			    m_SignalCollectiveECG->SetNumberOfSignals(1);
			    m_SignalCollectiveECG->SetSignal(0, signal);
            }
            // exit at the first good reader
            return;
        }
    }
    
    // arriving here means that no reader was found, throw an exception
    std::string message = "No Reader found for ";
	switch(m_DataSet->GetDataType())
	{
	    case dcmAPI::DataSet::STANDARD:
	        message += "'Standard'";
	        break;
	    case dcmAPI::DataSet::PHILIPS_US:
	        message += "'Philips US'";
	        break;
	    case dcmAPI::DataSet::GE_US:
	        message += "'GE US'";
	        break;
	    default:
	        message += "'Unknown type'";
	}
    message += " data.";
    throw ImageReaderException(message.c_str());
}

dcmAPI::DataSet::Pointer MultiSliceReader::GetDataSet() const
{
	return m_DataSet;
}

void MultiSliceReader::SetDataSet( dcmAPI::DataSet::Pointer val )
{
	m_DataSet = val;
}

std::string dcmAPI::MultiSliceReader::GetPath() const
{
	return m_Path;
}

void dcmAPI::MultiSliceReader::SetPath( std::string val )
{
	m_Path = val;
}

std::vector< vtkSmartPointer<vtkImageData> > 
dcmAPI::MultiSliceReader::GetOutput( )
{
	return m_vtkVolumesVector;
}

std::vector< float > dcmAPI::MultiSliceReader::GetVolumesTimestamps() const
{
	return m_VolumesTimestamps;
}

blSignalCollective::Pointer dcmAPI::MultiSliceReader::GetSignalCollectiveECG() const
{
	return m_SignalCollectiveECG;
}
