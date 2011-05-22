// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blPolyDataDistanceTransformFilterTest.h"
#include "blPolyDataDistanceTransformFilter.h"


//ITK
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"

//VTK
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkVoxelModeller.h>
#include <vtkPolyDataNormals.h>
#include <vtkCleanPolyData.h>
#include <vtkImageWriter.h>
#include <vtkSystemIncludes.h>
#include <vtkPolyDataWriter.h>

// NOT WORKING!

//int blPolyDataDistanceTransformFilterTest( char *input_name, char *output_name, double spacing[3], double offset[3], bool sign )
//void blPolyDataDistanceTransformFilterTest::TestPolyDataDistanceTransformFilter()
//	{
//		unsigned int idim[3];
//		char *input_name;
//		char *output_name;
//		double spacing[3]; 
//		double offset[3];
//		bool sign;
//
//		vtkPolyDataReader *reader = vtkPolyDataReader::New();
//		reader->SetFileName(input_name);
//
//		try
//		{
//			reader->Update();
//		}
//		catch ( vtkstd::exception &err )
//		{
//			std::cout << "ExceptionObject caught !" << std::endl; 
//			std::cout << err.what() << std::endl; 
//			return;
//		}
//
//		vtkPolyData *surf = reader->GetOutput();
//
//		///////////////////////////////////////////////
//		vtkCleanPolyData *cleaner;
//		vtkPolyDataNormals *normals;
//
//		if ( sign )
//		{
//  			normals = vtkPolyDataNormals::New();
//			normals->SetInput( surf );  
//			//normals->FlipNormalsOff();
//			normals->ComputePointNormalsOn();
//			normals->ComputeCellNormalsOn();
//			normals->ConsistencyOn();
//			normals->AutoOrientNormalsOn();
//			normals->Update();
//
//			double clean = 1.0e-10;
//			double abs   = 1.0e-10;
//			cleaner = vtkCleanPolyData::New();
//			cleaner->SetInput( normals->GetOutput() );
//			cleaner->SetTolerance( clean );
//			cleaner->SetAbsoluteTolerance( abs );
//			cleaner->Update();
//
//			surf = cleaner->GetOutput();
//		}
//		///////////////////////////////////////////////
//
//		vtkPolyDataWriter *polyWriter = vtkPolyDataWriter::New();
//		polyWriter->SetInput( surf );
//		polyWriter->SetFileName( "surfkk.vtk" );
//		polyWriter->Update();
//
//
//		blPolyDataDistanceTransformFilter::Pointer dtFilter = blPolyDataDistanceTransformFilter::New();
//		dtFilter->SetInput( surf );
//		dtFilter->SetSpacing( spacing );
//		dtFilter->SetOffset( offset );
//		dtFilter->SetSigned( sign );
//		dtFilter->Update();
//
//		std::cout << dtFilter->GetComputationTime() << " secons to compute." << std::endl;
//
//		/*
//		vtkImageWriter *writer = vtkImageWriter::New();
//		writer->SetFileName( output_name );
//		writer->SetInput( image );
//		writer->SetFileDimensionality( 3 );
//		writer->Write();
//		*/
//		////////////////////////////////////////////////////////////////////////////////
//		///// Conversor ImageData -> ITK
//		////////////////////////////////////////////////////////////////////////////////
//
//		//Creation of the distance image
//
//		typedef float PixelType;
//		typedef itk::Image< PixelType, 3 >  ImageType;
//		typedef itk::ImageRegionIterator< ImageType> IteratorType;
//		typedef itk::ImageFileWriter< ImageType > WriterType;
//
//		ImageType::Pointer distance = ImageType::New();
//
//		//Associated region
//
//		ImageType::RegionType region;
//		ImageType::IndexType startR;
//		startR[0]=0;
//		startR[1]=0;
//		startR[2]=0;
//
//		ImageType::SizeType sizeR;
//		dtFilter->GetDimensions( idim );
//		//  dtFilter->GetOutput()->GetExtent( x1, x2, y1, y2, z1, z2 );
//		sizeR[0]=idim[0];
//		sizeR[1]=idim[1];
//		sizeR[2]=idim[2];
//
//		region.SetSize( sizeR );
//		region.SetIndex( startR );
//
//		//Allocating distance image
//		distance->SetRegions( region );
//		distance->SetSpacing( dtFilter->GetSpacing() );
//		distance->SetOrigin( dtFilter->GetOutput()->GetOrigin() );
//		distance->Allocate();
//
//		//Creation of image iterator
//
//		IteratorType it( distance, region );
//		double *ptr = (double *) dtFilter->GetOutput()->GetScalarPointer();
//
//		for( it.GoToBegin(); !it.IsAtEnd(); ++it  )
//		{
//			it.Set( *ptr );
//			ptr++;
//		}
//
//		WriterType::Pointer writer = WriterType::New();
//		writer->SetFileName( output_name );
//		writer->SetInput( distance );
//
//		try
//		{
//			writer->Update();
//		}
//		catch ( itk::ExceptionObject &err)
//		{
//			std::cerr << "ExceptionObject caught !" << std::endl; 
//			std::cerr << err << std::endl; 
//			return ;   
//		}
//
//		return;
//	}
