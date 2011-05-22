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
	#include <vector>
	#include <string>

	using namespace std;


	//--------------------------
	// CLASS DEFINITION
	//--------------------------


	/**
	 * \brief Basic parameters for testing
	 * \ingroup blUtilities
	 *
	 * This is a simple class that will provide standard names 
	 * for the correct output and current output data for testing
	 * using the input names and other information.
	 *
	 * - Input data is the data that will be used as the input for test. 
	 * - Current output data is the current output of the test. 
	 * - Correct output data is the output data that will be used to 
	 * check that the algorithm is working fine, comparing it to the 
	 * current output data 
	 *
	 * \section blTestParamsCreatingOutputNames Creating output names
	 *
	 * To create the output names, this information is used:
	 * - The name of the test module 
	 * - The number of the test in this test module,
	 * - Very short description of the test
	 * - Input file
	 * - The word "Out"
	 * - Extension passed as parameter to the constructor
	 *
	 * For example: 
	 * - Input file name: "Cube.vtk"
	 * - Current output filename: "meDemoMesh.T03.Refine.Cube.Out.vtk"
	 *
	 * \section blTestParamsFolderStructure Folder structure:
	 *
	 * This is an example of the folder structure used to store the
	 * input, correct output and current output for MeshLib:
	 * - Input: F:/ToolKit/Source/Data/Cardio/Meshes
	 *   - LV_Myo_opt.stl
	 * - Correct output: F:/ToolKit/Source/cilabModules/MeshLib/Data/CorrectOutput
	 *   - meDemoMesh.T00.Optimize.LV_Myo_opt.Out.vtk
	 *   - meDemoMesh.T01.Smooth.LV_Myo_opt.Out.vtk
	 * - Current output: F:/ToolKit/Build/Temporary
	 *   - meDemoMesh.T00.Optimize.LV_Myo_opt.Out.vtk
	 *   - meDemoMesh.T01.Smooth.LV_Myo_opt.Out.vtk
	 *
	 * \section blTestParamsUsage Usage:
	 *
	 * You must initialize blTestParamsFolderInfo singleton instance
	 * in the constructor of the test class:
	 * \code
	 *	blTestParamsFolderInfo::Instance( )->Initialize(
	 *				STR_PATH_INPUT_DATA,
	 *				STR_PATH_TEMPORARY,
	 *				STR_PATH_CORRECT_DATA_MESHLIB,
	 *				"meDemoMesh" );
	 * \endcode
	 *
	 * The definitions of the constants are:
	 * - STR_PATH_INPUT_DATA: Is defined in the current test and is based 
	 *   on TOOLKIT_DATA_ROOT that is defined in baselibConfigure.h:
	 *	 For example:
	 *   \code
	 *		const char* STR_PATH_INPUT_DATA = TOOLKIT_DATA_ROOT;
	 *	 \endcode
	 * - STR_PATH_TEMPORARY: Is defined in baselibConfigure.h
	 *	using CMake commands:
	 *   \code
	 *	#define STR_PATH_TEMPORARY "F:/ToolKit/Build/Temporary"
	 *   \endcode
	 * - STR_PATH_CORRECT_DATA_MESHLIB: Is defined in meshlibConfigure.h
	 *  using CMake commands:
	 *   \code
	 *  #define STR_PATH_CORRECT_DATA_MESHLIB 
	 *		"F:/ToolKit/Source/cilabModules/MeshLib/Data/CorrectOutput"
	 *   \endcode
	 *
	 * Constructor of this class: 
	 * \code
	 * const int MAX_INPUT_DATA = 9;
	 * InputData inputDataArray[ MAX_INPUT_DATA ] = {
	 *		InputData( 0, 1, "LV_Myo_opt.stl", ".vtk" )
	 * }
	 * \endcode
	 *
	 * If you want to apply specific parameters for each input data,
	 * you can create a subclass of this class:
	 * \code
	 *
	 * class InputData : public blTestParams
	 * {
	 *		public:
	 *		INITIAL_SHAPE_TYPE m_initialPos;
	 *
	 *		InputData( 
	 *			int iTestNum = -1,
	 *			bool bEnable = false,
	 *			char* strInputBaseName = NULL, 
	 *			char* strOutputExtension = NULL,
	 *			INITIAL_SHAPE_TYPE initialPos = INITIAL_SHAPE_TYPE_NOT_DEFINED ):
	 *		blTestParams( iTestNum, bEnable, strInputBaseName, strOutputExtension )
	 *		{
	 *			m_initialPos = initialPos;
	 *		}
	 *
	 * };
	 * \endcode
	 *
	 * 
	 */
	class blTestParams 
	{

	//--------------------------
	// OPERATIONS
	//--------------------------
	public:

		/**
		 * \brief Constructor using one input and one output extension
		 *
		 * For example: 
		 * \code
		 *	blTestParams( 0, 1, 
		 *			"/Cardio/CT/Input/pa73_phs-shape-1.vtk",
		 *			".vtk" );
		 * \endcode
		 */
		blTestParams(	
			int iNumTest = -1,
			bool bEnable = false,
			const char* strInputBaseName = NULL, 
			const char* strOutputExtension = NULL);

		/**
		 * \brief Constructor using two input names and two output extensions
		 *
		 * For example: 
		 * \code
		 *	blTestParams( 0, 1, "w-057-1.png", "w-057-1.vtk", ".png", ".vtk" );
		 * \endcode
		 */
		blTestParams(	
			int iNumTest,
			bool bEnable,
			const char* strInputBaseName, 
			const char* strInputBaseName1, 
			const char* strOutputExtension,
			const char* strOutputExtension1 = NULL);

		/**
		 * \brief Get correct output filename
		 *
		 * \param [in] iIndex strInputBaseName that will be used:
		 *		strInputBaseName or strInputBaseName1
		 * \param [in] pStrTestModuleStep Put a short description of
		 *	this output
		 * \param [in] iNumber You can add a number at the end of the 
		 *  input base name. For example: 
		 *  - InputBaseName: "/Cardio/CT/Input/pa73_phs-shape-1.vtk";
		 *  - CorrectFileName: "smlDemoCardiacAnalysis.T00..pa73_phs_r_01.Out.vtk"
		 */
		std::string GetCorrectFileName(
			int iIndex = 0,
			const char *pStrTestModuleStep = "",
			int iNumber = -1 );

		/**
		 * \brief Get current output filename
		 */
		std::string GetWorkingFileName( 
			int iIndex = 0,
			const char *pStrTestModuleStep = "",
			int iNumber = -1 );

		/**
		 * \brief Get input file name
		 */
		std::string GetInputFileName( int iIndex = 0, int iNumber = -1 );

		bool GetEnable( );


	//--------------------------
	// PRIVATE OPERATIONS
	//--------------------------
	private:

		/// Add the info to the internal member variables
		void AddData( 
						const char* strInputDataFileName, 
						const char* strOutputDataExtension );

		/// Create input base name formating the string with the iNumber
		std::string CreateInputBaseName( 
						int iIndex, int iNumber );


	//--------------------------
	// ATTRIBUTES
	//--------------------------
	private:

		/// Enable the test for this input data
		bool		m_bEnable;

		/**
		 * \brief Base name of the input data filename
		 *
		 * For example: "/Cardio/US/Input/InputImage_3D_US_.vtk"
		 * The base name will be "InputImage_3D_US_"
		 */
		std::vector<std::string>	m_strVectorInputBaseName;

		/**
		 * \brief Extension of the input data filename
		 *
		 * For example: "/Cardio/US/Input/InputImage_3D_US_.vtk"
		 * The extension will be ".vtk"
		 */
		std::vector<std::string>	m_strVectorInputBaseExtension;

		/**
		 * \brief Local path of the input data filename
		 *
		 * For example: "/Cardio/US/Input/InputImage_3D_US_.vtk"
		 * The local path will be "/Cardio/US/Input/"
		 */
		std::vector<std::string>	m_strVectorInputLocalPath;

		/**
		 * \brief Output data extension 
		 * \note (the same file name of m_strInputDataFileName will be used
		 */
		std::vector<std::string>	m_strVectorOutputExtension;


		std::string	m_strTestModuleNumber;


	};



