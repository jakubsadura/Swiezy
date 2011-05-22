/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseTest.h"

#include "blShapeUtils.h"
#include "blSignalCompare.h"
#include "blImageUtils.h"

#include "boost/format.hpp"
#include "itksys/SystemTools.hxx"

#include <sstream>


Core::BaseTest::BaseTest( const std::string &outputFolder )
{
	PATH_TEST = outputFolder + "/bin/Release/Data/Test/";
	PATH_TEST_DATA = PATH_TEST + "TestData/";
	PATH_TEST_OUTPUT = PATH_TEST + "TestOutput/";
}

Core::BaseTest::~BaseTest( )
{
}

bool Core::BaseTest::CheckOutput()
{
	// Check output
	bool res = true;
	float eps = 0.;
	for ( int iOutput = 0 ; iOutput < m_ReferenceOutputfilenames.size() ; iOutput++ )
	{
		for ( int iTimeStep = 0 ; 
			iTimeStep < m_ReferenceOutputfilenames[ iOutput ].m_FileNames.size() ; 
			iTimeStep++ )
		{
			std::string filename1 = m_Outputfilenames[ iOutput ][ iTimeStep ];
			std::string filename2 = m_ReferenceOutputfilenames[ iOutput ].m_FileNames[ iTimeStep ];
			switch( m_ReferenceOutputfilenames[ iOutput ].m_DataEntityType )
			{
				case Core::SurfaceMeshTypeId:
					res &= blShapeUtils::ShapeUtils::CompareShapes(
						filename1.c_str( ),
						filename2.c_str( ) );
					break;
				case Core::SkeletonTypeId:
				{
					// Compare skeletons (they are meshes)
					res &= blShapeUtils::ShapeUtils::CompareShapes(
						filename1.c_str( ),
						filename2.c_str( ) );
					break;
				}
				case Core::SignalTypeId:
					{
						blSignalCompare::Pointer signalCompare = blSignalCompare::New( );
						signalCompare->SetFilename1( filename1.c_str( ) );
						signalCompare->SetFilename2( filename2.c_str( ) );
						signalCompare->Update();
						res &= signalCompare->GetOutput();
						break;
					}
				case Core::ImageTypeId:
					{
						res &= blImageUtils::CompareImages( 
							filename1.c_str( ),
							filename2.c_str( ),
							eps);
						break;
					}
			}
		}
	}
	return res;
}

void Core::BaseTest::AddInputFilename( int i, std::string filename )
{
	while ( i >= m_Inputfilenames.size() )
	{
		m_Inputfilenames.push_back( std::vector<std::string>() );
	}
	m_Inputfilenames[ i ].push_back( filename );
}

void Core::BaseTest::AddReferenceOutputFilename( 
	int i, 
	std::string filename,
	Core::DataEntityType type )
{
	while ( i >= m_ReferenceOutputfilenames.size() )
	{
		m_ReferenceOutputfilenames.push_back( BaseTestData( ) );
	}
	m_ReferenceOutputfilenames[ i ].m_FileNames.push_back( filename );
	m_ReferenceOutputfilenames[ i ].m_DataEntityType = type;
}

void Core::BaseTest::AddInputFilenamesSet( const Core::TestInputSetEntry* pTestInputSetEntry, std::string& InputSetEntryPath )
{
	int iInputFile = 0;
	while( 1 )
	{
		const Core::TestInputSetEntry* pCurrentTestInput = &pTestInputSetEntry[iInputFile];
		if( pCurrentTestInput->m_FileName != NULL )
		{
			std::string inputFullPath = InputSetEntryPath + 
				pCurrentTestInput->m_FileName;
			AddInputFilenames( iInputFile, inputFullPath,
				pCurrentTestInput->m_iSize );

			iInputFile++;
		}
		else
		{
			break;
		}
	}
}

void Core::BaseTest::AddReferenceOutputFilenamesSet( const Core::TestReferenceOutputSetEntry* pTestReferenceOutputInputSetEntry, std::string& ReferenceOutputSetEntryPath )
{
	int iOutputFileFile = 0;
	while( 1 )
	{
		const Core::TestReferenceOutputSetEntry* pCurrentTestReferenceOutput = &pTestReferenceOutputInputSetEntry[iOutputFileFile];
		if( pCurrentTestReferenceOutput->m_FileName != NULL )
		{
			std::string outputFullPath = ReferenceOutputSetEntryPath + 
				pCurrentTestReferenceOutput->m_FileName;
			AddReferenceOutputFilenames( iOutputFileFile,
				outputFullPath, pCurrentTestReferenceOutput->m_DataEntityType );

			iOutputFileFile++;
		}
		else
		{
			break;
		}
	}
}

void Core::BaseTest::AddOutputFilename( int i, std::string filename )
{
	while ( i >= m_Outputfilenames.size() )
	{
		m_Outputfilenames.push_back( std::vector<std::string>() );
	}
	m_Outputfilenames[ i ].push_back ( filename );
}

