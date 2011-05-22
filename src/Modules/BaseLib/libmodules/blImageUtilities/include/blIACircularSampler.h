/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _blIACircularSampler_h
#define _blIACircularSampler_h

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
#include <itkPoint.h>
#include <vnl/vnl_vector.h>


// constants
// ----------------------------------------------
const double PI = 3.142592;
//const unsigned int DIMENSION = 2;



//-----------------------------------------------
/** 
 * \brief This class implements the Independent Component Analysis algorithm
 * \ingroup blImageUtilities
 *
 *  Defines the ICA (execute) and other utility methods for the class
 */
class blIACircularSampler : public blIASampler<itk::Image<unsigned char> > 
{
public:

	typedef blIACircularSampler Self;
	typedef blSmartPointer<Self> Pointer;

	typedef itk::Image<unsigned char> ImageType;
	typedef blIASampler<ImageType> Superclass;
	typedef blIALinearSampler<ImageType> LinearSamplerType;
	typedef LinearSamplerType::PointType PointType;
	typedef Superclass::OutputVectorType OutputVectorType;

	blNewMacro(Self);


	/** \brief Get sampled values for current point in current image
	 *
	 *  Actually computes the sampling on the point
	 *  and returns the results
	 *
	 *	As a Circular sampler, it will return 
	 *  ( 2*(size+1) ) * (360/alpha) pixel values.
	 */
	 OutputVectorType GetOutputForGivenPoint();	 
	

	/** \brief Get the gradient of a vector
	 *
	 *	Tooks the output from GetOutputForGivenPoint
	 *	and returns the gradient in the direction of each radius (VERIFY ME!!!!)
	 *
	 *	\todo verify if the gradient has sense in the Circular sampler
	 */
	//----------------------------------------------------
	vnl_vector<double> GetGradient(OutputVectorType vector) const;


	/** \brief Get angle between consecutive radius (in degrees)
	 * 
	 *	\returns degrees between consecutive radius
	 */
	double GetAngleInDegrees() const
	{
		return this->alpha * 180.0 / PI;	
		// RETURNING IN DEGREES (but stored and used in radians)
	};
	
	/** \brief Get angle between consecutive radius (in radians)
	 *  
	 *	\returns degrees between consecutive radius
	 */
	double GetAngleInRadians() const
	{
		return this->alpha;	
	};



	/**	\brief Return the type of this sampler
	 *
	 */
	virtual blIASamplerTypes::SamplerType GetType() const
	{ return blIASamplerTypes::Circle;};

	
	/** \brief Get coords of the (i-th,j-th) sample being 0 the sampled landmark
	 *  
	 *	use the settled params. 
	 *  \param numberOfSample must be 0 for the original point
	 *	-k for a point k*spacing distanced over the normal (in reverse sense)
	 *	k for a point k*spacing distanced over the normal (in its sense)
	 *
	 *	\todo DIMENSION DEPENDENT CODE!
	 */
	PointType GetCoordinatesOfSampleNumber(int numberOfSampleX, 
											int numberOfSampleY, 
											int numberOfSampleZ = 0);


	
	/** \brief Set image input 
	 *  
	 *	\param size number of samples in each radius
	 *	\param spacing distance in World Coordinates between consecutive samples
	 *	\param alpha degrees between consecutive radius (0.0,..,2*PI)
	 */
	void SetParameters(int size, double spacing, double alpha);
	
	/** \brief Set point to sample around 
	 *  
	 */
///	void SetPoint(PointType point);

protected:

	/**\brief Constructor */
	blIACircularSampler();

	/**\brief Destructor */
	virtual ~blIACircularSampler();


private:

	blIACircularSampler(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	double alpha;	// set/get in degrees but internally used and stored in radians


};

#endif


