/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreCgnsFileWriter.h"

// LCore
#include "coreVTKUnstructuredGridHolder.h"

// baselib
#include <blShapeUtils.h>
#include "blCgnsFileWriter.h"

#include "vtkCellData.h"


using namespace Core::IO;
using namespace std;


CGNSFileWriter::CGNSFileWriter ( ) 
{
	m_ValidExtensionsList.push_back( ".cgns" );
	m_ValidExtensionsList.push_back( ".vtk" );

	m_ValidTypesList.push_back( VolumeMeshTypeId );
}


CGNSFileWriter::~CGNSFileWriter ( )
{
	
}


void CGNSFileWriter::WriteData( )
{
	if ( GetInputDataEntity( 0 ).IsNull( ) )
	{
		throw Core::Exceptions::Exception( 
			"CGNSFileWriter::UpdateData()", 
			"Input data entity is NULL" );
	}

	int CGNSTimePoints = 0;
	blTag::Pointer tag = GetInputDataEntity( 0 )->GetMetadata()->FindTagByName( "CGNSTimePoints" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( CGNSTimePoints );
	}

	for ( size_t iTimeStep = 0 ; iTimeStep < CGNSTimePoints ; iTimeStep++ )
	{
		std::string strFileName;

		if ( CGNSTimePoints > 1 )
		{
			if ( m_Filenames.size( ) == 1 )
			{
				strFileName = AddFileNumber( m_Filenames[ 0 ], iTimeStep );
			}
			else
			{
				strFileName = m_Filenames.at( iTimeStep );
			}
		}
		else
		{
			strFileName = m_Filenames[ 0 ];
		}

		WriteSingleTimeStep( strFileName, GetInputDataEntity( 0 ), iTimeStep );
	}

	if ( CGNSTimePoints == 0 )
	{
		if ( m_Filenames.size() > 0 )
		{
			WriteSingleTimeStep( m_Filenames[ 0 ], GetInputDataEntity( 0 ), 0 );
		}
	}
}

void CGNSFileWriter::WriteSingleTimeStep ( 
	const std::string& fileName, 
	Core::DataEntity::Pointer dataEntity, 
	int iTimePoint )
{
	int iTimeStep = iTimePoint;
	int numberOfTimeSteps = dataEntity->GetNumberOfTimeSteps();

	if ( numberOfTimeSteps == 1 )
	{
		iTimeStep = 0; // we read the cgns file using compression
	}

	vtkSmartPointer<vtkUnstructuredGrid> gridWithFlowSolution;
	dataEntity->GetProcessingData(gridWithFlowSolution,iTimeStep);

	blCgnsFileWriter::Pointer writer = blCgnsFileWriter::New();
	writer->SetFilename(fileName);

	Core::vtkUnstructuredGridPtr	newProcessingDataPtr = Core::vtkUnstructuredGridPtr::New();
	blVTKHelperTools::ScaleVolume( 
		gridWithFlowSolution,
		newProcessingDataPtr,
		0.001 );
	gridWithFlowSolution->DeepCopy( newProcessingDataPtr );

	writer->SetVTKData(gridWithFlowSolution);
	
	this->FillScalarsAndVectors(dataEntity,iTimePoint,iTimeStep);

	writer->SetScalarsListPoint(m_scalarsNamesPoint);
	writer->SetVectorsListPoint(m_vectorsNamesPoint);	
	writer->SetScalarsListCell(m_scalarsNamesCell);
	writer->SetVectorsListCell(m_vectorsNamesCell);

	writer->Update();
}

