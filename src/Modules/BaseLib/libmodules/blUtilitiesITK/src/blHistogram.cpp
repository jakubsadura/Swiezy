/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <math.h>
#include <blHistogram.h>

/**
\brief   This class can be used to generate 1D or 2D histograms.
\ingroup 
\author  Sri Kaushik Pavani
\date    
*/

/**
 * \brief This overloaded function can be used to obtain the histogram counts and 
 * the bin centers, when the number of bins in each dimension is known.
 *\param nDimension The dimension of the histogram (the value is 1 or 2)
 *\param data A vnl_matrix holding data arranged in columns.
 *\param nBins A vnl_vector holding the number of desired 
 * bins for each dimension. If nDimension  == 1, make nBins[2] = 1
 *\param count Output parameter: A double matrix holding the histogram counts
 *\param binCenters Output parameter: A vector holding the bin centers
 *\param N A char variable when set to 'N' or 'n' outputs normalized histogram
 *\return True, if process successful, else, False.
 */
template <typename T>
bool blHistogram<T>::GetHistogram (int nDimension,
								   vnl_matrix<T> data,
								   vnl_vector<int> nBins,
								   vnl_matrix<double> & count, 
								   std::vector< vnl_vector<double> > & binCenters,
								   char N)
{

	if (!this->SetNumberOfDimensions(nDimension))
		return false;
	if (!this->SetData (data))
		return false;
	if (!this->SetDefaultMaxMin ())
		return false;
	if (!this->SetNumberOfBins(nBins))
		return false;
	if (!this->GenerateBinCenters())
		return false;
	if (!this->Update())
		return false;

	if ((N == 'N') || (N == 'n'))
		count = this->normCount;
	else if ((N == 'C') || (N == 'c'))
		count = this->cdf;
	else
		count = this->count;

	binCenters = this->binCenters;

	return true;
}


/**
 * \brief This overloaded function can be used to obtain the histogram counts and 
 * the bin centers, when the number of bins in each dimension, max and min values 
 * in the each dimension are known.
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
 *\param N A char variable when set to 'N' or 'n' outputs normalized histogram
 *\return True, if process successful, else, False.
 */
template <typename T>
bool blHistogram<T>::GetHistogram (int nDimension,
								   vnl_matrix<T> data,
								   vnl_vector<int> nBins,
								   vnl_vector<T> maxVal,
								   vnl_vector<T> minVal,
								   vnl_matrix<double> & count, 
								   std::vector< vnl_vector<double> > & binCenters,
								   char N)
{
	if (!this->SetNumberOfDimensions(nDimension))
		return false;
	if (!this->SetData (data))
		return false;
	if (!this->SetMaxMin (maxVal, minVal))
		return false;
	if (!this->SetNumberOfBins(nBins))
		return false;
	if (!this->GenerateBinCenters())
		return false;
	if (!this->Update())
		return false;

	if ((N == 'N') || (N == 'n'))
		count = this->normCount;
	else if ((N == 'C') || (N == 'c'))
		count = this->cdf;
	else
		count = this->count;

	binCenters = this->binCenters;

	return true;
}


/**
 * \brief This overloaded function can be used to obtain the histogram counts  
 * when the bin centers are known.
 *\param nDimension The dimension of the histogram (the value is 1 or 2)
 *\param data A vnl_matrix holding data arranged in columns.
 *\param binCenters A vector holding the bin centers
 * bins for each dimension. If nDimension  == 1, make nBins[2] = 1
 *\param N A char variable when set to 'N' or 'n' outputs normalized histogram
 *\param count Output parameter: A double matrix holding the histogram counts
 *\return True, if process successful, else, False.
 */
template <typename T>
bool blHistogram<T>::GetHistogram (int nDimension,
							       vnl_matrix<T> data,
								   std::vector< vnl_vector<double> > binCenters,
								   vnl_matrix<double> & count,
								   char N)
{
	if (!this->SetNumberOfDimensions(nDimension))
		return false;
	if (!this->SetData (data))
		return false;
	if (!this->SetBinCenters(binCenters))
		return false;
	if (!this->Update())
		return false;

	if ((N == 'N') || (N == 'n'))
		count = this->normCount;
	else if ((N == 'C') || (N == 'c'))
		count = this->cdf;
	else
		count = this->count;

	return true;
}



/**
 * \brief This function sets the dimension of the histogram
 *\param nDimension The dimension of the histogram (the value is 1 or 2)
 *\return True, if parameters set properly, else, False.
 */
