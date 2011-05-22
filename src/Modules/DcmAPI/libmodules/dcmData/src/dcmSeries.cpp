/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmSeries.h"

using namespace dcmAPI;

Series::Series()
{
	m_timePoint = new std::vector<TimePoint::Pointer>();
}

Series::~Series()
{
	//TODO: delete all elements in the vectors
	delete m_timePoint;
}

void Series::AddTimePoint(TimePoint::Pointer _timePoint)
{
	m_timePoint->push_back(_timePoint);
}

void Series::RemoveAllTimepoints()
{
	m_timePoint->clear();
}

TimePointIdVectorPtr Series::TimePointIds() const
{
	TimePointIdVectorPtr myTimePointIdVector(new TimePointIdVector());

	for(unsigned int i=0; i < m_timePoint->size(); i++)
	{
		TimePoint::Pointer timePoint = m_timePoint->at(i);
		myTimePointIdVector->push_back(timePoint->GetTagAsText(tags::TimePointId));
	}

	return myTimePointIdVector;
}

TimePoint::Pointer Series::TimePoint(const TimePointId& _timePointId) const
{
	TimePoint::Pointer timePointPtr;
	for(unsigned int i=0; i < m_timePoint->size(); i++)
	{
		TimePoint::Pointer timePoint = m_timePoint->at(i);
		if (timePoint->GetTagAsText(tags::TimePointId) == _timePointId)
		{
			timePointPtr = timePoint;
			break;
		}
	}
	return timePointPtr;
}

void Series::GetAllTags(TagsMap& tagsMap)
{
}

std::vector<TimePoint::Pointer>* dcmAPI::Series::GetAllTimePoints()
{
	return m_timePoint;
}
