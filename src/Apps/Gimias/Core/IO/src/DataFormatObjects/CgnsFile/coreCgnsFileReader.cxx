/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreCgnsFileReader.h"

// LCore
#include "coreVTKUnstructuredGridHolder.h"


// baselib
#include <blShapeUtils.h>
#include "blCgnsFileReader.h"
#include "blVTKHelperTools.h"

#include "vtkUnstructuredGrid.h"
#include "vtkFloatArray.h"

#include "boost/foreach.hpp"

#include "vtkUnstructuredGridWriter.h"

using namespace Core::IO;
using namespace std;

//!
CGNSFileReader::CGNSFileReader ( ) 
{
	m_ValidExtensionsList.push_back( ".cgns" );
	m_ValidTypesList.push_back( VolumeMeshTypeId );
	m_scalarsNamesPoint = std::vector<std::string>(0);
	m_vectorsNamesPoint = std::vector<std::string>(0);
	m_scalarsNamesPoint = std::vector<std::string>(0);
	m_vectorsNamesPoint = std::vector<std::string>(0);
	m_compress = true;
}

//!
CGNSFileReader::~CGNSFileReader ( )
{
}

bool CGNSFileReader::ReadHeader( )
{
	blCgnsFileReader::Pointer reader = blCgnsFileReader::New();
	reader->SetFilenames(m_Filenames);
	std::pair<std::vector<std::string>,std::vector<std::string> > variablesPair;
	
	blCgnsFileReader::FourStringVectors fsv;

	fsv = reader->ReadVariables();

	std::vector<std::string> scalarsPoint = fsv.ScalarPoint;
	std::vector<std::string> vectorsPoint = fsv.VectorPoint;	
	std::vector<std::string> scalarsCell = fsv.ScalarCell;
	std::vector<std::string> vectorsCell = fsv.VectorCell;


	Core::DataEntityType type = Core::VolumeMeshTypeId;
	m_TagMap->AddTag( "DataEntityType", Core::VolumeMeshTypeId );

	for ( int i = 0; i < scalarsPoint.size(); i++ )
	{
		m_TagMap->AddTag(scalarsPoint[i],0);
		m_tagsTypeMap[scalarsPoint[i]] = 0;
	}

	for ( int i = 0; i < scalarsCell.size(); i++ )
	{
		m_TagMap->AddTag(scalarsCell[i],1);
		m_tagsTypeMap[scalarsCell[i]] = 1;
	}

	for ( int i = 0; i < vectorsPoint.size(); i++ )
	{
		m_TagMap->AddTag(vectorsPoint[i],2);
		m_tagsTypeMap[vectorsPoint[i]] = 2;
	}

	for ( int i = 0; i < vectorsCell.size(); i++ )
	{
		m_TagMap->AddTag(vectorsCell[i],3);
		m_tagsTypeMap[vectorsCell[i]] = 3;
	}

	m_TagMap->AddTag("UseCompression",4);

	return true;
}

//!
void CGNSFileReader::Update ( )
{
	//init scalars and vectors settings
	blTagMap::Iterator it;
	for ( it = m_TagMap->GetIteratorBegin(); it != m_TagMap->GetIteratorEnd(); ++it )
	{
		blTag::Pointer tag = m_TagMap->GetTag(it);
		std::string tagName = tag->GetName();
		int type;
		if ( !tag->GetValue( type ) )
		{
			continue;
		}

		if ( type == 0 )
		{
			m_scalarsNamesPoint.push_back(tagName);
		}
		else if ( type == 1 )
		{
			m_scalarsNamesCell.push_back(tagName);
		}	
		else if ( type == 2 )
		{
			m_vectorsNamesPoint.push_back(tagName);
		}	
		else if ( type == 3 )
		{
			m_vectorsNamesCell.push_back(tagName);
		}	
	}

	//init compression settings
	blTag::Pointer compressTag = m_TagMap->FindTagByName("UseCompression");
	int compressValue =  boost::any_cast<int>(compressTag->GetValue());
	m_compress = (compressValue != 0);

	if ( m_Filenames.size() == 0 ) return;

	Core::DataEntity::Pointer dataEntity = NULL;

	blCgnsFileReader::Pointer reader = blCgnsFileReader::New();

	reader->SetFilenames(m_Filenames);
	reader->setCompress(m_compress);
	reader->SetScalarsListPoint(m_scalarsNamesPoint);
	reader->SetVectorsListPoint(m_vectorsNamesPoint);
	reader->SetScalarsListCell(m_scalarsNamesCell);
	reader->SetVectorsListCell(m_vectorsNamesCell);
	reader->Update();

	std::vector<Core::vtkUnstructuredGridPtr> UnstructuredGridVector = reader->GetOutput();

	if ( dataEntity.IsNull() )
	{
		// Create a new data entity
		dataEntity = Core::DataEntity::New( VolumeMeshTypeId );
		dataEntity->GetMetadata()->SetName( "CgnsFile" );
		dataEntity->Resize( UnstructuredGridVector.size(), typeid( vtkUnstructuredGridPtr ) );
	}

	// Copy processing data
	for ( unsigned i = 0 ; i < UnstructuredGridVector.size() ; i++ )
	{
		Core::vtkUnstructuredGridPtr	processingData;
		Core::vtkUnstructuredGridPtr	newProcessingDataPtr = Core::vtkUnstructuredGridPtr::New();
		dataEntity->GetProcessingData( processingData, i );
		
		if ( UnstructuredGridVector[ i ] )
		{
			if (abs(UnstructuredGridVector[ i ]->GetBounds()[1]-UnstructuredGridVector[ i ]->GetBounds()[0 ]) < 0.10 ||
				abs(UnstructuredGridVector[ i ]->GetBounds()[3]-UnstructuredGridVector[ i ]->GetBounds()[2 ])< 0.10 ||
				abs(UnstructuredGridVector[ i ]->GetBounds()[5]-UnstructuredGridVector[ i ]->GetBounds()[4 ]) < 0.10 )
				
				blVTKHelperTools::ScaleVolume( 
					UnstructuredGridVector[ i ], 
					newProcessingDataPtr, 
					1000 );
			else
			{
				newProcessingDataPtr = UnstructuredGridVector[ i ];
			}
			processingData->DeepCopy( newProcessingDataPtr );
		}
	}

	dataEntity->GetMetadata()->AddTag( "IsCGNS", true );
	dataEntity->GetMetadata()->AddTag( "CGNSTimePoints", reader->GetTimeSteps() );
	SetOutputDataEntity( 0, dataEntity );
}


boost::any Core::IO::CGNSFileReader::ReadSingleTimeStep( int iTimeStep, const std::string &filename )
{
	return NULL;
}
