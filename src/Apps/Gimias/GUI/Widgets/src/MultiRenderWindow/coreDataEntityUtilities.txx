/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityUtilities_TXX
#define coreDataEntityUtilities_TXX

//core
#include "coreDataEntityUtilities.h"
#include "coreDataEntity.h"
#include "coreVTKImageDataHolder.h"
#include "blImageUtils.h"

template <typename outputItkImageType>
typename outputItkImageType::Pointer Core::AdaptToItkImage
(
	Core::DataEntity::Pointer inputImageDataEntity,
	int pixelType
)
{
	Core::DataEntity::Pointer m_inputImageDataEntity;
	m_inputImageDataEntity = inputImageDataEntity;
	int m_pixelType = pixelType;
	//!
	Core::vtkImageDataPtr currentVtkInputImage;
	//The output image
	typename outputItkImageType::Pointer itkOutputImage = NULL;

	//check if it is a volume image
	if( !m_inputImageDataEntity->IsImage() )
	{
		//Return Null if the image isn't a volume image
		return NULL;
	}

	//check if the image is a vtkImageData and convert the image to itk image
	if ( m_inputImageDataEntity->GetProcessingData(currentVtkInputImage) )
	{

		itkOutputImage = blImageUtils::ComputeVtkToItkImageAdaptation<outputItkImageType>
		(
			currentVtkInputImage,
			m_pixelType
		);
		return itkOutputImage;
	}
	else
	{
		//Volume Image type not supported!
		return NULL; 
	}
}


/**
 */
template <typename outputItkImageType>
typename outputItkImageType::Pointer Core::AdaptVtkToItkImage(
	Core::DataEntityHolder::Pointer inputImageDataEntityHolder,
	int iTimeStep )
{
	Core::DataEntity::Pointer inputImageDataEntity;
	vtkImageDataPtr vtkInputImage;
	typename outputItkImageType::Pointer itkOutputImage = NULL;

	// Check if input data entity is null
	if ( inputImageDataEntityHolder->GetSubject().IsNull() )
	{
		return NULL;
	}
	inputImageDataEntity = inputImageDataEntityHolder->GetSubject();

	//check if it is a volume image
	if( !inputImageDataEntity->IsImage() )
	{
		return NULL;
	}
	inputImageDataEntity->GetProcessingData( vtkInputImage, iTimeStep );

	//check if the image is a vtkImageData 
	if ( vtkInputImage == NULL )
	{
		return NULL;
	}

	// get the vtk image pixel type and convert the image to itk image
	int pixelType = vtkInputImage->GetScalarType();
	itkOutputImage = blImageUtils::ComputeVtkToItkImageAdaptation<outputItkImageType>(
						vtkInputImage,
						pixelType);
	return itkOutputImage;
}

#endif //coreDataEntityUtilities_TXX
