/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKImageDataRenDataBuilder.h"
#include <vtkImageData.h>
#include <mitkImage.h>
#include "vtkArrayCalculator.h"

#include "coreDataEntity.h"
#include "coreVTKImageDataHolder.h"

using namespace Core;

//!
VTKImageDataRenDataBuilder::VTKImageDataRenDataBuilder(void)
{
}

//!
VTKImageDataRenDataBuilder::~VTKImageDataRenDataBuilder(void)
{
}

void VTKImageDataRenDataBuilder::Update( )
{
	// Get input
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );

	if ( !CheckInput( ) )
	{
		return;
	}

	// Create an empty mitk::Image
	Core::DataEntity::Pointer tempDataEntity;
	tempDataEntity = Core::DataEntity::New( Core::ImageTypeId );
	tempDataEntity->SwitchImplementation( typeid( mitk::Image::Pointer ) );

	// Initialize the old pointer if any
	mitk::BaseData::Pointer mitkData;
	boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
	Core::CastAnyProcessingData( anyData, mitkData );
	mitk::Image::Pointer mitk3DImage = dynamic_cast<mitk::Image*> ( mitkData.GetPointer() );
	if ( mitk3DImage.IsNotNull() )
	{
		Core::DataEntityImpl::Pointer mitkImageDataEntity;
		mitkImageDataEntity = tempDataEntity->GetTimeStep( );
		mitkImageDataEntity->SetDataPtr( mitk3DImage );
	}

	// Get Number of components
	blTagMap::Pointer properties = blTagMap::New( );
	dataEntity->GetTimeStep( 0 )->GetData( properties );
	int numComponents = 1;
	if ( properties.IsNotNull() )
	{
		blTag::Pointer tag;
		tag = properties->FindTagByName( "NumberOfComponents" );
		if ( tag.IsNotNull() ) 
		{
			numComponents = tag->GetValueCasted<int>();
		}
	}

	if ( numComponents == 1)
	{

		// Copy the processing data to rendering data, referencing the memory
		if ( !tempDataEntity->Copy( dataEntity, Core::gmReferenceMemory ) )
		{
			return;
		}
		tempDataEntity->GetProcessingData( mitk3DImage );

		dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitk3DImage) );
	}
	else if  ( numComponents == 2)
	{
		// Get vtkImageData
		Core::vtkImageDataPtr vtkImage;
		Core::DataEntity::Pointer vtkDataEntity = Core::DataEntity::New( Core::ImageTypeId );
		vtkDataEntity->SwitchImplementation( typeid( Core::vtkImageDataPtr ) );
		vtkDataEntity->Copy( dataEntity, Core::gmReferenceMemory );

		tempDataEntity->SwitchImplementation( typeid( Core::vtkImageDataPtr ) );
		for ( TimeStepIndex i = 0 ; i < vtkDataEntity->GetNumberOfTimeSteps() ; i++ )
		{

			vtkDataEntity->GetProcessingData( vtkImage, i );

			// Compute magnitude
			vtkSmartPointer<vtkArrayCalculator> calc = vtkSmartPointer<vtkArrayCalculator>::New();
			calc->SetInput( vtkImage );
			calc->SetAttributeModeToUsePointData();
			calc->AddScalarVariable("s_0","scalars",0);
			calc->AddScalarVariable("s_1","scalars",1);
			calc->SetFunction( "sqrt(s_0*s_0 + s_1*s_1)");
			calc->SetResultArrayName("scalars");
			calc->SetResultArrayType( vtkImage->GetScalarType() );
			calc->Update();

			// We need to copy the data, otherwise this doesn't work!!!
			Core::vtkImageDataPtr vtkImageMagnitude = Core::vtkImageDataPtr::New();
			vtkImageMagnitude->DeepCopy(calc->GetOutput());
			tempDataEntity->AddTimeStep( vtkImageMagnitude );
		}

		// Convert it to MITK
		tempDataEntity->SwitchImplementation( typeid( mitk::Image::Pointer ) );
		tempDataEntity->GetProcessingData( mitk3DImage );
		dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitk3DImage) );
	}
}

