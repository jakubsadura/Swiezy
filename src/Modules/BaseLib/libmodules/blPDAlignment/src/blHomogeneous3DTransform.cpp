/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <iostream>
#include <vnl/algo/vnl_matrix_inverse.h>

#include "blHomogeneous3DTransform.h"


/**
 *	Constructor
 */
//--------------------------------------------------
blHomogeneous3DTransform::blHomogeneous3DTransform()
//--------------------------------------------------
{
	// identity transformation matrix
	this->matrix.set_size(4,4);
	this->matrix.set_identity();
}



/**
 *	Destructor.
 */
//--------------------------------------------------
blHomogeneous3DTransform::~blHomogeneous3DTransform()
//--------------------------------------------------
{

}



/**
 *	Returns the transformation matrix.
 */
//--------------------------------------------------
vnl_matrix<double> & blHomogeneous3DTransform::GetMatrix()
//--------------------------------------------------
{
	return this->matrix;
}



/**\brief creates the inverse transformation */
//--------------------------------------------------
blHomogeneous3DTransform::Pointer blHomogeneous3DTransform::Inverse()
//--------------------------------------------------
{

	blHomogeneous3DTransform::Pointer inverseTransformPt = 
										blHomogeneous3DTransform::New();

	// inverse matrix

	vnl_matrix_inverse<double> inverseMatrix(this->matrix);
	inverseTransformPt->matrix = inverseMatrix.inverse();

	return inverseTransformPt;
}



/**\brief sets matrix that defines a new transform */
//--------------------------------------------------
void blHomogeneous3DTransform::SetMatrix(const vnl_matrix<double> & matrix4x4)
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_blHOMOGENEOUS3DTRANSFORM
	std::cout << "blHomogeneous3DTransform::SetMatrix(): " 
				<< "Setting transformation matrix " << std::endl;
#endif

	this->matrix.copy_in(matrix4x4.data_block());

}




/**\brief applies the transform on the points */
//--------------------------------------------------
void blHomogeneous3DTransform::TransformPoints(const double points[],
								double outputPoints[],
								unsigned int numberOfPoints) const
//--------------------------------------------------
{

	unsigned int xIndex = 0;

	for (size_t i = 0; i < numberOfPoints; i++)
	{
		outputPoints[xIndex] = this->matrix[0][0] * points[xIndex] +
								this->matrix[0][1] * points[xIndex + 1] +
								this->matrix[0][2] * points[xIndex + 2] +
								this->matrix[0][3];

		outputPoints[xIndex + 1] = this->matrix[1][0] * points[xIndex] +
									this->matrix[1][1] * points[xIndex + 1] +
									this->matrix[1][2] * points[xIndex + 2] +
									this->matrix[1][3];

		outputPoints[xIndex + 2] = this->matrix[2][0] * points[xIndex] +
									this->matrix[2][1] * points[xIndex + 1] +
									this->matrix[2][2] * points[xIndex + 2] +
									this->matrix[2][3];

		xIndex += 3;	// dimensions
	}

}


/**
 *	This method transforms an set of points applying this transform.
 *
 *	\param points input points to be transformed
 *	\param outputPoints transformed points
 *	\param numberOfPoints number of points in points vector
 */
//--------------------------------------------------
void blHomogeneous3DTransform::TransformPoints(
										const vnl_vector<double> & points,
										vnl_vector<double> & outputPoints,
										unsigned int numberOfPoints) const
//--------------------------------------------------
{

	outputPoints.set_size(3 * numberOfPoints);
	this->TransformPoints(points.data_block(), 
							outputPoints.data_block(), numberOfPoints);

}


/**
 *
 */
//--------------------------------------------------
void blHomogeneous3DTransform::TransformPoints( vnl_vector<double> & points, 
												unsigned int numberOfPoints) const
//--------------------------------------------------
{
	this->TransformPoints(points.data_block(), 
							points.data_block(), numberOfPoints);

}




