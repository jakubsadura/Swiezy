/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Implementation file for the Independent Component Analysis.
 
#ifndef __blIALinearSampler_txx
#define __blIALinearSampler_txx


//#include "blIALinearSampler.h"

////////////////////////////////////////////////////////
//
//	20031204: checked only with uchar o scalar short vtk images
//
////////////////////////////////////////////////////////

/**
 *	Constructor
 */
template <class TImage>
blIALinearSampler<TImage>::blIALinearSampler()
{
	// default values
	this->size = 0;
	this->spacing = 1;
	
};

/**
 *	Destructor
 */
template <class TImage>
blIALinearSampler<TImage>::~blIALinearSampler() 
{
};




/**	\brief Get sampled values for current point in current image
 *
 *  Actually computes the sampling on the point and returns the results
 *
 *	\param vector vector with 2 * size + 1 elements where values are copied.
 */
template <class TImage>
 void blIALinearSampler<TImage>::GetOutputForGivenPoint(OutputVectorType & samples)
 {
	// --------------------------------------------
	// General declarations
	int dimension = 2;
	int i;

	int numberOfSamples = 2 * this->size + 1; // number of samples to be done
	double xOrigin, yOrigin, zOrigin = 0.0;
	double xToSample, yToSample, zToSample = 0.0;
	

	// --------------------------------------------
	// Points and Index definitions
	typename ImageType::IndexType pixelIndex;
	
	// --------------------------------------------
	// Setting origin Point coordinates
	xOrigin = this->xPoint;
	yOrigin = this->yPoint;
	if (dimension == 3)
		zOrigin = this->zPoint;

	// --------------------------------------------
	// Getting Normal Vector
	double xNormal = this->xNormal;
	double yNormal = this->yNormal;
//	if (dimension == 3)
//		double zNormal = this->zNormal;

	// --------------------------------------------
	// DIMENSION DEPENDING CODE.
	if (dimension == 2)	// we can use here the GetCoordinatesOfLandmarkNumber() but it's more unneficient
	{
		// we sample the following:
		//
		// the landmark (xOrigin, yOrigin)
		//
		// size points before (-size, ..., -1)
		//
		// size points after (1, ..., size)
		//
		// first point (0) will be 
		//		xOrigin - k * xSpacing
		//		yOrigin - k * ySpacing
		//
		// i-th point will be
		//		xOrigin - (k - i) * xSpacing
		//		yOrigin - (k - i) * ySpacing
		// so if i == k, then we are sampling the landmark itself
		// and if i == 2k (we'd sampled 2k+1 points) 
		//	we are sampling on xOr + k*xSp, yOr +  k*ySp
		

		//--------------------------------------------
		// quick and dirty normalization of the normal vector

		double magnitude = sqrt(xNormal*xNormal+yNormal*yNormal);
		
		xNormal /= magnitude;
		yNormal /= magnitude;

		double xSpacing, ySpacing;
		xSpacing = this->spacing * xNormal;
		ySpacing = this->spacing * yNormal;


		PointType point;
		for (i = 0; i < numberOfSamples; i++)	
		{
			// Computing where to sample
			// --------------------------------------------
			xToSample = xOrigin + (i - this->size) * xSpacing;
			yToSample = yOrigin + (i - this->size) * ySpacing;
			
			// Getting Pixeles from coordinates
			// --------------------------------------------
			point[0] = xToSample;
			point[1] = yToSample;
			
			// Transforming index
			// --------------------------------------------
			bool isInside = this->image->TransformPhysicalPointToIndex(point, pixelIndex);
			
				// transformation relies on the image creator
				//	who has to have put good values on Origin and Spacing.
			if (isInside)
			{
				samples[i] = this->image->GetPixel(pixelIndex);
			}
			else
			{
				#ifdef DEBUG_MESSAGES_IALINEARSAMPLER
					std::cerr << "blIALinearSampler: point is out of the image" << std::endl;
				#endif

				if (i > 0)
				{
					samples[i] = samples[i - 1];
				}
				else
				{
					samples[i] = 255;
				}
			}
		}
	}
	else
	{
		std::cerr << "blIALinearSampler: ERROR!! 3D is not still implemented!!" << std::endl;
	}


 }




