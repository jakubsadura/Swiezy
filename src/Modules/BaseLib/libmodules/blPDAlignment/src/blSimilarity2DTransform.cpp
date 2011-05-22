/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//This class defines a similarity transform for 2D points

#include <iostream>
#include "blSimilarity2DTransform.h"

/**
 *	Constructor
 */
//--------------------------------------------------
blSimilarity2DTransform::blSimilarity2DTransform()
//--------------------------------------------------
{
	this->a = 1.0;
	this->b = 0.0;
	this->tx = 0.0;
	this->ty = 0.0;
}



/**
 *	Destructor.
 */
//--------------------------------------------------
blSimilarity2DTransform::~blSimilarity2DTransform()
//--------------------------------------------------
{
}


/**\brief sets new params that defines a new transform */
//--------------------------------------------------
void blSimilarity2DTransform::SetParameters(double a, double b, 
											double tx, double ty)
//--------------------------------------------------
{
	this->a = a;
	this->b = b;
	this->tx = tx;
	this->ty = ty;
}



/**
 *	This method transforms an set of points applying this transform.
 *
 *	\param points input points to be transformed
 *	\param outputPoints transformed points
 *	\param numberOfPoints number of points in points vector
 */
//--------------------------------------------------
void blSimilarity2DTransform::TransformPoints(
										const vnl_vector<double> & points,
										vnl_vector<double> & outputPoints,
										unsigned int numberOfPoints)
//--------------------------------------------------
{
	outputPoints.set_size(points.size());

	unsigned int xIndex = 0;

	for (size_t i = 0; i < numberOfPoints; i++)
	{
		outputPoints[xIndex] = this->a * points[xIndex] 
								- this->b * points[xIndex + 1] + this->tx;

		outputPoints[xIndex + 1] = this->b * points[xIndex] 
								+ this->a * points[xIndex + 1] + this->ty;

		xIndex += 2;
	}


}


void blSimilarity2DTransform::TransformPoints( vnl_vector<double> & points )
//--------------------------------------------------
{
	double x,y;

	for (unsigned int xIndex = 0; xIndex < points.size(); xIndex+=2)
	{
		x = points[xIndex];
		y = points[xIndex + 1];

		points[xIndex] = this->a * x 
								- this->b * y + this->tx;

		points[xIndex + 1] = this->b * x 
								+ this->a * y + this->ty;

	}


}



/**
 *	This method applies the inverse transform on a set of points. 
 *	So, first the translation and then the rotation-scaling
 *
 *	\param points input points to be transformed
 *	\param outputPoints transformed points
 *	\param numberOfPoints number of points in points vector
 */
//--------------------------------------------------
void blSimilarity2DTransform::InverseTransformPoints(
										const vnl_vector<double> & points,
										vnl_vector<double> & outputPoints,
										unsigned int numberOfPoints)
//--------------------------------------------------
{
	double scale2 = this->a * this->a + this->b * this->b;


	double inverseA = this->a / scale2;
	double inverseB = this->b / scale2;

	unsigned int xIndex = 0;

	for (size_t i = 0; i < numberOfPoints; i++)
	{
		outputPoints[xIndex] = 
			inverseA * (points[xIndex] - this->tx)
				+ inverseB * (points[xIndex + 1] - this->ty);

		outputPoints[xIndex + 1] = 
			inverseA * (points[xIndex + 1] - this->ty)
				- inverseB * (points[xIndex] - this->tx);

		xIndex += 2;
	}

}



void blSimilarity2DTransform::InverseTransformPoints( vnl_vector<double> & points )
//--------------------------------------------------
{
	double scale2 = this->a * this->a + this->b * this->b;


	double inverseA = this->a / scale2;
	double inverseB = this->b / scale2;

	double x,y;

	for (unsigned int xIndex  = 0; xIndex < points.size(); xIndex+=2)
	{
		x = points[xIndex];
		y = points[xIndex + 1];

		points[xIndex] = 
			inverseA * (x - this->tx)
				+ inverseB * (y - this->ty);

		points[xIndex + 1] = 
			inverseA * (y - this->ty)
				- inverseB * (x - this->tx);
	}

}
