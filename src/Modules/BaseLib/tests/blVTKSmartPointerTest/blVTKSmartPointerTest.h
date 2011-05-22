// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#pragma once

#include "cxxtest/TestSuite.h"

// VTK
#include "vtkStructuredPointsWriter.h"

// ITK
#include "itkImageToVTKImageFilter.h"
#include "itkImage.h"


// CLASS DEFINITION

/**
\brief Tests for itk::ImageToVTKImageFilter
\ingroup BaseLibVTKTest
\author Xavi Planes
\date 16-05-08
*/
class blVTKSmartPointerTest : public CxxTest::TestSuite 
{
// TYPEDEFS
typedef itk::Image< float, 3 > ImageType;


// OPERATIONS
public:

	/**
	\brief This test gives some VTK error messages "Unknown exception."
	
	ImageToVTKImageFilter has one itk::VTKImageExport and one vtkImageImport.

	- ImageToVTKImageFilter is constructed: vtkImageImport is 
	initialized with some references to itk::VTKImageExport function 
	callbacks and a pointer to the instance.
	  - The function vtkImageImport::UpdateInformationCallback( ) 
	  - The pointer to the instance (vtkImageImport::CallbackUserData)
	- ImageToVTKImageFilter is destroyed, itk::VTKImageExport is destroyed
	but the references inside vtkImageImport are still there
	- vtkStructuredPointsWriter is constructed and when Update( ) is called
	UpdateInformationCallback( ) of itk::VTKImageExport object is called
	in the function vtkImageImport::InvokeUpdateInformationCallbacks().
	This throws an exception because the memory of the itk::VTKImageExport
	instance was deleted in step 2.
	*/
	void test0( void )
	{
		return;

		// Create an itk::Image
		ImageType::Pointer inputImage;
		inputImage = ImageType::New( );

		//------------------------------------
		// Step 1: Convert to vtkImageData
		itk::ImageToVTKImageFilter<ImageType>::Pointer imageAdaptor;
		imageAdaptor = itk::ImageToVTKImageFilter<ImageType>::New();
		imageAdaptor->SetInput( inputImage );
		imageAdaptor->Update();

		// Decrease itk itkImage reference count
		inputImage = NULL;

		// Get the output vtkImageData 
		vtkImageData* vtkImageOutput = imageAdaptor->GetOutput( );
		vtkImageOutput->Register( NULL );

		//------------------------------------
		// Step 2: Destroy itk filter
		imageAdaptor = NULL;

		//------------------------------------
		// Step 3: Save Vtk image to disk
		vtkStructuredPointsWriter* writer = vtkStructuredPointsWriter::New();
		writer->SetFileName( "a.vtk" );
		writer->SetInput( vtkImageOutput );//pImageData->get( ));
		writer->Update();
		writer->Delete();

		// Delete vtkImageData
		vtkImageOutput->UnRegister( NULL );

		getchar( );
	}


	
	/**
	\brief This is the same test0 but, the output of the filter is
	not reused. A new vtkImageData is created and the output of the
	filter is copied.
	*/
	void test1( void )
	{

		// Create an itk::Image
		ImageType::Pointer inputImage;
		inputImage = ImageType::New( );

		// Convert to vtkImageData
		itk::ImageToVTKImageFilter<ImageType>::Pointer imageAdaptor;
		imageAdaptor = itk::ImageToVTKImageFilter<ImageType>::New();
		imageAdaptor->SetInput( inputImage );
		imageAdaptor->Update();

		// Decrease itk itkImage reference count
		inputImage = NULL;

		// Create a vtkImageData using a vtkSmartPointer
		vtkImageData* vtkImageOutput = imageAdaptor->GetOutput( );
		vtkSmartPointer<vtkImageData> pImageData;

		// Reference counting: 1
		vtkImageData* vtkImageNew = vtkImageData::New( );

		// We call the constructor with false because we don't want 
		// to increase the reference counting
		pImageData = vtkSmartPointer<vtkImageData>::New( );
		pImageData.TakeReference( vtkImageNew );

		// Reference counting is increased by 2
		pImageData->DeepCopy( vtkImageOutput );

		// Destroy itk filter
		imageAdaptor = NULL;

		// Save Vtk image to disk
		vtkStructuredPointsWriter* writer = vtkStructuredPointsWriter::New();
		writer->SetFileName( "a.vtk" );
		writer->SetInput( pImageData->get( ));
		writer->Update();
		writer->Delete();

		// Delete vtkImageData
		delete pImageData;
	}





// ATTRIBUTES
protected:

};