/** \brief Get the gradient of a vector
 *
 *	\param gradient vector with size (2 * k + 1) elements
 */
//----------------------------------------------------
template <class TImage>
void blIALinearSampler<TImage>::GetGradient(vnl_vector<double> & gradient)
{
	
	int i;	
///	int delta = 2;
	//int outputSize = 2 * this->size + 1;
	
	this->size ++;	
	// increment the size to allow the gradient vector to have 2k+1 elements

	OutputVectorType vector(2 * this->size + 1);
	this->GetOutputForGivenPoint(vector);

	this->size --;
	// restore the original size 
	
	// for each position, the gradient is the next minus the previous position
	// since vector has 2 elements more than gradient (2 * size + 1)

	for (i = 0; i < gradient.size(); i++)										
	{
		gradient[i] = (double)vector[i+2] - (double)vector[i];
	}


	
	//////////////////////////////////////////////////////////////////////////////
	// NORMALIZING 
	//////////////////////////////////////////////////////////////////////////////
	double Magnitude = 0.0;	
	for (i = 0; i < gradient.size(); i++)
	{		
		#ifdef FAST_GLP_NORMALIZATION 
			Magnitude += fabs(gradient[i]);
		#else
			Magnitude += gradient[i] * gradient[i];
		#endif
    }
	 
	#ifndef FAST_GLP_NORMALIZATION 
		Magnitude = sqrt (Magnitude);
	#endif 

	// Normalizing the gray level values
	if (Magnitude != 0.0)
	{		
		for(i = 0; i < gradient.size(); i++)
		{
			gradient[i] /= Magnitude;
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	// END OF NORMALIZING 
	//////////////////////////////////////////////////////////////////////////////
}



/** \brief Get coords of the i-th sample
 */
//----------------------------------------------------
template <class TImage>
typename blIALinearSampler<TImage>::PointType 
blIALinearSampler<TImage>::GetCoordinatesOfSampleNumber(int numberOfSample)
//----------------------------------------------------
{
	// Getting Normal Vector
	double xNormal = this->xNormal;
	double yNormal = this->yNormal;
///	if (DIMENSION == 3)
	//	double zNormal = this->zNormal;

	// --------------------------------------------
	// DIMENSION DEPENDING CODE.
///	if (DIMENSION == 2)
///	{
		// we sample the following:
		//
		// the landmark (xOrigin, yOrigin)
		//
		// size points before (-size, ..., -1)
		//
		// size points after (1, ..., size)
		//
		// first point (0) will be 
		//		xOrigin - k * xSpacing
		//		yOrigin - k * ySpacing
		//
		// i-th point will be
		//		xOrigin - (k - i) * xSpacing
		//		yOrigin - (k - i) * ySpacing
		// so if i == k, then we are sampling the landmark itself
		// and if i == 2k (we'd sampled 2k+1 points) 
		//	we are sampling on xOr + k*xSp, yOr +  k*ySp
		
		//double normalDirection = xNormal/yNormal; // sets the adress for the normal.
									// xNormal, yNormal 
		double xSpacing, ySpacing;
/*
		//--------------------------------------------
		// projecting distance in axis
		double alpha;
		alpha = atan(yNormal/xNormal); // what if xNormal == 0?? IMPLEMENT ME!!!
		
		xSpacing = this->spacing * cos(alpha);
		ySpacing = this->spacing * sin(alpha);
*/
		//--------------------------------------------
		// quick and dirty normalization of the normal vector

		double magnitude = sqrt(xNormal*xNormal+yNormal*yNormal);
		
		xNormal /= magnitude;
		yNormal /= magnitude;

		xSpacing = this->spacing * xNormal;
		ySpacing = this->spacing * yNormal;


		// Computing coordinates (counting from the current point)
		// --------------------------------------------

		PointType point;
		point[0] = this->xPoint + numberOfSample * xSpacing;
		point[1] = this->yPoint + numberOfSample * ySpacing;


		return point;
///	} // end of dimension == 2		
}

#endif
