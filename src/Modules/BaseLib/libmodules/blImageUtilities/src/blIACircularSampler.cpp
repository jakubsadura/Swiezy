/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
//Implementation file for the Independent Component Analysis.

#include "blIACircularSampler.h"
#include "blIALinearSampler.h"

/**
 *	Constructor 
 */
//----------------------------------------------------
blIACircularSampler::blIACircularSampler()
//----------------------------------------------------
{

}



/**
 *	Destructor 
 */
//----------------------------------------------------
blIACircularSampler::~blIACircularSampler()
//----------------------------------------------------
{

}



//----------------------------------------------------
/** \brief Get sampled values for a landmark of current shape 
 *  on current image
 *
 *  Actually computes the sampling on every landmark 
 *  and returns the results
 *
 *	As a Circular sampler, it will return 
 *  ( (2*size+1) * (360/alpha) ) pixel values.
 *
 *	Fills to 255 all samples that goes out from the image
 *
 *	\todo dimension dependent code
 */
blIACircularSampler::OutputVectorType blIACircularSampler::GetOutputForGivenPoint()
{
	// --------------------------------------------
	// General declarations
	int dimension = DIMENSION;
	double alpha = this->GetAngleInRadians();
	int numberOfRadius = (int)ceil(PI / alpha );		// we assume PI as max (180�) because of simmetry
	int numberOfSamples = (2 * this->size + 1) * numberOfRadius; // number of samples to be done
	int k = this->size;
	double xOrigin, yOrigin, zOrigin = 0.0;
	
	// --------------------------------------------
	// Normal and Output vectors
	OutputVectorType samples;
	OutputVectorType auxOutput(2 * k + 1);

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
	//if (dimension == 3)
	//	double zNormal = this->zNormal;

	// --------------------------------------------
	// DIMENSION DEPENDING CODE.
	if (dimension == 2)	// we can use here the GetCoordinatesOfLandmarkNumber() but it's more unneficient
	{

		//--------------------------------------------
		// quick and dirty normalization of the normal vector

		double magnitude = sqrt(xNormal*xNormal+yNormal*yNormal);
		
		xNormal /= magnitude;
		yNormal /= magnitude;

		samples.set_size(numberOfSamples); // numberOfSamples means (2k+1)^2

		PointType point;
		
		// creating linear sampler (auxiliar)
		std::cout << "IACircularSampler: creating Linear sampler (for subsampling)" << std::endl;

		
		LinearSamplerType::Pointer mySampler = LinearSamplerType::New();

		// defining constant features for the sampler
		mySampler->SetInput(this->image);
		mySampler->SetParameters(this->size, this->spacing);

		int magnitudeOfRadius = 2 * this->size + 1;
		// loop for sampling each "radius" of the Circular (starting from normal-radius)

		// Getting Pixeles (to put the aux linear sampler)
		// --------------------------------------------
		point[0] = xOrigin;
		point[1] = yOrigin;
		mySampler->SetPoint(point); // linear sampler ever set in the same point

		for (int radius = 0;  radius < numberOfRadius; radius++)
		{
			double currentAngle = radius * alpha;
			
/*
std::cout << "blIACircularSampler: Current Angle(degrees) = " << currentAngle *180 / PI << std::endl;
std::cerr << "blIACircularSampler: Current Angle(degrees) = " << currentAngle *180 / PI << std::endl;
*/

			// Getting next radius. 
			//-----------------------------------------------
			// Set this way to keep the order of the samples
			//	counterclockwise.
			// signs are set to preserve the "mano derecha" rule
			// currentAngle must be in radians
			double xNextNormal = xNormal *  cos(currentAngle) - yNormal * sin(currentAngle);
			double yNextNormal = xNormal *  sin(currentAngle) + yNormal * cos(currentAngle);

			point[0] = xNextNormal;
			point[1] = yNextNormal;
			mySampler->SetNormal(point);

/*
std::cout << "blIACircularSampler: setting next normal to = " << point[0] << "," <<point[1] << std::endl;
std::cerr << "blIACircularSampler: setting next normal to = " << point[0] << "," <<point[1] << std::endl;
*/

			// Getting the outputs
			//------------------------------------------------
			mySampler->GetOutputForGivenPoint(auxOutput);

			// we fill samples by radius ( samples = [radius(0)...radius(alpha * k)]
			for (int row = 0; row < 2*k+1; row++)
			{
				samples[(magnitudeOfRadius * radius) + row] = 
						auxOutput(row);
				//std::cout << "blIACircularSampler:: sampled("<<radius<<","<<row<<")= "<<(unsigned int)auxOutput(row)<<std::endl;
			}
		}
	}
	else
	{
		std::cerr << "blIACircularSampler: ERROR!! 3D is not still implemented!!" << std::endl;
	}

	return samples;
}


/** \brief Get the gradient of a vector
 */
//----------------------------------------------------
vnl_vector<double> blIACircularSampler::GetGradient(OutputVectorType vector) const
//----------------------------------------------------
{
	vnl_vector<double> gradientOutput(vector.size() );

	////////////////////////////////////////////////////////////////////
	// I'm waiting for discover if I'd any use. If you find some use for me, please...
	// IMPLEMENT ME!!!!
	////////////////////////////////////////////////////////////////////
	
	return gradientOutput;
}


/** \brief Get coords of the i-th sample
 */
//----------------------------------------------------
blIACircularSampler::PointType 
blIACircularSampler::GetCoordinatesOfSampleNumber(int numberOfSampleX, 
														  int numberOfSampleY, 
														  int numberOfSampleZ)
//----------------------------------------------------
{
	////////////////////////////////////////////////////////////////////
	// I'm waiting for discover if I'd any use. If you find some use for me, please...
	// IMPLEMENT ME!!!!
	////////////////////////////////////////////////////////////////////
	PointType output;

	std::cerr << "blIACircularSampler: I'M STILL NOT IMPLEMENTED!!!! " << std::endl;

	return output;
}




/** \brief Set sampler parameters 
 */
//----------------------------------------------------
void blIACircularSampler::SetParameters(int size, double spacing, double alpha)
//----------------------------------------------------
{
	this->size = size;
	this->spacing = spacing;
	if (
		( alpha < 0.0) || (alpha > 2*PI)
	   )
	{
		std::cerr << "blIACircularSampler: Error in alpha. Must be between 0.0 and 2*PI" << std::endl;
	}
	else
	{
		this->alpha = alpha;	// WE STORE THE ANGLE IN RADIANS
	}
}
	
