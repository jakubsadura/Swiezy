/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blPDShape2D_H
#define _blPDShape2D_H


#include "blPDShape.h"
#include "blSmartPointer.h"
#include "blMacro.h"



// #define DEBUG_MESSAGES_PDSHAPE2D



/**
 * \brief This class defines a 2D blPDShape
 * \ingroup blPDShape
 *
 *	blPDShape2D objects contain a number of 2D points, which form polylines
 *	and polygons.
 */
class blPDShape2D: public blPDShape<2>
{

    public:
		typedef blPDShape2D Self;
		typedef blSmartPointer<Self> Pointer;
		typedef blPDShape<2> Superclass;

		typedef Superclass::BoundingBoxType BoundingBoxType;

		blNewMacro(Self);

		//------------------------------------------------------------
		// Methods
		//------------------------------------------------------------
		

		/** \brief compute normals at blPDShape points in a 2D way */
		virtual void ComputeNormals();			

		/** \brief creates a copy of this PDShape*/
		virtual blPDShapeInterface::Pointer GetCopy();


		/**\brief creates a new shape with the specified points */
		blPDShapeInterface::Pointer CreateSubShape(unsigned int landmarksIds[],
											unsigned int numberOfLandmarks);


		/**\brief returns the cell ID containing the specified landmark */
		const unsigned long GetCellId(unsigned int landmarkId);




		/**\brief determines if the given point is inside/outside the specified cell*/
		int PointInCell(const double point[2], unsigned int cellId);


		static int PointInPolygon(const double point[], 
								const double polygon[], int numberOfPoints);



		//added by Costa, 20/11/03
		/** \brief Overriding parent function. Use GetTesselationCells or GetTeselationCell, to retrieve result*/
		virtual void Tesselate(bool forceTesselation=false);
		//:~

	protected:
		blPDShape2D();
		virtual ~blPDShape2D();


    private: 
		blPDShape2D(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented		


		/**\brief returns true if two points are connected	*/
		bool AreConnected( MeshType::PointIdentifier pointId1, 
							MeshType::PointIdentifier pointId2, 
							const MeshType::CellType * cell);


		/**\brief computes normal vector between 2 points */
		void ComputeNormalVector(double x0, double y0, double x1, double y1, 
                                  TNormalVector &output);

		/**\brief computes the mean normal vector */
		void ComputeMeanNormalVector(const TNormalVector &vector1, 
										const TNormalVector &vector2,
										TNormalVector &output);


};

#endif
