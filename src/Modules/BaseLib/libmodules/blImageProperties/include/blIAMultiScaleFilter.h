/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAMultiScaleFilter_h
#define __blIAMultiScaleFilter_h

//#include "blImage.h"

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


/** 
 * \brief Wraps the itk::RecursiveMultiResolutionPyramidImageFilter
 * \ingroup blImageProperties
 *
 *	Takes a blImage as Input and returns a vector of blImages
 *	having each blImage different scale depending on the Schedule
 */
template <class inputImage, class outputImage>
class blIAMultiScaleFilter
{
public:

	typedef blIAMultiScaleFilter Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);


	/** \brief Setting Input
	 *
	 */
	void SetInput(inputImage *);

	/** \brief Setting Scale Schedule
	 *
	 */
	void SetSchedule(int *Schedule, int numberOfSchedules);
	
	/** \brief Getting Output corresponding to i-th scale
	 *
	 */
	inputImage GetOutput(int numberOfOutput);


protected:
	/** \brief Default Constructor
	 *
	 */
	blIAMultiScaleFilter();
	
	/** \brief Default Destructor
	 *
	 */
	virtual ~blIAMultiScaleFilter();

private:
	blIAMultiScaleFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

};

#endif
