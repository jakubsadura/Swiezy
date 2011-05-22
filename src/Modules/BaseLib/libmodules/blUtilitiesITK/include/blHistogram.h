/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//This class can be used to generate 1D or 2D histograms.

#ifndef __blhistogram_h
#define __blhistogram_h


/** -----------------------------------------------------------------------
 *		Includes														   
 * ------------------------------------------------------------------------
 */
#include "blLightObject.h"		// Generic base class
#include "blSmartPointer.h"		// Smart Pointer
#include "blMacro.h"			// Smart Pointer

#include <vnl/vnl_matrix.h>		// Data storage
#include <vnl/vnl_vector.h>		// Data storage
#include <vector>				// Data storage

#include <iostream>			// For couts



// ___________________________________________________
// ---------------------------------------------------
/**
 * \def ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM
 * \brief If defined, prints debug messages
 */
#define ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM

/**
 * \def ENABLE_bl_HISTOGRAM_EXAMPLES
 * \brief If defined, enables the access of the example code
 */
#define ENABLE_bl_HISTOGRAM_EXAMPLES
// ___________________________________________________
// ---------------------------------------------------




/** -----------------------------------------------------------------------
 * \brief This class can be used to generate 1D or 2D histograms.
 * \ingroup blUtilities
 *
 * This class can be used to produce
 * (1) 1D histogram
 * (2) 2D histogram
 * 
 * An example in blHistogram.cpp file demonstrates how this class 
 * can be instantiated and run.
 *
 * Three types of outputs can be generated
 * (1) Histogram counts
 * (2) Normalized histogram counts (Normalized wrt to the highest count
 * (3) CDF (Cumulative Distribution Function) of histogram counts
 * ------------------------------------------------------------------------
 */
// ___________________________________________________
// ---------------------------------------------------
template<typename T>
class blHistogram : public blLightObject 
// ___________________________________________________
// ---------------------------------------------------
{

public:


	// ----------------------------------------------------------------
	// Smart pointer initialization
	// ----------------------------------------------------------------
	typedef blHistogram Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);
	// ----------------------------------------------------------------


	// ----------------------------------------------------------------
	// Get() functions
	// ----------------------------------------------------------------
	/**
	 * \brief This overloaded function can be used to obtain 
	 * normalized histogram counts and the bin centers.
	 *\param nDimension The dimension of the histogram (the value is 1 or 2)
	 *\param data A vnl_matrix holding data arranged in columns.
	 *\param nBins A vnl_vector holding the number of desired 
	 * bins for each dimension. If nDimension  == 1, make nBins[2] = 1
	 *\param count Output parameter: A double matrix holding the histogram counts
	 *\param binCenters Output parameter: A vector holding the bin centers
	 *\param N A char variable when set to 
	 * 'N' or 'n' outputs normalized histogram
	 * 'C' or 'c' outputs cumulative sidtribution function
	 *\return True, if process successful, else, False.
	 */
	bool GetHistogram (int nDimension,
				       vnl_matrix<T> data,
					   vnl_vector<int> nBins,
					   vnl_matrix<double> & count, 
					   std::vector< vnl_vector<double> > & binCenters,
					   char N = 'a');


	/**
	 * \brief This overloaded function can be used to obtain 
	 * normalized histogram counts and the bin centers, when the number of
	 * bins in each dimension, max and min values in the each dimension are known.
	 *\param nDimension The dimension of the histogram (the value is 1 or 2)
	 *\param data A vnl_matrix holding data arranged in columns.
	 *\param nBins A vnl_vector holding the number of desired 
	 *\param maxVal A vector holding maximum values for each dimension.
	 * If nDimension  == 1, then maxVal.size() = 1
	 * If nDimension  == 2, then maxVal.size() = 2
	 *\param minVal A vector holding minimum values for each dimension.
	 * If nDimension  == 1, then minVal.size() = 1
	 * If nDimension  == 2, then minVal.size() = 2
	 * bins for each dimension. If nDimension  == 1, make nBins[2] = 1
	 *\param count Output parameter: A double matrix holding the histogram counts
	 *\param binCenters Output parameter: A vector holding the bin centers
	 *\param N A char variable when set to 
	 * 'N' or 'n' outputs normalized histogram
	 * 'C' or 'c' outputs cumulative sidtribution function
	 *\return True, if process successful, else, False.
	 */
	bool GetHistogram (int nDimension,
					   vnl_matrix<T> data,
					   vnl_vector<int> nBins,
					   vnl_vector<T> maxVal,
					   vnl_vector<T> minVal,
					   vnl_matrix<double> & count, 
					   std::vector< vnl_vector<double> > & binCenters,
					   char N = 'a');


	/**
	 * \brief This overloaded function can be used to obtain 
	 * normalized histogram counts and the bin centers.
	 *\param nDimension The dimension of the histogram (the value is 1 or 2)
	 *\param data A vnl_matrix holding data arranged in columns.
	 *\param binCenters Output parameter: A vector holding the bin centers
	 * bins for each dimension. If nDimension  == 1, make nBins[2] = 1
	 *\param count Output parameter: A double matrix holding the histogram counts
	 *\param N A char variable when set to 
	 * 'N' or 'n' outputs normalized histogram
	 * 'C' or 'c' outputs cumulative sidtribution function
	 *\return True, if process successful, else, False.
	 */
	bool GetHistogram (int nDimension,
				       vnl_matrix<T> data,
					   std::vector< vnl_vector<double> > binCenters,
					   vnl_matrix<double> & count,
					   char N = 'a');




