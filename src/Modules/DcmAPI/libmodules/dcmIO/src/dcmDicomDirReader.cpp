/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmDicomDirReader.h"
#include "dcmImageUtilities.h"
#include "dcmIOUtils.h"

// dcmtk
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcdicdir.h"

// Baselib
#include "blTextUtils.h"

using namespace dcmAPI;

dcmAPI::DicomDirReader::DicomDirReader()
{
	m_DataSet = dcmAPI::DataSet::New( );
}

dcmAPI::DataSet::Pointer dcmAPI::DicomDirReader::GetDataSet() const
{
	return m_DataSet;
}

void dcmAPI::DicomDirReader::SetDataSet( dcmAPI::DataSet::Pointer val )
{
	m_DataSet = val;
}

std::string dcmAPI::DicomDirReader::GetPath() const
{
	return m_Path;
}

void dcmAPI::DicomDirReader::SetPath( std::string val )
{
	m_Path = val;
}

void DicomDirReader::Update( )
{
	DataSet::PatientsVector myPatientList;

	DcmDicomDir* dcmDirObj = new DcmDicomDir(m_Path.c_str());

	DcmDirectoryRecord* ddrRoot = &(dcmDirObj->getRootRecord());
	if(ddrRoot != NULL)
	{
		Patient::Pointer patient;
		Study::Pointer study;
		Series::Pointer series;
		TimePoint::Pointer timePoint;
		Slice::Pointer slice;
		DcmDirectoryRecord * pImageRecord = NULL;
		DcmDirectoryRecord * pPatientRecord = NULL;
		DcmDirectoryRecord * pStudyRecord   = NULL;
		DcmDirectoryRecord * pSeriesRecord  = NULL;
		OFString tagValue;
		const char *tagValue_str = NULL;
		int patientId = 1;
		int studyId = 1;
		int seriesId = 1;
		int sliceId = 1;
		std::ostringstream idStr;

		// Parse Patient
		while ((pPatientRecord = ddrRoot->nextSub(pPatientRecord)) != NULL && 
				pPatientRecord->getRecordType() == ERT_Patient)
		{	
			bool patientDescriptorsFound = false;

			// new PatientData object
			patient = Patient::New();
			myPatientList.push_back(patient);

			// Patient ID
			idStr.str("");
			idStr<<patientId++;
			patient->AddTag(tags::PatientId, idStr.str());


			// Patient Name
			if (pPatientRecord->findAndGetOFString(DCM_PatientsName,tagValue).good())
			{
				patient->AddTag(tags::PatientName,tagValue.c_str());
			}

			// Parse Study
			while ((pStudyRecord = pPatientRecord->nextSub(pStudyRecord)) != NULL && 
				    pStudyRecord->getRecordType() == ERT_Study)
			{
				// new study data structure
				study = Study::New();
				patient->AddStudy(study);

				// Study Date
				if (pStudyRecord->findAndGetOFString(DCM_StudyDate, tagValue).good())
				{
					study->AddTag(tags::StudyDate,tagValue.c_str());
				}

				// Study Time
				if (pStudyRecord->findAndGetOFString(DCM_StudyTime, tagValue).good())
				{
					study->AddTag(tags::StudyTime,tagValue.c_str());
				}

				// Study Description
				if (pStudyRecord->findAndGetOFString(DCM_StudyDescription, tagValue).good())
				{
					study->AddTag(tags::StudyDescription,tagValue.c_str());
				}

				// Study UID (serves as ID)
				idStr.str("");
				idStr<<studyId++;
				study->AddTag(tags::StudyId, idStr.str());

				// Parse Serie
				while ((pSeriesRecord = pStudyRecord->nextSub(pSeriesRecord)) != NULL  && 
					    pSeriesRecord->getRecordType() == ERT_Series)
				{

					// new series data structure
					series = Series::New();
					study->AddSeries(series);
					
					TimePoint::Pointer timePoint = TimePoint::New();
					int timePointId = 1;
					std::string timeValue = "";
					bool hasTimeInformation = true;
					bool seriesDescrFound = false;
					bool pixelTypeFound = false;
					std::string filePath;
					std::string pixelType;

					// Series UID (serves as ID)
					//series id
					idStr.str("");
					idStr<<seriesId++;
					series->AddTag(tags::SeriesId, idStr.str());

					// Modality
					if (pSeriesRecord->findAndGetOFString(DCM_Modality, tagValue).good())
					{
						series->AddTag(tags::Modality, tagValue.c_str());
					}

					//Series manufacturer
					if (pSeriesRecord->findAndGetOFString(DCM_Manufacturer, tagValue).good())
					{
						series->AddTag(tags::Manufacturer, tagValue.c_str());
					}

					//check if time information exists
					std::string modality = series->GetTagAsText(tags::Modality);
					TagId timeTagId = IOUtils::GetTimeTagForSpecifiedModality(modality);
					if(timeTagId.m_group==0 && timeTagId.m_element==0)
						hasTimeInformation = false;

					//Parse Slice
					while ((pImageRecord = pSeriesRecord->nextSub(pImageRecord)) != NULL && 
						    pImageRecord->getRecordType() == ERT_Image)
					{
						// new Slice data structure
						slice = Slice::New();

						//slice id
						idStr.str("");
						idStr<<sliceId++;
						slice->AddTag(tags::SliceId, idStr.str());

						//image information
						if ((pImageRecord->findAndGetString(DCM_ReferencedFileID, tagValue_str).good()))
						{	
							//slice file
							std::string fileName = IOUtils::GetSliceFileNameFromReferenceFileID(tagValue_str);
							if(fileName.length() > 0)
							{
								slice->AddTag(tags::SliceFileName, fileName);
							}
							
							//slice file path 
							filePath = IOUtils::GetSliceFilePathFromReferenceFileID(tagValue_str, m_Path);
							if(filePath.length() > 0)
							{
								slice->AddTag(tags::SliceFilePath, filePath);
							}
						}
								
						//open file and read tags available from file level
						if(filePath.length() > 0)
						{
							File::Pointer dcmFile = File::New();
							if( dcmFile->Open(filePath) == true )
							{
								// read patient info
								if(!patientDescriptorsFound)
								{
									// Patient Birth Date
									std::string patientBirthDate = dcmFile->ReadTagValue(tags::PatientDate);
									if(!patientBirthDate.empty())
										patient->AddTag(tags::PatientDate, patientBirthDate);
									
									
									// Patient Sex
									std::string patientSex = dcmFile->ReadTagValue(tags::PatientDate);
									if(!patientSex.empty())
										patient->AddTag(tags::PatientSex, patientSex);
									
									if(!patientBirthDate.empty() && !patientSex.empty())
										patientDescriptorsFound = true;
								}

								//Series Description
								if(!seriesDescrFound)
								{
									std::string seriesDescription = dcmFile->ReadTagValue(tags::SeriesDescription);
									if(!seriesDescription.empty())
									{
										series->AddTag(tags::SeriesDescription, seriesDescription);
										seriesDescrFound = true;
									}
								}

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
								
								//image position
								std::string imagePosition = dcmFile->ReadTagValue(tags::ImagePositionPatient);
								if(!imagePosition.empty())
								{
									slice->AddTag(tags::ImagePositionPatient, imagePosition);
								}

								//image orientation
								std::string imageOrientation = dcmFile->ReadTagValue(tags::ImageOrientationPatient);
								if(!imageOrientation.empty())
								{
									slice->AddTag(tags::ImageOrientationPatient, imageOrientation);
								}
		
								//Window width
								std::string windowWidth = dcmFile->ReadTagValue(tags::WindowWidth);
								if(!windowWidth.empty())
								{
									slice->AddTag(tags::WindowWidth, windowWidth);
								}

								//Window center
								std::string windowCenter = dcmFile->ReadTagValue(tags::WindowCenter);
								if(!windowWidth.empty())
								{
									slice->AddTag(tags::WindowCenter, windowCenter);
								}	
								
								//ReadTimeValue
								if(hasTimeInformation)
								{
									timeValue = dcmFile->ReadTagValue(timeTagId); 
								}
								
								//find timepoint
								timePoint = IOUtils::FindTimepoint(timeValue, series);
								if(timePoint.get() == NULL)
								{
									//timepoint not found, create new timepoint
									timePoint = TimePoint::New();
									idStr.str("");
									idStr<<timePointId++;
									timePoint->AddTag(tags::TimePointId, idStr.str());
									timePoint->AddTag(tags::TimeValue, timeValue);
									series->AddTimePoint(timePoint);
									timePointId++;
								}

								// add slice to timepoint
								timePoint->AddSlice(slice);
							}
						}//while Image
					}

					// sort slices in timepoints
					dcmAPI::TimePointIdVectorPtr timePointIdVector = series->TimePointIds();
					for(size_t i=0; i < timePointIdVector->size(); i++)
					{
						std::string timepointId = timePointIdVector->at(i);
						IOUtils::SortTimepoint(series->TimePoint(timepointId));
					}

					IOUtils::SortSeriesByTime( series );

				}//while Serie

			}//while Study

		}//while Patient

	}//if(ddrRoot != NULL)
	delete dcmDirObj;

	m_DataSet->SetPatientsVector( myPatientList );
}

