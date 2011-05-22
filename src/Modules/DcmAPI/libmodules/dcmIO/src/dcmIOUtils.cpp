/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmIOUtils.h"
#include "dcmImageUtilities.h"

//gdcm
#include "gdcmUtil.h"

// Baselib
#include "blTextUtils.h"
#include <blLogger.h>

using namespace dcmAPI;

TimeTagForModalityMap IOUtils::m_timeTagForModalityMap;

std::string IOUtils::GetSliceFilePathFromReferenceFileID
(
 std::string referenceFileID,
 std::string path,
 bool dicomdir
 )
{
	std::string filePath = "";
	try
	{
		if(dicomdir)
		{
			size_t lastPos = path.find_last_of("/\\");
			if(lastPos != std::string::npos)
				filePath = path.substr(0, lastPos+1);
		}
		else
			filePath = path;
		
		filePath += referenceFileID;
		blTextUtils::ConvertPath(filePath);
	}
	catch(...)
	{
		filePath = "";
	}

	return filePath;
}

std::string IOUtils::GetSliceFileNameFromReferenceFileID(std::string referenceFileID)
{
	std::string fileName;
	try
	{
 		size_t lastPos = referenceFileID.find_last_of("/\\");
		if(lastPos != std::string::npos)
		{
			fileName = referenceFileID.substr(lastPos+1);
		}
		else if(!referenceFileID.empty())
		{
			fileName = referenceFileID;
		}
	}
	catch(...)
	{
		fileName = "";
	}

	return fileName;
}

void IOUtils::InitializeTimeTagForModalityMap()
{
	m_timeTagForModalityMap[ "MR" ] = tags::TriggerTime;
	m_timeTagForModalityMap[ "US" ] = tags::PhaseNumber;
	m_timeTagForModalityMap[ "CT" ] = tags::PercentageOfFullHeartBeat;
}

TagId IOUtils::GetTimeTagForSpecifiedModality(const std::string& modality)
{
	InitializeTimeTagForModalityMap( );

	TagId tagId;
	TimeTagForModalityMap::iterator iter = m_timeTagForModalityMap.find(modality);
	if( iter != m_timeTagForModalityMap.end() )
	{
		tagId = iter->second;
		LOG4CPLUS_DEBUG(blLogger::getInstance("dcmapi.IOUtils"), "Using time tag: " << tagId);
	}
	
	return tagId;
}

TimePoint::Pointer IOUtils::FindTimepoint(std::string& timeValue, Series::Pointer series)
{
	TimePoint::Pointer timePoint;
	TimePointIdVectorPtr timePointIdVector = series->TimePointIds();
	for(unsigned int i=0; i < timePointIdVector->size(); i++)
	{
		std::string& id = timePointIdVector->at(i);
		TimePoint::Pointer tempTimePoint = series->TimePoint(id);
		if (tempTimePoint->GetTagAsText(tags::TimeValue) == timeValue)
		{
			timePoint = tempTimePoint;
			break;
		}
	}
	return timePoint;
}

void IOUtils::SortTimepoint(TimePoint::Pointer timepoint)
{
	if (SortTimepointByImagePosition(timepoint) == false)
	{
		LOG4CPLUS_WARN(blLogger::getInstance("dcmapi.IOUtils"), "Cannot sort by image orientation/position, sorting by image number.");
		SortTimepointByImageNumber(timepoint);
	}
}

