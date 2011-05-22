/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmFileReader.h"
#include "dcmImageUtilities.h"
#include "dcmIOUtils.h"

//gdcm
#include "gdcmUtil.h"

namespace dcmAPI
{

FileReader::FileReader()
{
	m_DataSet = dcmAPI::DataSet::New( );
}

FileReader::~FileReader()
{
}

void FileReader::Update()
{
	DataSet::PatientsVector patientList;
	GdcmFilePtr gdcmFile(new gdcm::File());
	gdcmFile->SetFileName( m_Path );
	if(gdcmFile->Load())
	{
		GdcmValEntryPtr valEntry;
		gdcm::DocEntry* entry = NULL;
		std::string val, modality, manufacturer;

		/*
		fill structure from patient to slice,
		we assume that there is 1 patient, 1 study, 1 series
		all the id tags must be filled so if the id value is not found in the tag initialize it with default value
		*/
		//create and fill patient object
		Patient::Pointer patient = Patient::New();
		//patient id
		entry = gdcmFile->GetDocEntry(tags::PatientRealID.m_group, tags::PatientRealID.m_element);
		valEntry.reset(new gdcm::ValEntry(entry));
		val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
		if(val.size() == 0)
			val = "1";
		patient->AddTag(tags::PatientId, val);
		
		//-----------------------------PATIENT---------------------------------------
		//patient name
		entry = gdcmFile->GetDocEntry(tags::PatientName.m_group, tags::PatientName.m_element);
		valEntry.reset(new gdcm::ValEntry(entry));
		val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
		patient->AddTag(tags::PatientName, val);

		//patient birth date
		entry = gdcmFile->GetDocEntry(tags::PatientDate.m_group, tags::PatientDate.m_element);
		valEntry.reset(new gdcm::ValEntry(entry));
		val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
		patient->AddTag(tags::PatientDate, val);

		//patient sex
		entry = gdcmFile->GetDocEntry(tags::PatientSex.m_group, tags::PatientSex.m_element);
		valEntry.reset(new gdcm::ValEntry(entry));
		val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
		patient->AddTag(tags::PatientSex, val);

		// add to patient list
		patientList.push_back(patient);
		
		//-----------------------------STUDY---------------------------------------
		//create study object and retrieve required tags
		Study::Pointer study = Study::New();
			
		// Study Date
		entry = gdcmFile->GetDocEntry(tags::StudyDate.m_group, tags::StudyDate.m_element);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			study->AddTag(tags::StudyDate, val);
		}

		// Study Description
		entry = gdcmFile->GetDocEntry(tags::StudyDescription.m_group, tags::StudyDescription.m_element);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			study->AddTag(tags::StudyDescription, val);	
		}

