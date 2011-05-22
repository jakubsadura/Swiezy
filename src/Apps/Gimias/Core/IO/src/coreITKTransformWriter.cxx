/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreITKTransformWriter.h"
#include "coreITKTransformImpl.h"

#include <itkTransformFileWriter.h>

//!
Core::IO::ITKTransformWriter::ITKTransformWriter(void) 
{
	m_ValidExtensionsList.push_back( ".dof");
	m_ValidExtensionsList.push_back( ".txt");
	m_ValidTypesList.push_back( Core::TransformId);
}

//!
Core::IO::ITKTransformWriter::~ITKTransformWriter(void)
{
}

//!
void Core::IO::ITKTransformWriter::WriteData()
{
	
	if ( m_Filenames.size() == 0 )
	{
		return ;
	}

	
	if( GetInputDataEntity( 0 ).IsNull() )
	{
		throw Core::Exceptions::Exception( 
			"ITKTransformWriter::WriteData()", 
			"Input data entity is NULL" );
	}

	typedef itk::TransformFileWriter TrWriter;
	TrWriter::Pointer trwr;

	// Get the itk transform file
	trwr = TrWriter::New();
	trwr->SetFileName( m_Filenames[ 0 ].c_str() );

	for ( size_t iTimeStep = 0 ; iTimeStep < GetInputDataNumberOfTimeSteps( 0 ) ; iTimeStep++ )
	{
		Core::ITKTransformImpl::TransformPointer transform;
		bool worked = GetInputDataEntity( 0 )->GetProcessingData( transform, iTimeStep );
		trwr->AddTransform( transform.GetPointer() );
	}

	trwr->Update();
	
}