bool IOUtils::SortTimepointByImagePosition(TimePoint::Pointer timepoint)
{
	double cosines[6];	
	double normal[3];
	double ipp[3];
	double dist;
	double min = 0, max = 0;
	bool first = true;
	std::multimap<double, Slice::Pointer> distmultimap;
	
	for(size_t i = 0; i<timepoint->SliceIds()->size(); i++)
	{
		std::string currentSliceId = timepoint->SliceIds()->at(i);
		Slice::Pointer slice = timepoint->Slice(currentSliceId);
		if(first)
		{
			// You only have to do this once for all slices in the volume. Next, 
			// for each slice, calculate the distance along the slice normal 
			// using the IPP ("Image Position Patient") tag.
			// ("dist" is initialized to zero before reading the first slice) :
			
			std::string imOr = slice->GetTagAsText(tags::ImageOrientationPatient);
			if(imOr.size() == 0)
			{
		        LOG4CPLUS_WARN(blLogger::getInstance("dcmapi.IOUtils"), "No image orientation.");
				return false;
			}

			cosines[0] = cosines[4] = 1.;
			cosines[1] = cosines[2] = cosines[3] = cosines[5] = 0.;

			ImageUtilities::StringToDouble(imOr.c_str(), cosines, 6);
			normal[0] = cosines[1]*cosines[5] - cosines[2]*cosines[4];
			normal[1] = cosines[2]*cosines[3] - cosines[0]*cosines[5];
			normal[2] = cosines[0]*cosines[4] - cosines[1]*cosines[3];

		first = false;
		}

		std::string imPos = slice->GetTagAsText(tags::ImagePositionPatient);
		if(imPos.size() == 0)
		{
			LOG4CPLUS_WARN(blLogger::getInstance("dcmapi.IOUtils"), "No image position patient.");
			return false;
		}

		ImageUtilities::StringToDouble(imPos.c_str(), ipp, 3);
		dist = 0;
		for ( int i = 0; i < 3; ++i )
		{
			dist += normal[i]*ipp[i];
		}

		min = (min < dist) ? min : dist;
		max = (max > dist) ? max : dist;

		distmultimap.insert(std::pair<double, Slice::Pointer>(dist, slice));
	}

	// Find out if min/max are coherent
	if ( min == max )
	{
		LOG4CPLUS_WARN(blLogger::getInstance("dcmapi.IOUtils"), "Equal min and max position.");
		return false;
	}

	// Check to see if image shares a common position
	for (std::multimap<double, Slice::Pointer>::iterator it2 = distmultimap.begin(); it2 != distmultimap.end();
		++it2)
	{
		if (distmultimap.count((*it2).first) != 1)
		{
			LOG4CPLUS_WARN(blLogger::getInstance("dcmapi.IOUtils"), "More than one image at the same postition.");
        	return false;
		}
	}
	
	// create new order
	timepoint->RemoveAllSlices();
	for (std::multimap<double, Slice::Pointer>::iterator it3 = distmultimap.begin(); it3 != distmultimap.end();
		++it3)
	{
		Slice::Pointer s = (*it3).second;
		timepoint->AddSlice((*it3).second );
	}

	return true;
}

bool IOUtils::SortTimepointByImageNumber(TimePoint::Pointer timepoint)
{
	int minImageNumber = 0;
	int maxImageNumber = 0;
	int imageNumber = 0;
	std::multimap<int, Slice::Pointer> nrmultimap;
	std::string sliceId;
	Slice::Pointer slice;
	
	for(unsigned int i = 0; i<timepoint->SliceIds()->size(); i++)
	{
		sliceId = timepoint->SliceIds()->at( i );
		slice = timepoint->Slice( sliceId );
		
		imageNumber = atoi( slice->GetTagAsText(tags::ImageNumber).c_str() );

		minImageNumber = (minImageNumber < imageNumber) ? minImageNumber : imageNumber;
		maxImageNumber = (maxImageNumber > imageNumber) ? maxImageNumber : imageNumber;

		nrmultimap.insert(std::pair<int, Slice::Pointer>(imageNumber, slice));
	}

	// Find out if image numbers are coherent (consecutive)
	if ( minImageNumber == maxImageNumber || maxImageNumber == 0)
	{
		LOG4CPLUS_WARN(blLogger::getInstance("dcmapi.IOUtils"), "Incoherent min, max: " << minImageNumber << ", " << maxImageNumber);
		return false;
	}

	// create new order
	timepoint->RemoveAllSlices();
	for (std::multimap<int, Slice::Pointer>::iterator it = nrmultimap.begin(); it != nrmultimap.end();
		++it)
	{
		timepoint->AddSlice((*it).second );
	}

	return true;
}


std::string IOUtils::CleanGdcmEntryStringValue(const std::string& value)
{
	std::string ret = value;
	gdcm::Util::CreateCleanString(ret);

	//if the last character is a blank space or is NULL(0) remove it
	int length = ret.length();
	if(length > 0)
	{
		if(ret.at(length-1) == ' ' || ret.at(length-1) == 0)
			ret.resize(length - 1);
	}

	return ret;
}