		// Study UID (serves as ID)
		entry = gdcmFile->GetDocEntry(tags::StudyInstanceUID.m_group, tags::StudyInstanceUID.m_element);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			if(val.size() == 0)
				val = "1";
		}
		else
			val = "1";
		study->AddTag(tags::StudyId, val);

		// add study to patient
		patient->AddStudy(study);
		//---------------------------------------------------------------------------------------
		
		//-----------------------------SERIES---------------------------------------
		//create series object and retrieve required tags
		Series::Pointer series = Series::New();

		// Series UID (serves as ID)
		entry = gdcmFile->GetDocEntry(tags::SeriesInstanceUID.m_group, tags::SeriesInstanceUID.m_element);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			if(val.size() == 0)
				val = "1";
		}
		else
			val = "1";
		series->AddTag(tags::SeriesId, val);


		// Series modality
		entry = gdcmFile->GetDocEntry(tags::Modality.m_group, tags::Modality.m_element);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			modality = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			series->AddTag(tags::Modality, modality);
		}

		//Series manufacturer
		entry = gdcmFile->GetDocEntry(tags::Manufacturer.m_group, tags::Manufacturer.m_element);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			manufacturer = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			series->AddTag(tags::Manufacturer, manufacturer);
		}

		// Series description
		entry = gdcmFile->GetDocEntry(tags::SeriesDescription.m_group, tags::SeriesDescription.m_element);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			series->AddTag(tags::SeriesDescription, val);
		}

		//add series to study
		study->AddSeries(series);

		//-----------------------------TIMEPOINT->SLICE-------------------------------------
		// get information for all slices
		// Pixel Type 
		std::string pixelType = ImageUtilities::ReadPixelType(m_Path);

		// Image Orientation
		std::string orientation;
		entry = gdcmFile->GetDocEntry
			(
			tags::ImageOrientationPatient.m_group,
			tags::ImageOrientationPatient.m_element
			);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			orientation = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
		}

		// Window width
		std::string windowWidth;
		entry = gdcmFile->GetDocEntry(tags::WindowWidth.m_group, tags::WindowWidth.m_element);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			windowWidth = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
		}

		//Window center
		std::string windowCenter; 
		entry = gdcmFile->GetDocEntry(tags::WindowCenter.m_group, tags::WindowCenter.m_element);
		if(entry != NULL)
		{
			valEntry.reset(new gdcm::ValEntry(entry));
			windowCenter = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
		}
		
		DcmDataDimension dataDim = GetDataDimensionFromDcmFile(gdcmFile, manufacturer, modality);
		for(int i=0; i<dataDim.timepointsCount; i++)
		{
			TimePoint::Pointer timePoint = TimePoint::New();
			std::ostringstream timePointId;
			timePointId << i+1;
			timePoint->AddTag(tags::TimePointId, timePointId.str());

			for(int j=0; j<dataDim.slicesCount; j++)
			{
				//create slice object and retrieve required tags
				Slice::Pointer slice = Slice::New();

				// ImageNumber
				std::ostringstream imgNr;
				imgNr << j+1;
				slice->AddTag(tags::SliceId, imgNr.str());

				//slice file name
				std::string fileName = IOUtils::GetSliceFileNameFromReferenceFileID(m_Path) + "-slice" + imgNr.str();
				if(fileName.length() > 0)
				{
					slice->AddTag(tags::SliceFileName, fileName);
				}

				//slice file path 
				slice->AddTag(tags::SliceFilePath, m_Path);
				
				// Image Orientation
				slice->AddTag(tags::ImageOrientationPatient, orientation);

				// Window width
				slice->AddTag(tags::WindowWidth, windowWidth);

				//Window center
				slice->AddTag(tags::WindowCenter, windowCenter);

				//Pixel type
				slice->AddTag(tags::PxType, pixelType);

				// add slice to timepoint
				timePoint->AddSlice(slice);
			}
			// add timepoint to series
			series->AddTimePoint(timePoint);
		}
	}		

	m_DataSet->SetPatientsVector( patientList );
}

DcmDataDimension FileReader::GetDataDimensionFromDcmFile(GdcmFilePtr gdcmFile, std::string manufacturer, std::string modality)
{
	// default
	DcmDataDimension dataDim = { 0, 0 };

	if( gdcmFile.get() != NULL )
	{
	    // dimension is stored in various tags, depending on manufacturer and modality
	    // get size for specified manufacturer
	    for( unsigned int i = 0; i < m_Readers.size(); ++i )
	    {
	        if( m_Readers[i]->CanRead(manufacturer, modality) )
	        {
	            m_Readers[i]->SetFilename(gdcmFile.get()->GetFileName());
	            m_Readers[i]->Update();
	            return m_Readers[i]->GetDataDimension();
	        }
	    }
	    // arriving here means that no reader was found, use the standard one
        dataDim.slicesCount = gdcmFile->GetZSize();
        dataDim.timepointsCount = 1;
    }
	return dataDim;
}

dcmAPI::DataSet::Pointer FileReader::GetDataSet() const
{
	return m_DataSet;
}

void FileReader::SetDataSet( dcmAPI::DataSet::Pointer val )
{
	m_DataSet = val;
}

std::string dcmAPI::FileReader::GetPath() const
{
	return m_Path;
}

void dcmAPI::FileReader::SetPath( std::string val )
{
	m_Path = val;
}

} // namespace dcmAPI
