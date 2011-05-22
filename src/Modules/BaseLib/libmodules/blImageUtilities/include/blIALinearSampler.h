/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef __blIALinearSampler_h
#define __blIALinearSampler_h


// std dependencies
//-----------------------------------------------
#include <math.h>		// Sqrt


// ITK dependencies
//-----------------------------------------------
#include <itkPoint.h>
#include <vnl/vnl_vector.h>


// own dependencies
//-----------------------------------------------
#include "blIASampler.h"

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


#define FAST_GLP_NORMALIZATION 
// Tu switch between sum(abs)=1 and length=1 normalization

// defines
//-----------------------------------------------
//#define DEBUG_MESSAGES_IALINEARSAMPLER

/**
 * \brief This class defines a linear sampler that gets pixel values at 
 *	some locations along a line.
 * \ingroup blImageUtilities
 *
 */
//-----------------------------------------------
template <class TImage>
class blIALinearSampler : public blIASampler<TImage>
{

public:

	// typedefs
	// ---------------------------------------------

	// Self
	typedef blIALinearSampler Self;
	typedef blSmartPointer<Self> Pointer;

	typedef TImage ImageType;
	typedef blIASampler<ImageType> Superclass;
	
	typedef typename Superclass::PointType PointType;
	typedef typename Superclass::OutputVectorType OutputVectorType;

	blNewMacro(Self);



	/**	\brief Get sampled values for current point in current image
	 *
	 *  Actually computes the sampling on the point and returns the results
	 *
	 *	\param vector vector with 2 * k + 1 elements where values are copied.
	 */
	 void GetOutputForGivenPoint(OutputVectorType & samples);	 

	
	/** \brief Get the gradien of a vector with sampled values
	 *
	 *	This method takes 2 * (size + 1) + 1 values from the image and 
	 *	calculates gradient generating 2 * k + 1 values.
	 *	
	 *	\param gradient vector with size (2 * k + 1) elements
	 */
	//----------------------------------------------------
	void GetGradient(vnl_vector<double> & gradient);

	/**	\brief Return the type of this sampler
	 *
	 */
	virtual blIASamplerTypes::SamplerType GetType() const
	{ return blIASamplerTypes::Line;};

	
	/** \brief Get coords of the i-th sample being 0 the sampled landmark
	 *  
	 *	use the settled params. 
	 *  \param numberOfSample must be 0 for the original point
	 *	-k for a point k*spacing distanced over the normal (in reverse sense)
	 *	k for a point k*spacing distanced over the normal (in its sense)
	 */
	PointType GetCoordinatesOfSampleNumber(int numberOfSample);
	


protected:

	/**\brief Constructor */
	blIALinearSampler();

	/**\brief Destructor */
	virtual ~blIALinearSampler();

private:

	blIALinearSampler(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented


};

#include "blIALinearSampler.txx"

#endif


