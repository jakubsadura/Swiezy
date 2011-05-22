/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blPDSet_templ.h"
#include "blLinearAlgebraOperations.h"



/** 
 * This constructor creates a new blPDSet_templ
 */
//--------------------------------------------------
template< class ArgMatrixType >
blPDSet_templ<ArgMatrixType>::blPDSet_templ() : shapes(NULL)
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
template< class ArgMatrixType >
blPDSet_templ<ArgMatrixType>::~blPDSet_templ()
//--------------------------------------------------
{
	delete [] this->shapes;
}



/** 
 *	This constructor creates a new blPDSet_templ with a number of shapes
 *
 *	This method DOES copy the array.
 *
 * \param numberOfShapes number of shapes in array of shapes
 *
 * \param shapes array of pointers to blPDShape objects
 */
//--------------------------------------------------
template< class ArgMatrixType >
void blPDSet_templ<ArgMatrixType>::SetParameters(unsigned int numberOfShapes, 
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
	std::cout << "New blPDSet_templ with ";
	std::cout << this->numberShapes << " shapes" << std::endl;
#endif

}

/** 
 *	This constructor creates a new blPDSet_templ with a number of shapes
 *
 *	This method does NOT copy the array.
 *
 * \param m sotres the array of pointers of the shapes
 * All the other info that is required is used from fisrtShape
 * so you should always call SetFirstShape() before using this method
 */
//--------------------------------------------------
template< class ArgMatrixType >
void blPDSet_templ<ArgMatrixType>::SetParameters(
	MatrixPointer m_ptr)
//--------------------------------------------------
{

	//copy shapes
	
	if (this->numberLandmarks)
	{
		this->thePoints = m_ptr;
		this->numberShapes = bllao::NrRows ( *this->thePoints );
	}

    	

#ifdef DEBUG_MSG_CONSTRUCTOR
	std::cout << "New blPDSet_templ with ";
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
template< class ArgMatrixType >
void blPDSet_templ<ArgMatrixType>::GetPoints(double ** points)
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
template< class ArgMatrixType >
void blPDSet_templ<ArgMatrixType>::GetPoints(blLandmarkGroup * landmarkGroup, double ** points)
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
template< class ArgMatrixType >
void blPDSet_templ<ArgMatrixType>::GetPoints(vnl_matrix<double> & points)
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


