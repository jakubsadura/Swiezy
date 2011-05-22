/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//----------------------------------------------------------------------------
/** \brief This implements some math utilities, like covariances or products...
 *
 *  Implements Covariance, SmallCovariance, etc.
 */

#include "blMath.h"
#include "blMath.txx"

#include <iostream>
#include <limits>
#include <float.h>

#include <vnl/vnl_transpose.h>
#include <vnl/vnl_fastops.h>


#define PRECISION 1e-6	// current precison 
//Ir M_PI just include <math.h> #define M_PI 3.1415915	// \todo PI MUST BE well-defined but I got no time now.

/** 
 *
 *	blMath started as a class containing covar computations and things
 *	like that, but has evolve grouping all functions
 *	general enough to be in any part of the library or even out of it.
 */


// const double blMath::PI = 4*atan((double)1);



//Quick select
//right - included
int blMath::partition(double* list, int left, int right, int pivotIndex)
{
	double pivotValue = list[pivotIndex];
	double temp = list[pivotIndex];
	list[pivotIndex] = list[right];
	list[right] = temp;

	int storeIndex = left;
				
	for( int i=left; i<right; i++ )
	{
		if( list[i] < pivotValue )
		{
			temp = list[storeIndex];
			list[storeIndex] = list[i];
			list[i] = temp;
			storeIndex++;
		}
	}
				
	temp = list[right];
	list[right] = list[storeIndex];  // Move pivot to its final place
	list[storeIndex] = temp;
	
	return storeIndex;

}

double blMath::QuickSelect(double *list, int left, int right, int k)
{
	//select pivotIndex between left and right
	//it is optimal to choose pivotIndex randomly, that way on average the computation is th fastest
	//but if you feel that it slows down, uncomment the following line and remove the randomizer
	//int pivotIndex = left;
	srand(time(NULL));
	int pivotIndex = left + floor(static_cast<double>(right-left)*(static_cast<double>(rand())/RAND_MAX));


	int pivotNewIndex = partition(list, left, right, pivotIndex);

	if( k == pivotNewIndex )
		return list[k];
	else if( k < pivotNewIndex )
		return QuickSelect(list, left, pivotNewIndex-1, k);
	else
		return QuickSelect(list, pivotNewIndex+1, right, k);
}

double blMath::QuickSelect(std::vector<double>& array,int k)
{
	vnl_vector<double> array_copy( array.size() );
	array_copy.copy_in(&array[0]);
	return QuickSelect(array_copy.begin(),0,array.size()-1,k);
}

double blMath::QuickSelect(vnl_vector<double>& array,int k)
{
	vnl_vector<double> array_copy = array;
	return QuickSelect(array_copy.begin(),0,array.size()-1,k);
}

void blMath::ComputeWeightedCovariance( const vnlMatrixDouble& theMatrix,
                                        vnl_vector<double>& weights, 
                                        vnl_vector<double>& theMean,
                                        vnlMatrixDouble& covarianceMatrix )
{	
	double weightsMean;
	weightsMean = (double)0.0;
	vnlMatrixDouble auxMatrix;

	ComputeWeightedMeanRow(theMatrix, theMean, weights);

	ComputeRowSubstraction(theMatrix, theMean, auxMatrix);

	// Multiply by weights
	for (unsigned int iter1 = 0; iter1 < auxMatrix.rows(); iter1++)
	{
		for (unsigned int iter2 = 0; iter2 < auxMatrix.columns(); iter2++)
		{
			auxMatrix[iter1][iter2] = auxMatrix[iter1][iter2] * std::sqrt(weights[iter1]);
		}

		weightsMean += weights[iter1]*weights[iter1];
	}

	covarianceMatrix.set_size(auxMatrix.cols(), auxMatrix.cols());

	// covarianceMatrix = A' * A;
	vnl_fastops::AtA( covarianceMatrix, auxMatrix);
	// covarianceMatrix /= ((double) (auxMatrix.rows()  - 1));
	covarianceMatrix /= ((double)1 - weightsMean);

}




double blMath::GaussRand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

  phase = 1 - phase;

  return X;
}


/** \brief computes covariance and mean (shapes in rows)
 *
 *  Implements standard method for covariance.
 *
 *	This writes the new covariance matrix in given matrix 	
 */
//----------------------------------------------------------------------------
void blMath::ComputeMeanAndCov(const vnlMatrixDouble& theMatrix,
	vnlVectorDouble& theMean, vnlMatrixDouble & covarianceMatrix)
//----------------------------------------------------------------------------
{	
	vnlMatrixDouble auxMatrix;

	// vnl_vector<double> theMean;
	ComputeMeanRow(theMatrix, theMean);
	ComputeRowSubstraction(theMatrix, theMean, auxMatrix);

	// vnl_fastops::AtA()
	bllao::Resize(covarianceMatrix, bllao::NrRows(auxMatrix), bllao::NrCols(auxMatrix));

	// covarianceMatrix = A' * A;
	bllao::AtA( covarianceMatrix, auxMatrix);
	covarianceMatrix /= ((double) (bllao::NrRows(auxMatrix)  - 1));
}

// Modified to avoid vector copies
/** \brief computes mean
 *
 *  Implements computing mean of a matrix by rows
 *  \param matrix the mean to be computed
 *	\param theMean result. not need to be set_sized outside.
 */
//----------------------------------------------------------------------------
void blMath::ComputeMeanColumn(const vnlMatrixDouble& theMatrix,
								vnl_vector<double>& theMean)
//----------------------------------------------------------------------------
{
	int r,c;

	int rows = theMatrix.rows();
	int cols = theMatrix.cols();

	theMean.set_size(rows);
	theMean.fill(0.0);

	const double * rowElementPt;

	for (r = 0; r < rows; r++)
	{
		rowElementPt = theMatrix[r];

		for (c = 0; c < cols; c++)
		{
			theMean[r] += *rowElementPt;
			rowElementPt++;
		}

		theMean[r] /= cols;
	}
}





