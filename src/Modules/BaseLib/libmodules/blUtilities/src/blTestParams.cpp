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

#include "boost/format.hpp"

#include "blTestParams.h"
#include "blTestParamsFolderInfo.h"

using namespace std;

const char *SEP_CHAR = ".";


//--------------------------
// OPERATIONS
//--------------------------

/**
*/
blTestParams::blTestParams(	
						   int iNumTest,
						   bool bEnable,
						   const char* strInputDataFileName, 
						   const char* strOutputDataExtension )
{
	m_bEnable = bEnable;
	boost::format format( "T%02d" );
	m_strTestModuleNumber = (format % iNumTest).str();

	AddData( strInputDataFileName, strOutputDataExtension );
};


blTestParams::blTestParams(	
						   int iNumTest,
						   bool bEnable,
						   const char* strInputBaseName, 
						   const char* strInputBaseName1, 
						   const char* strOutputExtension,
						   const char* strOutputExtension1 )
{
	m_bEnable = bEnable;
	boost::format format( "T%02d" );
	m_strTestModuleNumber = (format % iNumTest).str();

	AddData( strInputBaseName, strOutputExtension );
	AddData( strInputBaseName1, strOutputExtension1 );
}



/**
*/
void blTestParams::AddData( 
						   const char* strInputDataFileName, 
						   const char* strOutputDataExtension )
{

	if ( strInputDataFileName )
	{

		std::string	strFileName( strInputDataFileName );


		//----------------------------------------------
		// Find positions of strFileName to split
		long lPosExtension = strFileName.rfind( '.' );
		long lPosLocalPath = strFileName.rfind( '/' ) + 1;
		std::string	strLocalPath;
		std::string	strBaseFileName;
		std::string	strExtensionFileName;

		if ( lPosExtension != -1 )
		{
			// local path with final '/' char
			strLocalPath = std::string( 
				strFileName, 
				0, 
				lPosLocalPath );

			// base file name without the '/' char
			strBaseFileName = std::string( 
				strFileName, 
				lPosLocalPath, 
				lPosExtension - lPosLocalPath );

			// Extension with the initial '.' char
			strExtensionFileName = std::string(
				strFileName, 
				lPosExtension, 
				strFileName.size( ) );
		}
		m_strVectorInputBaseExtension.push_back( strExtensionFileName );
		m_strVectorInputBaseName.push_back( strBaseFileName );
		m_strVectorInputLocalPath.push_back( strLocalPath );


		//----------------------------------------------
		// If we defined a output extension, add this, else use
		// the same as input
		if ( strOutputDataExtension ) 
		{
			m_strVectorOutputExtension.push_back( strOutputDataExtension );
		}
		else
		{
			if ( lPosExtension != -1 )
			{
				m_strVectorOutputExtension.push_back( strExtensionFileName );
			}
		}
	}
}



/**
*/
std::string blTestParams::GetCorrectFileName( 
	int iIndex,
	const char *pStrTestModuleStep,
	int iNumber  )
{
	return 
		blTestParamsFolderInfo::Instance()->m_strCorrectDataFolder + '/' +
		blTestParamsFolderInfo::Instance()->m_strTestModuleName + SEP_CHAR +
		m_strTestModuleNumber + SEP_CHAR +
		pStrTestModuleStep  + SEP_CHAR + 
		CreateInputBaseName( 0, iNumber ) + SEP_CHAR +
		"Out" + 
		m_strVectorOutputExtension[ iIndex ];
};



/**
*/
std::string blTestParams::GetWorkingFileName( 
	int iIndex,
	const char *pStrTestModuleStep,
	int iNumber )
{
	return 
		blTestParamsFolderInfo::Instance()->m_strWorkingDataFolder + '/' +
		blTestParamsFolderInfo::Instance()->m_strTestModuleName + SEP_CHAR +
		m_strTestModuleNumber + SEP_CHAR +
		pStrTestModuleStep  + SEP_CHAR + 
		CreateInputBaseName( 0, iNumber ) + SEP_CHAR +
		"Out" +
		m_strVectorOutputExtension[ iIndex ];
};



/**
*/
std::string blTestParams::GetInputFileName( 
	int iIndex, int iNumber )
{
	return 	
		blTestParamsFolderInfo::Instance()->m_strInputDataFolder + '/' +
		m_strVectorInputLocalPath[ iIndex ] + 
		CreateInputBaseName( iIndex, iNumber ) + 
		m_strVectorInputBaseExtension[ iIndex ];
};



/**
*/
std::string blTestParams::CreateInputBaseName( 
	int iIndex, int iNumber )
{
	std::string strInputBaseName;

	strInputBaseName = m_strVectorInputBaseName[ iIndex ];

	if ( iNumber != -1 )
	{
		boost::format format( "%02d" );
		strInputBaseName += (format % iNumber).str();
	}

	return strInputBaseName;
}



/**
*/
bool blTestParams::GetEnable( )
{
	return m_bEnable;
}
