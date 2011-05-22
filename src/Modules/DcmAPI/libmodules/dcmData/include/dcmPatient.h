/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmPATIENT_H
#define _dcmPATIENT_H

#include "dcmAPIDataWin32Header.h"

//Common libraries
#include <string>
#include <vector>

//DcmAPI
#include "dcmTypes.h"
#include "dcmBase.h"
#include "dcmStudy.h"

namespace dcmAPI
{

/** 
* \brief Class that holds the structure of a DICOM Patient.
* 
* Reminder: the DICOM data structure can be modeled as a hierarchy tree which levels are:
* DataSet > Patient > Study > Series > TimePoint > Slice
* 
* \ingroup data
* \author Pedro Omedas
* \date 15 May 2008
*/
class DCMAPIDATA_EXPORT Patient : public Base
{
public:
	cistibBoostPointerMacro(Patient);
	//! \note Add it only if it not inside
	void AddStudy(Study::Pointer _study);
	//!
	void RemoveAllStudies();
	//!
	StudyIdVectorPtr StudyIds() const;
	//! Return a study 
	dcmAPI::Study::Pointer Study(const StudyId& _studyId) const;
	//!
	void GetAllTags(TagsMap& tagsMap);
	//!
	Patient();
	//!
	virtual ~Patient();

	//!
	bool FindStudy(const StudyId& _studyId) const;

private:
	//!
	void InitializeTagsMap();
	//!
	std::vector<Study::Pointer>* m_studies;
};

} // namespace dcmAPI

#endif //end _dcmPATIENT_H
