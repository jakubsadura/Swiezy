/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreCroppingProcessor.h"
#include "itkImageToVTKImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "coreImageDataEntityMacros.h"
#include "itkImageBase.h"
#include "itkImageRegion.h"

namespace Private
{
	struct CroppingProcessorHelper
	{
		template< class ItkImageType >
		void Crop(const typename ItkImageType::Pointer image)
		{
			typedef typename itk::RegionOfInterestImageFilter< ItkImageType, ItkImageType > ROIFilter;
			typename ROIFilter::Pointer roif = ROIFilter::New();
			roif->SetRegionOfInterest( processor->GetProcessorDataHolder()->GetSubject()->GetBox( ) );
			roif->SetInput(image);
			roif->Update();
			// HACK: convert to a vtk image, because the core does not handle ITK images well
			typedef itk::ImageToVTKImageFilter<ItkImageType> ToVtkImageFilter;
			typename ToVtkImageFilter::Pointer toVtkImage = ToVtkImageFilter::New();
			roif->Update();
			toVtkImage->SetInput(roif->GetOutput());
			toVtkImage->Update();
			Core::vtkImageDataPtr imageVTK = Core::vtkImageDataPtr::New();
			imageVTK->DeepCopy(toVtkImage->GetOutput());
			Core::DataEntity::Pointer dataEntity = Core::DataEntity::New( Core::ImageTypeId );
			dataEntity->AddTimeStep( imageVTK );
			dataEntity->GetMetadata()->SetName( "Cropped image" );
			dataEntity->GetMetadata( )->SetModality(modality);
			processor->SetOutputDataEntity( 0, dataEntity);
		}

		Core::CroppingProcessor* processor;
		Core::ModalityType modality;
	};
}

Core::CroppingProcessor::CroppingProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input image" );
	GetInputPort( 0 )->SetDataEntityType( Core::ImageTypeId );
	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetName( "Output image" );

	SetName( "CroppingProcessor" );

	m_ProcessorDataHolder = BoundingBoxHolder::New( );
	m_ProcessorDataHolder->SetSubject(Core::BoundingBox::New());
}

Core::BoundingBoxHolder::Pointer 
Core::CroppingProcessor::GetProcessorDataHolder() const
{
	return m_ProcessorDataHolder;
}

void Core::CroppingProcessor::Update()
{
	if( !GetInputDataEntity( 0 ) )
		return;

	if( GetInputDataEntity( 0 )->IsImage() )
	{
		Private::CroppingProcessorHelper helper;
		helper.processor = this;
		helper.modality = GetInputDataEntity( 0 )->GetMetadata()->GetModality();
		coreImageDataEntityItkMacro( GetInputDataEntity( 0 ), helper.Crop)
	}
}

