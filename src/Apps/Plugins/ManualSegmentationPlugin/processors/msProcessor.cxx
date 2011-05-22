/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "msProcessor.h"


ManualSegmentationProcessor::ManualSegmentationProcessor() 
{
	SetNumberOfInputs( NUMBER_OF_INPUTS );

	GetInputPort( INPUT_ROI )->SetName( "ROI image" );
	GetInputPort( INPUT_ROI )->SetDataEntityType( Core::ImageTypeId | Core::ROITypeId );
	GetInputPort( INPUT_ROI )->SetNotValidDataEntityType( Core::ImageTypeId );

	GetInputPort( REFERENCE_IMAGE )->SetName( "Reference image" );
	GetInputPort( REFERENCE_IMAGE )->SetDataEntityType( Core::ImageTypeId );
}


ManualSegmentationProcessor::~ManualSegmentationProcessor() 
{

}
