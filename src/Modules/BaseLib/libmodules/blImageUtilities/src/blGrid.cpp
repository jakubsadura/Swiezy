/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include <cmath>
#include <vnl/vnl_vector_fixed.h>
#include "blGrid.h"

/**
 *	Constructor.
 *
 *	Default params:
 *	
 *	- radius = 1
 *	- central point = (0,0)
 *	- spacing = 1.0
 *	- normal = (0,0)
 */
//-----------------------------------------------------------
blGrid::blGrid()
//-----------------------------------------------------------
{
	this->SetRadius(1);
	
	// central point[d] = 0.0 for all dimensions
	int d;
	for (d = 0; d < blGRID_DIMENSION; d++)
	{
		this->centralPoint[d] = 0.0;
	}

	this->spacing = 1.0;

	this->normal_WE[0] = 0.0;
	this->normal_WE[1] = 0.0;

	this->normal_SN[0] = 0.0;
	this->normal_SN[1] = 0.0;
}



/**
 *	Constructor.
 *
 *	Default params:
 *	
 *	- central point = (0,0)
 *	- spacing = 1.0
 *	- normal = (0,0)
 *
 *	\param radius: number of positions at each side of the central point
 */
//-----------------------------------------------------------
blGrid::blGrid(unsigned int radius)
//-----------------------------------------------------------
{
	this->SetRadius(radius);

	// central point[d] = 0.0 for all dimensions
	int d;
	for (d = 0; d < blGRID_DIMENSION; d++)
	{
		this->centralPoint[d] = 0.0;
	}

	this->spacing = 1.0;

	this->normal_WE[0] = 0.0;
	this->normal_WE[1] = 0.0;

	this->normal_SN[0] = 0.0;
	this->normal_SN[1] = 0.0;
}



/**
 *	Destructor
 */
//-----------------------------------------------------------
blGrid::~blGrid() 
//-----------------------------------------------------------
{

}


/**
 *	Returns the coordinates of the point located in given position.
 *
 *	\param x: position index along the x-axis of the grid
 *	\param y: position index along the y-axis of the grid
 */
//-----------------------------------------------------------
const blGrid::PointType & blGrid::GetPoint(int x, int y)
//-----------------------------------------------------------
{	
	unsigned int radius_x = this->GetRadius(0);
	unsigned int radius_y = this->GetRadius(1);

	unsigned int stride_y = this->GetStride(1);

	int index =  x + ((int) this->GetRadius(0)) + 
				((int) stride_y) * ( y + ((int) radius_y));

#ifdef DEBUG_MESSAGES_blGRID
	std::cout << "blGrid::GetPoint(): index : " << index << std::endl;
#endif

	return GetElement(index);
}




/**
 *	Sets the central point of the grid
 *
 *	\param point point coordinates
 */
//-----------------------------------------------------------
void blGrid::SetCentralPoint(double * point)
//-----------------------------------------------------------
{
	this->centralPoint[0] = point[0];
	this->centralPoint[1] = point[1];
}
	


/**
 *	This normal is in the direction WEST --> EAST (X-axis of the grid)
 *
 *	The given coordinates will be normalized.
 *
 *	\param normal normal vector coordinates
 */
//-----------------------------------------------------------
void blGrid::SetNormal(double * normal)
//-----------------------------------------------------------
{
	double norm = sqrt(normal[0] * normal[0] + normal[1] * normal[1]);

	if (norm != 0.0)
	{
		// vector in X axis (x, y) --> vector in Y axis (-y, x)
		this->normal_WE[0] = normal[0] / norm;
		this->normal_WE[1] = normal[1] / norm;

		this->normal_SN[0] = - normal_WE[1];
		this->normal_SN[1] = normal_WE[0];
	}

	else
	{
		this->normal_WE[0] = 0.0;
		this->normal_WE[1] = 0.0;

		this->normal_SN[0] = 0.0;
		this->normal_SN[1] = 0.0;
	}

#ifdef DEBUG_MESSAGES_blGRID	
	std::cout << "blGrid::SetNormal(): normal WE " << 
					normal_WE[0] << " " << normal_WE[1] << std::endl;
	
	std::cout << "blGrid::SetNormal(): normal SN " << 
					normal_SN[0] << " " << normal_SN[1] << std::endl;
#endif

}




/**
 *	Compute new positions of this grid
 *
 */
//-----------------------------------------------------------
void blGrid::ComputePositions()
//-----------------------------------------------------------
{
	const SizeType radius = this->GetRadius();
	
	PointType currentPoint;	

	

	// init indexes
	int x_index = - ((int) radius[0]);
	int y_index = - ((int) radius[1]);


	// aux vectors
	// ---------------------------------------

	// increment vectors for x and y
	VectorType increment_x;
	VectorType increment_y;

	increment_x[0] = this->normal_WE[0] * this->spacing;
	increment_x[1] = this->normal_WE[1] * this->spacing;

	increment_y[0] = this->normal_SN[0] * this->spacing;
	increment_y[1] = this->normal_SN[1] * this->spacing;


#ifdef DEBUG_MESSAGES_blGRID	
		std::cout << "blGrid::ComputePositions(): increment_x " << 
						increment_x[0] << " " << increment_x[1] << std::endl;

		std::cout << "blGrid::ComputePositions(): increment_y " << 
						increment_y[0] << " " << increment_y[1] << std::endl;
#endif


	// initial vectors for x and y
	VectorType initialVector_x;
	VectorType initialVector_y;

	initialVector_x[0] = increment_x[0] * x_index;
	initialVector_x[1] = increment_x[1] * x_index;

	initialVector_y[0] = increment_y[0] * y_index;
	initialVector_y[1] = increment_y[1] * y_index;

#ifdef DEBUG_MESSAGES_blGRID	
		std::cout << "blGrid::ComputePositions(): initialVector_x " << 
						initialVector_x[0] << " " << initialVector_x[1] << std::endl;

		std::cout << "blGrid::ComputePositions(): initialVector_y " << 
						initialVector_y[0] << " " << initialVector_y[1] << std::endl;
#endif

	// current vectors for x and y
	VectorType vector_x;
	VectorType vector_y;

	vector_x = initialVector_x;
	vector_y = initialVector_y;



	Iterator iter = this->Begin();
	while (iter != this->End())
	{		
		PointType & pointRef = *iter;		

		// central point
		pointRef = this->centralPoint;

		// vector in X
		pointRef += vector_x;
		
		// vector in Y 
		pointRef += vector_y;

#ifdef DEBUG_MESSAGES_blGRID	
//		std::cout << "blGrid::ComputePositions(): vector x " << 
//						vector_x[0] << " " << vector_x[1] << std::endl;

//		std::cout << "blGrid::ComputePositions(): vector y " << 
//						vector_y[0] << " " << vector_y[1] << std::endl;


		std::cout << "blGrid::ComputePositions(): " << 
						pointRef[0] << " " << pointRef[1] << std::endl;
#endif


		// update indexes
		x_index++;
		if (x_index > ((int) radius[0]))
		{
			x_index = -((int) radius[0]);
			y_index++;

			// update vector x
			vector_x = initialVector_x;

			// update vector y
			vector_y += increment_y;
		}
		else
		{
			// update vector x
			vector_x += increment_x;
		}

		iter++;
	}

}
