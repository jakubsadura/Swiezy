/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Implementation file for the Independent Component Analysis.

#ifndef __blIAROI_txx
#define __blIAROI_txx

//#include "blIAROI.h"

# define DIMENSION 2


/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
blIAROI<TInputImage, TOutputImage>::blIAROI() 
{
}



/**
 *	Destructor
 */		
template <class TInputImage, class TOutputImage>
blIAROI<TInputImage, TOutputImage>::~blIAROI() 
{
}




/** \brief (update and) get the image belonging to the ROI. The rest is discarded
 *
 *	Region will always have squared shape
 */
//----------------------------------------------------------
template <class TInputImage, class TOutputImage>
TOutputImage * blIAROI<TInputImage, TOutputImage>::GetOutput()
//----------------------------------------------------------
{
	typename TOutputImage::RegionType desiredRegion; 
	desiredRegion.SetSize( size );
	desiredRegion.SetIndex( start );

	this->SetRegionOfInterest( desiredRegion );

	return this->Superclass::GetOutput();	
}


/** \brief Set region size (real coordinates)
 *
 *	Region will always have squared shape
 */
//----------------------------------------------------------
template <class TInputImage, class TOutputImage>
void blIAROI<TInputImage, TOutputImage>::SetRegionSize(float * size)
//----------------------------------------------------------
{
	int i;
	
	for (i = 0; i < DIMENSION; i++)
	{
		this->size[i] = size[i];
	}
}


/** \brief Set region start
 *
 *	("low-left" corner, real coordinates)
 */
//----------------------------------------------------------
template <class TInputImage, class TOutputImage>
void blIAROI<TInputImage, TOutputImage>::SetRegionStart(float * start)
//----------------------------------------------------------
{
	int i;

	for (i = 0; i < DIMENSION; i++)
	{
		this->start[i] = start[i];
	}
}


/** \brief Set image where ROI will be applied
 *
 *	Region will always have squared shape
 */
//----------------------------------------------------------
template <class TInputImage, class TOutputImage>
void blIAROI<TInputImage, TOutputImage>::SetInput(TInputImage * image)
//----------------------------------------------------------
{
	this->Superclass::SetInput( image );
}


#endif