/**

  @author   Mikkel B. Stegmann
  @version  6-6-2002

  @memo     Linear alignment of this vector to another vector using the L2 norm.

  @doc      Linear alignment of this vector to another vector using the L2 norm.  
  
  @param    v   Vector that this vector is being least squares fitted to.  

  @param	a	Optional pointer to store the resulting transformation in.

  @param	b	Optional pointer to store the resulting transformation in.
  
  @return   Nothing.
  
*/
void blMath::AlignVectors(	double *vector, double *target, unsigned length, double *pA, double *pB	)
{

	//vector->set_size( target->size() );

   //int n = vector->size();
   int n = length;

   // calc sums
   double x, y, a, b, Sx = .0, Sy = .0, Sxx = .0, Sxy = .0;
   for(int i=0;i<n;i++) 
   {
       x    = vector[i];
       y    = target[i];
       Sx  += x;
       Sy  += y;
       Sxx += x*x;
       Sxy += x*y;
   }

   // in an L2 sense, the optimal scaling and offset of this vector
   double d = n*Sxx-Sx*Sx;
   a = (n*Sxy-Sx*Sy)/d;
   b = (Sy*Sxx-Sx*Sxy)/d;           

   // apply tranformation
   for(int i=0;i<n;i++) 
   {
    	vector[i] = a * vector[i] + b;
   }

	if (pA) { *pA = a; }
	if (pB) { *pB = b; }
};

// Modified to improve performance (avoid row copies)
/** \brief computes mean
 *
 *  Implements computing mean of a matrix by columns
 *  \param matrix the mean to be computed
 *	\param theMean result. not need to be set_sized outside.
 */
//----------------------------------------------------------------------------
void blMath::ComputeMeanRow(const vnlMatrixDouble& theMatrix, vnl_vector<double>& theMean)
//----------------------------------------------------------------------------
{
	baselib::ComputeMeanRowImpl(theMatrix, theMean);
}

void blMath::ComputeWeightedMeanRow(const vnlMatrixDouble& theMatrix,
									vnl_vector<double>& theMean, vnl_vector<double>& weights)
{
	baselib::ComputeWeightedMeanRowImpl(theMatrix, theMean, weights);
}

double blMath::MSE( const vnlVectorDouble& v1, const vnlVectorDouble& v2, const int number_of_landmarks )
{
	return sqrt( (v1-v2).squared_magnitude()/number_of_landmarks );
}



// vnl_fastops::ABt() without .transpose()

//----------------------------------------------------------------------------
/** \brief computes matrix(col) - vector for each column in matrix
 *
 *  Implements computing substraction of a column vector from a matrix
 *	\return resulting matrix.
 */
vnlMatrixDouble blMath::ComputeSubstraction(const vnlMatrixDouble& matrix, const vnlVectorDouble& vector)
{
	size_t i,j;

	vnlMatrixDouble matrixResult(matrix.rows(), matrix.cols());

	if (matrix.rows() != vector.size() )
	{
		std::cerr << "blMath: computeSubstraction error! different number of rows!" << std::endl;
		return matrixResult;
	}

	for (i = 0; i < matrix.rows(); i++)
	{
		for (j = 0; j < matrix.columns(); j++)
			matrixResult(i,j) = matrix.get(i,j) - vector(i);
	}

	return matrixResult;
}


//faster method var(x) = E(x^2) - E(x)^2
/** \brief computes variance of a vector of data
 *
 *  Implements computing variance
 */
//----------------------------------------------------------------------------
double blMath::ComputeVariance(const vnlVectorDouble& theVector, double* the_mean)
//----------------------------------------------------------------------------
{
	return ComputeVariance( theVector.begin(), theVector.size(), the_mean );
}



double blMath::ComputeVariance(const double* theVector, unsigned int length, double* the_mean)
{
	size_t i;

	double mean = 0.0;
	double mean_sqr = 0.0;


	for (i = 0; i < length; i++)
	{
		mean += theVector[i];
		mean_sqr += theVector[i] * theVector[i];
	}

	mean /= length;
	mean_sqr /= length;

	if( the_mean!=NULL ) *the_mean = mean;

	return ( mean_sqr - mean * mean );
}



/** \brief compute gaussian with given mean and stddev
 */
//----------------------------------------------------------------------------
double blMath::Gaussian(double mean, double stddev, double x)
//----------------------------------------------------------------------------
{
    return GaussRand() * stddev + mean;
}



//----------------------------------------------------------------------------
/** \brief Vectorial division (component by component) of two matrices
 *
 *	emules the ./ operator in Matlab for matrices (overloaded for vectors)
 */
vnlMatrixDouble blMath::DivisionComp2Comp(const vnlMatrixDouble& matrix1, 
								  const vnlMatrixDouble& matrix2)
{
	size_t i,j;
	
	vnlMatrixDouble result(matrix1.rows(), matrix1.columns() );

	if ( (matrix1.rows() != matrix2.rows() )||(matrix1.columns() != matrix2.columns() ) )
	{
		std::cerr << "blMath: error dividing matrices. sizes doesn't fit" << std::endl;
		return result;
	}

	for (i = 0; i < matrix1.rows(); i++)
		for (j = 0; j < matrix1.columns(); j++)
	{
		result(i,j) = matrix1(i,j) / matrix2(i,j);
	}
	return result;
}


//----------------------------------------------------------------------------
/** \brief Vectorial division (component by component) of two vectors
 *
 *	emules the ./ operator in Matlab for vectors (overloaded for matrices)
 */
vnlVectorDouble blMath::DivisionComp2Comp(const vnlVectorDouble& vector1, 
								  const vnlVectorDouble& vector2)
{
	size_t i;
	
	vnlVectorDouble result(vector1.size() );

	if (vector1.size() != vector2.size() )
	{
		std::cerr << "blMath: error dividing vectors. sizes doesn't fit" << std::endl;
		return result;
	}

	for (i = 0; i < vector1.size(); i++)
	{
		result(i) = vector1(i) / vector2(i);
	}
	return result;
}


