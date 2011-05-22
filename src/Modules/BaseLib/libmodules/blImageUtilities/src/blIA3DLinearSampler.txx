/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Implementation file for the Independent Component Analysis.

#ifndef __blIA3DLinearSampler_txx
#define __blIA3DLinearSampler_txx


//#include "blIA3DLinearSampler.h"


/**
 *	Constructor
 */
template <class TImage>
blIA3DLinearSampler<TImage>::blIA3DLinearSampler()
{
	this->mSize = 0;
	this->mSpacing = 1.0;
	this->mGeometryType = Superclass::LINE;
//	this->mNormalizationType = Superclass::normZeroMeanUnitVar;
};

/**
 *	Destructor
 */
template <class TImage>
blIA3DLinearSampler<TImage>::~blIA3DLinearSampler() 
{
};

/**	\brief Get sampled values for current point in current image
 *
 *  Actually computes the sampling on the point and returns the results
 *
 *	\param vector vector with 2 * size + 1 elements where values are copied.
 */
template <class TImage>
int blIA3DLinearSampler<TImage>::NormalizeProfile(OutputVectorType & samples, typename blIA3DSampler<TImage>::NormalizationType normalizationType)
 {	
	
	switch (normalizationType)
	{
		case Superclass::normZeroMeanUnitVar: 
			{
				const double mean = samples.mean();
				vnl_vector<typename blIA3DSampler<TImage>::PixelType> pr = samples - mean;			
				const double var = pr.squared_magnitude()/(samples.size()-1); //unbiased
				if( var==0 && samples[0]!=0 ) samples.normalize();
				else if( var!=0 ) samples /= var;
			}
			break;
		case Superclass::normL1:
			{
				double norm = samples.one_norm();
				if (norm > 1E-12)
				samples /= norm;
			}
			break;
		default: 
			std::cerr << "Unknown profile normalization method" <<std::endl;
			throw "Unknown profile normalization method";

	}

	return EXIT_SUCCESS;
 }

/**	\brief Get sampled values for current point in current image
 *
 *  Actually computes the sampling on the point and returns the results
 *
 *	\param vector vector with 2 * size + 1 elements where values are copied.
 */
template <class TImage>
int blIA3DLinearSampler<TImage>::GetRawCumulatedProfile(OutputVectorType & profile)
 {	
	
	if (this->mImage.IsNull())
	{
		std::cerr << " blIA3DLinearSampler: image to sample is NULL " << std::endl;
		return EXIT_FAILURE;
	}

	int numberOfSamples = 2 * this->mSize + 1; // total number of samples

	if (numberOfSamples != profile.size())
	{
		std::cerr << " blIA3DLinearSampler: size mismatch. Please check input vector" << std::endl;
		return EXIT_FAILURE;
	}
	
	OutputVectorType aux_vec(2 * this->mSize + 1);
	this->GetRawProfile(aux_vec);
	
	for (int i = 0; i < numberOfSamples; i++)	
	{
		double pre_acc = 0.0;
		double post_acc = 0.0;
		for (int k = i; k < numberOfSamples; k++)	
		{
			post_acc += aux_vec[k];
		}
		for (int k = 0; k < i; k++)	
		{
			pre_acc += aux_vec[k];
		}
		if (post_acc != 0.0)
			profile[i] =  pre_acc/post_acc;		
		else
			profile[i] =  0.0;			
	}

	return EXIT_SUCCESS;
 }
 
/**	\brief Get sampled values for current point in current image
 *
 *  Actually computes the sampling on the point and returns the results
 *
 *	\param vector vector with 2 * size + 1 elements where values are copied.
 */
template <class TImage>
int blIA3DLinearSampler<TImage>::GetRawProfile(OutputVectorType & raw_samples)
 {	
	
	if (this->mImage.IsNull())
	{
		std::cerr << " blIA3DLinearSampler: image to sample is NULL " << std::endl;
		return EXIT_FAILURE;
	}

	unsigned numberOfSamples = 2 * this->mSize + 1; // total number of samples

	if (numberOfSamples != raw_samples.size())
	{
		std::cerr << " blIA3DLinearSampler: size mismatch. Please check input vector" << std::endl;
		return EXIT_FAILURE;
	}

	typename Superclass::IndexType originIndex;

	if (!this->mImage->TransformPhysicalPointToIndex(this->mOrigin, originIndex))
	{
		return EXIT_FAILURE;
	}

	int samplerIndex;
	for (samplerIndex = 0; samplerIndex < numberOfSamples; samplerIndex++)	
	{
		
		typename Superclass::PointType samplingPoint;
		typename Superclass::IndexType samplingIndex;

		samplingPoint[0] = this->mOrigin[0] + ( samplerIndex - (int) this->mSize ) * this->mNormal[0] * this->mSpacing;
		samplingPoint[1] = this->mOrigin[1] + ( samplerIndex - (int) this->mSize ) * this->mNormal[1] * this->mSpacing;
		samplingPoint[2] = this->mOrigin[2] + ( samplerIndex - (int) this->mSize ) * this->mNormal[2] * this->mSpacing;
					
		bool isInside = this->mImage->TransformPhysicalPointToIndex(samplingPoint, samplingIndex);

		if (isInside)
		{
			raw_samples[samplerIndex] = this->mImage->GetPixel(samplingIndex);
		}
		else
		{
			#ifdef DEBUG_MESSAGES_IALINEARSAMPLER
				std::cerr << "blIA3DLinearSampler: warning point is out of the image" << std::endl;
			#endif

			if (samplerIndex > 0)
			{
				raw_samples[samplerIndex] = raw_samples[samplerIndex - 1];
			}
			else
			{
				raw_samples[samplerIndex] = 0;
			}
		}
	}

	return EXIT_SUCCESS;
 }

