/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmSearch_H
#define _dcmSearch_H

#include "dcmAPIDataWin32Header.h"

//Common libraries
#include <string>
#include <vector>
#include <map>

//DcmAPI
#include "dcmDataSet.h"

namespace dcmAPI
{

/** 
\brief Search DICOM data or tags in a dcmDataSet
\ingroup dcmapi
\author Xavi Planes
\date 31 March 2010
*/

class DCMAPIDATA_EXPORT Search
{
public:
	//! Check that ImageOrientationPatient tag is present in all slices
	static bool CheckOrientationTag( DataSet::Pointer dataSet );

private:
};

} // namespace dcmAPI

#endif // End _dcmSearch_H
