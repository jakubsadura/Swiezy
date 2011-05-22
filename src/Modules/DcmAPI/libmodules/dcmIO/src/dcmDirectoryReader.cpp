/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmDirectoryReader.h"
#include "dcmImageUtilities.h"
#include "dcmIOUtils.h"

// BaseLib
#include "blTextUtils.h"

//gdcm
#include "gdcmUtil.h"
#include "gdcmDicomDir.h"
#include "gdcmDicomDirPatient.h"
#include "gdcmDicomDirStudy.h"
#include "gdcmDicomDirSerie.h"
#include "gdcmDicomDirImage.h"

//itk
#include "itkExtractImageFilter.h"
#include "itkGDCMImageIO.h"

using namespace dcmAPI;

DirectoryReader::DirectoryReader()
{
	m_DataSet = DataSet::New();
	m_logger = blLogger::getInstance("dcmapi.DirectoryReader");
	
	m_TruncateSeparator = '\0';
	m_TruncateLevel = 0;
	m_IsTimeTagInFile = false;
}

TagId DirectoryReader::GetTimeTag() const
{
    return m_TimeTag;
}

void DirectoryReader::SetTimeTag(const TagId& timeTag)
{
    m_TimeTag = timeTag;
}

void DirectoryReader::SetTimeTagTruncate(char separator, int level)
{
    m_TruncateSeparator = separator;
    m_TruncateLevel = level;
}

void DirectoryReader::SetTimeTagLocation(bool isInFile)
{
    m_IsTimeTagInFile = isInFile;
}

std::string DirectoryReader::TruncateTagValue(const std::string& inputStr, char separator, int level) const
{
    std::string str = inputStr;
    for( int i = 0; i < level; ++i )
    {
        str = str.substr( 0, str.find_last_of(separator) );
    }
    return str;
}

TagId DirectoryReader::GetFinalTimeTag(const std::string& modality) const
{
    // if no time tage was set use the default one related to modality
    if(m_TimeTag.m_group==0 && m_TimeTag.m_element==0)
    {
	    return IOUtils::GetTimeTagForSpecifiedModality(modality);
    }
    // use the set one
    return m_TimeTag;
}

std::string DirectoryReader::GetTimeTagValue(gdcm::DocEntry* docEntry) const
{
    std::string value;
    if(docEntry != NULL)
    {
	    GdcmValEntryPtr valEntry;
	    valEntry.reset(new gdcm::ValEntry(docEntry));
	    value = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
	    if( m_TruncateSeparator != '\0' )
	    {
	        value = TruncateTagValue( value, m_TruncateSeparator, m_TruncateLevel );
	    }
    }
    else
    {
        // cannot find entry...
        LOG4CPLUS_WARN(m_logger, "No value for time tag.");
    }
    return value;
}