template <typename T>
bool blHistogram<T>::SetNumberOfDimensions(int nDimension)
{

	// Sanity checks
	// To check if the number of dimensions is less 
	// than zero or greater than 2
	if ((nDimension <= 0) || (nDimension > 2))
	{
	#ifdef ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM
		std::cout << "Debug: blHistogram->SetNumberOfDimensions: "
			 << "This class can handle only 1D and 2D histograms, "
			 << "Check input for nDimension" << std::endl;
	#endif
	return false;
	}

	this->nDimension = nDimension;
	return true;
}



/**
 * \brief This function sets the data for generating the histogram
 *\param data A vnl_matrix holding data arranged in columns.
 *\return True, if parameters set properly, else, False.
 */
template <typename T>
bool blHistogram<T>::SetData (vnl_matrix<T> data)
{
	// To check the dimensions of data
	if (this->nDimension != data.cols())
	{
	#ifdef ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM
		std::cout << "Debug: blHistogram->SetData: "
			 << "Error in the input data " << std::endl;
		std::cout << "Debug: blHistogram->SetData: "
			 << "Hint: Arrange data in columns. " <<std::endl;
		std::cout << "Debug: blHistogram->SetData: "
			 << "The number of dimensions of the "
			 << "histogram is equal to the number of "
			 << "colums in data matrix" << std::endl;
	#endif
	return false;
	}

	this->data = data;
	return true;
}



/**
 *\brief This function is used to manually set the maximum and minimum 
 * limits of the histogram.
 *\param maxVal A vector holding maximum values for each dimension.
 *\param minVal A vector holding minimum values for each dimension.
 *\return True, if parameters were set properly, else, false.
 */
template <typename T>
bool blHistogram<T>::SetMaxMin (vnl_vector<T> maxVal, vnl_vector<T> minVal)
{
	//Sanity checks
	// To compare the size of maxVal with the dimension of histogram
	if (maxVal.size() != this->nDimension)
	{
	#ifdef ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM
		std::cout << "Debug: blHistogram->SetMaxMin: "
			 << "The number of elements in maxVal "
			 << "must be equal to dimension of histogram" << std::endl;
	#endif
	return false;
	}

	// To compare the size of minVal with the dimension of histogram
	if (minVal.size() != this->nDimension)
	{
	#ifdef ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM
		std::cout << "Debug: blHistogram->SetMaxMin: "
			 << "The number of elements in minVal "
			 << "must be equal to dimension of histogram" << std::endl;
	#endif
	return false;
	}

	// To check if max value is greater than min value
	for (unsigned int iter = 0; iter < this->nDimension; iter++)
	{
		if (maxVal[iter] <= minVal[iter])
		{
			#ifdef ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM
			std::cout << "Debug: blHistogram->SetMaxMin: "
				 << "Maximum value must be greater than minimum value."
				 << std::endl;
			#endif
			return false;
		}
	}


	this->maxVal.clear();
	this->minVal.clear();

	this->maxVal = maxVal;
	this->minVal = minVal;

	return true;
}


/**
 *\brief This function checks for the maximum and minimum 
 * values of the data, and sets them as the limits of the histogram.
 *\return True, if process successful, else, false.
 */
template <typename T>
bool blHistogram<T>::SetDefaultMaxMin ()
{
	vnl_vector<T> temp;
	vnl_vector<T> maxVal(this->nDimension);
	vnl_vector<T> minVal(this->nDimension);

	for (unsigned int iter = 0; iter < this->nDimension; iter++)
	{
		temp = data.get_column(iter);
		maxVal[iter] = temp.max_value();
		minVal[iter] = temp.min_value();
	}

	this->maxVal = maxVal;
	this->minVal = minVal;

	return true;
}

template <typename T>
bool blHistogram<T>::GenerateBinCenters ()
{
	this->binCenters.clear();

	// Calculate the binCenters
	double step = ((double)maxVal[0] - (double)minVal[0])/(double)nBins[0];

	// Define vector to hold binCenters of one dimension
	vnl_vector<double> temp(nBins[0]); 
	for (int iter = 0; iter < nBins[0]; iter++)
	{
		// step/2 is added to find center of bin
		temp[iter] = ((double)minVal[0] + iter * step) + step/2;
	}
	this->binCenters.push_back(temp);

	if (this->nDimension == 2)
	{
		step = ((double)maxVal[1] - (double)minVal[1])/(double)nBins[1];
		// Define vector to hold binCenters of one dimension
		vnl_vector<double> temp(nBins[1]); 
		for (int iter = 0; iter < nBins[1]; iter++)
		{
			// step/2 is added to find center of bin
			temp[iter] = ((double)minVal[1] + iter * step) + step/2;
		}
		this->binCenters.push_back(temp);
	}

	return true;
}