/**
*	Sorts a vector in ascending order. Recursive algorithm.
*
*	Index must be initialized (size and data).
*
*	\param vector vector to be sorted
*	\param index vector of indexes 
*	\param left	index of left element
*	\param right index of right element
*/
//----------------------------------------------------------------------------
void blMath::Q_Sort_Asc(std::vector<int> & vector,
						std::vector<int>& index, 
						int left, 
						int right) 
{
	//----------------------------------------------------------------------------

	int l_hold, r_hold;

	int pivot;
	int pivotIndex;

	l_hold = left;
	r_hold = right;

	pivot = vector[left];
	pivotIndex = index[left];

	while (left < right)
	{
		while ((vector[right] >= pivot) && (left < right))
		{
			right--;
		}

		if (left != right)	// smaller than pivot
		{
			vector[left] = vector[right];
			index[left] = index[right];

			left++;
		}

		while ((vector[left] <= pivot) && (left < right))
		{
			left++;
		}

		if (left != right)	// greater than pivot
		{
			vector[right] = vector[left];
			index[right] = index[left];
			right--;
		}
	}

	vector[left] = pivot;
	index[left] = pivotIndex;

	if (l_hold < left)
		blMath::Q_Sort_Asc(vector, index, l_hold, left-1);

	if (r_hold > left)
		blMath::Q_Sort_Asc(vector, index, left + 1, r_hold);
}




/**
 *	Sorts a vector in ascending order. Recursive algorithm.
 *
 *	Index must be initialized (size and data).
 *
 *	\param vector vector to be sorted
 *	\param index vector of indexes 
 *	\param left	index of left element
 *	\param right index of right element
 */
//----------------------------------------------------------------------------
void blMath::Q_Sort_Asc(std::vector<float> & vector,
 						std::vector<int>& index, 
						int left, 
						int right) 
{
//----------------------------------------------------------------------------

int l_hold, r_hold;

float pivot;
int pivotIndex;

l_hold = left;
r_hold = right;

pivot = vector[left];
pivotIndex = index[left];

while (left < right)
{
	while ((vector[right] >= pivot) && (left < right))
	{
		right--;
	}

	if (left != right)	// smaller than pivot
	{
		vector[left] = vector[right];
		index[left] = index[right];

		left++;
	}

	while ((vector[left] <= pivot) && (left < right))
	{
		left++;
	}

	if (left != right)	// greater than pivot
	{
		vector[right] = vector[left];
		index[right] = index[left];
		right--;
	}
}

vector[left] = pivot;
index[left] = pivotIndex;

if (l_hold < left)
	blMath::Q_Sort_Asc(vector, index, l_hold, left-1);

if (r_hold > left)
	blMath::Q_Sort_Asc(vector, index, left + 1, r_hold);
}




/**
 *	Sorts a vector in ascending order. Recursive algorithm.
 *
 *	Index must be initialized (size and data).
 *
 *	\param vector vector to be sorted
 *	\param index vector of indexes 
 *	\param left	index of left element
 *	\param right index of right element
 */
//----------------------------------------------------------------------------
void blMath::Q_Sort_Asc(std::vector<float> & vector,
 						std::vector<float>& index, 
						int left, 
						int right) 
{
//----------------------------------------------------------------------------

int l_hold, r_hold;

float pivot;
float pivotIndex;

l_hold = left;
r_hold = right;

pivot = vector[left];
pivotIndex = index[left];

while (left < right)
{
	while ((vector[right] >= pivot) && (left < right))
	{
		right--;
	}

	if (left != right)	// smaller than pivot
	{
		vector[left] = vector[right];
		index[left] = index[right];

		left++;
	}

	while ((vector[left] <= pivot) && (left < right))
	{
		left++;
	}

	if (left != right)	// greater than pivot
	{
		vector[right] = vector[left];
		index[right] = index[left];
		right--;
	}
}

vector[left] = pivot;
index[left] = pivotIndex;

if (l_hold < left)
	blMath::Q_Sort_Asc(vector, index, l_hold, left-1);

if (r_hold > left)
	blMath::Q_Sort_Asc(vector, index, left + 1, r_hold);
}





//----------------------------------------------------------------------------
/** \brief Inner Vectorial product (dot) of two vectors
 *
 *	emules the * operator in Matlab for vectors
 *	\returns std::numeric_limits<double>::max() if vectors have different sizes
 */
double blMath::Dot(const vnlVectorDouble& v1, 
		   const vnlVectorDouble& v2)
{
	size_t i;
	double accum = 0.0;

	if (v1.size() != v2.size())
	{
		return std::numeric_limits<double>::max();
	}

	for (i = 0; i < v1.size(); i++)
///		accum += v1(i) * v2(i);
		accum += v1[i] * v2[i];

	return accum;
}

//----------------------------------------------------------------------------
/** \brief generate a matrix of points representing quads
 *
 *	\param magnitude means size of smaller side
 *	\param proportion means times "high" bigger than "base"
 *	\return vnlMatrixDouble of size (numberSensors, numberColumns) with a
 *	0..magnitude uniform distribution.
 */
vnlMatrixDouble blMath::GenerateQuads(int numberShapes, double magnitude, double proportion, double errorMagnitude)
{
	size_t j;

	vnlMatrixDouble auxMatrix(8, numberShapes);

	//The function rand() returns a pseudorandom integer between zero and RAND_MAX.
	srand( time(NULL) );
    
	// non-noised quad would be on (1,1)(-1,1)(-1,-1)(1,-1)
	// noise is always in the same mean direction
	for (j = 0; j < auxMatrix.columns(); j++)
	{
		// generating xy format
		// generating point 1
		auxMatrix(0,j) = magnitude + errorMagnitude * rand() / RAND_MAX; // RAND_MAX defined system constant
		auxMatrix(1,j) = magnitude * proportion + errorMagnitude * rand() / RAND_MAX;
		// generating point 2
		auxMatrix(2,j) = -magnitude + errorMagnitude * rand() / RAND_MAX;
		auxMatrix(3,j) = magnitude * proportion + errorMagnitude * rand() / RAND_MAX;
		// generating point 3
		auxMatrix(4,j) = -magnitude + errorMagnitude * rand() / RAND_MAX;
		auxMatrix(5,j) = -magnitude + errorMagnitude * rand() / RAND_MAX;
		// generating point 4
		auxMatrix(6,j) = magnitude + errorMagnitude * rand() / RAND_MAX;
		auxMatrix(7,j) = -magnitude + errorMagnitude * rand() / RAND_MAX;
	}
	return auxMatrix;
}