/** \brief Get the gradient of a vector
 *
 *	\param gradient vector with size (2 * k + 1) elements
 */
//----------------------------------------------------
template <class TImage>
int blIA3DLinearSampler<TImage>::GetGradientProfile(OutputVectorType & gradient_samples)
{
	if (this->mImage.IsNull())
	{
		std::cerr << " blIA3DLinearSampler: input image is NULL " << std::endl;
		return EXIT_FAILURE;
	}

	typename Superclass::IndexType originIndex;

	if (!this->mImage->TransformPhysicalPointToIndex(this->mOrigin, originIndex))
	{
		return EXIT_FAILURE;
	}

	int numberOfSamples = 2 * this->mSize + 1;

	if (numberOfSamples != gradient_samples.size())
	{
		std::cerr << " blIA3DLinearSampler: size mismatch. Please check input vector" << std::endl;
		return EXIT_FAILURE;
	}
	
	this->mSize ++;	
	// increment the size to allow the gradient vector expand one extra position

	OutputVectorType aux_vec(2 * this->mSize + 1);
	this->GetRawProfile(aux_vec);

	this->mSize --;
	// restore the original size 
	
	// for each position, the gradient is the next minus the previous position
	// note: aux_vec has 2 more elements than gradient
			
	for (int i = 0; i < gradient_samples.size(); i++)										
	{
		gradient_samples[i] = (aux_vec[i+2] - aux_vec[i]);// / (2 * this->mSpacing);
	}

	return EXIT_SUCCESS;
}

/** \brief Get the gradient of a vector
 *
 *	\param gradient vector with size (2 * k + 1) elements
 */
//----------------------------------------------------
template <class TImage>
int blIA3DLinearSampler<TImage>::GetRawAndGradientProfiles(OutputVectorType & raw_samples, OutputVectorType & gradient_samples)
{
	if (this->mImage.IsNull())
	{
		std::cerr << " blIA3DLinearSampler: input image is NULL " << std::endl;
		return EXIT_FAILURE;
	}

	typename Superclass::IndexType originIndex;

	if (!this->mImage->TransformPhysicalPointToIndex(this->mOrigin, originIndex))
	{
		// point not accepted
		return EXIT_FAILURE;
	}

	unsigned numberOfSamples = 2 * this->mSize + 1;

	if (numberOfSamples != raw_samples.size())
	{
		std::cerr << " blIA3DLinearSampler: size mismatch. Please check input vector" << std::endl;
		return EXIT_FAILURE;
	}
	
	// increment the size to allow the gradient vector expand one extra position
	this->mSize ++;	

	OutputVectorType aux_vec(2 * this->mSize + 1);
	this->GetRawProfile(aux_vec);

	// restore the original size 
	this->mSize --;
	
	// for each position, the gradient is the next minus the previous position
	// note: aux_vec has 2 more elements than gradient
	
	for (unsigned i = 0; i < gradient_samples.size(); i++)										
	{
		gradient_samples[i] = (aux_vec[i+2] - aux_vec[i]);// / (2 * this->mSpacing);
		// fill the raw_samples vector
		raw_samples[i] =  aux_vec[i+1];
	}

	return EXIT_SUCCESS;
}

/** \brief Get coords of the i-th (signed) sample
 */
//----------------------------------------------------
template <class TImage>
typename blIA3DLinearSampler<TImage>::PointType
blIA3DLinearSampler<TImage>::GetIndexWorldCoordinates(int index)
//----------------------------------------------------
{
		typename Superclass::PointType point;
		point[0] = this->mOrigin[0] + index * this->mSpacing * this->mNormal[0];
		point[1] = this->mOrigin[1] + index * this->mSpacing * this->mNormal[1];
		point[2] = this->mOrigin[2] + index * this->mSpacing * this->mNormal[2];

		return point;
}

#endif
