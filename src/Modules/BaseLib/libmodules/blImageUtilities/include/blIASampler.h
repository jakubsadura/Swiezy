/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blIASampler_h
#define _blIASampler_h

// itk dependencies
//-----------------------------------------------
#include "itkImage.h"
#include <itkPoint.h>

#include <vnl/vnl_vector.txx>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_c_vector.txx>
#include <vnl/vnl_c_vector.h>


// own dependencies
//-----------------------------------------------
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


// defines
//-----------------------------------------------
//#define DEBUG_MESSAGES_IASAMPLER

//const unsigned int DIMENSION = 2;
#define DIMENSION 2

//-----------------------------------------------
// declaring the class

/**
 *	Namespace for definitions related to samplers but independent 
 *	from template parameters.
 */
namespace blIASamplerTypes
{
	/**\brief type of sampler */
	enum SamplerType {Line, Circle, Square, Triangle};

	typedef double CoordinateType;

	/**\brief type of point 2D */
	typedef itk::Point<CoordinateType, 2> PointType;
}



/** 
 * \brief Implements the generic class for a Sampler
 * \ingroup blImageUtilities
 *
 */
template <class TImage>
class blIASampler: public blLightObject
{

public:

	// typedefs 
	// ----------------------------------------------

	// Self
	typedef blIASampler Self;
	typedef blSmartPointer<Self> Pointer;

	// other
	typedef TImage ImageType;
	typedef typename ImageType::Pointer ImageTypePointer;
	typedef typename ImageType::PixelType PixelType;
	

	typedef blIASamplerTypes::CoordinateType CoordinateType;
	typedef blIASamplerTypes::PointType PointType;
	typedef vnl_vector<PixelType> OutputVectorType;


///	blNewMacro(Self);


	/** \brief enum type for identifying what kind of sampling do
	 *
	 */
	// Params to be defined for each type:
	// Line:	size	spacing
	// Square:	size	spacing
	// Circle:	size	spacing	angle
	// Triangle:ldmk1	ldmk2	ldmk3	 ??	 ??
///	enum SamplerType {Line, Circle, Square, Triangle};
	
	
	/** \brief Get current sampler size
	 *  
	 */
	int GetSize() const
		{ return this->size; }; 
	
	/** \brief Get current sampler spacing
	 *  
	 */
	double GetSpacing() const
		{ return this->spacing; };



	/** \brief Retrieves the type of Sampler to use
	 *
	 */
	virtual blIASamplerTypes::SamplerType GetType() const = 0;

	

	/** \brief Set image input 
	 *
	 */
	void SetInput(ImageTypePointer image)
	{ 
		this->image = image;
	};


	/** \brief Set Normal direction
	 *  
	 */
	void SetNormal(PointType point);


	/** \brief Set image input 
	 *  
	 */
	void SetParameters(int size, double spacing)
	{
		this->size = size;
		this->spacing = spacing;
	};



	/** \brief Set point to sample around 
	 *  
	 */
	void SetPoint(PointType point);


	/** \brief Set size value
	 *  
	 *	This sampler gets 2 * size + 1 values
	 *
	 *	\param size number of sampled values at each side of the origin point
	 */
	void SetSize(int size)
	{
		this->size = size;
	};


	/** \brief Set spacing value
	 *   
	 *	\param spacing distance between two consecutive sampled points
	 */
	void SetSpacing(double spacing)
	{
		this->spacing = spacing;
	};

	
protected:

	/** \brief Default Constructor */
	blIASampler();

	/** \brief Default Destructor */
	virtual ~blIASampler();

	
	ImageTypePointer image;		// current image to sample	


	int size;		//!< number of samples to be done on each side of the normal
	double spacing;	//!< distance between consecutive samples


	CoordinateType xPoint;
	CoordinateType yPoint;
	CoordinateType zPoint; // point to sample around

	CoordinateType xNormal;
	CoordinateType yNormal;
	CoordinateType zNormal;	
		// components of the Normal vector (not needed to have magnitude one)

private:	

	blIASampler(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

};

#include "blIASampler.txx"

#endif

