/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blPDShapeInterface_H
#define _blPDShapeInterface_H


#include <vnl/vnl_vector.h>
#include <vector>
#include "blLandmarkGroup.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


/**
 * \brief This class defines common methods for a generic PDShape object.
 * \ingroup blPDShape
 *
 *	All these methods must be defined by its subclasses.
 *
 */
class blPDShapeInterface: public blLightObject
{

	public:
		typedef blPDShapeInterface Self;
		typedef blSmartPointer<Self> Pointer;

		//blNewMacro(Self);
		
		//------------------------------------------------------------
		// Definitions
		//------------------------------------------------------------

		// Definitions for identifiers vectors
		// ---------------------------------------------------

		/** container for points identifiers */
		typedef std::vector<unsigned int> PointIdsContainer;

		/** container for PointIdsContainers (= cells) */
		typedef std::vector<PointIdsContainer> ShapeCellsContainer;



		//------------------------------------------------------------
		// Methods
		//------------------------------------------------------------


		/**\brief computes normal vectors at points in this shape */
		virtual void ComputeNormals() = 0;		


		/** \brief returns the dimension of points */
		virtual unsigned int GetDimension() const = 0;	

		/** \brief creates a copy of this PDShape*/
		virtual Pointer GetCopy() = 0;

		/// Returns the size along every dimension (width, height, ...)
		virtual void GetShapeDimensions( vnl_vector<double> &vectorDimensions ) = 0;

		/// Returns the size of the shape (norm2 of centered points)
		virtual double GetShapeSize() = 0;

		/** \brief return the normal at the specified landmark */
		virtual void GetNormal(unsigned int landmarkId, double normal[]) = 0;


		/** \brief return the number of landmarks in the shape */
		virtual unsigned int GetNumberOfLandmarks()  const = 0;

		
		/** \brief return the number of lines in this blPDShape */
		virtual unsigned int GetNumberOfPolylines() const = 0;

		/** \brief return the number of lines in this blPDShape */
		virtual unsigned int GetNumberOfPolygons() const = 0;

		/** \brief return a copy of a point */
		virtual void GetPoint(unsigned int landmarkId, double point[]) const = 0;

		/** \brief return a copy of the blPDShape points */
		virtual void GetPoints(double points[]) const = 0;				

		/** \brief return a copy of the blPDShape points */
		virtual void GetPoints(vnl_vector<double>  &points) const = 0 ;

		/** \brief return a copy of the blPDShape points that belong to 
			the specified landmark group */
		virtual void GetPoints(blLandmarkGroup * landmarkGroup, 
								double * points) const = 0;

		/**\brief return the coordinates of points corresponding to 
		the specified landmarks ids */
		virtual void GetPoints(unsigned int numberOfLandmarks, 
								const unsigned int landmarksIds[],
								vnl_vector<double> & points) const = 0;

		/**\brief return the coordinates of points corresponding to 
		the specified landmarks ids */
		virtual void GetPoints(unsigned int numberOfLandmarks, 
								const unsigned int landmarksIds[],
								double * points) const = 0;



		
		/** \brief return the polygons in this blPDShape */
		virtual void GetPolygons(ShapeCellsContainer & polygons) const = 0;

		/** \brief return the polylines in this blPDShape */
		virtual void GetPolylines(ShapeCellsContainer & cellsContainer) const = 0;




		/** \brief sets the coordinates for a point */
		virtual void SetPoint(unsigned int landmarkId, 
								const double pointCoordinates[]) = 0;


		/** \brief set new point coordinates to this blPDShape */
		virtual void SetPoints(const double points[]) = 0;		

		/** \brief set new point coordinates to the points in the 
			specified landmark group */
		virtual void SetPoints(blLandmarkGroup * landmarkGroup, 
								const double * points) = 0;

		/** \brief set new point coordinates to this blPDShape */
		virtual void SetPoints(vnl_vector<double> const &points) = 0;


		/**\brief set new point coordinates to the points specified by the
		array of landmarks ids */
		virtual void SetPoints(unsigned int numberOfLandmarks, 
								const unsigned int landmarksIds[],
								const vnl_vector<double> & points) = 0;

		/**
		 * \brief Compare two shapes point by point
		 *
		 * Compare:
		 * - Number of landmarks, polylines and polygons
		 * - Position of points, using precision fEpsilon
		 * - Points id of each polygon
		 */
		virtual bool Compare( 
						blPDShapeInterface::Pointer		ptrShape2,
						float fEpsilon = 0.0001) = 0;

	protected:
		blPDShapeInterface();
		virtual ~blPDShapeInterface();

	private:
		blPDShapeInterface(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented

		
};

#endif
