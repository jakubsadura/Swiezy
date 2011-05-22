/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blProcrustesAlignmentFilter.h"
#include "blAlignment.h"
#include "blMath.txx"
#include "blSimilarity2DTransform.h"
#include <iostream>
#include <fstream>

/** implementation file for the Procrustes Alignment algorithm
 *
 *  Implements Procrustes (Execute) and some other utility functions for the class.
 */

#include <vnl/vnl_vector_ref.h>

/** \brief Usual generation data method 
 *	
 *	\return int result of operation (-1 means error).
 */
//----------------------------------------------------------------------------
template< class ArgMatrixType >
int blProcrustesAlignmentFilter<ArgMatrixType>::Update()
//----------------------------------------------------------------------------
{
	const bool correctAngle = _correctMeanShapeAngle;
	int iterations=0;
	const int MAX_ITERATIONS = this->maxNumberOfIterations; ///10000;
	double difference = 0.0; 
	int i,v;
	double MIN_CONVERGENCE = this->epsilon; ///1e-30;
	int numberOfDimensions = this->numberOfDimensions;
	int numberOfRows = bllao::NrRows(this->GetMatrix());
	int numberOfLandmarks = this->numberOfPoints;
	//int numberOfComponents = numberOfLandmarks * numberOfDimensions;

	if(this->numberOfInputs == 0)
	{
		std::cerr << "Procrustes: blProcrustesF..: No input!" << std::endl;
		return -1;
    }

	// the number of points has been determined by the first 
	// input (they must all be the same) in SetInput()
	if(numberOfLandmarks <= 0)
    {
		std::cerr << "Procrustes: blProcrustesF...: No points!" << std::endl;
		return 0;
    }

//-------------------------------------------------------------------------------
// 1. GET CENTROIDS TO THE ORIGIN -----------------------------------------------
//-------------------------------------------------------------------------------

	double center[3] = {0.0, 0.0, 0.0};
	double translation[3];

	for (i = 0; i < numberOfRows; i++)
	{
		VectorType centeredShape;
		bllao::CopyRowToVector(this->GetMatrix(), i, centeredShape);
		blAlignment::CenterShapeTo(center, bllao::StartOfMemoryBuffer(centeredShape), 
			bllao::StartOfMemoryBuffer(centeredShape), this->numberOfPoints, this->numberOfDimensions, translation);
		bllao::CopyVectorToRow(this->GetMatrix(), i, centeredShape);
	}

//-------------------------------------------------------------------------------
// 2. TAKING FIRST SHAPE AS INITIAL MEAN ESTIMATION -----------------------------
//-------------------------------------------------------------------------------
	// all the inputs must have the same number of points to consider executing
	// but this has been seen in SetInput()
	baselib::VnlVectorType mean;
	bllao::CopyRowToVector(this->GetMatrix(), 0, mean);

	if (mean.one_norm() == 0)
	{
		std::cerr << "Procrustes: Entrada inicial nula. ERROR." << std::endl;
		return -1;
	}
	
	// If the similarity transform is used, the mean shape must be normalised
	// to avoid shrinking
	if ( !this->doNotRemoveScale )
	{
		mean.normalize();
	}
	
//-------------------------------------------------------------------------------
// 3. RECORDING FIRST ESTIMATION AS FIRSTMEAN  ----------------------------------
//-------------------------------------------------------------------------------
	// our initial estimate of the mean comes from the first example in the set
	// we keep a record of the first mean to fix the orientation and scale
	// (which are otherwise undefined and the loop will not converge)
	VectorType firstMean;
	bllao::Copy(mean, firstMean);

	// storage for the new mean that is being calculated
	vnlVectorDouble newMean;
	bllao::Resize(newMean, bllao::Size(mean));

	#ifdef DEBUG_MESSAGES_ALIGNMENT
		std::cout << "Procrustes: mean, firstmean, newmean, stored and normalized if necessary " << std::endl;
	#endif

	VectorType angles(numberOfRows);
	int converged = 0; // bool converged=false;

	// compute mean and align all the shapes to it, until convergence
	while (!converged)
	{
		#ifdef DEBUG_MESSAGES_ALIGNMENT		
			std::cout << std::endl;
			std::cout << "Procrustes: " << 
				"starting new iteration until convergence or maximum of " << 
				MAX_ITERATIONS << std::endl;
		#endif
		
//-------------------------------------------------------------------------------
// 4. ALIGNING EVERY SHAPE TO THE ESTIMATED MEAN --------------------------------
//-------------------------------------------------------------------------------
		// align each pointset with the mean

		// point coordinates for a shape
		VectorType output(bllao::NrCols(this->GetMatrix())); 
		if (this->numberOfDimensions == 2)
		{
			for(i = 0; i < numberOfRows; i++) 
			{
				VectorType row;
				bllao::CopyRowToVector(this->GetMatrix(), i, row);
				angles[i] = AlignShape2Mean(row, mean, output);	
				bllao::CopyVectorToRow(this->GetMatrix(), i, output);
			}
		}
		else if (this->numberOfDimensions == 3)
		{
			for(i = 0; i < numberOfRows; i++) 
			{
				VectorType row;
				bllao::CopyRowToVector(this->GetMatrix(), i, row);
				AlignShape3Mean(row, mean, output);	
				bllao::CopyVectorToRow(this->GetMatrix(), i, output);
			}
		}
		else
		{
			std::cerr << "ProcrustesFilter: " << this->numberOfDimensions 
						<< "-D alignment not implemented!" << std::endl;
			return -1;
		}

//-------------------------------------------------------------------------------
// 5. ESTIMATING NEW MEAN AFTER ALIGNMENT ---------------------------------------
//-------------------------------------------------------------------------------
		#ifdef DEBUG_MESSAGES_ALIGNMENT		
			std::cout << "Procrustes: each point aligned with the mean" << std::endl;
		#endif

		// compute the new mean (just average the point locations)	
		if ( bllao::Size(_weights) == 0 )
			baselib::ComputeMeanRowImpl(this->GetMatrix(), newMean);
		else 
			baselib::ComputeWeightedMeanRowImpl(this->GetMatrix(), newMean, _weights);

		if( correctAngle )
		{
			if (this->numberOfDimensions == 2 && iterations==0) //only on first iteration, rotate the shape so it is rotated normally
			{
				const double mean_angle= -bllao::Mean(angles);
				blSimilarity2DTransform::Pointer trn = blSimilarity2DTransform::New();
				trn->SetParameters(cos(mean_angle),sin(mean_angle),0,0);
				trn->TransformPoints( newMean );
			}
		}
		
//-------------------------------------------------------------------------------
// 6. ALIGNING NEW ESTIMATION TO FIRST MEAN -------------------------------------
//-------------------------------------------------------------------------------
		// align the new mean with the fixed mean if the transform
		// is similarity or rigidbody. It is not yet decided what to do with affine	
		
		if (this->numberOfDimensions == 2)
		{			
			if( !correctAngle ) 
			{
				// this is weird, I don't want this in my code without an explanation!!!
//<FMS>
//				this->AlignShape2Mean(newMean, firstMean, output);
//	
//				// copy, since newMean cannot be used as output param above
//				newMean = output; 
			}
		}
		else
		{				
			this->AlignShape3Mean(newMean, firstMean, output);

			// copy, since newMean cannot be used as output param above
			bllao::Copy(output, newMean);
		}

		// If the similarity transform is used, the mean shape must be normalised
	    // to avoid shrinking
		if ( !this->doNotRemoveScale )
		{
			newMean.normalize();
		}

		// compute the difference between the newMean and last mean estimated
		difference = 0.0; 
		for(v = 0; v < numberOfLandmarks * numberOfDimensions; v++)
		{
			difference = difference + (mean[v] - newMean[v])*(mean[v] - newMean[v]);
		}

		// the new mean becomes our mean
		mean = newMean;

		#ifdef DEBUG_MESSAGES_ALIGNMENT
			std::cout << "Procrustes: newMean becomes current mean " << std::endl;
		#endif

//-------------------------------------------------------------------------------
// 7. TESTING CONVERGENCE -------------------------------------------------------
//-------------------------------------------------------------------------------
		// test for convergence
		iterations++;
		#ifdef DEBUG_MESSAGES_ALIGNMENT
			std::cout << "Procrustes: Difference after " << 
					iterations << " iteration(s) is: " << difference << std::endl;
		#endif

		if(difference < MIN_CONVERGENCE || iterations >= MAX_ITERATIONS) 
		{
			converged = 1; // true
		}

		// The convergence test is that the sum of the distances between the
		// points on mean(t) and mean(t-1) is less than a very small number.
		// Procrustes shouldn't need more than 2 or 3 iterations but things could go wrong
		// so we impose an iteration limit to avoid getting stuck in an infinite loop
	} // del while !converged


	if(iterations >= MAX_ITERATIONS) 
	{
		std::cerr << "Procrustes did not converge in  " << MAX_ITERATIONS << 
			" iterations! Objects may not be aligned. Difference = " 
				<< difference << std::endl;
		// we don't throw an Error here since the shapes most probably *are* aligned, but the 
		// numerical precision is worse than our convergence test anticipated.
    }
	else 
	{
		#ifdef DEBUG_MESSAGES_ALIGNMENT
			std::cout << "Procrustes required " << iterations << 
				" iterations to converge to " << difference << std::endl;
		#endif
    }

	bllao::Copy(mean, _mean);
	return 0;
}




