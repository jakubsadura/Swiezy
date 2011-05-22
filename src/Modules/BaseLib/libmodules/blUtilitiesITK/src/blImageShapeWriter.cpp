/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

/**
\brief   This file reads a shape, an image and prints image on the shape.
\ingroup agonzalez
\author  
\date    
*/

// own dependencies
//-------------------------------------------
#include "blImageShapeWriter.h"

#include "blImageReader.h"
#include "blImageWriter.h"

// some types
//-------------------------------------------
typedef itk::Image<unsigned char, 2> ImageType;
typedef ImageType::Pointer ImageTypePointer;

typedef itk::Image<double, 2> DoubleImageType;

typedef blImageReader<ImageType> ImageReaderType;
typedef blImageWriter<ImageType> ImageWriterType;


/** \brief default constructor
*
*/
//--------------------------------------------------
blImageShapeWriter::blImageShapeWriter()
//--------------------------------------------------
{
	;
}

/** \brief default destructor
*
*/
//--------------------------------------------------
blImageShapeWriter::~blImageShapeWriter()
//--------------------------------------------------
{
	;
}

/** \brief set file where get the image
*
*	It actually reads the image and stores in the attribute
*/
//--------------------------------------------------
void blImageShapeWriter::SetImageFilename(char *filename)
//--------------------------------------------------
{
	// Reading image
	ImageReaderType::Pointer reader = ImageReaderType::New();

	reader->SetFileName( filename  );
	reader->Update();
	ImageType::Pointer theImage = ImageType::New();
	this->theImage = reader->GetOutput();
}

/** \brief set file where get the shape
*
*	It actually reads the shape and stores it in the attribute
*/
//--------------------------------------------------
void blImageShapeWriter::SetShapeFilename(char *filename)
//--------------------------------------------------
{
	// Reading shape
	blPDShapeReader::Pointer shapeReader = blPDShapeReader::New(); 
	shapeReader->SetFilename(filename);
	shapeReader->Update(); 

	this->theShape = shapeReader->GetOutput();
}

/** \brief set file for writting the output
*
*	writes the output into the filename
*/
//--------------------------------------------------
void blImageShapeWriter::WriteOutput(char *filename)
//--------------------------------------------------
{
	// Writing image
	ImageWriterType::Pointer writer = ImageWriterType::New();
	writer->SetFileName( filename );
	writer->SetInput( this->theImage );

	try 
	{ 
		writer->Update();
	} 
	catch( itk::ExceptionObject & err ) 
	{ 
		std::cout << "ExceptionObject caught !" << std::endl; 
		std::cout << err << std::endl; 
	}
}

/** \brief actually do the computations and writting
*
*/
//--------------------------------------------------
void blImageShapeWriter::Update()
//--------------------------------------------------
{
	unsigned int i,j;

	// Modifying image according to the shape
	// -1 as last point has no line to be plotted
	for (i = 0; i < this->theShape->GetNumberOfLandmarks()-1; i++) 
	{		
		ImageType::IndexType startingPoint;
		ImageType::IndexType endingPoint;

		double pointAux[2]; // auxiliar
		unsigned int landmark[1];

		// setting point #1
		landmark[0] = i;
		theShape->GetPoints(1, landmark, pointAux);
		startingPoint[0] = (long int)pointAux[0];	// conversion between coords and images
		startingPoint[1] = (long int)pointAux[1];

		// setting point #2
		landmark[0] = i + 1;
		theShape->GetPoints(1, landmark, pointAux);
		endingPoint[0] = (long int)pointAux[0];	// conversion between coords and images
		endingPoint[1] = (long int)pointAux[1];

		// Getting spacing (which is the pixel size)
		const ImageType::SpacingType & spacing = theImage->GetSpacing();

		// this difference will guide the drawing
		double differenceOnX = endingPoint[0] - startingPoint[0];
		double differenceOnY = endingPoint[1] - startingPoint[1];

		// difference in pixels
		int pixelDifferenceOnX = (int) (differenceOnX / spacing[0]);
		int pixelDifferenceOnY = (int) (differenceOnY / spacing[1]);

		// number of pixels to plot in, and increase (in real coords) for plotting
		//		        *o
		//           ***
		//        ***
		//		o*
		unsigned int pixelsToPlot;
		if (abs(pixelDifferenceOnX) > abs(pixelDifferenceOnY))
		{
			pixelsToPlot = abs(pixelDifferenceOnX);
		}
		else
		{
			pixelsToPlot = abs(pixelDifferenceOnY);
		}

		double increaseOnX = differenceOnX / pixelsToPlot;
		double increaseOnY = differenceOnY / pixelsToPlot;

		// --------------------------------------------
		// Points and Index definitions
		double xToPaint;	// X real coordinate to plot
		double yToPaint;	// Y real coordinate to plot
		ImageType::IndexType pixelIndex;
		itk::Point<double,2> point;

		// --------------------------------------------
		// Actually drawing the line
		for (j = 0; j < pixelsToPlot; j++)
		{
			// Computing where to sample
			// --------------------------------------------
			xToPaint = startingPoint[0] + j * increaseOnX;
			yToPaint = startingPoint[1] + j * increaseOnY;

			// Getting Pixeles from coordinates
			// --------------------------------------------
			point[0] = xToPaint;
			point[1] = yToPaint;

			// Transforming index
			// --------------------------------------------
			bool isInside;
			isInside = theImage->TransformPhysicalPointToIndex(point, pixelIndex);

			// transformation relies on the image creator
			//	who has to have put good values on Origin and Spacing.
			if (isInside)
				theImage->SetPixel(pixelIndex, 0/*.0*/);
			else
				std::cout << "blIALinearSampler: point is out of the image" << std::endl;
		}
	}
}
