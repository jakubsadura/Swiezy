/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAGaussianWindow_txx
#define __blIAGaussianWindow_txx

//#include "blIAGaussianWindow.h"
#include "blImageMask.h"

//-----------------------------------------------------------
template <class TPixel, unsigned int VDimension>
blIAGaussianWindow< TPixel, VDimension>::blIAGaussianWindow():
sigma(1.0)
//-----------------------------------------------------------
{
	this->SetRadius(1);
	// the superclass allocates memory for this size

}



/**\brief constructor*/
//-----------------------------------------------------------
/*
template <class TPixel, unsigned int VDimension>
blIAGaussianWindow< TPixel, VDimension>::
blIAGaussianWindow(unsigned int radius): sigma(1.0)
//-----------------------------------------------------------
{
	this->SetRadius(radius);
	// the superclass allocates memory for this size
}

*/


/**
 *	Destructor
 */
//-----------------------------------------------------------
template <class TPixel, unsigned int VDimension>
blIAGaussianWindow<TPixel, VDimension>::~blIAGaussianWindow() 
//-----------------------------------------------------------
{

}





/**
 *	Sets the sigma value for the gaussian function.
 *
 *	ComputeCoefficients() method must be called to update coefficients
 */
//-----------------------------------------------------------
template <class TPixel, unsigned int VDimension>
void blIAGaussianWindow<TPixel, VDimension>::SetSigma(double sigma)
//-----------------------------------------------------------
{
	this->sigma = sigma;
}



/**
 *	Compute all coeficients with the gaussian function:
 *
 *	exp(-1.0 * (i*i + j*j) / (2 * sigma * sigma))
 */
//-----------------------------------------------------------
template <class TPixel, unsigned int VDimension>
void blIAGaussianWindow<TPixel, VDimension>::ComputeCoefficients()
//-----------------------------------------------------------
{
	
	const typename Superclass::SizeType radius = this->GetRadius();
	
	int dimIndex[VDimension];	// index

#ifdef DEBUG_MESSAGES_blIAGAUSSIANWINDOW
	std::cout << "Initialize Dim index[]" << std::endl;
#endif

	unsigned dim;
	for (dim = 0; dim < VDimension; dim++)
	{
		dimIndex[dim] = -1 * ((int)radius[dim]);

#ifdef DEBUG_MESSAGES_blIAGAUSSIANWINDOW
		std::cout << "Dim index[" << dim << "] = " <<  dimIndex[dim] << std::endl;
#endif
	}

#ifdef DEBUG_MESSAGES_blIAGAUSSIANWINDOW
	std::cout << std::endl;
#endif

	typename Superclass::Iterator iter = this->Begin();

	// precompute 2 * sigma * sigma
	double sigmaSigma2 = 2 * this->sigma * this->sigma;
	
	while (iter != this->End())
	{
		// calculate numerator (i*i + j*j + k*k...)
		double numerator = 0.0;
		for (dim = 0; dim < VDimension; dim++)
		{
			numerator += dimIndex[dim] * dimIndex[dim];
		}


#ifdef DEBUG_MESSAGES_blIAGAUSSIANWINDOW
		std::cout << std::endl;
		for (dim = 0; dim < VDimension; dim++)
		{
			std::cout << "Dim index[" << dim << "] = " << dimIndex[dim] << std::endl;
		}
		std::cout << std::endl;
#endif
		

		*iter = exp( -1.0 * numerator / sigmaSigma2);

		// update dimension indexes
		// -----------------------------
		bool dimIndexUpdated = false;
		dim = 0;

		while (dim < VDimension && !dimIndexUpdated)
		{
			dimIndex[dim]++;	

			if (dimIndex[dim] > ((int)radius[dim]))
			{
				dimIndex[dim] = - ((int) radius[dim]);
				dim++;

//				std::cout << "Dim index[" << dim - 1 << "] = 0" << std::endl;
//				std::cout << "Dim: " << dim << std::endl;
			}
			else
			{
				dimIndexUpdated = true;
			}
		}
//		std::cout << "Dim index[] updated" << std::endl;


		iter++;
	}

}


#endif
