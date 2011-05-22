/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmStudy_h
#define _dcmStudy_h

#include "dcmAPIDataWin32Header.h"

//Common libraries
#include <string>
#include <vector>

//DcmAPI
#include "dcmTypes.h"
#include "dcmSeries.h"

namespace dcmAPI
{

/** 
* \brief Class that holds the structure of a DICOM Study.
* 
* Reminder: the DICOM data structure can be modeled as a hierarchy tree which levels are:
* DataSet > Patient > Study > Series > TimePoint > Slice
* 
* \ingroup data
* \author Pedro Omedas
* \date 15 May 2008
*/

class DCMAPIDATA_EXPORT Study : public Base
{
public:
	//! 
	cistibBoostPointerMacro(Study);
	//! \note Add it only if it not inside
	void AddSeries(Series::Pointer series);
	//!
	void RemoveAllSeries();
	//!
	SeriesIdVectorPtr SeriesIds() const;
	//!
	dcmAPI::Series::Pointer Series(const SeriesId& seriesId) const;
	//!
	void GetAllTags(TagsMap& tagsMap);
	//!Contructor
	Study();
	//!Destructor
	~Study();
	//!
	bool FindSeries( const SeriesId& seriesId ) const;

private:
	std::vector<Series::Pointer>* m_series;
};

} // namespace dcmAPI

#endif // _dcmStudy_h
