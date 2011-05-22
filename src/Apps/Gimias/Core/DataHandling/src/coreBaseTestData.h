/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#pragma once

#include "gmDataHandlingWin32Header.h"

#include <string>
#include <vector>

#include "coreDataEntity.h"

namespace Core
{
/**
Data for testing with the 3D+T filenames and the type of data
\ingroup gmDataHandling
\author Xavi Planes
\date 18 09 2009
*/
class BaseTestData
{
public:
	//! 3D + T filenames complete path
	std::vector<std::string> m_FileNames;

	//! Type of data entity to perform the Checkoutput( )
	Core::DataEntityType m_DataEntityType;
};

} // namespace Core