//----------------------------------------------------------------------------
/** \brief implements MATLAB's all() function
 *
 */
int blMath::Have_zeros(const vnlVectorDouble& theVector)
{
	size_t i = 0;
	int areZeros = 0;
	while (i < theVector.size() && !areZeros)
	{
		if (theVector(i) == 0.0)
			areZeros++;
	}
	return areZeros;
}



//----------------------------------------------------------------------------
/** \brief computes median as the medium value of theVector sorted
 *
 */
double blMath::Median(const vnlVectorDouble& theVector)
{
	int size = theVector.size();

	vnl_vector<int> index(size);

// new sorting method	
//	vnlVectorDouble sorted = Sort(theVector, &index);

	vnlVectorDouble sorted;
	blMath::QuickSortDesc(theVector, sorted, index);

	double medianValue; 

	if ((size % 2) == 0)
	{
		// even number of elements: mean of central elements
		medianValue = ( sorted[size / 2 - 1] + sorted[size / 2] ) 
								/ (double) 2.0;
		
	}
	else
	{
		// odd number of elements: central element
		medianValue = sorted[size / 2];
	}

	return medianValue;
}


//----------------------------------------------------------------------------
/** \brief computes median as the vector of mediums of rows or columns
 *
 *	\param type = 1 for rows, 2 for columns
 */
vnlVectorDouble blMath::Median(const vnlMatrixDouble& theMatrix, int type)
{
	vnlVectorDouble sorted;
	size_t i;

	if (type == 1)
	{
		sorted.set_size(theMatrix.rows() );
		for (i = 0; i < theMatrix.rows(); i++)
			sorted(i) = Median(theMatrix.get_row(i) );
	}
	else // type == 2
	{
		sorted.set_size(theMatrix.columns() );
		for (i = 0; i < theMatrix.columns(); i++)
			sorted(i) = Median(theMatrix.get_column(i) );		
	}
	
	return sorted;
}



//----------------------------------------------------------------------------
/** \brief print a vnlMatrixDouble in stdin
 *
 */
void blMath::PrintMatrix(const vnlMatrixDouble& theMatrix, char *name)
{
	size_t i, j;

	std::cout << std::endl << "Printing " << name << " matrix for tracing" << std::endl;
	for (i = 0; i < theMatrix.rows(); i++)
	{
		for (j = 0; j < theMatrix.columns(); j++)
			std::cout << theMatrix(i,j) << std::endl;
		std::cout << std::endl;
	}
}

//----------------------------------------------------------------------------
/** \brief print a vnlVectorDouble in stdin
 *
 */
void blMath::PrintVector(const vnlVectorDouble& theVector, char *name)
{
	int i;

	std::cout << std::endl << "Printing " << name << " vector for tracing" << std::endl;
	for (i = 0; theVector.size(); i++)
		std::cout << theVector(i) << std::endl;

}

//----------------------------------------------------------------------------
/** \brief Vectorial product (component by component) of two vectors
 *
 *	emules the .* operator in Matlab for vectors (overloaded for matrices)
 */
vnlVectorDouble blMath::ProductComp2Comp(const vnlVectorDouble& vector1, 
								 const vnlVectorDouble& vector2)
{
	size_t i;
	
	vnlVectorDouble result(vector1.size() );

	if (vector1.size() != vector2.size() )
	{
		std::cerr << "blMath: error multiplying vectors. sizes doesn't fit" << std::endl;
		return result;
	}

	for (i = 0; i < vector1.size(); i++)
	{
		result(i) = vector1(i) * vector2(i);
	}
	return result;
}

//----------------------------------------------------------------------------
/** \brief Matricial product (component by component) of two matrices
 *
 *	emules the .* operator in Matlab for matrices. (overloaded for vectors)
 */
vnlMatrixDouble blMath::ProductComp2Comp(const vnlMatrixDouble& matrix1, 
								 const vnlMatrixDouble& matrix2)
{
	size_t i, j;
	
	vnlMatrixDouble result(matrix1.rows(), matrix1.columns() );

	if ( (matrix1.rows() != matrix2.rows() ) || (matrix1.columns() != matrix2.columns() ) )
	{
		std::cerr << "blMath: error multiplying matrices. sizes doesn't fit" << std::endl;
		return result;
	}

	for (i = 0; i < matrix1.rows(); i++)
		for (j = 0; j < matrix1.columns(); j++)
			result(i,j) = matrix1(i, j) * matrix2(i, j);
	return result;
}



/** 
 *	Sorts a vnl_vector in ascending order. 
 *
 *	QuickSort algorithm. O(n*log(n)) <--> O(n^2)
 *
 * 	\param vector input vector
 *	\param sortedVector vector returned
 *	\param index returned vector with indexes: 
 *		sortedVector(i) = original(index(i))
 */
//----------------------------------------------------------------------------
void blMath::QuickSortAsc(vnl_vector<double> & vector, 
					   vnl_vector<double> & sortedVector,
						vnl_vector<int> & index)
//----------------------------------------------------------------------------
{
	sortedVector = vector;	// copy	
	index.set_size(vector.size());	// set_size

	for (size_t i = 0; i < vector.size(); i++)
	{
		index[i] = i;
	}

	blMath::Q_Sort_Asc(sortedVector, index, 0, sortedVector.size() - 1);
}



/** 
 *	Sorts a vnl_vector in ascending order
 *
 *	QuickSort algorithm. O(n*log(n)) <--> O(n^2)
 *
 * 	\param vector vector to be ordered
 *	\param index returned vector with indexes: 
 *		sortedVector(i) = original(index(i))
 */
//----------------------------------------------------------------------------
void blMath::QuickSortAsc(vnl_vector<double> & vector,
										vnl_vector<int> & index)
//----------------------------------------------------------------------------
{
	index.set_size(vector.size());
	for (size_t i = 0; i < vector.size(); i++)
	{
		index[i] = i;
	}
	blMath::Q_Sort_Asc(vector, index, 0, vector.size() - 1);
}



