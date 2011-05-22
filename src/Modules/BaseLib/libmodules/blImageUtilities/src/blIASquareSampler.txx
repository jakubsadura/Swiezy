/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIASquareSampler_txx
#define __blIASquareSampler_txx

/**
 *	Constructor
 */
template <class TImage>
blIASquareSampler<TImage>::blIASquareSampler()
{
	// default values
	this->size = 0;
	this->spacing = 1;
	this->sizeX = 0;
	this->sizeY = 0;
};

/**
 *	Destructor
 */
template <class TImage>
blIASquareSampler<TImage>::~blIASquareSampler() 
{
};

/**	\brief Get sampled values for current point in current image
 *
 *  Actually computes the sampling on the point and returns the results
 *
 *	\param vector vector with (2 * size + 1) * (2 * size + 1) elements where values are copied.
 */
template <class TImage>
 void blIASquareSampler<TImage>::GetOutputForGivenPoint(OutputVectorType & samples)
 {
	// --------------------------------------------
	// General declarations
	int dimension = 2;
	int totalNumberOfSamples = (2 * this->sizeX + 1) * (2 * this->sizeY + 1); // number of samples
//	int numberOfSamples = (2 * this->size + 1) * (2 * this->size + 1); // number of samples to be done
	int totalSamplesX = (2 * this->sizeX + 1);
	int totalSamplesY = (2 * this->sizeY + 1);

	//int k = this->size;
	double xOrigin, yOrigin, zOrigin = 0.0;
	double xToSetSampler, yToSetSampler, zToSetSampler = 0.0;
	
	// --------------------------------------------
	// Normal and Output vectors
	//OutputVectorType samples;

	OutputVectorType auxOutput(totalSamplesX);

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

	if (dimension == 3)
		double zNormal = this->zNormal;

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
		// size points before and after the points said above in the +-size parallels to the normal
		//
		//	(-k,k)		...		(0,k)	...		(k,k)
		//	...			...		...		...		...
		//	(-k,0)		...		(0,0)	...		(k,0)
		//	...			...		...		...		...
		//	(-k,-k)		...		(0,-k)	...		(k,-k)
		//
		//	stored by columns (for an easier gradient computation (direction of the normal))
		//

	//	double xBSpacing, yBSpacing;	// spacing in the BNormal

		//--------------------------------------------
		// quick and dirty normalization of the normal vector

		double magnitude = sqrt(xNormal*xNormal+yNormal*yNormal);
		
		xNormal /= magnitude;
		yNormal /= magnitude;

		// perpendicular to the normal. Set this way to keep the order of the samples
		//	from left to right ( (-k,-k) sampler is left-down from the origin.)
		double xBNormal = yNormal;
		double yBNormal = -xNormal;

		//xBSpacing = this->spacing * xBNormal;
		//yBSpacing = this->spacing * yBNormal;

		PointType point;

		typename LinearSamplerType::Pointer mySampler = LinearSamplerType::New();

		// defining constant features for the sampler
		mySampler->SetParameters(this->sizeX, this->spacing);
		mySampler->SetInput(this->image);


//		int magnitudeOfSide = 2 * this->size + 1;
//		// loop for sampling each "column" of the square
//		for (column = -k; column < k+1; column++)

		// loop for sampling each "row" of the square
		int rowCount = 0;
		int row;

		int minrow = -this->sizeY;
		int maxrow = this->sizeY+1;
		for (row = minrow; row < maxrow; row++)
		{
			// Computing where to set the linear sampler
			// --------------------------------------------
			//xToSetSampler = xOrigin + column * xBSpacing;
			//yToSetSampler = yOrigin + column * yBSpacing;
			
			xToSetSampler = xOrigin + row * this->spacing * xNormal;
			yToSetSampler = yOrigin + row * this->spacing * yNormal;
			
			// Getting Pixeles from coordinates
			// --------------------------------------------
			point[0] = xToSetSampler;
			point[1] = yToSetSampler;
			
			mySampler->SetPoint(point);

			point[0] = xBNormal;
			point[1] = yBNormal;

			mySampler->SetNormal(point);

			mySampler->GetOutputForGivenPoint(auxOutput);

			int x;
			for (x = 0; x < totalSamplesX; x++)
			{
				samples[rowCount * totalSamplesX + x ] = 
						auxOutput(x);
			//	std::cout << "data: " << samples[rowCount * totalSamplesX + x ] << std::endl;
			}

			rowCount ++;
		}
	}
	else
	{
		std::cerr << "blIASquareSampler: ERROR!! 3D is not still implemented!!" << std::endl;
	}
 }




/** \brief Get the gradient of a vector
 *
 *	\param gradient vector with size (2 * k + 1) elements
 */
//----------------------------------------------------
template <class TImage>
void blIASquareSampler<TImage>::GetGradient(vnl_vector<double> & gradient)
{
	////////////////////////////////////////////////////////////////////
	// IMPLEMENT ME!!!!
	////////////////////////////////////////////////////////////////////
	PointType output;

	std::cerr << "blIASquareSampler: I'M STILL NOT IMPLEMENTED!!!! " << std::endl;
}



/** \brief Get coords of the i-th sample
 */
//----------------------------------------------------
template <class TImage>
typename blIASquareSampler<TImage>::PointType 
blIASquareSampler<TImage>::GetCoordinatesOfSampleNumber(int numberOfSample)
//----------------------------------------------------
{
	////////////////////////////////////////////////////////////////////
	// IMPLEMENT ME!!!!
	////////////////////////////////////////////////////////////////////
	PointType output;

	std::cerr << "blIASquareSampler: I'M STILL NOT IMPLEMENTED!!!! " << std::endl;

	return output;
}

#endif
