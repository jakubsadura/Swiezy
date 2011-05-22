/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmStudy.h"

using namespace dcmAPI;

Study::Study()
{
	m_series = new std::vector<Series::Pointer>();
}

Study::~Study()
{
	//TODO: iterate over the vector and delete all tags
	delete m_series;
}

void Study::AddSeries(Series::Pointer _series)
{
	if ( !FindSeries( _series->GetTagAsText(tags::SeriesId) ) )
	{
		m_series->push_back(_series);
	}
}

void Study::RemoveAllSeries()
{
	m_series->clear();
}

SeriesIdVectorPtr Study::SeriesIds() const
{
	SeriesIdVectorPtr seriesIdVector(new SeriesIdVector());
	for(unsigned int i=0; i < m_series->size(); i++){
		Series::Pointer series = m_series->at(i);
		seriesIdVector->push_back(series->GetTagAsText(tags::SeriesId));
	}
	return seriesIdVector;
}

Series::Pointer Study::Series(const SeriesId& _seriesId) const
{
	Series::Pointer seriesPtr;

	for(unsigned int i=0; i < m_series->size(); i++)
	{
		Series::Pointer series = m_series->at(i);
		if (series->GetTagAsText(tags::SeriesId) == _seriesId)
		{
			seriesPtr = series;
			break;
		}
	}

	return seriesPtr;
}

void Study::GetAllTags(TagsMap& tagsMap)
{
}


bool dcmAPI::Study::FindSeries( const SeriesId& seriesId ) const
{
	bool found = false;

	for(unsigned int i=0; i < m_series->size(); i++)
	{
		Series::Pointer series = m_series->at(i);
		if (series->GetTagAsText(tags::SeriesId) == seriesId)
		{
			found = true;
		}
	}

	return found;
}
