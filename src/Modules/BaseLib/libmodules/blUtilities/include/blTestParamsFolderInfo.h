/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#pragma once

	//--------------------------
	// HEADERS
	//--------------------------


	#include <iostream>
	#include <fstream>
	#include "blSingleton.h"



	//--------------------------
	// CLASS DEFINITION
	//--------------------------

	/**
	 * \brief Info folder for blTestParams
	 * \ingroup blUtilities
	 */
	class blTestParamsFolderInfo : public blSingleton<blTestParamsFolderInfo>
	{

	//--------------------------
	// OPERATIONS
	//--------------------------
	public:

		void Initialize(
			std::string	strInputDataFolder,
			std::string	strWorkingDataFolder,
			std::string	strCorrectDataFolder,
			std::string	strTestModuleName );



	//--------------------------
	// ATTRIBUTES
	//--------------------------
	public:

		/** 
		 * \brief Path for input data
		 * \note In the constructor, blTestParamsFolderInfo will be 
		 * used to initialize it
		 */
		std::string	m_strInputDataFolder;

		/** 
		 * \brief Path for output data
		 * \note In the constructor, blTestParamsFolderInfo will be 
		 * used to initialize it
		 */
		std::string	m_strWorkingDataFolder;

		/** 
		 * \brief Path for correct data
		 * \note In the constructor, blTestParamsFolderInfo will be 
		 * used to initialize it
		 */
		std::string	m_strCorrectDataFolder;

		std::string	m_strTestModuleName;

	};