void Core::BaseTest::SetOutputPath( const std::string &outputPath )
{
	m_OutputPath = outputPath;
}

void Core::BaseTest::AddInputFilenames( 
	int iInput, 
	std::string inputFilename, 
	int size )
{
	std::string path = itksys::SystemTools::GetFilenamePath(inputFilename);
	std::string ext = itksys::SystemTools::GetFilenameLastExtension(inputFilename);
	std::string basename = itksys::SystemTools::GetFilenameWithoutLastExtension( inputFilename );

	std::string filename;
	if ( size > 1 )
	{
		boost::format format( "%02d" );
		for ( int i = 0 ; i < size ; i++ )
		{
			filename = path + "/" + basename + (format % i).str() + ext;
			AddInputFilename( iInput, filename );
		}
	}
	else
	{
		filename = path + "/" + basename + ext;
		AddInputFilename( iInput, filename );
	}
}

void Core::BaseTest::AddReferenceOutputFilenames( 
	int iOutput, 
	std::string outputFilename, 
	Core::DataEntityType type,
	int size )
{
	std::string path = itksys::SystemTools::GetFilenamePath( outputFilename );
	std::string ext = itksys::SystemTools::GetFilenameLastExtension( outputFilename );
	std::string basename = itksys::SystemTools::GetFilenameWithoutLastExtension( outputFilename );

	std::string filename;
	if ( size > 1 )
	{
		boost::format format( "%02d" );
		for ( int i = 0 ; i < size ; i++ )
		{
			filename = path + "/" + basename + (format % i).str() + ext;
			AddReferenceOutputFilename( iOutput, filename, type );
			filename = m_OutputPath + "/" + basename + (format % i).str() + ext;
			AddOutputFilename( iOutput, filename );
		}
	}
	else
	{
		filename = path + "/" + basename + ext;
		AddReferenceOutputFilename( iOutput, filename, type );
		filename = m_OutputPath + "/" + basename + ext;
		AddOutputFilename( iOutput, filename );
	}
}

void Core::BaseTest::Clean()
{
	m_Inputfilenames.clear();
	m_Outputfilenames.clear();
	std::vector< BaseTestData >::iterator it;
	for (it = m_ReferenceOutputfilenames.begin(); it != m_ReferenceOutputfilenames.end(); ++it)
	{
		(*it).m_FileNames.clear();
	}
	m_ReferenceOutputfilenames.clear();
	
	std::vector< Core::DataEntity::Pointer >::iterator it2;
	for (it2 = m_InputDataEntities.begin(); it2 != m_InputDataEntities.end(); ++it2)
		(*it2) = NULL;
	m_InputDataEntities.clear();
}

std::vector<std::string> Core::BaseTest::GetReferenceOutputFilenames( int i )
{
	return m_ReferenceOutputfilenames.at( i ).m_FileNames;
}

std::vector<std::string> Core::BaseTest::GetInputFilenames( int i )
{
	return m_Inputfilenames.at( i );
}
std::vector<Core::DataEntity::Pointer> Core::BaseTest::GetInputDataEntities( )
{
	return m_InputDataEntities;
}

std::vector<std::string> Core::BaseTest::GetOutputFilenames( int i )
{
	return m_Outputfilenames.at( i );
}
void Core::BaseTest::SetLogFileName (const std::string logFileName)
{
	m_LogFileName = logFileName;
	FILE* logFile =  fopen(m_LogFileName.c_str(), "a+");
	fprintf(logFile, "test_name; n_test; id_Test; n_input; result\n");
	fclose(logFile);
}

void Core::BaseTest::SetTestName (std::string testName)
{
	cout<<"log file name = "<<m_LogFileName<<endl;
	cout<<"test name = "<<testName<<endl;
	FILE* logFile =  fopen(m_LogFileName.c_str(), "a+");
	fprintf(logFile, "%s;", testName.c_str());
	fclose(logFile);

}
void Core::BaseTest::StoreTestResult (std::string testName, int n_test, std::string testID, int n_input, bool result )
{
	std::string result_string;
	if (result == true)
		result_string = "PASSED";
	else result_string = "FAILED";
	cout<<"result string = " << result_string<<endl;
	std::stringstream n_test_string;
	n_test_string << n_test;
	std::string test_string = "test" + n_test_string.str();
	cout<<	"test_string = "<< test_string<<endl;

	FILE* logFile =  fopen(m_LogFileName.c_str(), "a+");

	fprintf(logFile, "%s;%s;%s;%d;%s;", testName.c_str(), test_string.c_str(), testID.c_str(), n_input, result_string.c_str());
	fprintf(logFile, "\n");
	fclose(logFile);
}


void Core::BaseTest::SetInputDataEntities( int i, Core::DataEntity::Pointer inputDataEntity)
{
	while ( i >= m_InputDataEntities.size() )
	{
		m_InputDataEntities.push_back( NULL );
	}
	m_InputDataEntities[i] = inputDataEntity;
}
