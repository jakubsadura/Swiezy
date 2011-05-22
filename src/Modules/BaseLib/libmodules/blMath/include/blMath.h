/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blMath_h
#define _blMath_h

#include "blMath.txx"
#include "blTextUtils.h"

//#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <complex>		// for ComplexSectorization()
#include <vector>		// for DataUnion()

#include <vnl/algo/vnl_qr.h>
#include <vnl/algo/vnl_svd.h>
#include <vnl/algo/vnl_determinant.h>

//#include "blDefines.h"
#include "BaseLibWin32Header.h"

#define ENABLE_blMATH_TESTS_BUILDING
#define PI 3.14159265

#ifndef __min
#define __min(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef __max
#define __max(a,b) ((a)>(b)?(b):(a))
#endif

//cut x to be in [a,b]
#define __limit(x,a,b) ( ((x)<(a))?(a):( ((x)>(b))?(b):(x) ) )

typedef vnl_matrix<double> vnlMatrixDouble;
typedef vnl_vector<double> vnlVectorDouble;
//typedef std::vector<float> VectorOfFloatType;
//typedef std::vector<int> VectorOfIntType;




/**
 * \brief This implements some utilities, like covariances or products...
 * \ingroup blMath
 *
 *  Implements Covariance, SmallCovariance, etc.
 *
 *	blMath started as a class containing covar computations and things
 *	like that, but has evolve into a namespace grouping all functions
 *	general enough to be in any part of the library or even out of it.
 */
