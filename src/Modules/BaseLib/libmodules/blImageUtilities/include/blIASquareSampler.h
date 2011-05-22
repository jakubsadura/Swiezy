/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIASquareSampler_h
#define __blIASquareSampler_h

//-----------------------------------------------
// Our files
#include "blIASampler.h"
#include "blIALinearSampler.h"	// use for samples parallels to the normal

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


//-----------------------------------------------
// std files
#include <math.h>				// Sqrt

//-----------------------------------------------
// ITK dependencies
#include <vnl/vnl_vector.h>
#include <itkPoint.h>

//-----------------------------------------------
/**
 * \brief A Square sampler
 * \ingroup blImageUtilities
 *
 * As a square sampler, it will return ( (2*size+1) * (2*size+1) ) pixel 
 * values.
 *
 */
template <class TImage>
class blIASquareSampler : public blIASampler<TImage>
//class blIASquareSampler : public blIASampler< blImage<unsigned char> >
{
public:

	typedef blIASquareSampler Self;
	typedef blSmartPointer<Self> Pointer;

//	typedef blImage<unsigned char> ImageType;
	typedef TImage ImageType;
	typedef blIASampler<ImageType> Superclass;
	typedef blIALinearSampler<ImageType> LinearSamplerType;

	typedef typename Superclass::PointType PointType;
	typedef typename Superclass::OutputVectorType OutputVectorType;

	blNewMacro(Self);

	///** \brief Get sampled values for current point in current image
	// *
	// *  Actually computes the sampling on the point
	// *  and returns the results
	// *
	// *	As a square sampler, it will return 
	// *  ( (2*size+1) * (2*size+1) ) pixel values.
	// */
	// OutputVectorType GetOutputForGivenPoint();	 


	///** \brief Get sampled values for current point in current image
	// *
	// *  Actually computes the sampling on the point
	// *  and returns the results
	// *
	// *	As a square sampler, it will return 
	// *  ( (2*size+1) * (2*size+1) ) pixel values.
	// */
	 void GetOutputForGivenPoint(OutputVectorType & samples);	

	 void SetSizeX(unsigned int sizex)
	 {
		 this->sizeX = sizex;
	 };

	 void SetSizeY(unsigned int sizey)
	 {
		 this->sizeY = sizey;
	 };
	
	/** \brief Get the gradient of a vector
	 *
	 *	Tooks the output from GetOutputForGivenPoint
	 *	and returns the gradient in the direction of the normal (VERIFY ME!!!!!)
	 *
	 *	\todo verify if the gradient has sense in the square sampler
	 */
	//----------------------------------------------------
	//vnl_vector<double> GetGradient(OutputVectorType vector) const;


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
	{ return blIASamplerTypes::Square;};

	
	/** \brief Get coords of the (i-th,j-th) sample being 0 the sampled landmark
	 *  
	 *	use the settled params. 
	 *  \param numberOfSample must be 0 for the original point
	 *	-k for a point k*spacing distanced over the normal (in reverse sense)
	 *	k for a point k*spacing distanced over the normal (in its sense)
	 *
	 *	\todo DIMENSION DEPENDENT CODE!
	 */
//	PointType GetCoordinatesOfSampleNumber(int numberOfSampleX, 
//											int numberOfSampleY, 
//											int numberOfSampleZ = 0);

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
	blIASquareSampler();

	/**\brief Destructor */
	virtual ~blIASquareSampler();


private:

	blIASquareSampler(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	unsigned int sizeX;
	unsigned int sizeY;

};

#include "blIASquareSampler.txx"

#endif


