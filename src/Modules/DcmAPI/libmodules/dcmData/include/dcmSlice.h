/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmSLICE_H
#define _dcmSLICE_H

#include "dcmAPIDataWin32Header.h"

//DcmAPI
#include "dcmTypes.h"
#include "dcmBase.h"

namespace dcmAPI
{

/** 
* \brief Class that holds the structure of a DICOM Slice.
* 
* Reminder: the DICOM data structure can be modeled as a hierarchy tree which levels are:
* DataSet > Patient > Study > Series > TimePoint > Slice
* 
* \ingroup data
* \author Pedro Omedas
* \date 15 May 2008
*/

class DCMAPIDATA_EXPORT Slice : public Base
{
public:
	//! 
	cistibBoostPointerMacro(Slice);
	//!
	void GetAllTags(TagsMap& tagsMap);
	//!
	Slice();
	//!
	~Slice();
	//!
};

} // namespace dcmAPI

#endif //end _dcmSLICE_H
