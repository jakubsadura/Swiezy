/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreITKTensorRenDataBuilder.h"
#include "coreAssert.h"
#include <mitkITKImageImport.h>
#include <typeinfo>

#include "itkDTITensor.h"
#include "itkComputeFAFilter.h"
#include "itkImageToVTKImageFilter.h"

#include "vtkImageData.h"

using namespace Core;

//!
ITKTensorRenDataBuilder::ITKTensorRenDataBuilder(void)
{
}

//!
ITKTensorRenDataBuilder::~ITKTensorRenDataBuilder(void)
{
}

//!
void ITKTensorRenDataBuilder::Update( )
{
	// Get input
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );

	if ( !CheckInput( ) )
	{
		return;
	}

	mitk::BaseData::Pointer mitkData;
	boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
	Core::CastAnyProcessingData( anyData, mitkData );
	mitk::Image::Pointer mitk3DImage = dynamic_cast<mitk::Image*> ( mitkData.GetPointer() );
	if ( mitk3DImage.IsNull() )
	{
		mitk3DImage = mitk::Image::New();
	}
	if(dataEntity->GetTimeStep( 0 )->GetDataPtr().empty())
	{
		return;
	}

	const std::type_info* typeID = &dataEntity->GetTimeStep( 0 )->GetDataPtr().type();
	
	if(*typeID == typeid(itk::Image<itk::DTITensor<float>,3>::Pointer))
		mitk3DImage = GetMitkImageFromItkImage(dataEntity->GetTimeStep( 0 )->GetDataPtr());
	
	
	dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitk3DImage) );
}

mitk::BaseData::Pointer Core::ITKTensorRenDataBuilder::BuildSingleTimeStep(
	const Core::DataEntityImpl::Pointer timeStep)
{
	if(timeStep->GetDataPtr().empty())
		return mitk::BaseData::Pointer();

	const std::type_info* typeID = &timeStep->GetDataPtr().type();
	mitk::Image::Pointer result;


	if(*typeID == typeid(itk::Image<itk::DTITensor<float>,3>::Pointer))
		result = GetMitkImageFromItkImage(timeStep->GetDataPtr());
	
	return mitk::BaseData::Pointer(result);
}

mitk::Image::Pointer 
Core::ITKTensorRenDataBuilder::GetMitkImageFromItkImage( 
	AnyProcessingData processingData )
{
	typedef itk::Image<itk::DTITensor<float>,3> TensorImageType;
	typedef TensorImageType::Pointer TensorImageTypePtr;
	typedef itk::Image<float,3> InputImageType;
	TensorImageTypePtr itkImage = NULL;
	CastAnyProcessingData<TensorImageTypePtr>(processingData, itkImage);

	mitk::Image::Pointer mitkImage = mitk::Image::New( );

	if (itkImage)
	{

		typedef itk::ComputeFAFilter<TensorImageType,InputImageType> FAfilterType;
		FAfilterType::Pointer FAfilter = FAfilterType::New();
		FAfilter->SetInput(itkImage);
		FAfilter->Update();
		const InputImageType* inputImage;
		inputImage = FAfilter->GetOutput();

		vtkImageData* imageData = vtkImageData::New();
		typedef itk::ImageToVTKImageFilter<InputImageType> ToVtkImageFilter;
		ToVtkImageFilter::Pointer imageAdaptor = ToVtkImageFilter::New();
		imageAdaptor->SetInput( inputImage );
		imageAdaptor->Update();
		imageData->DeepCopy(imageAdaptor->GetOutput());
		
		// Reuse the processing data buffer
		mitkImage->Initialize(imageData);
		mitkImage->SetImportVolume(imageData->GetScalarPointer(),0, 0, mitk::Image::ReferenceMemory );
		mitk::Point3D origin;
		origin[0] = mitk::Point3D::ValueType( imageData->GetOrigin()[0] );
		origin[1] = mitk::Point3D::ValueType( imageData->GetOrigin()[1] );
		origin[2] = mitk::Point3D::ValueType( imageData->GetOrigin()[2] );
		mitkImage->GetGeometry( 0 )->SetOrigin(origin);
	}
	

	return mitkImage;
}