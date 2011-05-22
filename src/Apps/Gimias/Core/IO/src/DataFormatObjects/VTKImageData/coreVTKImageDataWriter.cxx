/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKImageDataWriter.h"
#include "coreDataEntity.h"
#include "coreDataEntityInfoHelper.h"
#include "coreException.h"

#include <stdio.h>
#include <string.h>

#include "blImageUtils.h"

#include "coreVTKImageDataHolder.h"

using namespace Core::IO;

//!
VTKImageDataWriter::VTKImageDataWriter(void)
{
	m_ValidExtensionsList.push_back( ".vtk" );
	m_ValidExtensionsList.push_back( ".vti" );
	m_ValidExtensionsList.push_back( ".v3d" );
	m_ValidExtensionsList.push_back( ".mhd" );
	m_ValidExtensionsList.push_back( ".hdr" );
	m_ValidExtensionsList.push_back( ".nii" );
	m_ValidExtensionsList.push_back( ".dcm" );
	m_ValidTypesList.push_back( ImageTypeId );
	m_ValidTypesList.push_back( ROITypeId );
}

//!
VTKImageDataWriter::~VTKImageDataWriter(void)
{
}

//!
void VTKImageDataWriter::WriteData()
{
	WriteAllTimeSteps( );
}

void Core::IO::VTKImageDataWriter::WriteSingleTimeStep( 
	const std::string& fileName, 
	Core::DataEntity::Pointer dataEntity, 
	int iTimeStep )
{
	// Get the image
	Core::vtkImageDataPtr image;
	bool worked = dataEntity->GetProcessingData( image, iTimeStep );
	if( !worked || image == NULL )
	{
		throw Core::Exceptions::Exception(
			"VTKImageDataWriter::WriteSingleTimeStep",
			"Input data is not of the correct type" );
	}

	// Save the image
	blImageUtils::SaveImageToFile(
		image, 
		fileName.c_str(), 
		NULL, 
		false );

}

