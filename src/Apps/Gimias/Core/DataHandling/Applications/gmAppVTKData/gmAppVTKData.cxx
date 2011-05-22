/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityBuildersRegistration.h"
#include "coreDataEntityFactory.h"
#include "coreVTKPolyDataHolder.h"
#include "coreVTKImageDataHolder.h"
#include "blShapeUtils.h"
#include "mitkSurface.h"

int main (int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << "input shape" << std::endl;
		return EXIT_FAILURE;
	}

	bool success = true;
	Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();

	// Load shape
	Core::vtkPolyDataPtr shape = blShapeUtils::ShapeUtils::LoadShapeFromFile( argv[ 1 ] );
	Core::vtkImageDataPtr image = Core::vtkImageDataPtr::New();
	mitk::Surface::Pointer surface = mitk::Surface::New();

	// Build vtkPolyData data entity 
	Core::DataEntity::Pointer dataEntity;
	dataEntity = Core::DataEntityFactory::Build( Core::SurfaceMeshTypeId, shape );

	// Add shape time step -> Ok
	success = dataEntity->AddTimeStep( shape );

	// Add shape time step -> Ok
	success = dataEntity->AddTimeStep( shape );

	// Add shape time step is mitk::Surface -> Fail
	success = dataEntity->AddTimeStep( surface );

	// Add image time step -> Fail
	success = dataEntity->AddTimeStep( image );

	// Get time step 0 -> Ok
	success = dataEntity->GetProcessingData( shape, 0, true );

	// Get time step 2 -> Fail
	success = dataEntity->GetProcessingData( shape, 3, true );

	// Get image processing data -> Fail
	success = dataEntity->GetProcessingData( image, 0, true );


	///////////////////////////////////////////////////////////////////
	// Switch to mitk::Surface
	success = dataEntity->GetProcessingData( surface, 0 , true );

	// Add shape time step -> Ok
	success = dataEntity->AddTimeStep( shape );

	// Add image time step -> Fail
	success = dataEntity->AddTimeStep( image );

	// Get time step 0 -> Ok
	success = dataEntity->GetProcessingData( shape, 0, true );

	// Get time step 0 -> Ok
	success = dataEntity->GetProcessingData( surface, 0, true );

	// Get image processing data -> Fail
	success = dataEntity->GetProcessingData( image, 0, true );

	// Build MITK Surface
	Core::DataEntity::Pointer dataEntitySurface;
	dataEntitySurface = Core::DataEntityFactory::Build( Core::SurfaceMeshTypeId, surface );

	Core::DataEntity::Pointer dataEntityImage;
	dataEntityImage = Core::DataEntityFactory::Build( Core::ImageTypeId, image );

	return EXIT_SUCCESS;
}