void DirectoryReader::Update()
{
	DataSet::PatientsVector myPatientList;
	boost::shared_ptr< gdcm::DicomDir > dcmdir(new gdcm::DicomDir());
	dcmdir->SetDirectoryName(m_Path);
	
	if(dcmdir->Load() == true)
	{
		// fill patients list
		gdcm::DicomDirPatient* gdcmPatient(dcmdir->GetFirstPatient());
		GdcmValEntryPtr valEntry;
		gdcm::DocEntry* entry = NULL;
		std::string val;
		std::ostringstream idStr;
		int patientId = 1;
		int studyId = 1;
		int seriesId = 1;
		int sliceId = 1;
		while(gdcmPatient != NULL)
		{
			//create patient object and retrieve required tags
			Patient::Pointer patient = Patient::New();

			//patient id
			/*entry = gdcmPatient->GetDocEntry(tags::PatientID.m_group, tags::PatientID.m_element);
			valEntry.reset(new gdcm::ValEntry(entry));
			val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());*/
			idStr.str("");
			idStr<<patientId++;
			patient->AddTag(tags::PatientId, idStr.str());

			//patient name
			entry = gdcmPatient->GetDocEntry(tags::PatientName.m_group, tags::PatientName.m_element);
			valEntry.reset(new gdcm::ValEntry( entry));
			val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			patient->AddTag(tags::PatientName, val);

			//patient birth date
			entry = gdcmPatient->GetDocEntry(tags::PatientDate.m_group, tags::PatientDate.m_element);
			valEntry.reset(new gdcm::ValEntry( entry));
			val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			patient->AddTag(tags::PatientDate, val);

			//patient sex
			entry = gdcmPatient->GetDocEntry(tags::PatientSex.m_group, tags::PatientSex.m_element);
			valEntry.reset(new gdcm::ValEntry( entry));
			val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
			patient->AddTag(tags::PatientSex, val);
			
			// add to patient list
			myPatientList.push_back(patient);

			//fill study list
			gdcm::DicomDirStudy* gdcmStudy(gdcmPatient->GetFirstStudy());
			while(gdcmStudy != NULL)
			{	
				//create study object and retrieve required tags
				Study::Pointer study = Study::New();
				
				// Study Date
				entry = gdcmStudy->GetDocEntry(tags::StudyDate.m_group, tags::StudyDate.m_element);
				if(entry != NULL)
				{
					valEntry.reset(new gdcm::ValEntry(entry));
					val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
					study->AddTag(tags::StudyDate, val);
				}

				// Study Description
				entry = gdcmStudy->GetDocEntry(tags::StudyDescription.m_group, tags::StudyDescription.m_element);
				if(entry != NULL)
				{
					valEntry.reset(new gdcm::ValEntry(entry));
					val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
					study->AddTag(tags::StudyDescription, val);	
				}

				// Study UID (serves as ID)
				/*entry = gdcmStudy->GetDocEntry(tags::StudyInstanceUID.m_group, tags::StudyInstanceUID.m_element);
				if(entry != NULL)
				{
					valEntry.reset(new gdcm::ValEntry(entry));
					val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
					study->AddTag(tags::StudyInstanceUID, val);
				}*/

				//study id
				idStr.str("");
				idStr<<studyId++;
				study->AddTag(tags::StudyId, idStr.str());

				// add study to patient
				patient->AddStudy(study);

				//fill series list
				gdcm::DicomDirSerie* gdcmSeries(gdcmStudy->GetFirstSerie());
				while(gdcmSeries != NULL)
				{
					//create series object and retrieve required tags
					Series::Pointer series = Series::New();

					// Series  modality
					entry = gdcmSeries->GetDocEntry(tags::Modality.m_group, tags::Modality.m_element);
					if(entry != NULL)
					{
						valEntry.reset(new gdcm::ValEntry(entry));
						val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
						series->AddTag(tags::Modality, val);
					}
					
					//Series manufacturer
					entry = gdcmSeries->GetDocEntry(tags::Manufacturer.m_group, tags::Manufacturer.m_element);
					if(entry != NULL)
					{
						valEntry.reset(new gdcm::ValEntry(entry));
						val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
						series->AddTag(tags::Manufacturer, val);
					}

					//check if time information exists
					TimePoint::Pointer timePoint = TimePoint::New();
					int timePointId = 1;
					std::string timeValue = "";
					bool hasTimeInformation = true;
					std::string modality = series->GetTagAsText(tags::Modality);
					TagId timeTagId = GetFinalTimeTag(modality);
					LOG4CPLUS_DEBUG(m_logger, "Time tag: " << timeTagId);
					hasTimeInformation = timeTagId.IsSet();
					
					// Series UID (serves as ID)
					/*entry = gdcmSeries->GetDocEntry(tags::SeriesInstanceUID.m_group, tags::SeriesInstanceUID.m_element);
					if(entry != NULL)
					{
						valEntry.reset(new gdcm::ValEntry(entry));
						val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
						series->AddTag(tags::SeriesInstanceUID, val);
					}*/

					//series id
					idStr.str("");
					idStr<<seriesId++;
					series->AddTag(tags::SeriesId, idStr.str());
					
					// Series description
					entry = gdcmSeries->GetDocEntry(tags::SeriesDescription.m_group, tags::SeriesDescription.m_element);
					{
						valEntry.reset(new gdcm::ValEntry(entry));
						val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
						series->AddTag(tags::SeriesDescription, val);
					}
					
					//add series to study
					study->AddSeries(series);
					
					//helper variables
					std::string filePath;
					std::string pixelType;
					bool pixelTypeFound = false;

					//fill timepoint list
					gdcm::DicomDirImage* gdcmImage(gdcmSeries->GetFirstImage());
					while(gdcmImage != NULL)
					{   
						//create slice object and retrieve required tags
						Slice::Pointer slice = Slice::New();
						
						// ReferencedFileID
						entry = gdcmImage->GetDocEntry
							(
							tags::ReferencedFileID.m_group,
							tags::ReferencedFileID.m_element
							);
						if(entry != NULL)
						{
							valEntry.reset(new gdcm::ValEntry(entry));
							val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());

							//slice file name
							std::string fileName = IOUtils::GetSliceFileNameFromReferenceFileID(val);
							if(fileName.length() > 0)
							{
								slice->AddTag(tags::SliceFileName, fileName);
							}

							//slice file path 
							filePath = IOUtils::GetSliceFilePathFromReferenceFileID(val, m_Path, false);
							if(filePath.length() > 0)
							{
								slice->AddTag(tags::SliceFilePath, filePath);
							}
						}

						// ImageNumber
						entry = gdcmImage->GetDocEntry(tags::ImageNumber.m_group, tags::ImageNumber.m_element);
						if(entry != NULL)
						{
							valEntry.reset(new gdcm::ValEntry(entry));
							val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
							slice->AddTag(tags::ImageNumber, val);
						}

						//slice id
						idStr.str("");
						idStr<<sliceId++;
						slice->AddTag(tags::SliceId, idStr.str());
						
						//pixel type is the same for all slices so it has to be read just 1 time
						if(!pixelTypeFound)
						{
							pixelType = ImageUtilities::ReadPixelType(filePath);
							pixelTypeFound = true;
						}
						//pixel type
						if(!pixelType.empty())
						{
							slice->AddTag(tags::PxType, pixelType);
						}
						else
							pixelTypeFound = false;

						// Image Position
						entry = gdcmImage->GetDocEntry
						(
						tags::ImagePositionPatient.m_group,
						tags::ImagePositionPatient.m_element
						);
						if(entry != NULL)
						{
							valEntry.reset(new gdcm::ValEntry(entry));
							val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
							slice->AddTag(tags::ImagePositionPatient, val);
						}

						// Image Orientation
						entry = gdcmImage->GetDocEntry
							(
							tags::ImageOrientationPatient.m_group,
							tags::ImageOrientationPatient.m_element
							);
						if(entry != NULL)
						{
							valEntry.reset(new gdcm::ValEntry(entry));
							val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
							slice->AddTag(tags::ImageOrientationPatient, val);
						}
						
						// Window width
						entry = gdcmImage->GetDocEntry(tags::WindowWidth.m_group, tags::WindowWidth.m_element);
						if(entry != NULL)
						{
							valEntry.reset(new gdcm::ValEntry(entry));
							val = IOUtils::CleanGdcmEntryStringValue(valEntry->GetValue());
							slice->AddTag(tags::WindowWidth, val);
						}

						//Window center
						entry = gdcmImage->GetDocEntry(tags::WindowCenter.m_group, tags::WindowCenter.m_element);
						if(entry != NULL)
						{
							slice->AddTag(tags::WindowCenter, val);
						}

						//ReadTimeValue
						if(hasTimeInformation)
						{
							// read the information in a gdcm file
							if( m_IsTimeTagInFile )
							{
						    	gdcm::File gdcmFile;
	                            gdcmFile.SetFileName( filePath );
	                            gdcmFile.Load();
	                            entry = gdcmFile.GetDocEntry(timeTagId.m_group, timeTagId.m_element);
							    timeValue = GetTimeTagValue(entry);
							}
							// get the information from the image
							else
							{
	                            entry = gdcmImage->GetDocEntry(timeTagId.m_group, timeTagId.m_element);
							    timeValue = GetTimeTagValue(entry);
							}
						}
						
						//find timepoint
						timePoint = IOUtils::FindTimepoint(timeValue, series);
						if(timePoint.get() == NULL)
						{
							//timepoint not found, create new timepoint
							timePoint = TimePoint::New();

							//Convert the integer into a string
							idStr.str("");
							idStr<<timePointId++;
							timePoint->AddTag(tags::TimePointId, idStr.str());
							timePoint->AddTag(tags::TimeValue, timeValue);
							series->AddTimePoint(timePoint);
						}

						// add slice to timepoint
						timePoint->AddSlice(slice);

						gdcmImage = gdcmSeries->GetNextImage();
					}

					// sort slices in timepoints
					dcmAPI::TimePointIdVectorPtr timePointIdVector = series->TimePointIds();
					for(size_t i=0; i < timePointIdVector->size(); i++)
					{
						std::string timepointId = timePointIdVector->at(i);
						IOUtils::SortTimepoint(series->TimePoint(timepointId));
					}

					IOUtils::SortSeriesByTime( series );

					gdcmSeries = gdcmStudy->GetNextSerie();
				}
				gdcmStudy = gdcmPatient->GetNextStudy();
			}
			gdcmPatient = dcmdir->GetNextPatient();
		}
	}
	
	m_DataSet->SetPatientsVector( myPatientList );
}

dcmAPI::DataSet::Pointer DirectoryReader::GetDataSet() const
{
	return m_DataSet;
}

void DirectoryReader::SetDataSet( dcmAPI::DataSet::Pointer val )
{
	m_DataSet = val;
}
std::string dcmAPI::DirectoryReader::GetPath() const
{
	return m_Path;
}

void dcmAPI::DirectoryReader::SetPath( std::string val )
{
	m_Path = val;
}
