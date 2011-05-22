/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blPDSet.h"
#include "blLinearAlgebraOperations.h"


/** 
 * This constructor creates a new blPDSet
 */
//--------------------------------------------------
blPDSet::blPDSet() : shapes(NULL)
//--------------------------------------------------
{
	this->numberDimensions = 0;
	this->numberLandmarks = 0;
	this->numberShapes = 0;
}



/** 
 * The destructor.
 *
 *	This method will removed all the PDShapes objects in this PDSet
 */
//--------------------------------------------------
blPDSet::~blPDSet()
//--------------------------------------------------
{
	delete [] this->shapes;
}



/** 
 *	This constructor creates a new blPDSet with a number of shapes
 *
 *	This method DOES copy the array.
 *
 * \param numberOfShapes number of shapes in array of shapes
 *
 * \param shapes array of pointers to blPDShape objects
 */
//--------------------------------------------------
void blPDSet::SetParameters(unsigned int numberOfShapes, 
							blPDShapeInterface::Pointer shapes[])
//--------------------------------------------------
{
	bool newSet = ( numberOfShapes != this->numberShapes ) || 
						( this->shapes == NULL );

	this->numberDimensions = 0;
	this->numberLandmarks = 0;
	this->numberShapes = numberOfShapes;
	
	//copy shapes
	if ( newSet )
	{
		if ( this->shapes != NULL ) 
		{
			delete[] this->shapes;
		}

		this->shapes = new blPDShapeInterface::Pointer [numberOfShapes];
	}

	for (unsigned int i=0; i<numberOfShapes; i++)
	{
		this->shapes[i] = shapes[i];
	}
	
	if (numberOfShapes > 0)
	{
		this->numberDimensions = this->shapes[0]->GetDimension();
		this->numberLandmarks = this->shapes[0]->GetNumberOfLandmarks();
	}
	

#ifdef DEBUG_MSG_CONSTRUCTOR
	std::cout << "New blPDSet with ";
	std::cout << this->numberShapes << " shapes" << std::endl;
#endif

}


	 
/** 
 *	This method returns a copy of all the points of all the PDShapes.
 *
 *	\param points an array of pointers [numberOfShapes] to arrays of 
 *	double[numPoints x numDimension] 
 */
//--------------------------------------------------
void blPDSet::GetPoints(double ** points)
//--------------------------------------------------
{	
	unsigned int i;
	for (i = 0; i < this->numberShapes; i++)
	{	
		this->shapes[i]->GetPoints(points[i]);
	}		
}	



/** 
 *	This method returns a copy of the points coordinates of points that
 *	belongs to a landmark group.
 *	
 *	\param landmarkGroup the specified landmark group
 *	\param points an array of pointers [numberOfShapes] to array of 
 *	double[lg * dimension], where lg is the number of landmarks in the 
 *	landmark group.
 */
//--------------------------------------------------
void blPDSet::GetPoints(blLandmarkGroup * landmarkGroup, double ** points)
//--------------------------------------------------
{	
	unsigned int i;
	for (i = 0; i < this->numberShapes; i++)
	{
		this->shapes[i]->GetPoints(landmarkGroup, points[i]);
	}	
}	
			


/** 
 *	This method returns the points of all the PDShapes.
 *
 * 	\param points a vnl_matrix<double> with a size of 
 *	[numberOfShapes][numPoints * numDimension]
 */
//--------------------------------------------------
void blPDSet::GetPoints(vnl_matrix<double> & points)
//--------------------------------------------------
{	
	// number of cols 
//TODO::UNUSED VARIABLE
// 	int numCols = this->numberLandmarks * this->numberDimensions;
////////////
		
	// copy the points coordinates in the matrix
	
	unsigned int i;
	for (i = 0; i < numberShapes; i++)
	{			
		this->shapes[i]->GetPoints(points[i]);
	}	
}			

blPDSet::Pointer blPDSet::GetCopy()
{
	Pointer result = New();

	result->numberDimensions = numberDimensions;
	result->numberLandmarks = numberLandmarks;
	result->numberShapes = numberShapes;
	
	//copy shapes
	result->shapes = new blPDShapeInterface::Pointer [numberShapes];


	for (unsigned int i=0; i<numberShapes; i++)
	{
		result->shapes[i] = shapes[i]->GetCopy();
	}	

	return result;
}
