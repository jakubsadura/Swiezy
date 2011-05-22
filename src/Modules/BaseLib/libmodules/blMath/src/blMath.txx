/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLMATH_TXX
#define BLMATH_TXX

#include "CILabNamespaceMacros.h"
#include "blLinearAlgebraOperations.h"

CILAB_BEGIN_NAMESPACE(baselib)

template< class MatrixType, class VectorType >
void ComputeMeanRowImpl(const MatrixType& theMatrix, VectorType& theMean)
{
	int r,c;
	int rows = bllao::NrRows(theMatrix);
	int cols = bllao::NrCols(theMatrix);

	bllao::Resize(theMean, cols);
	bllao::Fill(theMean, 0);

	for (r = 0; r < rows; r++)
	{
		for (c = 0; c < cols; c++)
		{
			theMean[c] += theMatrix(r, c);
		}
	}

	for (c = 0; c < cols; c++)
	{
		theMean[c] /= rows;
	}
}

template< class MatrixType, class VectorType1, class VectorType2 >
void ComputeWeightedMeanRowImpl(const MatrixType& theMatrix, VectorType1& theMean, const VectorType2& weights)
{
	int r,c;
	int rows = bllao::NrRows(theMatrix);
	int cols = bllao::NrCols(theMatrix);

	bllao::Resize(theMean, cols);
	bllao::Fill(theMean, 0);

	for (r = 0; r < rows; r++)
	{
		for (c = 0; c < cols; c++)
		{
			theMean[c] += theMatrix(r, c) * weights[r];
		}
	}
}

CILAB_END_NAMESPACE(baselib)

#endif //BLMATH_TXX