void IOUtils::UpdateTagsMap( dcmAPI::Patient::Pointer patient )
{
	// get first study for this patient
	StudyIdVectorPtr studyVector = patient->StudyIds();
	if(studyVector->size() <= 0)
		return;

	std::string studyId = studyVector->at(0);
	Study::Pointer study= patient->Study(studyId);
	if(study.get() == NULL)
		return;

	// get first serie for this study
	SeriesIdVectorPtr seriesVector = study->SeriesIds();
	if(seriesVector->size() <= 0)
		return;

	std::string seriesId = seriesVector->at(0);
	Series::Pointer series = study->Series(seriesId);
	if(series.get() == NULL)
		return;

	// get first timepoint for this serie
	TimePointIdVectorPtr timepointVector = series->TimePointIds();
	if(timepointVector->size() <= 0)
		return;

	std::string timepointId = timepointVector->at(0);
	TimePoint::Pointer timepoint = series->TimePoint(timepointId);
	if(timepoint.get() == NULL)
		return;

	// get first slice for this serie
	SliceIdVectorPtr sliceVector = timepoint->SliceIds();
	if(sliceVector->size() <= 0)
		return;

	std::string sliceId = sliceVector->at(0);
	Slice::Pointer slice = timepoint->Slice(timepointId);
	if(slice.get() == NULL)
		return;

	//read all patient tags from file
	std::string filepath = slice->GetTagAsText(tags::SliceFilePath);
	IOUtils::ReadPatientTagsFromFile(filepath, patient);
}

void IOUtils::ReadPatientTagsFromFile(
	std::string& path, 
	dcmAPI::Patient::Pointer patient)
{
	if(patient->GetTagsMap() == NULL)
		return;

	File::Pointer file = File::New();
	if(file.get() == NULL)
		return;

	if(file->Open(path) == false)
		return;

	TagsMap::iterator iter;
	for( iter = patient->GetTagsMap()->begin(); iter != patient->GetTagsMap()->end(); ++iter )
	{
		std::string value = file->ReadTagValue(iter->first);
		iter->second = value;
	}
}

bool IOUtils::TimepointContainsNonParallelSlices(dcmAPI::TimePoint::Pointer timepoint, double delta)
{
	bool ret = false;
	SliceIdVectorPtr sliceIdVector = timepoint->SliceIds();
	Slice::Pointer firstSlice = timepoint->Slice(sliceIdVector->at(0));
	std::string firstOrientation = firstSlice->GetTagAsText(tags::ImageOrientationPatient);
	double ori[6] = {0, 0, 0, 0, 0, 0}; 
	double refOr[6] = {0, 0, 0, 0, 0, 0};
	double diff= 0;
	for (unsigned id = 0; id < sliceIdVector->size(); id++)
	{
		Slice::Pointer currentSlice = timepoint->Slice(sliceIdVector->at(id));
		std::string position = currentSlice->GetTagAsText(
			tags::ImagePositionPatient);
		std::string orientation = currentSlice->GetTagAsText(
			tags::ImageOrientationPatient);

		ImageUtilities::StringToDouble(orientation.c_str(),ori, 7);
		ImageUtilities::StringToDouble(firstOrientation.c_str(),refOr, 7);
		for (size_t it= 0; it <6; it++)
			diff += fabs( refOr[it] -ori[it]) ;

		if (diff > 0.001 )
			return true;
	}

	return ret;
}

bool CompareTimePoints( 
	TimePoint::Pointer tempTimePoint1,
	TimePoint::Pointer tempTimePoint2 )
{
	return ( tempTimePoint1->GetTagAsNumber(tags::TimeValue) < 
		     tempTimePoint2->GetTagAsNumber(tags::TimeValue) );
}

void IOUtils::SortSeriesByTime(Series::Pointer series)
{
	std::vector<TimePoint::Pointer>* timePointsVector = series->GetAllTimePoints();
	std::sort( timePointsVector->begin(), timePointsVector->end(), CompareTimePoints );

	// Update time point IDs
	for ( unsigned int timePointId = 0; timePointId < timePointsVector->size(); timePointId++ )
	{
		std::ostringstream idStr;
		idStr<<(timePointId+1);
		timePointsVector->at( timePointId )->AddTag(tags::TimePointId, idStr.str());
	}
}