/**
 *	Sorts a vnl_vector in ascending order. Recursive algorithm.
 *
 *	QuickSort algorithm. O(n*log(n)) <--> O(n^2)
 *
 *	Index must be initialized (size and data).
 *
 *	\param vector vector to be sorted
 *	\param index vector of indexes 
 *	\param left	index of left element
 *	\param right index of right element
 */
//----------------------------------------------------------------------------
void blMath::Q_Sort_Asc(vnl_vector<double> & vector,
						vnl_vector<int> & index, int left, int right)
{
//----------------------------------------------------------------------------

	int l_hold, r_hold;

	double pivot;
	int pivotIndex;

	l_hold = left;
	r_hold = right;

	pivot = vector[left];
	pivotIndex = index[left];

	while (left < right)
	{
		while ((vector[right] >= pivot) && (left < right))
		{
			right--;
		}

		if (left != right)	// smaller than pivot
		{
			vector[left] = vector[right];
			index[left] = index[right];

			left++;
		}

		while ((vector[left] <= pivot) && (left < right))
		{
			left++;
		}

		if (left != right)	// greater than pivot
		{
			vector[right] = vector[left];
			index[right] = index[left];
			right--;
		}
	}

	vector[left] = pivot;
	index[left] = pivotIndex;

	if (l_hold < left)
		blMath::Q_Sort_Asc(vector, index, l_hold, left-1);

	if (r_hold > left)
		blMath::Q_Sort_Asc(vector, index, left + 1, r_hold);
}



/** 
 *	Sorts a vnl_vector in descending order
 *
 *	QuickSort algorithm. O(n*log(n)) <--> O(n^2)
 *
 *	\param vector input vector
 *	\param sortedVector vector returned
 *	\param index returned vector with indexes: 
 *		sortedVector(i) = original(index(i))
 */
//----------------------------------------------------------------------------
void blMath::QuickSortDesc(const vnl_vector<double> & vector, 
							vnl_vector<double> & sortedVector,
							vnl_vector<int> & index)
//----------------------------------------------------------------------------
{
	sortedVector = vector;	// copy	
	index.set_size(vector.size());	// set_size

	for (size_t i = 0; i < vector.size(); i++)
	{
		index[i] = i;
	}

	blMath::Q_Sort_Desc(sortedVector, index, 0, sortedVector.size() - 1);
}


/** 
 *	Sorts a vnl_vector in descending order
 *
 *	QuickSort algorithm. O(n*log(n)) <--> O(n^2)
 *
 *	\param vector vector to be ordered
 *	\param index returned vector with indexes: 
 *		sortedVector(i) = original(index(i))
 */
//----------------------------------------------------------------------------
void blMath::QuickSortDesc(vnl_vector<double> & vector,
										vnl_vector<int> & index)
//----------------------------------------------------------------------------
{

	for (size_t i = 0; i < vector.size(); i++)
	{
		index[i] = i;
	}
	blMath::Q_Sort_Desc(vector, index, 0, vector.size() - 1);
}



/**
 *	Sorts a vnl_vector in descending order. Recursive algorithm.
 *
 *	QuickSort algorithm. O(n*log(n)) <--> O(n^2)
 *
 *	Index must be initialized (size and data).
 *
 *	\param vector vector to be sorted
 *	\param index vector of indexes 
 *	\param left	index of left element
 *	\param right index of right element
 */
//----------------------------------------------------------------------------
void blMath::Q_Sort_Desc(vnl_vector<double> & vector,
							vnl_vector<int> & index, int left, int right)
{
//----------------------------------------------------------------------------

	int l_hold, r_hold;

	double pivot;
	int pivotIndex;

	l_hold = left;
	r_hold = right;

	pivot = vector[left];
	pivotIndex = index[left];

	while (left < right)
	{
		while ((vector[right] <= pivot) && (left < right))
		{
			right--;
		}

		if (left != right)	// smaller than pivot
		{
			vector[left] = vector[right];
			index[left] = index[right];

			left++;
		}

		while ((vector[left] >= pivot) && (left < right))
		{
			left++;
		}

		if (left != right)	// greater than pivot
		{
			vector[right] = vector[left];
			index[right] = index[left];
			right--;
		}
	}

	vector[left] = pivot;
	index[left] = pivotIndex;

	if (l_hold < left)
		blMath::Q_Sort_Desc(vector, index, l_hold, left-1);

	if (r_hold > left)
		blMath::Q_Sort_Desc(vector, index, left + 1, r_hold);
}




//	Pointers removed.	vnl_transpose()
/** \brief returns the angle between two subspaces
 *
 *	subspaces are defined by the columns of the params.
 *	If the params are matrices of 1 row or 1 column and normalized
 *	this is the same as ACOS(theMatrix1, theMatrix2)
 */
//----------------------------------------------------------------------------
double blMath::Subspace(vnlMatrixDouble theMatrix1, vnlMatrixDouble theMatrix2)
//----------------------------------------------------------------------------
{
	if (theMatrix1.rows() != theMatrix2.rows() )
		std::cerr << "blMath: Error in subspace. Rows doesn't fit" << std::endl;

	vnl_qr<double> myQr1(theMatrix1);
	vnl_qr<double> myQr2(theMatrix2);

	// [QA,ignore] = qr(A,0);
	// [QB,ignore] = qr(B,0);

	// s = svd(QA'*QB);
	vnl_svd<double> mySvd((vnl_transpose(myQr1.Q()) * myQr2.Q() ), 
							PRECISION);
	
	double minimum = mySvd.W(mySvd.W().size() );	
						// W is sorted from largest to smallest
						// and is a diagonal matrix

	// The smallest singular value gives the largest angle.
	// But make sure it's magnitude is less than 1.
	if (minimum < 1.0)
		return acos(minimum);
	else
		return acos(1.0);
}



//----------------------------------------------------------------------------
/** \brief returns a vector containing the sum of each column of a matrix
 *
 *	emules the sum() operator of MATLAB
 */
vnlVectorDouble blMath::Sum(const vnlMatrixDouble& theMatrix)
{
	size_t i,j;
	
	vnlVectorDouble sum(theMatrix.columns() );
	
	sum.fill(0.0);
	
	for (j = 0; j < theMatrix.columns(); j++)
		for (i = 0; i < theMatrix.rows(); i++)
			// pick all rows of column j
			sum(j) += theMatrix[i][j];
	
	return sum;
}

