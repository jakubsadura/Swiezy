/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmTimePoint.h"

using namespace dcmAPI;

TimePoint::TimePoint()
{
	m_slices = new std::vector<Slice::Pointer>();
}

TimePoint::~TimePoint()
{
	//TODO Delete all objects in the vector. 
	delete m_slices;
}

void TimePoint::AddSlice(Slice::Pointer _image)
{
	m_slices->push_back(_image);
}

void TimePoint::RemoveAllSlices()
{
	m_slices->clear();
}

SliceIdVectorPtr TimePoint::SliceIds() const
{
	SliceIdVectorPtr mySliceIdVector(new SliceIdVector);
	for(unsigned int i=0; i < m_slices->size(); i++)
	{
		Slice::Pointer slice = m_slices->at(i);
		mySliceIdVector->push_back(slice->GetTagAsText(tags::SliceId));
	}
	return mySliceIdVector;
}

Slice::Pointer TimePoint::Slice(const SliceId& _sliceId) const
{
	Slice::Pointer slicePtr;

	for(unsigned int i=0; i < m_slices->size(); i++)
	{
		Slice::Pointer slice = m_slices->at(i);
		if (slice->GetTagAsText(tags::SliceId) == _sliceId)
		{
			slicePtr = slice;
			break;
		}
	}

	return slicePtr;
}

void TimePoint::GetAllTags(TagsMap& tagsMap)
{

}
