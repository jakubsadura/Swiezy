/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKPolyDataWriter.h"
#include "coreDataEntity.h"
#include <blShapeUtils.h>
#include "coreVTKPolyDataHolder.h"
#include "coreDataEntityInfoHelper.h"
#include "coreException.h"
#include "coreVTKPolyDataImpl.h"

using namespace Core::IO;

//!
VTKPolyDataWriter::VTKPolyDataWriter(void) 
{
	m_ValidExtensionsList.push_back( ".vtk" );
	m_ValidExtensionsList.push_back( ".vti" );
	m_ValidExtensionsList.push_back( ".stl" );
	m_ValidExtensionsList.push_back( ".vtp" );
	m_ValidTypesList.push_back( SurfaceMeshTypeId );
	m_ValidTypesList.push_back( PointSetTypeId );
	m_ValidTypesList.push_back( ContourTypeId );
}

//!
VTKPolyDataWriter::~VTKPolyDataWriter(void)
{
}

//!
void VTKPolyDataWriter::WriteData()
{
	WriteAllTimeSteps( );
}

void Core::IO::VTKPolyDataWriter::WriteSingleTimeStep( 
	const std::string& fileName, 
	Core::DataEntity::Pointer dataEntity, 
	int iTimeStep )
{
	vtkPolyDataPtr surface = NULL;

	bool worked = dataEntity->GetProcessingData( surface, iTimeStep );

	// Convert always to VTK to save
	if ( surface.GetPointer() == NULL )
	{
		VtkPolyDataImpl::Pointer polyDataTimeStep;
		polyDataTimeStep = VtkPolyDataImpl::New( );
		bool success = polyDataTimeStep->DeepCopy( dataEntity->GetTimeStep( iTimeStep ) );
		worked = success && CastAnyProcessingData( polyDataTimeStep->GetDataPtr( ), surface );
	}

	if( !worked || surface == NULL )
	{
		throw Core::Exceptions::Exception(
			"VTKPolyDataWriter::WriteSingleTimeStep",
			"Input data is not of the correct type" );
	}

	// Get the header
	blShapeUtils::ShapeUtils::SaveShapeToFile(surface, fileName.c_str(), NULL );
}

