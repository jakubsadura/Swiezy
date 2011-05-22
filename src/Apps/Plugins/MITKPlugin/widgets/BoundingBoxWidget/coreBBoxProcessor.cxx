/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBBoxProcessor.h"
#include "itkImageToVTKImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "coreImageDataEntityMacros.h"
#include "itkImageBase.h"
#include "itkImageRegion.h"
#include "vtkCubeSource.h"


Core::BBoxProcessor::BBoxProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input image" );
	GetInputPort( 0 )->SetDataEntityType( Core::ImageTypeId );
	SetNumberOfOutputs( 2 );
	GetOutputPort( 0 )->SetName( "Output image" );
	GetOutputPort( 1 )->SetName( "Output bbox" );

	SetName( "BBoxProcessor" );

	m_ProcessorDataHolder = CroppingBoundingBoxHolder::New( );
	m_ProcessorDataHolder->SetSubject(Core::BoundingBox::New());
}

Core::BBoxProcessor::CroppingBoundingBoxHolder::Pointer 
Core::BBoxProcessor::GetProcessorDataHolder() const
{
	return m_ProcessorDataHolder;
}

void Core::BBoxProcessor::ResetBox()
{
	if( !GetInputDataEntity( 0 ) )
		return;

	if( GetInputDataEntity( 0 )->IsImage() )
	{
		Core::vtkImageDataPtr image;
		GetProcessingData( 0, image );
		itk::ImageRegion<3> region;
		region.SetIndex( 0, image->GetExtent()[ 0 ] );
		region.SetIndex( 1, image->GetExtent()[ 2 ] );
		region.SetIndex( 2, image->GetExtent()[ 4 ] );
		region.SetSize( 0, image->GetExtent()[ 1 ] );
		region.SetSize( 1, image->GetExtent()[ 3 ] );
		region.SetSize( 2, image->GetExtent()[ 5 ] );
		GetProcessorDataHolder()->GetSubject()->SetBox( region );
		GetProcessorDataHolder()->GetSubject()->SetSpacing( image->GetSpacing() );
		GetProcessorDataHolder()->GetSubject()->SetOrigin( image->GetOrigin() );
		GetProcessorDataHolder()->NotifyObservers();
	}
}

void Core::BBoxProcessor::ExportBBox()
{
	Core::BoundingBox::Pointer parameters;
	parameters = GetProcessorDataHolder()->GetSubject();
	if( parameters.IsNull() )
		return ;

	double bounds[ 6 ];
	parameters->GetBounds( bounds );

	vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
	cube->SetBounds( bounds );
	cube->Update();

	UpdateOutput( 1, cube->GetOutput(), "Bounding Box", false, 1, GetInputDataEntity( 0 ) );
}