//----------------------------------------------------------------------------
/** \brief rounds to zero if ceil(datum) < precision
 *
 *	used to avoid troubles with sqrt()
 *	\return 0 if ceil(datum) < precision || datum if not
 */
double blMath::ToZero(double datum, double precision)
{
	if (ceil(datum) < precision )
		return 0.0;
	else
		return datum;
}

//----------------------------------------------------------------------------
/** \brief smallCovariance of two matrices
 *
 *	Computes covariance with the simplified method (if cols < rows) as Jesus done
 *	N.bene. MUST be invoqued WITH a matrix with SAMPLES in columns and vars in rows!!!!!
 */
vnlMatrixDouble blMath::SmallCovarianceMatrix(vnlMatrixDouble a) 
{ 
	vnlMatrixDouble c(a.rows(), a.rows());

	size_t i, j;

// substracting mean -----
	vnl_vector<double> theMean(a.rows() );

	for (i = 0; i < a.rows(); i++)	// avoiding side-effects
		theMean(i) = a.get_row(i).mean();
	
	for (i = 0; i < a.rows(); i++)
		for (j = 0; j < a.columns(); j++)
			a(i,j) = a.get(i,j) - theMean(i);

//--------------------------
  
	// c must have size acols*acols (we assume this)
	for(size_t i = 0; i < a.rows(); i++) 
	{
	    for(size_t j = 0; j < a.rows(); j++) 
		{
			// Use symmetry
			if (i <= j) 
			{
				c[i][j] = 0.0;
				for(size_t k = 0; k < a.cols(); k++) 
				{
					c[i][j] += a[i][k]*a[j][k];	// ��� ???
				}	
				c[i][j] /= a.rows();
				c[j][i] = c[i][j];
			}
		}
	}
	return c;
} 






//added by Costa, 20/11/03
//----------------------------------------------------------------------------
/** \brief Generate a random number in [-1,1)
 *
 *	Uses rand().
 *  if fSeed>0  - then fSeed is used to seed the random number generator
 */
double blMath::SymmetricRandom( double fSeed )
{
    if ( fSeed > 0 )
        srand((unsigned int)fSeed);

    double dRatio = ((double)rand())/((double)(RAND_MAX));
    return (2.0*dRatio - 1.0);
}



/** \public Return
 *   1: number>0 
 *   0: number=0 
 *  -1: number<0 
 */
int blMath::Sign(double number)
{
	int result;

	if(number>0) result=1;
	else if (number==0) result=0;
	else result=-1;

	return result;
}


/** \public Coordinate conversion Cell->Image
 * This is basically x=a*v1+b*v2+...
 * where a, b, .. are elements of vertexWeights (see return value of Coord_Image2Cell)
 * and v1, v2, ... are vertices of tesselation cells
 *
 */
void blMath::Coord_Cell2Image(const vnlMatrixDouble &vertices, const vnlVectorDouble &vertexWeights, vnlVectorDouble &outputPoint)
{
	int NumberOfParameters = vertexWeights.size();
	int NumberOfDimensions = NumberOfParameters-1;

	outputPoint.set_size( NumberOfDimensions ); //number of triangle vertices = number of dimensions + 1
	outputPoint.fill(0);

	int i, j;

	const double *matrixRow; //row of a vertex matrix

	for (i=0; i<NumberOfDimensions; i++)
	{
		matrixRow = vertices[i];

		for(j=0; j<NumberOfParameters; j++)
		{
			outputPoint[i] = outputPoint[i] + vertexWeights[j] * matrixRow[j];
		}
	}

}


/** \public Coordinate conversion Image - > Cell
 * 
 * solving system of numberDimensions+1 equations by Cramer's Rule
 * system:
 *
 *  point = vertices[0] * outputVertexWeights[0] + vertices[1] * outputVertexWeights[1] + ....
 *  1     = outputVertexWeights[0] + outputVertexWeights[1] + ...
 *
 *  point, vertices�[i] ... are given vectors
 *  outputVertexWeights[i] ... are unknown scalars
 *
 *  in order to define system matrices, vectors are appended by 1 and 
 *  copied into the system matrix
 *  
 *  denominator is a constant part of the solution and it is the same for all points
 *  in one trianlgle. If denominator = 0, it will be recalculted, otherwise it is advised to 
 *  use one from previous calculations
 * 
 */
void blMath::Coord_Image2Cell(const vnlMatrixDouble& vertices, const vnlVectorDouble& point, vnlVectorDouble& outputVertexWeights, double& denominator)
//--------------------------------------------------
{
	int i, j;

	int NumberOfDimensions = point.size();
	int NumberOfParameters = NumberOfDimensions + 1; //number of vertex weights = number of dimensions + 1
	

	outputVertexWeights.set_size(NumberOfParameters);

	//matrix of the system of linear equations:
	//point = Sum_by_i ( Vertex[i] * result[i] ) 
	vnlMatrixDouble SystemMatrix(NumberOfParameters, NumberOfParameters); 

	const double *vertexMatrixRow;
	double *systemMatrixRow;

	//set last row to 1's
	systemMatrixRow = SystemMatrix[NumberOfDimensions];
	for( i=0; i<NumberOfParameters; i++) systemMatrixRow[i] = 1;

	//copy vertices to system matrix
	for (i=0; i<NumberOfDimensions; i++)
	{
		systemMatrixRow = SystemMatrix[i];
		vertexMatrixRow = vertices[i];
		for( j=0; j<NumberOfParameters; j++)
		{
			systemMatrixRow[j] = vertexMatrixRow[j];
		}
	}

	//calculate determinant of the system and check that it is not zero	
	if ( denominator==0 ) denominator = vnl_determinant(SystemMatrix);

	assert(denominator!=0);

	//calculate first NumberOfParameters-1 parameters
	double SumOfParameters=0;

	for (i=0; i<NumberOfParameters-1; i++)
	{
		//modify matrix to calculate i-th parameter
		//SystemMatrix.set_column(i, Point);
		for(j=0; j<NumberOfDimensions; j++)
		{
			systemMatrixRow = SystemMatrix[j];
			systemMatrixRow[i] = point[j];
		}
		//:~


		outputVertexWeights[i] = vnl_determinant(SystemMatrix) / denominator;
		SumOfParameters += outputVertexWeights[i];

		//restore matrix
		//SystemMatrix.set_column(i, Vertex[i]);
		for(j=0; j<NumberOfDimensions; j++)
		{
			systemMatrixRow = SystemMatrix[j];
			vertexMatrixRow = vertices[j];
			systemMatrixRow[i] = vertexMatrixRow[i];
		}
		//:~

	}

	//last parameter, it is so because sum of all parameters = 1
	outputVertexWeights[NumberOfParameters-1]=1 - SumOfParameters;
}





