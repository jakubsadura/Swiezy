/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blGrid_h
#define __blGrid_h


#include <itkNeighborhood.h>
#include <itkPoint.h>

#include <vnl/vnl_vector.h>

//#define DEBUG_MESSAGES_blGRID



#define blGRID_DIMENSION 2


/**
 * \brief This class defines a NxN grid.
 * \ingroup blImageUtilities
 *
 *	This class defines a grid with size N x N. 
 *	N must be an odd number (= 2 * radius + 1)
 *
 *	The user must provide the following data:
 *
 *	- radius: number of points of the grid at each side of the central position
 *	- central point: point where the grid will be centred on
 *	- normal: vector that defines x-axis of the grid (WEST --> EAST)
 *	- spacing: between two consecutive positions of the grid
 *
 *	Before accessing to the positions, the method ComputePositions() 
 *	must be called.
 */
class blGrid : public itk::Neighborhood< itk::Point<double, 2>, 2> 
{
	public:
// 		TODO::ADDED BY VDIALINOS
		typedef itk::Point<double, blGRID_DIMENSION> PointType;
		typedef itk::Vector<double, blGRID_DIMENSION> VectorType;
////////////////////
// 		TODO::COMMENTED OUT BY VDIALINOS
// 		typedef itk::Point<double, 2> PointType;
////////////////////////////////////////////////////////

		/**\brief constructor*/
		blGrid();

		/**\brief constructor*/
		blGrid(unsigned int radius);


		/**\brief destructor*/
		virtual ~blGrid();

		/**\brief computes the coordinates of each position*/
		void ComputePositions();

		/**\brief return the point coordinates corresponding to indexes x and y*/
		const PointType & GetPoint(int x, int y);

		// SetRadius() --> superclass allocates memory for this container 
	
		/**\brief sets central point of the grid */
		void SetCentralPoint(double * point);

		/**\brief sets the normal W->E */
		void SetNormal(double * point);

		/**\brief sets the spacing between points in the grid*/
		void SetSpacing(double spacing)
		{
			this->spacing = spacing;
		};

		double GetSpacing ()
		{
			return this->spacing;
		};
	
// 		TODO::COMMENTED OUT BY VDIALINOS	
// 		typedef itk::Point<double, blGRID_DIMENSION> PointType;
// 		typedef itk::Vector<double, blGRID_DIMENSION> VectorType;
/////////////////

	private:
		blGrid(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented


		//double centralPoint[blGRID_DIMENSION];	//!< central point of the grid
		//double normal_WE[blGRID_DIMENSION];	//!< normal WEST -> EAST of the grid
		//double normal_SN[blGRID_DIMENSION];	//!< normal SOUTH -> NORTH of the grid

		PointType centralPoint;
		VectorType normal_WE;
		VectorType normal_SN;

		double spacing;		//!< spacing of the grid

};


#endif