class BASELIB_EXPORT blMath
{
public: 

// static const double PI;
	
//different order of multiplication from vnl's standard - saves time when U is small rectangular, V is usually huge
//tries to optimize speed
template <class TElement>
static vnl_matrix<TElement> pinverse( vnl_matrix<TElement>& matr )
{
	vnl_svd<TElement> svd( matr );

	vnl_matrix<TElement> Winverse( svd.Winverse().rows(), svd.Winverse().columns() );
	Winverse.fill(TElement(0));
	for (unsigned int i=0; i<svd.rank(); ++i)
		Winverse(i,i) = (svd.Winverse())(i,i);

	if( svd.V().rows() > svd.U().rows() )
		return svd.V() * ( Winverse * svd.U().conjugate_transpose() );
	else
		return ( svd.V() * Winverse ) * svd.U().conjugate_transpose();
}


//returns random number with Gaussian distribution with 0 mean and variance 1
//implements the polar form of the Box-Muller algorithm
static double GaussRand();

/** public Compute a mod b (not C++ remainder)
 * for some unexplainable reason, putting its body in .cpp file
 * caused linker error about nonexisting function
*/
static inline double Mod(int a, int b)
{
	double result = a%b;
	if( result<0 ) result += b;
	return result;
}




template <class T> 
static int GetShapeBoundingBox( vnl_vector<T>* shape, unsigned int dim, vnl_vector<T>* upper_left, vnl_vector<T>* lower_right )
{
	upper_left->set_size(dim);
	lower_right->set_size(dim);

	upper_left->copy_in(shape->begin());
	lower_right->copy_in(shape->begin());

	for( int i=0; i<shape->size(); )
	{
		for( int j=0; j<dim; j++, i++)
		{
			if( (*shape)[i] < (*upper_left)[j] ) (*upper_left)[j] = (*shape)[i];
			else if ( (*shape)[i] > (*lower_right)[j] ) (*lower_right)[j] = (*shape)[i];
		}
	}

	return 0;
};


/** 
* \brief Computes weighted covariance (shapes in rows)
* Implements standard method for weighted covariance.
* This writes the new covariance matrix in the given matrix.
*/
static void ComputeWeightedCovariance( const vnlMatrixDouble& theMatrix,
                                       vnl_vector<double>& weights,
                                       vnl_vector<double>& theMean,
                                       vnlMatrixDouble& covarianceMatrix );


// vnl_fastops::AtA()
/** \brief computes covariance (shapes in rows)
*
*  Implements standard method for covariance.
*
*	This writes the new covariance matrix in given matrix 	
*/
template< class MatrixType >
static void ComputeCovariance(const MatrixType& theMatrix, MatrixType& covarianceMatrix)
{	
	MatrixType auxMatrix;
	vnl_vector<double> theMean;
	baselib::ComputeMeanRowImpl(theMatrix, theMean);
	ComputeRowSubstraction(theMatrix, theMean, auxMatrix);
	bllao::Resize(covarianceMatrix, bllao::NrCols(auxMatrix), bllao::NrCols(auxMatrix));
	bllao::AtA(covarianceMatrix, auxMatrix);
	const double factor = 1.0 / (bllao::NrRows(auxMatrix) - 1);
	covarianceMatrix *= factor;
}

static void ComputeMeanAndCov(const vnlMatrixDouble& theMatrix,
	vnlVectorDouble& theMean, vnlMatrixDouble & covarianceMatrix);

/** public Implements standard method for mean	BY ROWS 
 */
static void ComputeMeanColumn(const vnlMatrixDouble& theMatrix,
									vnl_vector<double>& theMean);

/** public Implements computing mean of a matrix BY COLUMNS
 */
static void ComputeMeanRow(const vnlMatrixDouble& theMatrix, vnl_vector<double>& theMean);

/** one weight per row, weights sum up to 1
*/
static void ComputeWeightedMeanRow(const vnlMatrixDouble& theMatrix,
									vnl_vector<double>& theMean, vnl_vector<double>& weights);


/** \brief computes matrix(col) - vector for each row in matrix
*
*  Implements computing substraction of a row vector from a matrix
*	\param matrixResult resulting matrix. not need to be set_sized outside.
*/
template< class MatrixType, class VectorType >
static void ComputeRowSubstraction(const MatrixType& matrixSource, const VectorType& vectorSource,
									MatrixType& matrixResult)
{
	int i,j;
	const int nrRows = bllao::NrRows(matrixSource);
	const int nrCols = bllao::NrCols(matrixSource);

	bllao::Resize(matrixResult, nrRows, nrCols);

	if (nrCols != bllao::Size(vectorSource) )
	{
		std::cerr << "blMath: computeSubstraction error! different number of columns!" << std::endl;
	}


	for (i = 0; i < nrRows; i++)
	{
		for (j = 0; j < nrCols; j++)
		{
			matrixResult(i, j) = matrixSource(i, j) - vectorSource[j];
		}
	}
}

/** \brief smallCovariance of two matrices
*
*	Computes covariance with the simplified method (if cols < rows) as vtk done
*	Changed for being called with samples in rows
*/
template< class InputMatrixType, class OutputMatrixType >
static void ComputeSmallCovariance( InputMatrixType& theMatrix, OutputMatrixType& T, bool dispProgress = false)
{

	//vnl_matrix<double> auxMatrix;		
	vnl_vector<double> theMean(bllao::NrCols(theMatrix) );

	//this must be:
	//in each row calculate mean (scalar, not vector)
	//and in each row substract it from all the elements
	//so each row has it's own mean!
	//This operation means that each vector (they are in rows) is centerred at its zero 

	if (dispProgress)
	{
		std::cout << "\nCovariance: Removing mean row...";
	}

	baselib::ComputeMeanRowImpl(theMatrix, theMean);
	blMath::ComputeRowSubstraction(theMatrix, theMean, theMatrix); //auxMatrix);

	if (dispProgress)
	{
		std::cout << "done" << std::endl;
	}
	

	// so we use the other instead.
	//	T = auxMatrix * vnl_transpose(auxMatrix); // DIFFERENT FROM THE NORMAL METHOD

	//	T.set_size(auxMatrix.rows(), auxMatrix.rows());
	bllao::Resize(T, bllao::NrRows(theMatrix), bllao::NrRows(theMatrix));

	// T = auxMatrix * auxMatrix';
	//	vnl_fastops::ABt(auxMatrix, auxMatrix, &T);
	
	// The following two lines are commented temporarily......I don't know
	// why we were using them...........
	// const InputMatrixType& inputMatrix = theMatrix;
	// InputMatrixType temp; //, A(inputMatrix), Bt(inputMatrix);
	// -------

	// If the matrix is stored on disk, then this operation is not very efficient using Ublas:prod	
	// bllao::ABt(temp, inputMatrix, inputMatrix);
	// bllao::CopyMatrix(temp, T);
	// ------
	// So we'll do it "manually" reading a row at a time
	// ######## Manual operation starts here
	if (dispProgress)
	{
		std::cout << "Covariance: computing self product...";
	}
	
	// Calculation of the number of operations required
	unsigned int totalNops = 0;
	unsigned int numberOfRows = bllao::NrRows(theMatrix);	
	for (unsigned int jR = 0; jR < numberOfRows; jR++)
	{
		// At each cicle we have to read (and multiply):
		// - One row by itself (1 op)
		// - One row by all other rows AFTER it (numberOfRows-jR-1 ops)
		totalNops += (numberOfRows - jR);
	}	
	
	blTextProgressBar::Pointer barra;
	if (dispProgress)
	{
		// Setup progress bar
		barra = blTextProgressBar::New();
		std::cout << std::endl;		
		barra->DrawBackground ();
		barra->SetRange ( 0, totalNops );
	}

	// Start calculations
	vnl_vector<double> aux_row;
	vnl_vector<double> aux_row_transp;	
	aux_row.set_size( numberOfRows );
	aux_row_transp.set_size( numberOfRows );

	// jR is the row index (of the final matrix)
	double theProduct;	
	for (unsigned int jR = 0; jR < numberOfRows; jR++)
	{
		// std::cout << "\tAtA: row " << jR << " out of " << numberOfRows << std::endl;
		bllao::CopyRowToVector(theMatrix, jR, aux_row);
		theProduct = dot_product(aux_row, aux_row);
		T(jR, jR) = theProduct;
		if (dispProgress)
		{	
			barra->Add( 1 );
		}

		// jC is the column index (of the final matrix)
		for (unsigned int jC = jR + 1; jC < numberOfRows; jC++)
		{
			bllao::CopyRowToVector(theMatrix, jC, aux_row_transp);
			theProduct = dot_product(aux_row, aux_row_transp);
			T(jR, jC) = theProduct;
			T(jC, jR) = theProduct;
			if (dispProgress)
			{
				barra->Add( 1 );
			}
		}		
	}

	if (dispProgress)
	{
		// Force bar status to 100%
		barra->SetStatus (1);
		std::cout << std::endl << std::endl;
	}
	// ######## Manual operation ends here

	//	T /= auxMatrix.rows(); // - 1;
	const double factor = 1.0 / (bllao::NrRows(theMatrix) - 1);
	T *= factor;
}

/** public Computes matrix(col) - vector on each column of matrix.
 */
static vnlMatrixDouble ComputeSubstraction(const vnlMatrixDouble& matrix, 
										   const vnlVectorDouble& vector);

/** public Computes variance of a vector of data
 * if the_mean is not NULL also the mean is returned (it is calculated anyway)
 */
static double ComputeVariance(const vnlVectorDouble& theVector, double* the_mean=NULL);
static double ComputeVariance(const double* theVector, unsigned int length, double* the_mean=NULL);



/** public Vectorial product (component by component) of two matrices
 */
static vnlMatrixDouble DivisionComp2Comp(const vnlMatrixDouble& matrix1, 
										 const vnlMatrixDouble& matrix2);

/** public Vectorial product (component by component) of two vectors
 */
static vnlVectorDouble DivisionComp2Comp(const vnlVectorDouble& vector1, 
										 const vnlVectorDouble& vector2);

/** \public Inner Vectorial product (dot) of two vectors
 */
static double Dot(const vnlVectorDouble& v1, 
				  const vnlVectorDouble& v2);

/** \brief compute gaussian with given mean and stddev
 */
static double Gaussian(double mean, double stddev, double x);

/**	\public generate a matrix representing Quad forms
*/
static vnlMatrixDouble GenerateQuads(int numberShapes, 
									double magnitude = 1.0, 
									double proportion = 1.0,
									double errorMagnitude = 0.0);

/** \public implements MATLAB all() function.
 */
static int Have_zeros(const vnlVectorDouble& theVector);


/** \brief Linear alignment of this vector to another vector using the L2 norm.
 * 
 */
static void AlignVectors(	double *vector, double *target, unsigned length, double *pA=NULL, double *pB=NULL	);

/** public computes median as the medium value of theVector sorted
 * Median is inefficient, use QuickSelect instead
 * For enormous arrays Torben's method is more efficient
 * right - is the index of the last element ( list[right] must exist )
 */
static double Median(const vnlVectorDouble& theVector);
static int partition(double* list, int left, int right, int pivotIndex);
static double QuickSelect(double *list, int left, int right, int k); //this implementation changes order of list
static double QuickSelect(std::vector<double>& array,int k);
static double QuickSelect(vnl_vector<double>& array,int k);

/** public computes median as the vector of mediums of rows or columns
 */
static vnlVectorDouble Median(const vnlMatrixDouble& theMatrix, int type);


/** public print a vnlMatrixDouble in stdin
 */
static void PrintMatrix(const vnlMatrixDouble& theMatrix, 
						char *name);

/** \brief print a vnlVectorDouble in stdin
 */
static void PrintVector(const vnlVectorDouble& theVector, 
						char *name);

/** public Vectorial product (component by component) of two vectors
 */
static vnlVectorDouble ProductComp2Comp(const vnlVectorDouble& vector1, 
										const vnlVectorDouble& vector2);

/** public Vectorial product (component by component) of two matrices
 */
static vnlMatrixDouble ProductComp2Comp(const vnlMatrixDouble& matrix1, 
										const vnlMatrixDouble& matrix2);


/**\brief sorts in ascending way an input vector and 
returns the sorted vector and an index vector*/
static void QuickSortAsc( vnl_vector<double> & vector, 
								vnl_vector<double> & sortedVector,
									vnl_vector<int> & index);

/**\brief sorts in ascending way an input vector and 
returns an index vector*/
static void QuickSortAsc(vnl_vector<double> & vector,
										vnl_vector<int> & index);



/**\brief sorts in descending way an input vector and 
returns the sorted vector and an index vector*/
static void QuickSortDesc(const vnl_vector<double> & vector, 
									vnl_vector<double> & sortedVector,
										vnl_vector<int> & index);

/**\brief sorts in descending way an input vector and
and return also an index vector */
static void QuickSortDesc(vnl_vector<double> & vector,
										vnl_vector<int> & index);




/** public Computes angle between subspaces
 */
static double Subspace(vnlMatrixDouble theMatrix1, vnlMatrixDouble theMatrix2);

/** public Computes sum of each column of theMatrix
 */
static vnlVectorDouble Sum(const vnlMatrixDouble& theMatrix);

/** public Computes covariance with the simplified method
 */
static vnlMatrixDouble SmallCovarianceMatrix(vnlMatrixDouble a);

/** \public rounds to zero if ceil(datum) < precision
 */
static double ToZero(double datum, double precision);


//added by Costa, 20/11/03
/** \public Generate a random number in [-1,1)
 */
static double SymmetricRandom( double fSeed = 0 );

/** \public Return
 *   1: number>0 
 *   0: number=0 
 *  -1: number<0 
 */
static int Sign(double number);


static double MSE( const vnlVectorDouble& v1, const vnlVectorDouble& v2, const int number_of_landmarks );


/** \public Coordinate conversion Cell->Image
 * This is basically x=a*v1+b*v2+...
 * where a, b, .. are elements of vertexWeights (see return value of Coord_Image2Cell)
 * and v1, v2, ... are vertices of tesselation cells
 *
 */
static void Coord_Cell2Image(const vnlMatrixDouble &vertices, const vnlVectorDouble &vertexWeights, vnlVectorDouble &outputPoint);


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
static void Coord_Image2Cell(const vnlMatrixDouble& vertices, const vnlVectorDouble& point, vnlVectorDouble& outputVertexWeights, double& denominator);


/**\brief Calculate number of modes required to retain 
 * variance specified by retainedVariance
 */
static int CalcNumberOfModesRequired ( const vnl_vector<double>& eigenValues, double retainedVariance );


/** \brief Set vector to zero mean and unit length*/
static void ZeroMeanUnitLength( double* vector, int length );

static void ZeroMeanUnitLength( vnl_vector<double>* vector )
{
	ZeroMeanUnitLength( vector->begin(), vector->size() );
};


/** \brief Set vector to zero mean and unit variance*/
static void ZeroMeanUnitVariance( vnl_vector<double>* vector )
{
	ZeroMeanUnitVariance( vector->data_block(), vector->size() );
}

/** \brief Set vector to zero mean and unit variance*/
static void ZeroMeanUnitVariance( double* vector, const unsigned length);

/** \brief Set vector to zero mean only */
static void ZeroMean( vnl_vector<double>* vector );

/** \brief Shift points to origin 
 *  \param vector - set of coordinates ( x, y, z, x, y, z, ... )
 *  \param dim - dimensioanlity
 */
static void CenterPoints( vnl_vector<double>* vector, unsigned int dim );


/** \brief Shift elements of the vector to zero mean and project onto the tangent space to the mean vector
 *  \param vector - the vector to project
 *  \param meanVector - mean vector 
 *  \param length - vector length
 */
static void ZeroMeanTangentProject( double* vector, const double* meanVector, unsigned int length );


/** \brief Real data quantization*/
static bool DataQuantization (const double *inputData,
    long *outputData,
    const unsigned long numberOfInputs,
    const unsigned int numberOfLevels,
    const double lowerLimit, const double UpperLimit);

/** \brief Complex data quantization in complex plane sectors*/
static bool ComplexSectorization (const std::complex<double> *inputData,
    long *outputData,
    const unsigned long numberOfInputs,
    const unsigned int numberOfLevels);


/**
  * Computes the union of two sets of data
  * The repeated elements are excluded, unless they're in the
  * same input set. That is:
  *     dataUnion ({1, 2, 3, 4}, {3, 4, 5}) = {1, 2, 3, 4, 5}
  * but
  *     dataUnion ({1, 2, 3, 1}, {3, 4, 5}) = {1, 2, 3, 1, 4, 5})
  *
  * In the last case you could solve the problem by swapping the sets
  * because the elements in the second set are checked against
  * repetitino:
  *     dataUnion ({3, 4, 5}, {1, 2, 3, 1}) = {3, 4, 5, 1, 2}
  */ 
// ---------------------------------------------------
template<class T> static std::vector<T> DataUnion (
    std::vector<T> dataSet1, std::vector<T> dataSet2)
// ---------------------------------------------------
{
    std::vector<T> unionSet = dataSet1;
    unsigned unionSize = unionSet.size();
    bool repetido;
    unsigned j;

    for (unsigned i = 0; i < dataSet2.size(); i++)
    {
        repetido = false;
        j = 0;
        while (j < unionSize)
        {
            // Check if element is already in the union
            if ( dataSet2[i] == unionSet[j] )
            {
                repetido = true;
                j = unionSize;
            }
            j++;
        }
        if ( !repetido )
        {
            unionSize++;
            unionSet.resize (unionSize);
            unionSet[unionSize - 1] = dataSet2[i];
        }
    }

    #ifdef SHOW_DATAUNION_DEBUG_MESSAGES
        std::cout << "DataUnion:" << std::endl;
        std::cout << "\tSet 1 = ";
        for (unsigned i = 0; i < dataSet1.size(); i++)
        {
            std::cout << dataSet1[i] << ", ";
        }
        std::cout << std::endl << "\tSet 1 = ";
        for (unsigned i = 0; i < dataSet2.size(); i++)
        {
            std::cout << dataSet2[i] << ", ";
        }
        std::cout << std::endl << "\tUnion = ";
        for (unsigned i = 0; i < unionSet.size(); i++)
        {
            std::cout << unionSet[i] << ", ";
        }
        std::cout << std::endl;
    #endif // SHOW_DATAUNION_DEBUG_MESSAGES


    return unionSet;
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
static void Q_Sort_Asc(std::vector<int> & vector,
					   std::vector<int>& index, 
					   int left, 
					   int right);


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
static void Q_Sort_Asc(std::vector<float> & vector,
 			    	   std::vector<int>& index, 
				       int left, 
				       int right);


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
static void Q_Sort_Asc(std::vector<float> & vector,
 			    	   std::vector<float>& index, 
				       int left, 
				       int right);

private:

/**\brief sorts in ascending way an input vector and
returns also an index vector*/
static void Q_Sort_Asc(vnl_vector<double> & vector,
						vnl_vector<int> & index, int left, int right);




/**\brief sorts in descending way an input vector and return an index vector*/
static void Q_Sort_Desc(vnl_vector<double> & vector,
							vnl_vector<int> & index, int left, int right);


};

#ifdef ENABLE_blMATH_TESTS_BUILDING

namespace blClassesTests 
{

	void DataQuantization_Test ( unsigned long N );

	void ComplexSectorization_Test ( unsigned long N );

	void DataUnion_Test ( unsigned N1, unsigned N2 );

} // blClassesTests namespace

#endif // ENABLE_blMATH_TESTS_BUILDING

#endif // _blMath_h
