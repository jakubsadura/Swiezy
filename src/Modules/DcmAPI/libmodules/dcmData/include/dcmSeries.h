/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmSERIES_H
#define _dcmSERIES_H

#include "dcmAPIDataWin32Header.h"

//Common libraries
#include <string>
#include <vector>

//DcmAPI
#include "dcmTimePoint.h"
#include "dcmTypes.h"
#include "dcmBase.h"

namespace dcmAPI
{

/** 
* \brief Class that holds the structure of a DICOM Series.
* 
* Reminder: the DICOM data structure can be modeled as a hierarchy tree which levels are:
* DataSet > Patient > Study > Series > TimePoint > Slice
* 
* \ingroup data
* \author Pedro Omedas
* \date 15 May 2008
*/

class DCMAPIDATA_EXPORT Series : public Base
{
public:
	//! 
	cistibBoostPointerMacro(Series);
	//!
	void AddTimePoint(TimePoint::Pointer _timePoint);
	//!
	void RemoveAllTimepoints();
	//!
	TimePointIdVectorPtr TimePointIds() const;
	//!
	dcmAPI::TimePoint::Pointer TimePoint(const TimePointId& _timePointId) const;
	//!
	std::vector<TimePoint::Pointer>* GetAllTimePoints();
	//!
	void GetAllTags(TagsMap& tagsMap);
	//!
	Series();
	//!
	~Series();

private:
	//!
	std::vector<TimePoint::Pointer>* m_timePoint;
};

} // namespace dcmAPI

#endif //_dcmSERIES_H
