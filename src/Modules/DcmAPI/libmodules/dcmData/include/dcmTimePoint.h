/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmTIMEPOINT_H
#define _dcmTIMEPOINT_H

#include "dcmAPIDataWin32Header.h"

//Common libraries
#include <string>
#include <vector>

//DcmAPI
#include "dcmBase.h"
#include "dcmTypes.h"
#include "dcmSlice.h"

namespace dcmAPI
{

/** 
* \brief Class that holds the structure of a DICOM TimePoint.
* 
* Reminder: the DICOM data structure can be modeled as a hierarchy tree which levels are:
* DataSet > Patient > Study > Series > TimePoint > Slice
* 
* \ingroup data
* \author Pedro Omedas
* \date 15 May 2008
*/
class DCMAPIDATA_EXPORT TimePoint : public Base
{
public:
	//! 
	cistibBoostPointerMacro(TimePoint);
	//!
	void AddSlice(Slice::Pointer _image);
	//!
	void RemoveAllSlices();
	//!
	SliceIdVectorPtr SliceIds() const;
	//!
	dcmAPI::Slice::Pointer Slice(const SliceId& _sliceId) const;
	//!
	void GetAllTags(TagsMap& tagsMap);
	//!Contructor
	TimePoint();
	//!Destructor
	~TimePoint();
	
private:
	std::vector<Slice::Pointer>* m_slices;
	//!
	
};

} // namespace dcmAPI

#endif // _dcmTIMEPOINT_H
