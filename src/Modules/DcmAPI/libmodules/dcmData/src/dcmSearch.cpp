/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmSearch.h"


bool dcmAPI::Search::CheckOrientationTag( DataSet::Pointer dataSet )
{

	bool orientationTag = true;
	dcmAPI::PatientIdVectorPtr patiendIdVector = dataSet->GetPatientIds();
	for(unsigned i=0; i < patiendIdVector->size(); i++)
	{
		dcmAPI::Patient::Pointer patient = dataSet->GetPatient( patiendIdVector->at(i) );
		dcmAPI::StudyIdVectorPtr studiesIdVector = patient->StudyIds();
		for(unsigned i=0; i < studiesIdVector->size(); i++)
		{
			dcmAPI::Study::Pointer study = patient->Study(studiesIdVector->at(i));
			dcmAPI::SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
			for(unsigned i=0; i < seriesIdVector->size(); i++)
			{
				dcmAPI::Series::Pointer series = study->Series(seriesIdVector->at(i));
				dcmAPI::TimePointIdVectorPtr timePointIdVector = series->TimePointIds();
				for(unsigned i=0; i < timePointIdVector->size(); i++)
				{
					dcmAPI::TimePoint::Pointer timePoint = series->TimePoint(timePointIdVector->at(i));
					dcmAPI::SliceIdVectorPtr sliceIdVector = timePoint->SliceIds();
					for(unsigned i=0; i < sliceIdVector->size(); i++)
					{
						dcmAPI::Slice::Pointer slice = timePoint->Slice( sliceIdVector->at(i) );
						
						dcmAPI::TagsMap::iterator it;
						it = slice->GetTagsMap( )->find( dcmAPI::tags::ImageOrientationPatient );
						if ( it == slice->GetTagsMap( )->end() )
						{
							orientationTag = false;
						}
					}
				}
			}
		}
	}

	return orientationTag;
}