/**
 *\brief This function sets the number of desired bins in the histogram.
 * Once, nBins is set, it automatically calculates the bin centers
 * for each dimension.
 *\param nBins A vector holding the number of desired bins for each dimension.
 *\return True, if parameters were set properly, else, false.
 */
template <typename T>
bool blHistogram<T>::SetNumberOfBins(vnl_vector<int> nBins)
{
	// Sanity checks
	// To check if the number of elements in vnl_vector is 2
	if (nBins.size() != 2)
	{
	#ifdef ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM
		std::cout << "Debug: blHistogram->SetNumberOfBins: "
			 << "The number of elements in the vnl_vector "
			 << "has to be two." << std::endl;
	#endif
	return false;
	}

	
	// To check if number of bins is less than zero
	if ((nBins[0] <= 0) || (nBins[1] <= 0))
	{
	#ifdef ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM
		std::cout << "Debug: blHistogram->SetNumberOfBins: "
			 << "The elements in the vnl_vector "
			 << "has to be greater than zero." << std::endl;
	#endif
	return false;
	}


	// To check if the number of bins conforms to 
	// the dimension of the histogram desired.
	if ((this->nDimension == 1) & (nBins[1] != 1))
	{
	#ifdef ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM
		std::cout << "Debug: blHistogram->SetNumberOfBins: "
			 << "Error in number of bins, "
			 << "input value has to be equal to 1" << std::endl;
	#endif
	return false;
	}

	this->nBins.clear();

	this->nBins = nBins;

	
	
	return true;
}


/**
 *\brief This function can be used to set the desired bin centers.
 * Once, binCenters are set, it automatically calculates the number of bins in 
 * each dimension.
 *\param binCenters A matrix holding desired bin centers for each dimension
 * arranges as columns. Number of columns == nDimension.
 *\return True, if parameter(s) were set properly, else, false.
 */
template <typename T>
bool blHistogram<T>::SetBinCenters(std::vector< vnl_vector<double> > binCenters)
{
	// Sanity checks
	// To check the dimensions of binCenters
	if (this->nDimension != binCenters.size())
	{
	#ifdef ENABLE_DEBUG_MESSAGES_bl_HISTOGRAM

		std::cout << "Debug: blHistogram->SetBinCenters: "
			 << "Check the dimensions of binCenters "
			 << std::endl;

		std::cout << "Debug: blHistogram->SetBinCenters: "
			 << "Hint: Arrange binCenters as vnl_vectors "
			 << "and push them into a vector" << std::endl;

		std::cout << "Debug: blHistogram->SetBinCenters: "
			 << "Hint: The number of elements in binCenters must be "
			 << "equal to the dimension of histogram (nDimension)" << std::endl;
	#endif
	return false;
	}


	this->binCenters = binCenters;

	// Now, set the values of number of bins.
	this->nBins.set_size(2);
	this->nBins[0] = binCenters[0].size();
	this->nBins[1] = 1;

	// Incase nDimension is 2
	if (this->nDimension == 2)
	{
		this->nBins[1] = binCenters[1].size();
	}

	// Setting max, min values
	this->maxVal.set_size(this->nDimension);
	this->minVal.set_size(this->nDimension);

	for (unsigned int iter = 0; iter < this->nDimension; iter++)
	{
		double step = binCenters[iter][1] - binCenters[iter][0];
		this->maxVal[iter] = binCenters[iter].max_value() + step/2;
		this->minVal[iter] = binCenters[iter].min_value() - step/2; 
	}

	return true;
}


/**
 *\brief This function is internally used to compute histogram counts.
 * Examines each data element, and determines its corresponding 
 * histogram bin.
 *\return True, if process successful, else, False.
 */