void CGNSFileWriter::FillScalarsAndVectors(Core::DataEntity::Pointer dataEntity, int iTimePoint, int iTimeStep) { 

	std::vector<std::string> scalarsNamesPoint;
	std::vector<std::string> vectorsNamesPoint;	
	std::vector<std::string> scalarsNamesCell;
	std::vector<std::string> vectorsNamesCell;

	blTag::Pointer tag = dataEntity->GetMetadata()->FindTagByName( "ScalarsPoint" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( scalarsNamesPoint );
	}

	tag = dataEntity->GetMetadata()->FindTagByName( "VectorsPoint" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( vectorsNamesPoint );
	}

	tag = dataEntity->GetMetadata()->FindTagByName( "ScalarsCell" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( scalarsNamesCell );
	}

	tag = dataEntity->GetMetadata()->FindTagByName( "VectorsCell" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( vectorsNamesCell );
	}

	//choose only those that end with _00(timepoint+1)
	m_scalarsNamesPoint.clear();
	for (int i=0; i<scalarsNamesPoint.size(); i++)
	{
		if (SuffixIsNum(scalarsNamesPoint[i]))
		{
			if (CheckString(scalarsNamesPoint[i],iTimePoint+1))
			{
				m_scalarsNamesPoint.push_back(scalarsNamesPoint[i]);
			}
		}
		else
		{
			m_scalarsNamesPoint.push_back(scalarsNamesPoint[i]);
		}
	}

	//choose only those that end with _00(timepoint+1)
	m_scalarsNamesCell.clear();
	for (int i=0; i<scalarsNamesCell.size(); i++)
	{
		if (SuffixIsNum(scalarsNamesCell[i]))
		{
			if (CheckString(scalarsNamesCell[i],iTimePoint+1))
			{
				m_scalarsNamesCell.push_back(scalarsNamesCell[i]);
			}
		}
		else
		{
			m_scalarsNamesCell.push_back(scalarsNamesCell[i]);
		}
	}

	m_vectorsNamesPoint.clear();
	for (int i=0; i<vectorsNamesPoint.size(); i++)
	{
		if (SuffixIsNum(vectorsNamesPoint[i]))
		{
			if (CheckString(vectorsNamesPoint[i],iTimePoint+1))
			{
				m_vectorsNamesPoint.push_back(vectorsNamesPoint[i]);
			}
		}
		else
		{
			m_vectorsNamesPoint.push_back(vectorsNamesPoint[i]);
		}
	}

	m_vectorsNamesCell.clear();
	for (int i=0; i<vectorsNamesCell.size(); i++)
	{
		if (SuffixIsNum(vectorsNamesCell[i]))
		{
			if (CheckString(vectorsNamesCell[i],iTimePoint+1))
			{
				m_vectorsNamesCell.push_back(vectorsNamesCell[i]);
			}
		}
		else
		{
			m_vectorsNamesCell.push_back(vectorsNamesCell[i]);
		}
	}
	

}

bool CGNSFileWriter::SuffixIsNum(std::string str)
{
	if ( str.size() <= 4 ) 
		return false;

	std::string suffix;
	suffix = str.substr( str.size()-4, str.size() );

	if ( suffix[0] != '_' ) 
		return false;

	for ( int i = 1; i < 4; i++ )
	{
		if ( !isdigit(suffix[i]) ) 
			return false;
	}

	return true;
}


bool CGNSFileWriter::CheckString(std::string str, int n)
{
	if ( str.size() <= 4 ) return false;
	if ( str[str.size()-4] != '_' ) return false;

	char num [200];
	std::sprintf(num,"%d",n);
	std::string numStr(num);
	while (numStr.size()<3) numStr = "0" + numStr;

	for ( int i = 0; i < 3; i++ )
	{
		if ( str[str.size()-1-i] != numStr[numStr.size()-1-i] )
		{
			return false;
		}
	}
	return true;
}

void Core::IO::CGNSFileWriter::GenerateMetadata( Core::DataEntity::Pointer dataEntity )
{
	vtkSmartPointer<vtkUnstructuredGrid> gridWithFlowSolution;
	dataEntity->GetProcessingData(gridWithFlowSolution,0);
	int nA = gridWithFlowSolution->GetPointData( )->GetNumberOfArrays( );

	std::vector<std::string> scalarsPoint;
	std::vector<std::string> vectorsPoint;
	std::vector<std::string> scalarsCell;
	std::vector<std::string> vectorsCell;

	for ( int i = 0; i < nA; i++ ) 
	{
		std::string str( gridWithFlowSolution->GetPointData( )->GetArrayName( i ) );
		vtkDataArray * dataArray;
		dataArray = gridWithFlowSolution->GetPointData( )->GetArray( str.c_str( ) );

		if ( dataArray != NULL ) 
		{
			int nTuples = dataArray->GetNumberOfTuples( );
			int nComponents = dataArray->GetNumberOfComponents( ); 

			if ( nComponents == 1 ) 
			{
				scalarsPoint.push_back( str );
			} 
			else if ( nComponents == 3)
			{
				vectorsPoint.push_back( str );
			}
		}
	}

	int nB = gridWithFlowSolution->GetCellData( )->GetNumberOfArrays( );

	for ( int i = 0; i < nB; i++ ) 
	{
		std::string str( gridWithFlowSolution->GetCellData( )->GetArrayName( i ) );
		vtkDataArray * dataArray;
		dataArray = gridWithFlowSolution->GetCellData( )->GetArray( str.c_str( ) );

		if ( dataArray != NULL ) 
		{
			int nTuples = dataArray->GetNumberOfTuples( );
			int nComponents = dataArray->GetNumberOfComponents( ); 

			if ( nComponents == 1 ) 
			{
				scalarsCell.push_back( str );
			} 
			else if ( nComponents == 3)
			{
				vectorsCell.push_back( str );
			}
		}
	}

	dataEntity->GetMetadata()->AddTag( "ScalarsPoint", scalarsPoint );
	dataEntity->GetMetadata()->AddTag( "VectorsPoint", vectorsPoint );
	dataEntity->GetMetadata()->AddTag( "ScalarsCell", scalarsCell );
	dataEntity->GetMetadata()->AddTag( "VectorsCell", vectorsCell );

}
