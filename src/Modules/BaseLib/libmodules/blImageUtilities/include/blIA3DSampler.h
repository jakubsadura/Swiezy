/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _blIA3DSampler_h
#define _blIA3DSampler_h

// itk dependencies
//-----------------------------------------------
#include <itkPoint.h>

#include <vnl/vnl_vector.txx>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_c_vector.txx>
#include <vnl/vnl_c_vector.h>


// own dependencies
//-----------------------------------------------
//#include "blImage.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

// defines
//-----------------------------------------------
//#define DEBUG_MESSAGES_IASAMPLER

//-----------------------------------------------
// declaring the class

/**
 *	Namespace for definitions related to samplers but independent 
 *	from template parameters.
 */

/**
 * \brief Implements the generic class for a Sampler
 * \ingroup blImageUtilities
 *
 */
template <class TImage>
class blIA3DSampler: public blLightObject
{

public:

	// typedefs 
	// ----------------------------------------------

	// Self
	typedef blIA3DSampler Self;
	typedef blSmartPointer<Self> Pointer;

	// other
	typedef TImage ImageType;
	typedef typename ImageType::Pointer ImageTypePointer;
	typedef typename ImageType::PixelType PixelType;
	typedef typename ImageType::PointType PointType;
	typedef typename ImageType::IndexType IndexType;
	typedef typename ImageType::SpacingType SpacingType;


	typedef vnl_vector<PixelType> OutputVectorType;

	/**\brief type of sampler geometry */
	enum GeometryType {LINE, RECTANGLE} ;

	/**\brief type of profile normalization */
	enum NormalizationType { normZeroMeanUnitVar, normL1, normZeroMeanL1 }; 

	/** \brief Get current sampler size
	 *  
	 */
	int GetSize() const
	{ 
		return this->mSize; 
	}; 
	
	/** \brief Get current sampler spacing
	 *  
	 */
	double GetSpacing() const
	{ 
		return this->mSpacing; 
	};

	/** \brief Retrieves the type of sampler geometry to use
	 *
	 */
	virtual GeometryType GetGeometryType() const = 0;

	/** \brief Retrieves the type of normalization to use
	 *
	 */
//	virtual NormalizationType GetNormalizationType() const = 0;

	/** \brief Sets the type of sampler geometry to use
	 *
	 */
	void SetGeometryType(GeometryType geometryType);

	/** \brief Sets the type of normalization to use
	 *
	 */
//	void SetNormalizationType(NormalizationType normalizationType);
	
	/** \brief Set input image
	 *
	 */
	void SetInput(ImageTypePointer image)
	{ 
		this->mImage = image;
	};

	/** \brief Set sampler direction
	 *  
	 */
	void SetNormal(PointType normal)
	{
		this->mNormal = normal;
	};

	/** \brief Sets the point to sample around 
	 *  
	 */
	void SetOrigin(PointType origin)
	{
		this->mOrigin = origin;
	};

	/** \brief Set size value
	 *  
	 *	The sampler gets 2 * size + 1 values
	 *
	 *	\param size number of sampled values at each side of the origin point
	 */
	void SetSize(int size)
	{
		this->mSize = size;
	};

	/** \brief Set spacing value
	 *   
	 *	\param spacing distance between two consecutive sampled points
	 */
	void SetSpacing(double spacing)
	{
		this->mSpacing = spacing;
	};

protected:

	/** \brief Default Constructor */
	blIA3DSampler();

	/** \brief Default Destructor */
	virtual ~blIA3DSampler();

	ImageTypePointer mImage; // current image to sample	

	int mSize;		//!< number of samples on each side

	double mSpacing;
	PointType mOrigin;
	PointType mNormal;

	GeometryType mGeometryType;
	//NormalizationType mNormalizationType;
	
private:	

	blIA3DSampler(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

};

#include "blIA3DSampler.txx"

#endif

