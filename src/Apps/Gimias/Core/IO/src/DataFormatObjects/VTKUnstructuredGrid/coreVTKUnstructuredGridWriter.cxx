/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKUnstructuredGridWriter.h"
#include "coreDataEntity.h"
#include <blShapeUtils.h>
#include "coreVTKUnstructuredGridHolder.h"
#include "coreDataEntityInfoHelper.h"
#include "coreException.h"

using namespace Core::IO;

//!
VTKUnstructuredGridWriter::VTKUnstructuredGridWriter(void)
{
	m_ValidExtensionsList.push_back( ".vtk" );
	m_ValidExtensionsList.push_back( ".vti" );
	m_ValidExtensionsList.push_back( ".vol" );
	m_ValidTypesList.push_back( VolumeMeshTypeId );
}

//!
VTKUnstructuredGridWriter::~VTKUnstructuredGridWriter(void)
{
}

//!
void VTKUnstructuredGridWriter::WriteData( )
{
	WriteAllTimeSteps( );
}

void Core::IO::VTKUnstructuredGridWriter::WriteSingleTimeStep( 
	const std::string& fileName, 
	Core::DataEntity::Pointer dataEntity, 
	int iTimeStep )
{
	vtkUnstructuredGridPtr volume = NULL;
	bool worked = dataEntity->GetProcessingData(volume, iTimeStep);
	if( !worked || volume == NULL )
	{
		throw Core::Exceptions::Exception(
			"VTKPolyDataWriter::WriteSingleTimeStep",
			"Input data is not of the correct type" );
	}

	// Get the header
	blShapeUtils::ShapeUtils::SaveVolumeToFile(volume, fileName.c_str());
	
}