/** \brief Aligns 2 shapes in 2D. They're supposed to be centered on 0,0
 *	
 *	\return alignedshape
 */
//----------------------------------------------------------------------------
template< class ArgMatrixType >
double blProcrustesAlignmentFilter<ArgMatrixType>::AlignShape2Mean( const VectorType& data, 
													const VectorType& mean,
													VectorType& output)
//----------------------------------------------------------------------------
{	
	bllao::Resize(output, bllao::Size(data));
	double a, b;	// not used here
	blAlignment::AlignShape2D( (data), (mean), (output), this->numberOfPoints, a, b);
	return atan2(b,a); //angle
}




/** \brief Aligns 2 shapes in 2D. They're supposed to be centered on 0,0
 *	
 *	\return alignedshape
 */
//----------------------------------------------------------------------------
template< class ArgMatrixType >
void blProcrustesAlignmentFilter<ArgMatrixType>::AlignShape3Mean(const VectorType & data, 
												const VectorType & mean,
												VectorType & output)
//----------------------------------------------------------------------------
{	
	bllao::Resize(output, bllao::Size(data));
	vnlMatrixDouble transformMatrix(4,4);
	blAlignment::AlignShape3D((data), (mean), (output), this->numberOfPoints, transformMatrix);
}




/**
 *	Set input as a whole matrix. Copy the given matrix.
 *
 *	\param theMatrix input data where shapes are rows and 
 *	point coordinates are columns (xyzxyz..)
 *	\param numberOfPoints number of points of each shape
 *	\param numberOfDimensions number of dimensions of shapes (2D or 3D)
 */
