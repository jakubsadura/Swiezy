/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _COREBASETEST_H_
#define _COREBASETEST_H_

#include "gmDataHandlingWin32Header.h"

#include "coreBaseTestData.h"

#include <string>
#include <vector>


namespace Core
{

/**
\brief Base test class

\ingroup gmDataHandling
\author Xavi Planes
\date 18 09 2009
*/

typedef struct
{
	const char* m_FileName;
	int m_iSize;
} TestInputSetEntry;

typedef struct
{
	const char* m_FileName;
	Core::DataEntityType m_DataEntityType;
} TestReferenceOutputSetEntry;

//! Macros for defining test input(s)
#define BEGIN_TEST_INPUTS_SETS_ARRAY( input_name, n_inputs ) \
	const Core::TestInputSetEntry g_##input_name[][n_inputs+1] = {
#define BEGIN_TEST_INPUTS_SET() {
#define TEST_INPUTS_SET_ENTRY( name ) { name, 1 },
#define TEST_INPUTS_SET_ENTRY_2( name, size ) { name, size },
#define END_TEST_INPUTS_SET() { NULL, 0, } },
#define END_TEST_INPUTS_SETS_ARRAY() };

//! Macros for defining test reference output(s)
#define BEGIN_TEST_OUTPUTS_SETS_ARRAY( output_name, n_outputs ) \
	const Core::TestReferenceOutputSetEntry g_##output_name[][n_outputs+1] = {
#define BEGIN_TEST_OUTPUTS_SET() {
#define TEST_OUTPUTS_SET_ENTRY( name, output_type ) { name, output_type },
#define END_TEST_OUTPUTS_SET() { NULL, Core::UnknownTypeId } },
#define END_TEST_OUTPUTS_SETS_ARRAY() };


class GMDATAHANDLING_EXPORT BaseTest
{
public:

	std::string PATH_TEST;
	std::string PATH_TEST_DATA;
	std::string PATH_TEST_OUTPUT;
	//!
	BaseTest( const std::string &outputFolder );
	//!
	~BaseTest( );

	//! Add input set  of files
	void AddInputFilenamesSet( const Core::TestInputSetEntry* pTestInputSetEntry,
		std::string& InputSetEntryPath );

	//! Add reference output set of files
	void AddReferenceOutputFilenamesSet(
		const Core::TestReferenceOutputSetEntry* pTestReferenceOutputInputSetEntry,
		std::string& ReferenceOutputSetEntryPath );

	//!
	void AddInputFilenames( int i, std::string filename, int size = 1 );

	//!
	void SetInputDataEntities( int i, Core::DataEntity::Pointer inputDataEntity);

	//!
	std::vector<std::string> GetInputFilenames( int i );

	//!
	std::vector<Core::DataEntity::Pointer> GetInputDataEntities();

	//!
	void AddReferenceOutputFilenames( 
		int i, 
		std::string filename, 
		Core::DataEntityType type, 
		int size = 1 );

	//!
	void AddOutputFilename( int i, std::string filename );

	//!
	std::vector<std::string> GetReferenceOutputFilenames( int i );

	//! Compare two vtk shapes
	bool CheckOutput( );

	//! Output path for this test data
	void SetOutputPath( const std::string &outputPath );

	//!
	std::vector<std::string> GetOutputFilenames( int i );

	//!
	void Clean();

	//!
	void SetLogFileName (const std::string logFileName);

	//!
	void SetTestName (std::string testName);

	//!
	void StoreTestResult (std::string testName, int n_test, std::string testID, int n_input, bool result );
	


protected:
	//!
	void AddInputFilename( int i, std::string filename );


	//!
	void AddReferenceOutputFilename( 
		int i, 
		std::string filename, 
		Core::DataEntityType type );

private:

	//! Input data 
	std::vector<std::vector<std::string> > m_Inputfilenames;

	//! Reference output data filenames
	std::vector< BaseTestData > m_ReferenceOutputfilenames;

	/** Output data of this test. Created automatically combining
	m_OutputPath & m_ReferenceOutputfilenames
	*/
	std::vector<std::vector<std::string> > m_Outputfilenames;

	/** Input data entities for this test*/
	std::vector<Core::DataEntity::Pointer > m_InputDataEntities;

	//! Output path for this test 
	std::string m_OutputPath;

	//!Log File
	std::string m_LogFileName;


};

} // namespace Core

#endif	// #ifndef _COREBASETEST_H_