template <typename T>
bool blHistogram<T>::Update()
{
	// Set default values to histogram matrix
	this->count.set_size(nBins[0], nBins[1]);
	this->count.fill(0);

	// To determine step sizes of histogram in each dimension
	vnl_vector<double> step(2);
	step[0] = this->binCenters[0][1] - this->binCenters[0][0];

	if (this->nDimension == 2)
		step[1] = this->binCenters[1][1] - this->binCenters[1][0];
	else
		step[1] = 1;

	for (unsigned int iter1 = 0; iter1 < this->data.rows(); iter1++)
	{
		int coord [] = {0,0};
		for (unsigned int iter2 = 0; iter2 < this->nDimension; iter2++)
		{
// TODO::COMMENTED BY XAVI
// 			coord[iter2] = (this->data[iter1][iter2] - this->minVal[iter2]) / step[iter2];
//////////////////
//TODO::MODIFIED BY XAVI
 			coord[iter2] = static_cast<int>(floor((this->data[iter1][iter2] - this->minVal[iter2]) / step[iter2] ));
///////////////
			// Sanity checks
			// To check if coordinates lie within limits
			// If not, they are set to corresponding extremes
			if (coord[iter2] < 0)
				coord[iter2] = 0;
			if (coord[iter2] >= this->nBins[iter2])
				coord[iter2] = this->nBins[iter2]-1;
		}

		count[coord [0]][coord [1]]++;

		
	}

	this->normCount = this->count / this->count.max_value();

	GenerateCDF();

	return true;
}



/**
 *\brief Generates cdf (Cumulative Distribution Function)
 * of this->normCount.
 *\return True, if process successful, else, false.
 */
template <typename T>
bool blHistogram<T>::GenerateCDF()
{
	// Initially equating cdf to pdf
	this->cdf = this->normCount / this->normCount.array_one_norm();

	// Sum row wise
	for (unsigned int iter1 = 1; iter1 < this->cdf.rows(); iter1++)
	{
		// Adding previous row with present row
		this->cdf.set_row(iter1, (this->cdf.get_row(iter1)
								 + this->cdf.get_row(iter1-1)));
	}

	// Sum column wise
	for (unsigned int iter2 = 1; iter2 < this->cdf.cols(); iter2++)
	{
		// Adding previous row with present row
		this->cdf.set_column(iter2, (this->cdf.get_column(iter2)
								  + this->cdf.get_column(iter2-1)));
	}

	return true;
}


