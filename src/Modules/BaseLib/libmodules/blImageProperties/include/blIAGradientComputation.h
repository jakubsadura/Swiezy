/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAGradientComputation_h
#define __blIAGradientComputation_h

// itk dependencies
//------------------------------------------
#include <itkGradientMagnitudeImageFilter.h>

// own dependencies
//------------------------------------------
///#include "blIADefines.h"

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


/** 
 * \brief This class implements computation of all features of IA required 
 *	on a given Hierarchy.
 * \ingroup blImageProperties
 *
 */
template <typename InputImageType, typename OutputImageType>
class blIAGradientComputation: 
	public itk::GradientMagnitudeImageFilter<InputImageType, OutputImageType>
{
	
	public:
		typedef blIAGradientComputation Self;
		typedef blSmartPointer<Self> Pointer;

		blNewMacro(Self);

	protected:
		blIAGradientComputation();
		virtual ~blIAGradientComputation();

	private:
		blIAGradientComputation(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented

};

#include "blIAGradientComputation.txx"

#endif


