/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmIOUtils_H
#define _dcmIOUtils_H

// DCMAPIIO_EXPORT
#include "dcmAPIIOWin32Header.h"

#include "CILabExceptionMacros.h"

//DcmAPI
#include "dcmDataSet.h"
#include "dcmTypes.h"
#include "dcmFile.h"

namespace dcmAPI{

/** 
\brief IO Utils

\ingroup io
\author Xavi Planes
\date 24 July 2009
*/
class DCMAPIIO_EXPORT IOUtils 
{
public:

	//!
	static std::string GetSliceFilePathFromReferenceFileID(
		std::string referenceFileID,
		std::string path,
		bool dicomdir = true ); 

	//!
	static std::string GetSliceFileNameFromReferenceFileID(
		std::string referenceFileID); 

	/** create a map containing information which tag to use fot the time 
	information depending on modality
	*/
	static void InitializeTimeTagForModalityMap();
	//!
	static TagId GetTimeTagForSpecifiedModality(const std::string& modality);
	//!
	static TimePoint::Pointer FindTimepoint(
		std::string& timeValue, 
		Series::Pointer series);
	//!
	static void SortTimepoint(TimePoint::Pointer timepoint);
	//! 
	static bool SortTimepointByImagePosition(TimePoint::Pointer timepoint);
	//!
	static bool SortTimepointByImageNumber(TimePoint::Pointer timepoint);

	//!
	static std::string CleanGdcmEntryStringValue(const std::string& value);

	//!
	static void ReadPatientTagsFromFile(
		std::string& path, 
		dcmAPI::Patient::Pointer patient );

	//!
	static void UpdateTagsMap( dcmAPI::Patient::Pointer patient );

	//!
	static bool TimepointContainsNonParallelSlices(
		dcmAPI::TimePoint::Pointer tiempoint, double delta = 0.001);

	//!
	static void SortSeriesByTime(Series::Pointer series);

private:
	//!
	static TimeTagForModalityMap m_timeTagForModalityMap;

};

} // namespace dcmAPI{

#endif // _dcmIOUtils_H
