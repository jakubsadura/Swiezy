/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAMultiResolutionFilter_h
#define __blIAMultiResolutionFilter_h

#include <itkRecursiveMultiResolutionPyramidImageFilter.h>

//--------------------------------
// own dependencies

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"



/**
 * \brief Wraps the itk::RecursiveMultiResolutionPyramidImageFilter
 * \ingroup blImageProperties
 *
 *	Takes a blImage as Input and returns a Pyramid of blImages
 *	If blImage is multispectral probably we'll need to get a different
 *	pyramid for each image.
 */
template <class InputImageType, class OutputImageType>
class blIAMultiResolutionFilter: 
		public itk::RecursiveMultiResolutionPyramidImageFilter<
											InputImageType, OutputImageType>
{
public:

	typedef blIAMultiResolutionFilter Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);


	/** at first it won't give different new functionality
	 */

	/** \brief Setting Input
	 *
	 */
//	void SetInput(InputImageType *image);

	/** \brief Setting Scale Schedule
	 *
	 *  \param Schedule represents x,y,z resolution for each level.
	 */
//	void SetSchedule(int **Schedule, int numberOfLevels, int numberOfDimensions);
	
	/** \brief Getting Output corresponding to i-th scale
	 *
	 */
//	OutputImageType * GetOutput(int numberOfOutput);
	

protected:
	/** \brief Default Constructor
	 *
	 */
	blIAMultiResolutionFilter();
	
	/** \brief Default Destructor
	 *
	 */
	virtual ~blIAMultiResolutionFilter();

private:
	blIAMultiResolutionFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented


};

#include "blIAMultiResolutionFilter.txx"
#endif
