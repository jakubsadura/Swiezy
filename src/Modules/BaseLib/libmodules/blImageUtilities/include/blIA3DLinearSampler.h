/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIA3DLinearSampler_h
#define __blIA3DLinearSampler_h


// std dependencies
//-----------------------------------------------
#include <math.h>		// Sqrt


// ITK dependencies
//-----------------------------------------------
#include <itkPoint.h>
#include <vnl/vnl_vector.h>


// own dependencies
//-----------------------------------------------
#include "blIA3DSampler.h"

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


//#define FAST_GLP_NORMALIZATION 
// To switch between sum(abs)=1 and length=1 normalization

// defines
//-----------------------------------------------
//#define DEBUG_MESSAGES_IALINEARSAMPLER

/**
 * \brief This class defines a linear sampler that gets pixel values at some locations
 *	along a line.
 * \ingroup blImageUtilities
 *
 */
//-----------------------------------------------
template <class TImage>
class blIA3DLinearSampler : public blIA3DSampler<TImage>
{

public:

	// typedefs
	// ---------------------------------------------

	// Self
	typedef blIA3DLinearSampler Self;
	typedef blSmartPointer<Self> Pointer;

	typedef TImage ImageType;
	typedef blIA3DSampler<ImageType> Superclass;
	typedef typename Superclass::PointType PointType;

	typedef typename Superclass::OutputVectorType OutputVectorType;
	//typedef typename Superclass::NormalizationType NormalizationType;

	blNewMacro(Self);

	/**	\brief Normalizes the input vector with current normalization mode
	 *
	 *	\param vector vector with 2 * k + 1 elements as reference
	 */
	int NormalizeProfile(OutputVectorType & samples, typename blIA3DSampler<TImage>::NormalizationType normalizationType);

	/**	\brief Get raw and gradient sampled values for current point in current image
	 *
	 *	\param vector vector with 2 * k + 1 elements as reference
	 */
	 int GetRawAndGradientProfiles(OutputVectorType & raw_profile, OutputVectorType & gradient_profile);

	/**	\brief Get sampled values for current point in current image
	 *
	 *	\param vector vector with 2 * k + 1 elements as reference
	 */
	 int GetRawProfile(OutputVectorType & samples);

	 int GetRawCumulatedProfile(OutputVectorType & samples);
	
	/** \brief Get the gradient of a vector with sampled values
	 *
	 *	This method takes 2 * (size + 1) + 1 values from the image and 
	 *	calculates gradient generating 2 * k + 1 values.
	 *	
	 *	\param gradient vector with size (2 * k + 1) elements
	 */
	//----------------------------------------------------
	int GetGradientProfile(OutputVectorType & gradient);

	/**	\brief Return the sampler type of this sampler
	 *
	 */
	virtual typename Superclass::GeometryType GetGeometryType() const
	{ 
		return this->mGeometryType;
	};

	///**	\brief Return the sampler type of this sampler
	// *
	// */
	//virtual Superclass::NormalizationType GetNormalizationType() const
	//{ 
	//	return this->mNormalizationType;
	//};

	/**	\brief sets the sampler geometry of the sampler
	 *
	 */
	virtual void SetGeometryType(typename Superclass::GeometryType geometryType) 
	{ 
		this->mGeometryType = geometryType;
	};

	/**	\brief sets the normalization type of the sampler
	 *
	 */
	//virtual void GetNormalizationType(Superclass::NormalizationType normalizationType) 
	//{ 
	//	this->mNormalizationType = normalizationType;
	//};
	
	/** \brief Get coords of the i-th sample being 0 the sampled landmark
	 *  
	 *	use the settled params. 
	 *  \param numberOfSample must be 0 for the original point
	 *	-k for a point k*spacing distanced over the normal (in reverse sense)
	 *	k for a point k*spacing distanced over the normal (in its sense)
	 */
	PointType GetIndexWorldCoordinates(int index);
	
protected:

	/**\brief Constructor */
	blIA3DLinearSampler();

	/**\brief Destructor */
	virtual ~blIA3DLinearSampler();

private:

	blIA3DLinearSampler(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented


};

#include "blIA3DLinearSampler.txx"

#endif


