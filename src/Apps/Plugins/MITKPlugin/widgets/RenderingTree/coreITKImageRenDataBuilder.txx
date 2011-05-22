/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreITKImageRenDataBuilder_TXX
#define coreITKImageRenDataBuilder_TXX

#include "coreITKImageRenDataBuilder.h"
#include <itkCastImageFilter.h>
#include <mitkITKImageImport.h>
#include <mitkImageToItk.h>
#include <typeinfo>
#include "coreDataEntity.h"

using namespace Core;


//!
template <class itkImageType>
mitk::Image::Pointer ITKImageRenDataBuilder::GetMitkImageFromItkImage(AnyProcessingData processingData)
{	
	typename itkImageType::Pointer itkImage = NULL;
	CastAnyProcessingData(processingData, itkImage);

	mitk::Image::Pointer mitkImage = mitk::Image::New( );
	
	// Reuse the processing data buffer
	mitkImage->InitializeByItk(itkImage.GetPointer());
	mitkImage->SetImportChannel(itkImage->GetBufferPointer(), 0, mitk::Image::ReferenceMemory );

	// Using this filter causes crash. GenerateOtuputInformation( )
	// is called each time the mitk image is used.
	//mitkImage = mitk::ImportItkImage<itkImageType>(itkImage);

	return mitkImage;
}

#endif
