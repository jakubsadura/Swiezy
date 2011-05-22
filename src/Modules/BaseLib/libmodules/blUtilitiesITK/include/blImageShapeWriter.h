/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _blImageShapeWriter_h
#define _blImageShapeWriter_h

// own dependencies
//-------------------------------------------
#include "blPDShapeReader.h"
#include "blPDShape.h"

#include "itkImage.h"

// itk dependencies
//-------------------------------------------
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkVTKImageIO.h>
 
#include <iostream>

/**
 * \brief Writes a shape on an image
 * \ingroup blUtilities
 *
 * Set filenames for shape and input image. Update and WriteOutput to the desired file.
 */
class blImageShapeWriter
{
	public:
	
	/** \brief default constructor
	 *
	 */
	blImageShapeWriter();
	
	/** \brief default destructor
	 *
	 */
	~blImageShapeWriter();
	
	/** \brief set file where get the image
	 *
	 */
	void SetImageFilename(char *filename);

	/** \brief directly set the image
	 *
	 */
	void SetImage(itk::Image<unsigned char>::Pointer theImage)
	{	this->theImage = theImage;}

	/** \brief set file where get the shape
	 *
	 */
	void SetShapeFilename(char *filename);

	/** \brief directly set shape to be used
	 *
	 */
	void SetShape(blPDShapeInterface::Pointer theShape)
	{ this->theShape = theShape;};


	/** \brief set file for writting the output
	 *
	 */
	void WriteOutput(char *filename);
	
	/** \brief actually do the computations and writting
	 *
	 */
	void Update();

private:
		
	/** \brief storing the shape
	 *
	 */
	blPDShapeInterface::Pointer theShape;

	/** \brief storing the image
	 *
	 */
	itk::Image<unsigned char>::Pointer theImage;
		
};

#endif
