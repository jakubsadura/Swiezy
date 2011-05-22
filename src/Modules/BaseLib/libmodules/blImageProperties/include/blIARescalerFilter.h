/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef __blIARescalerFilter_h
#define __blIARescalerFilter_h

#include <itkRescaleIntensityImageFilter.h>

//--------------------------------
// own dependencies

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


/**
 * \brief Wraps the itk::RescaleIntensityImageFilter filter
 * \ingroup blImageProperties
 *
 */
template <class InputImageType, class OutputImageType>
class blIARescalerFilter: 
		public itk::RescaleIntensityImageFilter<
											InputImageType, OutputImageType>
{
public:

	typedef blIARescalerFilter Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);
	

protected:
	/** \brief Default Constructor
	 *
	 */
	blIARescalerFilter();
	
	/** \brief Default Destructor
	 *
	 */
	virtual ~blIARescalerFilter();

private:
	blIARescalerFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented


};

#include "blIARescalerFilter.txx"
#endif
