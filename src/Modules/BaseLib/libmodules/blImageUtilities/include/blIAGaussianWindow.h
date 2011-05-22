/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAGaussianWindow_h
#define __blIAGaussianWindow_h

#include <itkNeighborhood.h>

#include "blLightObject.h"
#include "blMacro.h"
#include "blSmartPointer.h"

// #define DEBUG_MESSAGES_blIAGAUSSIANWINDOW



/**
 * \brief Defines a gaussian window 
 * \ingroup blImageUtilities
 *
 *	This is a N-dimensional container that stores coefficients 
 *  calculated by a gaussian function.
 *  
 *  The following parameters must be defined:
 *
 *	- sigma: std. deviation for the gaussian function
 *	- radius: number of pixels at each side of the center pixel in each dimension
 *
 *	All the coefficients are the result of this function:
 *
 *	blGaussianWindow[i][j][k]... = 
 *		exp( -1.0 * (i * i + j * j + k * k +..) / (2 * sigma * sigma))
 *
 */
template <class TPixel, unsigned int VDimension = 2>
class blIAGaussianWindow : public itk::Neighborhood<TPixel, VDimension> , 
							public blLightObject 
{
	public:

		typedef blIAGaussianWindow Self;
		typedef blSmartPointer<Self> Pointer;		

  typedef itk::Neighborhood<TPixel, VDimension> Superclass;

		blNewMacro(Self);


		// SetRadius() --> superclass allocates memory for this container 
	
		/**\brief recompute all coefficients for the current size of this container*/
		void ComputeCoefficients();

		/**\brief sets sigma value for gaussian funtion*/
		void SetSigma(double sigma);


	protected:

		/**\brief constructor*/
		blIAGaussianWindow();


		/**\brief destructor*/
		virtual ~blIAGaussianWindow();


	private:
		blIAGaussianWindow(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented

		double sigma;	//!< sigma value for the gaussian function

};

#include "blIAGaussianWindow.txx"

#endif