#ifdef ENABLE_bl_HISTOGRAM_EXAMPLES
	namespace blClassesTests
	{
		int testHistogram()
		{

			// -----------------------------------------------------
			// Example 1
			// (1) Tests GetHistogram() function
			// (2) Data is double, and hence blHistogram<double>
			// (3) Please change the value of param N to obtain one of these
			//			- Histogram counts, N = arbitrary char, except N & C
			//			- Normalized histogram, N = n
			//			- CDF, N = 'c'
			//			- (If no value is assigned explicitly, N = 'a')
			// -----------------------------------------------------
			std::cout << std::endl << "Example 1" << std::endl;

			// Inputs;
			int nDimension = 1;
			
			// Data should have 1 coulmn, since nDimension == 1
			vnl_matrix<double> data(500,1);
			// Filling up data
			for (int iter = 0; iter < 500; iter++)
				data[iter][0] = iter;

			// Specifying the number of bins
			vnl_vector<int> nBins(2);
			// Lets say, we need 10 bins
			nBins[0] = 10;
			nBins[1] = 1; // Default: will make much more sense for 2D histograms

			// Now define containers to hold binCenters and histogram counts
			vnl_matrix<double> count;
			std::vector< vnl_vector<double> > binCenters;

			// Lets invoke a smart pointer to blHistogram Class
			// The type is double because the data is double
			blHistogram<double>::Pointer myHist = blHistogram<double>::New();

			myHist->GetHistogram(nDimension, data, nBins, count, binCenters);

			displayMatrices(count, "Histogram counts");
			displayVectors(binCenters[0], "Bin Centers");

			/**
			 * Matlab code for this example:
			 * [N,X] = hist([0:499], 10); disp(N'); disp(X');
			 */
			
			// The meat of this example is that we supplied ndimensions, data, nBins,
			// and obtained count and binCenters.

			// -----------------------------------------------------
			// Example 2
			// (1) Tests GetHistogram() function
			// (2) Data is double, and hence blHistogram<double>
			// (3) MinMax values for the histogram are set
			// (4) Please change the value of param N to obtain one of these
			//			- Histogram counts, N = arbitrary char, except N & C
			//			- Normalized histogram, N = n
			//			- CDF, N = 'c'
			//			- (If no value is assigned explicitly, N = 'a')
			// -----------------------------------------------------
			std::cout << std::endl << "Example 2" << std::endl;


			// The size of minVal and maxVal is one, cbl, we are dealing
			// with 1D histograms
			// Its of type double because the data is double.

			vnl_vector<double> maxVal(1);
			vnl_vector<double> minVal(1);

			// Set values
			maxVal[0] = 500;
			minVal[0] = -100;

			myHist->GetHistogram(nDimension, data, nBins, maxVal,
								 minVal, count, binCenters);

			displayMatrices(count, "Histogram counts");
			displayVectors(binCenters[0], "Bin Centers");


			// -----------------------------------------------------
			// Example 3
			// (1) Tests GetHistogram() function
			// (2) Data is double, and hence blHistogram<double>
			// (3) BinCenters are provided as input
			// -----------------------------------------------------
			std::cout << "Example 3" << std::endl;

			myHist->GetHistogram(nDimension, data, binCenters, count);

			displayMatrices(count, "Histogram counts");
			displayVectors(binCenters[0], "Bin Centers");


			// -----------------------------------------------------
			// Example 4
			// (1) Tests GetHistogram() function for 2D histogram
			// (2) Data is double, and hence blHistogram<double>
			// (3) Please change the value of param N to obtain one of these
			//			- Histogram counts, N = arbitrary char, except N & C
			//			- Normalized histogram, N = n
			//			- CDF, N = 'c'
			//			- (If no value is assigned explicitly, N = 'a')
			// -----------------------------------------------------
			std::cout << "Example 4" << std::endl;

			// Inputs;
			nDimension = 2;
			
			// Data should have 2 coulmns, since nDimension == 2
			data.set_size(500,2);
			// Filling up data
			for (int iter1 = 0; iter1 < 500; iter1++)
			{
				for (int iter2 = 0; iter2 < 2; iter2++)
				{
					data[iter1][iter2] = iter1;
				}
			}
				
			// Lets say, we need 10 and 20 bins for the two dimensions
			nBins[0] = 10; 
			nBins[1] = 5;

			myHist->GetHistogram(nDimension, data, nBins, count, binCenters);

			displayMatrices(count, "Histogram counts");
			displayVectors(binCenters[0], "Bin Centers for dimension1");
			displayVectors(binCenters[1], "Bin Centers for dimension2");


			// -----------------------------------------------------
			// Example 5
			// (1) Tests GetHistogram() function for 2D histogram
			// (2) Data is double, and hence blHistogram<double>
			// (3) MinMax values for the histogram are set
			// (4) Please change the value of param N to obtain one of these
			//			- Histogram counts, N = arbitrary char, except N & C
			//			- Normalized histogram, N = n
			//			- CDF, N = 'c'
			//			- (If no value is assigned explicitly, N = 'a')
			// -----------------------------------------------------
			std::cout << "Example 5" << std::endl;

			maxVal.set_size(2);
			minVal.set_size(2);

			// Set values
			maxVal[0] = 500;
			minVal[0] = -100;
			maxVal[1] = 500;
			minVal[1] = -100;

			myHist->GetHistogram(nDimension, data, nBins, maxVal,
								 minVal, count, binCenters);

			displayMatrices(count, "Histogram counts");
			displayVectors(binCenters[0], "Bin Centers for dimension1");
			displayVectors(binCenters[1], "Bin Centers for dimension2");


			// -----------------------------------------------------
			// Example 6
			// (1) Tests GetHistogram() function for 2D histogram
			// (2) Data is double, and hence blHistogram<double>
			// (3) BinCenters generated in Example 5 are provided as input
			// (4) Please change the value of param N to obtain one of these
			//			- Histogram counts, N = arbitrary char, except N & C
			//			- Normalized histogram, N = n
			//			- CDF, N = 'c'
			//			- (If no value is assigned explicitly, N = 'a')
			// -----------------------------------------------------
			std::cout << "Example 6" << std::endl;

			myHist->GetHistogram(nDimension, data, binCenters, count);

			displayMatrices(count, "Histogram counts");
			displayVectors(binCenters[0], "Bin Centers");
			displayVectors(binCenters[1], "Bin Centers");



			return 0;
		}

		void displayMatrices(vnl_matrix<double> test, const char* name)
		{
			std::cout << std::endl << name << ":" << std::endl;
			for (unsigned int iter1 = 0; iter1 < test.rows(); iter1++)
			{
				for (unsigned int iter2 = 0; iter2 < test.cols(); iter2 ++)
				{
					std::cout << test[iter1][iter2] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}

		void displayVectors(vnl_vector<double> test, const char* name)
		{
			std::cout << std::endl << name << ":" << std::endl;
			for (unsigned int iter1 = 0; iter1 < test.size(); iter1++)
			{
				std::cout << test[iter1] << std::endl;
			}
			std::cout << std::endl;
		}
	}
#endif // ENABLE_bl_HISTOGRAM_EXAMPLES