/**\brief Calculate number of modes required to retain 
 * variance specified by retainedVariance
 */
int blMath::CalcNumberOfModesRequired( const vnl_vector<double>& eigenValues, double retainedVariance )
{
	size_t i;

	double total = retainedVariance * eigenValues.sum(); // one_norm computes sum of abs values

	double currentVar = 0.0;
	for (i = 0; i < eigenValues.size(); i++)
	{
		currentVar += eigenValues[i];

		if (currentVar >= total) break;
	}

	return i+1;
};





/** \brief Set vector to zero mean and unitary length */
void blMath::ZeroMeanUnitLength( double* vector, int length ) 
{      
	double sqsum = .0, mean = 0;            
	const unsigned int n = length;
	size_t i;
    
	for(i=0; i<n; i++) mean += vector[i];
	mean /= n;

	for(i=0;i<n;i++) 
	{
		vector[i]  -= mean; 
		sqsum += vector[i]*vector[i];
	}

	const double a = 1./sqrt(sqsum);

	for(i=0;i<n;i++) 
	{
		vector[i] *= a;
	}    
}




/** \brief Set vector to zero mean and unit variance*/
void blMath::ZeroMeanUnitVariance( double* vector, const unsigned length)
{
	double mean = 0;            
	unsigned int i;
    
    for(i=0;i<length;i++) 
	{
		mean += vector[i];
	}

	mean /= length;


	//shift to zero mean
	//calculate variance
	double var = 0;
    for(i=0;i<length;i++) 
	{
		vector[i]  -= mean; 
		var += vector[i] * vector[i];
    }

	var = sqrt( var/length ); //complete standard deviation

	//divide by dot product with mean (scale to tangent plane)
	if (var>=DBL_EPSILON) 
	{
	    for(i=0;i<length;i++) 
		{
			vector[i] /= var;
		}
	}
}



/** \brief Set vector to zero mean only */
void blMath::ZeroMean( vnl_vector<double>* vector )
{      
	double mean = vector->mean();

	//shift 
    for(size_t i=0;i<vector->size();i++) 
	{
		(*vector)[i]  -= mean; 
    }

}



/** \brief Shift points to origin 
 *  \param vector - set of coordinates ( x, y, z, x, y, z, ... )
 *  \param dim - dimensioanlity
 */
void blMath::CenterPoints( vnl_vector<double>* vector, unsigned int dim )
{
	//move to zero mean
	double* mean = new double[dim];

	//init to 0
	memset(mean,0, dim*sizeof(mean[0]));

	for( size_t k=0; k<vector->size(); )
	{
		for (size_t d=0; d<dim; d++)
		{
			mean[d] += (*vector)[k++];
		}
	}

	double sz = vector->size()/dim;

	for (size_t d=0; d<dim; d++)
	{
		mean[d] /= sz;
	}

	for( size_t k=0; k<vector->size(); )
	{
		for (size_t d=0; d<dim; d++)
		{
			(*vector)[k++] -= mean[d];
		}
	}

	delete[] mean;
}





void blMath::ZeroMeanTangentProject( double* vector, const double* meanVector, unsigned int length )
{
	//equivalent to blMath::ZeroMeanUnitLength(vector);
	//but with added projection onto tangent space


	unsigned int i;

	ZeroMeanUnitLength( vector, length );

	double dp = 0;
	double mag = 0;
	for(i=0;i<length;i++) 
	{
		dp += vector[i] * meanVector[i];
		mag += meanVector[i] * meanVector[i];
	}

	//divide by dot product with mean (scale to tangent plane)
	if (dp>=DBL_EPSILON) 
	{
		const double k = mag/dp;
		for(i=0;i<length;i++) 
		{
			vector[i] *= k;
		}    
	}
}


/* 
 * Data discretization (or quantization)
 *
 * \param inputData an array of real (double) numbers
 * \paran outputData an array of unsigned long numbers. Each
 *		  of these numbers corresponds to one of the inputData
 *        array and represents the quantization level of that
 *        input, from 0 to numberOfLevels - 1
 * \param numberOfInputs The number of inputs in inputData array
 * \param numberOfLevels The number of levels to use for quantization
 * \param lowerLimit The lowest allowed data value. inputs smaller thatn
 *		  this value will be forced to match it
 * \param upperLimit The highest allowed data value. inputs greater thatn
 *		  this value will be forced to match it 
 * \return true if quantization performed, false otherwise
 */
// ---------------------------------------------------
bool blMath::DataQuantization (const double *inputData,
    long *outputData,
    const unsigned long numberOfInputs,
    const unsigned int numberOfLevels,
    const double lowerLimit, const double upperLimit)
// ---------------------------------------------------
{
    if (outputData == 0)
    {
        // Memory for outputData must be reserved outside the function
        return false;
    }

    // Size definitions based on bounds and numberOfLevels
    // --------------------------------------------------------
    if (numberOfLevels < 2 || !(upperLimit > lowerLimit))
    {
        // Wrong parameters values
        return false;
    }
    double quantStep = (upperLimit - lowerLimit) / numberOfLevels;

    // Quantization and bounding, all at once
    // --------------------------------------------------------
    const double *in_ptr = inputData;
    long *out_ptr = outputData;
    for (unsigned long i = 0; i < numberOfInputs; i++)
    {
        if (*in_ptr < lowerLimit)
        {
            *out_ptr = 0;
        }

        else if (*in_ptr >= upperLimit)
        {
            *out_ptr = numberOfLevels - 1;
        }
        else
        {
            *out_ptr = (long) ((*in_ptr - lowerLimit) / quantStep);
        }
        in_ptr++;
        out_ptr++;
    }
    return true;
}

