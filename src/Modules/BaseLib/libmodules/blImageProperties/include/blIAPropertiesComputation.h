/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAPropertiesComputation_h
#define __blIAPropertiesComputation_h

#include "itkImage.h"

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"



/**
 * \brief Implements computation of every property available in the module
 * \ingroup blImageProperties
 *
 *	The advantage of being a separate class is that you can use the 
 *	propertiesComputation facilities with no need of instantiating the 
 *	whole Precomputation class.
 */
class blIAPropertiesComputation: public blLightObject
{
public:

	typedef blIAPropertiesComputation Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);

	typedef itk::Image<unsigned char, 2> ImageType;


	/** \brief Set input image 
	 *
	 */
	void SetInput(ImageType inputImage);

	/** \brief Compute Gradient of an Image
	 *
	 *  At a first sight we will use ITK Gradient utilities.
	 */
	ImageType ComputeGradient();
	
	/** \brief Compute Derivative on X
	 *
	 */
	ImageType ComputeDerivativeOnX();
	
	// ...
	// and a new method for each new property.


protected:

	/** \brief Default Constructor.
	 *
	 */
	blIAPropertiesComputation();

	/** \brief Default Destructor.
	 *
	 */
	~blIAPropertiesComputation();

private:

	blIAPropertiesComputation(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

};

#endif


