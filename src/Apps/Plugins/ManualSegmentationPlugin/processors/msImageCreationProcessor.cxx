/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "msImageCreationProcessor.h"

#include "vtkPolyDataToImageStencil.h"
#include "vtkImageStencil.h"
#include "vtkImageMathematics.h"


using namespace Core;

//!
msImageCreationProcessor::msImageCreationProcessor(void)
{
	SetNumberOfInputs(2);
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetInputPort( 0 )->SetName( "Input Surface Mesh" );
	GetInputPort( 1 )->SetDataEntityType( Core::ImageTypeId );
	GetInputPort( 1 )->SetName( "Input Image" );
	SetNumberOfOutputs(1);
	GetOutputPort( 0 )->SetDataEntityType( Core::ImageTypeId );
	GetOutputPort( 0 )->SetReuseOutput( true );
	GetOutputPort( 0 )->SetName( "Output mask image" );
	GetOutputPort( 0 )->SetDataEntityName( "Output mask image" );
}

//!
msImageCreationProcessor::~msImageCreationProcessor(void)
{
}

//!
void msImageCreationProcessor::Update( )
{

	vtkPolyDataPtr inputMesh;
	GetProcessingData( 0, inputMesh );

	vtkImageDataPtr inputImage;
	GetProcessingData( 1, inputImage );

	vtkSmartPointer<vtkPolyDataToImageStencil> stencilFilter;
	stencilFilter = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
	stencilFilter->SetInput( inputMesh );
	stencilFilter->SetInformationInput( inputImage );
	stencilFilter->Update();

	// Create an empty image
	vtkSmartPointer<vtkImageMathematics> operation;
	operation = vtkSmartPointer<vtkImageMathematics>::New();
	operation->SetInput1(inputImage);
	operation->SetInput2(inputImage);
	operation->SetOperationToSubtract();
	operation->Update();
	Core::vtkImageDataPtr outputImage = operation->GetOutput();

	vtkSmartPointer<vtkImageStencil> imageStencil;
	imageStencil = vtkSmartPointer<vtkImageStencil>::New();
	imageStencil->SetInput( outputImage );
	imageStencil->SetStencil( stencilFilter->GetOutput( ) );
	imageStencil->SetBackgroundValue( 1 );
	imageStencil->Update();

	// 0->1 and 1->2
	vtkSmartPointer<vtkImageMathematics> operation1;
	operation1 = vtkSmartPointer<vtkImageMathematics>::New();
	operation1->SetInput1(imageStencil->GetOutput());
	operation1->SetOperationToAddConstant();
	operation1->SetConstantC( 1 );
	operation1->Update();

	// 2->0
	vtkSmartPointer<vtkImageMathematics> operation2;
	operation2 = vtkSmartPointer<vtkImageMathematics>::New();
	operation2->SetInput1(operation1->GetOutput());
	operation2->SetOperationToReplaceCByK();
	operation2->SetConstantC( 2 );
	operation2->SetConstantK( 0 );
	operation2->Update();

	outputImage = operation2->GetOutput();

	UpdateOutput( 0, outputImage, "Mask image", true, 1, GetInputDataEntity( 0 ) );

}