/* 
 * Complex data discretization in complex plane sectors
 *
 * \param inputData an array of complex<double> numbers
 * \paran outputData an array of unsigned long numbers. Each
 *		  of these numbers corresponds to one of the inputData
 *        array and represents the quantization level of that
 *        input, from 0 to numberOfSectors - 1
 * \param numberOfInputs The number of inputs in inputData array
 * \param numberOfLevels The number of sectors to use for quantization
 * \return true if quantization performed, false otherwise
 */
// ---------------------------------------------------
bool blMath::ComplexSectorization (const std::complex<double> *inputData,
    long *outputData,  const unsigned long numberOfInputs,
    const unsigned int numberOfSectors)
// ---------------------------------------------------
{
    if (outputData == 0)
    {
        // Memory for outputData must be reserved outside the function
        return false;
    }

    // Size definitions based on bounds and numberOfLevels
    // --------------------------------------------------------
    if (numberOfSectors < 2)
    {
        // Wrong parameters values
        return false;
    }
    double quantStep = (2 * PI) / numberOfSectors;

    // Quantization and bounding, all at once
    // --------------------------------------------------------
    const std::complex<double> *in_ptr = inputData;
    long *out_ptr = outputData;
    double z_angle;

    for (unsigned long i = 0; i < numberOfInputs; i++)
    {
        z_angle = std::arg ( *in_ptr );
        if (z_angle < 0)
        {
            z_angle += 2 * PI;
        }
        *out_ptr = (long) (z_angle / quantStep);

        in_ptr++;
        out_ptr++;
    }
    return true;
}




// ###############################################################
// ---------------------------------------------------------------
// ---------------------------------------------------------------
// ###############################################################
#ifdef ENABLE_blMATH_TESTS_BUILDING

namespace blClassesTests 
{
	// ---------------------------------------------------
	void DataQuantization_Test ( unsigned long N )
	// ---------------------------------------------------
	{
		double *randomData;
		long *quantData;
		randomData = new double [N];
		quantData = new long [N];

		srand (1);
		for (unsigned long i = 1; i < N - 1; i++)
		{
			// Generate random numbers from -1 to 11
			randomData [i] = 12 * ((double) rand () / RAND_MAX) - 1;
		}
		randomData [0] = 0;
		randomData [N - 1] = 10;

		// Call quantization
		std::cout << std::endl << "Computing quantization...";
		if (blMath::DataQuantization (randomData, quantData, N, 100,
			0, 10))
		{
			std::cout << "ok";
		}
		else
		{
			std::cout << "ERROR";
		}

		// Display input and output data
		std::cout << std::endl;
		for (unsigned long i = 0; i < N; i++)
		{
			std::cout << "" << randomData[i] << "\t\t" << quantData[i]
				<< std::endl;
		}

		delete [] randomData;
		delete [] quantData;
	}

	// ---------------------------------------------------
	void ComplexSectorization_Test ( unsigned long N )
	// ---------------------------------------------------
	{
		std::complex<double> *randomData;
		long *quantData;
		randomData = new std::complex<double> [N];
		quantData = new long [N];

		double x, y;

		srand (N);
		for (unsigned long i = 0; i < N - 5; i++)
		{
			// Generate random numbers from -1 and 1
			x = -1 + 2 * (double) rand () / RAND_MAX;
			y = -1 + 2 * (double) rand () / RAND_MAX;
			std::complex<double> temp (x, y);
			randomData [i] = temp;
		}
		randomData [N - 4] = std::complex<double> (0, 0);
		randomData [N - 4] = std::complex<double> (1, 0);
		randomData [N - 3] = std::complex<double> (0, 1);
		randomData [N - 2] = std::complex<double> (-1, 0);
		randomData [N - 1] = std::complex<double> (-1, 1);

		// Call quantization
		std::cout << std::endl << "Computing sectorization...";
		if (blMath::ComplexSectorization (randomData, quantData, N, 4))
		{
			std::cout << "ok";
		}
		else
		{
			std::cout << "ERROR";
		}

		// Display input and output data
		std::cout << std::endl;
		for (unsigned long i = 0; i < N; i++)
		{
			std::cout << "" << randomData[i].real() << "\t"
				<< randomData[i].imag() << "\t\t"
				<< quantData[i] << std::endl;
		}

		delete [] randomData;
		delete [] quantData;
	}

		
	// ---------------------------------------------------
	void DataUnion_Test ( unsigned N1, unsigned N2 )
	// ---------------------------------------------------
	{
		// Characters example
		// ------------------
		/*std::vector<char> c1 (7);
		std::vector<char> c2 (5);
		std::vector<char> c3;

		c1[0] = 'A';
		c1[1] = 'B';
		c1[2] = 'C';
		c1[3] = 'D';
		c1[4] = 'E';
		c1[5] = 'F';
		c1[6] = 'G';

		c2[0] = 'd';
		c2[1] = 'O';
		c2[2] = 'G';
		c2[3] = 'M';
		c2[4] = 'A';

		c3 = blMath::DataUnion (c1, c2);*/

		// Integers example
		// ----------------
		unsigned i = 0;
		srand (i);
		std::vector<int> x1 (N1);
		std::vector<int> x2 (N2);
		std::vector<int> x3;

		for (i = 0; i < N1; i++)
		{
			x1[i] = (N1 * rand()) / RAND_MAX;
		}

		for (i = 0; i < N2; i++)
		{
			x2[i] = (N2 * rand()) / RAND_MAX;
		}

		x3 = blMath::DataUnion (x1, x2);
		x3 = blMath::DataUnion (x2, x1);
	}

} // blClassesTests namespace

#endif // ENABLE_blMATH_TESTS_BUILDING

void TestCompile()
{
	baselib::VnlMatrixType mVnl, T;
	baselib::UblasMatrixType mUblas;
	blMath::ComputeSmallCovariance(mVnl, T);
	blMath::ComputeSmallCovariance(mUblas, T);
}


//:~


