/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _blPDShape3D_H
#define _blPDShape3D_H


//#define DEBUG_MESSAGES_PDSHAPE3D


#include "blPDShape.h"
#include "blSmartPointer.h"
#include "blMacro.h"



/**
 * \brief This class defines a 3D blPDShape. 
 * \ingroup blPDShape
 *
 *	blPDShape3D objects contain a number of 3D points, 
 *	which form polylines and polygons. 
 *
 */
class blPDShape3D: public blPDShape<3>
{
	public:
		typedef blPDShape3D Self;
		typedef blSmartPointer<Self> Pointer;
		typedef blPDShape<3> Superclass;

		typedef Superclass::BoundingBoxType BoundingBoxType;

		blNewMacro(Self);

		//added by Costa 20/11/03
		//------------------------------------------------------------
		// Defines
		//------------------------------------------------------------


		/** \brief compute normals at points in a 3D way*/
		virtual void ComputeNormals();		
		

		/** \brief creates a copy of this PDShape */		
		virtual blPDShapeInterface::Pointer GetCopy();
		

		//added by Costa, 20/11/03
		/** \brief Overriding parent function. Use GetTesselationCells or GetTeselationCell, to retrieve result*/
		virtual void Tesselate(bool forceTesselation=false);
		//:~

	protected:
		blPDShape3D();
		virtual ~blPDShape3D();

    private:        
		blPDShape3D(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented		
		
		// -----------------------------------------------
		// Methods
		// -----------------------------------------------

		/** \brief returns a normal vector given 3 points */
		void Compute3DNormalVector(MeshType::PointType &p0, 
									MeshType::PointType &p1, 
									MeshType::PointType &p2,
                                    TNormalVector &output);

		/** \brief compute normal vectors at cells */
		void Compute3DCellsNormalVectors();

		/** \brief compute normal vectors at points */
		void Compute3DPointsNormalVectors();
};

#endif
