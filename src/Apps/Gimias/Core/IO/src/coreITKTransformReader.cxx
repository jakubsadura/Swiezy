// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreITKTransformReader.h"
#include "coreITKTransformImpl.h"

#include <itkTransformFileReader.h>

//!
Core::IO::ITKTransformReader::ITKTransformReader(void) 
{
	m_ValidExtensionsList.push_back( ".dof" );
	m_ValidExtensionsList.push_back( ".txt" );
	m_ValidTypesList.push_back( Core::TransformId );
}

//!
Core::IO::ITKTransformReader::~ITKTransformReader(void)
{
}

//!
void Core::IO::ITKTransformReader::ReadData()
{
	Core::DataEntity::Pointer dataEntity = NULL;

	if ( m_Filenames.size() == 0 )
	{
		return ;
	}

	//! Read the input transform file
	itk::TransformFileReader::Pointer transformReader = itk::TransformFileReader::New();
	transformReader->DebugOff();
	std::cout << "Loading Itk Transform file..." << std::endl;
	transformReader -> SetFileName( m_Filenames[ 0 ].c_str() );
	transformReader -> Update();

	//! Deep copy of the transform file list. 
	itk::TransformFileReader::TransformListType transforms;
	transforms = *transformReader->GetTransformList();

	if( transforms.size() >= 1 )
	{
		std::vector<itk::TransformFileReader::TransformPointer> transformVector;
		itk::TransformFileReader::TransformListType::iterator it;
		for ( it = transforms.begin() ; it != transforms.end() ; it++ )
		{
			transformVector.push_back( *it );
		}

		// Use DataEntity factory to build a DataEntity from itk transform file
		dataEntity = Core::DataEntity::New( TransformId );
		dataEntity->AddTimeSteps( transformVector );
		SetOutputDataEntity( 0, dataEntity );
	}
	std::cout << "Itk Transform file loaded." << std::endl;

}

boost::any Core::IO::ITKTransformReader::ReadSingleTimeStep( int iTimeStep, const std::string &filename )
{
	return NULL;
}