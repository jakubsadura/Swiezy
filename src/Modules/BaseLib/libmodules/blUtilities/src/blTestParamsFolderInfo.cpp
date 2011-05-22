/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//--------------------------
// HEADERS
//--------------------------


#include <iostream>
#include <fstream>
#include "blTestParamsFolderInfo.h"



/**
*/
void blTestParamsFolderInfo::Initialize(
										std::string	strInputDataFolder,
										std::string	strWorkingDataFolder,
										std::string	strCorrectDataFolder,
										std::string	strTestModuleName )
{
	m_strInputDataFolder = strInputDataFolder;
	m_strWorkingDataFolder = strWorkingDataFolder;
	m_strCorrectDataFolder = strCorrectDataFolder;
	m_strTestModuleName = strTestModuleName;
}