private:


	// Private functions
	// ----------------------------------------------------------------
	// Set () functions
	// ----------------------------------------------------------------

	
	/**
	 * \brief This function sets the data for generating the histogram
	 *\param data A vnl_matrix holding data arranged in columns.
	 *\return True, if parameters set properly, else, False.
	 */
	bool SetData (vnl_matrix<T> data);


	/**
	 * \brief This function sets the dimension of the histogram
	 *\param nDimension The dimension of the histogram (the value is 1 or 2)
	 *\return True, if parameters set properly, else, False.
	 */
	bool SetNumberOfDimensions (int nDimension);



	/**
	 *\brief This function is used to manually set the maximum and minimum 
	 * limits of the histogram.
	 *\param maxVal A vector holding maximum values for each dimension.
	 *\param minVal A vector holding minimum values for each dimension.
	 *\return True, if parameters were set properly, else, false.
	 */
	bool SetMaxMin (vnl_vector<T> maxVal, vnl_vector<T> minVal);



	/**
	 *\brief This function checks for the maximum and minimum 
	 * values of the data, and sets them as the limits of the histogram.
	 *\return True, if process successful, else, false.
	 */
	bool SetDefaultMaxMin ();


	/**
	 *\brief Generates bin centers
	 *\return True, if process successful, else, false.
	 */
	bool GenerateBinCenters ();


	/**
	 *\brief Generates cdf (Cumulative Distribution Function)
	 * of this->normCount.
	 *\return True, if process successful, else, false.
	 */
	bool GenerateCDF ();



	/**
	 *\brief This function sets the number of desired bins in the histogram.
	 * Once, nBins is set, it automatically calculates the bin centers
	 * for each dimension.
	 *\param nBins A vnl_vector holding the number of desired 
	 * bins for each dimension. If nDimension  == 1, make nBins[2] = 1
	 *\return True, if parameters were set properly, else, false.
	 */
	bool SetNumberOfBins (vnl_vector<int> nBins);


	/**
	 *\brief This function can be used to set the desired bin centers.
	 * Once, binCenters are set, it automatically calculates the number of bins in 
	 * each dimension.
	 *\param binWidth A vector holding desired bin width for each dimension.
	 *\return True, if parameter(s) were set properly, else, false.
	 */
	bool SetBinCenters (std::vector< vnl_vector<double> > binCenters);


	/**
	 *\brief This function is internally used to compute histogram counts.
	 *\return True, if process successful, else, False.
	 */
	bool Update();


	// Private variables
	unsigned int nDimension;					//!< The dimension of the histogram
									//!< (the value for nDimension is 1 or 2)

	vnl_matrix<T> data;				//!< A matrix holding data in columns.

	vnl_vector<T> maxVal;			//!< A vector holding maximum values for each dimension.
									//!< If nDimension  == 1, then maxVal.size() = 1
									//!< If nDimension  == 2, then maxVal.size() = 2

	vnl_vector<T> minVal;			//!< A vector holding minimum values for each dimension.
									//!< If nDimension  == 1, then minVal.size() = 1
									//!< If nDimension  == 2, then minVal.size() = 2

	vnl_vector<int> nBins;			//!< An integer vector holding the number of desired bins 
									//!< for each dimension.
									//!< If nDimension  == 1, make nBins[2] = 1


	vnl_matrix<double> count;			//!< A matrix holding the histogram counts
									//!< If, nDimension == 1, 
									//!< the dimensions of count is [nBins[1], 1]
									//!< If, nDimension == 2, 
									//!< the dimensions of count is [nBins[1], nBins[2]]

	vnl_matrix<double> normCount;	//!< A matrix holding the normalized histogram counts
									//!< Normalizing is done by dividing each element in the 
									//!< count matrix by the maximum value in the count matrix.
									//!< If, nDimension == 1, 
									//!< the dimensions of normCount is [nBins[1], 1]
									//!< If, nDimension == 2, 
									//!< the dimensions of normCount is [nBins[1], nBins[2]]

	vnl_matrix<double> cdf;			//!< A matrix holding the cumulative distribution function 
									//!< Normalizing is done by dividing each element in the 
									//!< count matrix by the maximum value in the count matrix.
									//!< If, nDimension == 1, 
									//!< the dimensions of normCount is [nBins[1], 1]
									//!< If, nDimension == 2, 
									//!< the dimensions of normCount is [nBins[1], nBins[2]]

	std::vector< vnl_vector<double> > binCenters;
									//!< A vector of vnl_vectors holding bin centers
									//!< If, nDimension == 1, 
									//!< the dimensions of binCenters.size() = 1
									//!< If, nDimension == 2, 
									//!< the dimensions of binCenters.size() = 2

};



#ifdef ENABLE_bl_HISTOGRAM_EXAMPLES
	namespace blClassesTests
	{
		int testHistogram();
		void displayMatrices(vnl_matrix<double> test, const char* name);
		void displayVectors(vnl_vector<double> test, const char* name);
	}
#endif // ENABLE_bl_HISTOGRAM_EXAMPLES



#endif //__blhistogram_h
