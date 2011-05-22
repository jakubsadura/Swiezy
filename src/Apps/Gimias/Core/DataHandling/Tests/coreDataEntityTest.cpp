/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityTest.h"
#include "CISTIBToolkit.h"

#include "coreDataEntityBuildersRegistration.h"
#include "coreVTKImageDataImpl.h"
#include "coreVTKPolyDataImpl.h"
#include "coreDataEntity.h"
#include "coreAnyBuffer.h"

#include "blImageUtils.h"
#include "blShapeUtils.h"

#include "mitkSurface.h"

const std::string SHAPE_FILENAME = std::string( CISTIB_TOOLKIT_FOLDER ) + "/src/Apps/Gimias/Core/DataHandling/Tests/TestData/aneu.vtk";
const std::string IMAGE_FILENAME = std::string( CISTIB_TOOLKIT_FOLDER ) + "/src/Apps/Gimias/Core/DataHandling/Tests/TestData/SmallImage.vtk";

void CoreDataEntityTest::setUp()
{
	Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();

}

void CoreDataEntityTest::tearDown()
{
	Core::DataEntityBuildersRegistration::UnRegisterDataEntityBuilders();
}

void CoreDataEntityTest::TestShape()
{
	// Load shape
	Core::vtkPolyDataPtr shape = blShapeUtils::ShapeUtils::LoadShapeFromFile( SHAPE_FILENAME.c_str() );
	Core::vtkImageDataPtr image = Core::vtkImageDataPtr::New();
	mitk::Surface::Pointer surface = mitk::Surface::New();


	// Build vtkPolyData data entity 
	Core::DataEntity::Pointer dataEntity;
	dataEntity = Core::DataEntity::New( Core::SurfaceMeshTypeId );
	dataEntity->AddTimeStep( shape );

	// Add shape time step -> Ok
	TS_ASSERT( dataEntity->AddTimeStep( shape ) );

	// Add shape time step -> Ok
	TS_ASSERT( dataEntity->AddTimeStep( shape ) );

	// Add shape time step is mitk::Surface -> Fail
	TS_ASSERT( dataEntity->AddTimeStep( surface ) == false );

	// Add image time step -> Fail
	TS_ASSERT( dataEntity->AddTimeStep( image ) == false );

	// Get time step 0 -> Ok
	TS_ASSERT( dataEntity->GetProcessingData( shape, 0, true ) );

	// Get time step 2 -> Fail
	TS_ASSERT( dataEntity->GetProcessingData( shape, 3, true ) == false );

	// Get image processing data -> Fail
	TS_ASSERT( dataEntity->GetProcessingData( image, 0, true ) == false );


	///////////////////////////////////////////////////////////////////
	// Switch to mitk::Surface
	TS_ASSERT( dataEntity->GetProcessingData( surface, 0 , true ) );
	
	// Compare shapes after switch implementation
	float fMaxDifference;
	TS_ASSERT( blShapeUtils::ShapeUtils::CompareShapes( surface->GetVtkPolyData( 0 ), shape.GetPointer( ), fMaxDifference ) );

	// Add shape time step -> Ok
	TS_ASSERT( dataEntity->AddTimeStep( shape ) );

	// Add image time step -> Fail
	TS_ASSERT( dataEntity->AddTimeStep( image ) == false );

	// Get time step 0 -> Ok
	TS_ASSERT( dataEntity->GetProcessingData( shape, 0, true ) );

	// Get time step 0 -> Ok
	TS_ASSERT( dataEntity->GetProcessingData( surface, 0, true ) );

	// Get image processing data -> Fail
	TS_ASSERT( dataEntity->GetProcessingData( image, 0, true ) == false );

	// Build MITK Surface
	Core::DataEntity::Pointer dataEntitySurface;
	dataEntitySurface = Core::DataEntity::New( Core::SurfaceMeshTypeId );
	dataEntitySurface->AddTimeStep( surface );
}

void CoreDataEntityTest::TestImage()
{
	// Load shape
	Core::vtkImageDataPtr image1;
	image1 = blImageUtils::LoadImageFromFileAsVTK( IMAGE_FILENAME.c_str() );

	// Build vtkImageDataPtr data entity 
	Core::DataEntity::Pointer dataEntity;
	dataEntity = Core::DataEntity::New( Core::ImageTypeId );
	dataEntity->AddTimeStep( image1 );

	// Convert to itk
	itk::Image<char,3>::Pointer itkImage;
	TS_ASSERT( dataEntity->GetProcessingData( itkImage, 0, true ) );

	// Convert back to vtk
	Core::vtkImageDataPtr image2;
	TS_ASSERT( dataEntity->GetProcessingData( image2, 0, true ) );

	TS_ASSERT( blImageUtils::CompareImages( image1, image2 ) );

	// Convert to MITK
	mitk::Image::Pointer mitkImage;
	TS_ASSERT( dataEntity->GetProcessingData( mitkImage, 0, true ) );
	
	// There's a bug in MITK origin
	//TS_ASSERT( blImageUtils::CompareImages( image1, image2 ) );

	// Convert back to vtk
	TS_ASSERT( dataEntity->GetProcessingData( image2, 0, true ) );

	TS_ASSERT( blImageUtils::CompareImages( image1, image2 ) );

}

void CoreDataEntityTest::TestCopy()
{
	Core::vtkImageDataPtr image1;
	image1 = blImageUtils::LoadImageFromFileAsVTK( IMAGE_FILENAME.c_str() );

	Core::DataEntity::Pointer dataEntity;
	dataEntity = Core::DataEntity::New( Core::ImageTypeId );
	dataEntity->AddTimeStep( image1 );

	// Convert to itk
	itk::Image<char,3>::Pointer itkImage;
	TS_ASSERT( dataEntity->GetProcessingData( itkImage, 0, true ) );

	// Create a reference vtkImageData pointing to the buffer
	Core::DataEntity::Pointer dataEntityReference;
	dataEntityReference = Core::DataEntity::New( Core::ImageTypeId );
	dataEntityReference->SwitchImplementation( typeid( Core::vtkImageDataPtr ) );
	dataEntityReference->Copy( dataEntity, Core::gmReferenceMemory );

	// Get vtk image
	Core::vtkImageDataPtr image2;
	TS_ASSERT( dataEntityReference->GetProcessingData( image2, 0, true ) );

	// itk and 1st vtk don't use the same buffer
	TS_ASSERT( image1->GetScalarPointer() != itkImage->GetBufferPointer( ) );

	// itk and 2nd vtk use the same buffer
	TS_ASSERT( image2->GetScalarPointer() == itkImage->GetBufferPointer( ) );

}