//----------------------------------------------------------------------------
template< class ArgMatrixType >
void blProcrustesAlignmentFilter<ArgMatrixType>::SetInput(const MatrixType& theMatrix, 
											unsigned int numberOfPoints, 
											unsigned int numberOfDimensions)
//----------------------------------------------------------------------------
{
	this->GetMatrix() = theMatrix;
	this->numberOfInputs = bllao::NrRows(this->GetMatrix());
	this->numberOfDimensions = numberOfDimensions;
	this->numberOfPoints = numberOfPoints;

	#ifdef DEBUG_MESSAGES_ALIGNMENT
		std::cout << "blProcrustes: input dimension = (" << bllao::NrRows(this->GetMatrix()) <<
			"," << bllao::NrCols(this->GetMatrix()) << ")" << std::endl;
	
		std::cout << "Procrustes: setting input" << std::endl;
	#endif

	if (this->numberOfDimensions < 2 || this->numberOfDimensions > 3)
	{
		this->numberOfDimensions = 2;
		#ifdef DEBUG_MESSAGES_ALIGNMENT
			std::cout << "Procrustes: number of dimensions set to 2" << std::endl;
		#endif
	}
}

/** \brief Standard constructor
 *	
 *	Initializes dimensions to 2, points to -1 (allowing to distinguish when
 *	it has not be init).
 */
//----------------------------------------------------------------------------
template< class ArgMatrixType >
blProcrustesAlignmentFilter<ArgMatrixType>::blProcrustesAlignmentFilter(MatrixPointer output)
: _correctMeanShapeAngle(false)
//----------------------------------------------------------------------------
{
	// Set m_Data to a valid matrix for storing the aligned shapes.
	this->m_Data = output == NULL ? MatrixNamespace::New() : output;

	// 2D and no input by default
	this->numberOfInputs = 0;
	this->numberOfDimensions = 2;
	this->numberOfPoints = 0;

	// default values
	this->epsilon = 1e-20;
	this->maxNumberOfIterations = 1000;

	// WARNING: default for this parameter MUST be false - Please do not modify
	this->doNotRemoveScale = false;
}



/** \brief Standar destructor
 *	
 */
//----------------------------------------------------------------------------
template< class ArgMatrixType >
blProcrustesAlignmentFilter<ArgMatrixType>::~blProcrustesAlignmentFilter()
//----------------------------------------------------------------------------
{
}
