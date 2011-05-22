/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blPDShape_H
#define _blPDShape_H

#include <itkMesh.h>
#include <itkCovariantVector.h>
#include <itkSize.h>

#include <iostream>
#include <vector>
#include <vnl/vnl_vector.h>
#include <vnl/algo/vnl_determinant.h>

#include "blLandmarkGroup.h"
#include "blPDShapeInterface.h"
///#include "blDefines.h"
#include "blPDShapeDefines.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"
#include "blVector.h"

//TESTING CVS
//#define DEBUG_MESSAGES_PDSHAPE



typedef vnl_matrix<double> vnlMatrixDouble;
typedef vnl_vector<double> vnlVectorDouble;

/**
 * \brief This class defines defines the interface for a generic PDShape
 * \ingroup blPDShape
 *
 * blPDShape is an abstract class that defines some common methods implemented
 * in subclasses such as blPDShape2D and blPDShape3D
 */
template <unsigned int Dimension>
class blPDShape: public blPDShapeInterface
{
public:
	typedef blPDShape Self;
	typedef blSmartPointer<Self> Pointer;

	typedef itk::FixedArray<double, Dimension> SizeType;

///		blNewMacro(Self);

	// -----------------------------------------------
	// Definitions
	// -----------------------------------------------


	// Definitions for a DefaultStatisticMeshTraits
	// -------------------------------------------------


	// Vector to represent normals
	
		// itk::CovariantVector< T, NVectorDimension >

	typedef itk::CovariantVector<double,Dimension> TNormalVector;

	// Pixel Type
	typedef TNormalVector TPixelType;

	// Cell Pixel Type
	typedef TNormalVector TCellPixelType;

	// Coordinates Type 
	typedef double TCoordRep;	

	// Interpolation Weight Type
	typedef float TInterpolationWeight;


	// New MeshTraits

	//
	//	itk::DefaultStaticMeshTraits < 
	//		TPixelType, VPointDimension, VMaxTopologicalDimension, 
	//		TCoordRep, TInterpolationWeight, TCellPixelType > 

	
	typedef itk::DefaultStaticMeshTraits <	TPixelType, 
											Dimension, 
											Dimension, 
											TCoordRep, 
											TInterpolationWeight,
											TCellPixelType>	TMeshTraits;

	// MeshType
	typedef itk::Mesh<TPixelType, Dimension, TMeshTraits> MeshType;
	typedef typename MeshType::Pointer MeshTypePointer;
	typedef typename MeshType::BoundingBoxType BoundingBoxType;

	//------------------------------------------------------------
	// Methods
	//------------------------------------------------------------

	//
	// Constructors and Destructors

	/** \brief Initializes the number of points and its coordinates */
	void InitializeFromPoints(const double * points, 
									unsigned int numberOfPoints);

	/**\brief initializes the cells of this shape*/
	void InitializeCells(const ShapeCellsContainer & polygons, 
									const ShapeCellsContainer & polylines);

	/** \brief compute normals at points */
	virtual void ComputeNormals() = 0;		


	/** \brief creates a copy of this PDShape*/
	virtual blPDShapeInterface::Pointer GetCopy() = 0;


	/** \brief returns the dimension of points */
	virtual unsigned int GetDimension() const
		{ return this->numberDimensions; };	
	

	/**\brief get bounding box of the mesh points */
	const BoundingBoxType* GetMeshBoundingBox() const
	{
		return ptMesh->GetBoundingBox();
	};

	/** \brief return the normal vector at an 
		*	specified landmark without checking 
		*/
	const TNormalVector & GetNormal(unsigned int landmarkId) const;


	/** \brief return the normal at the specified landmark */
	virtual void GetNormal(unsigned int landmarkId, double normal[]);

	/** \brief return the number of landmarks */
	virtual unsigned int GetNumberOfLandmarks() const
		{ return this->numberLandmarks; };

	/** \brief return the number of lines in this blPDShape */
	virtual unsigned int GetNumberOfPolylines() const
		{ return this->numberLines; };

	/** \brief return the number of polygons in this blPDShape */
	virtual unsigned int GetNumberOfPolygons() const
		{ return this->numberPolys; };

	/** \brief return a copy of a point */
	virtual void GetPoint(unsigned int landmarkId, double point[]) const;

	/** \brief return a copy of a point into vector. If point.size() < numberDimensions -> point is resized, otherwise not!*/
	virtual void GetPoint(unsigned int landmarkId, vnlVectorDouble& point) const;

	/** \brief return a copy of the blPDShape points */
	virtual void GetPoints(double points[]) const;				

	/** \brief return a copy of the blPDShape points */
	virtual void GetPoints(vnl_vector<double>  &points) const;

	/** \brief return a copy of the blPDShape points that belong to 
		the specified landmark group */
	virtual void GetPoints(blLandmarkGroup * landmarkGroup, double * points) const;
	
	/**\brief return the coordinates of points corresponding to 
	the specified landmarks ids */
	virtual void GetPoints(unsigned int numberOfLandmarks, 
							const unsigned int landmarksIds[],
							vnl_vector<double> & points) const;

	/**\brief return the coordinates of points corresponding to 
	the specified landmarks ids */
	virtual void GetPoints(unsigned int numberOfLandmarks, 
							const unsigned int landmarksIds[],
							double * points) const;

	/** \brief return the polylines in this blPDShape */
	virtual void GetPolylines(ShapeCellsContainer & polylines) const;

	/** \brief return the polygons in this blPDShape */
	virtual void GetPolygons(ShapeCellsContainer & polygons) const;

	/** \brief sets the coordinates for a point */
	virtual void SetPoint(unsigned int landmarkId, 
							const double pointCoordinates[]);

	/** \brief set new point coordinates to this blPDShape */
	virtual void SetPoints(const double points[]);	
	
	/** \brief set new point coordinates to this blPDShape */
	virtual void SetPoints(const vnl_vector<double> & points);
	
	/** \brief set new point coordinates to the points in the 
		specified landmark group */
	virtual void SetPoints(blLandmarkGroup * landmarkGroup, 
							const double * points);

	/**\brief set new point coordinates to the points specified by the
	array of landmarks ids */
	virtual void SetPoints(unsigned int numberOfLandmarks, 
							const unsigned int landmarksIds[],
							const vnl_vector<double> & points);

	//added by Costa 20/11/03
	/** \brief get reference to TesselationCells*/
	const ShapeCellsContainer& GetTesselationCells() {return TesselationCells;};
	const PointIdsContainer& GetTesselationCell( int CellID ) {return TesselationCells[CellID];};

	/** \brief Set triangulation cells to cells (because triangulation is made only once for the mean shape, and this data can be copied to the deformed versions)*/
	void SetTesselationCells(const ShapeCellsContainer& cells) { TesselationCells=cells; };

	/** \brief Tesselate the mesh and store result to TesselationCells
	* Tesselation will be performed only once. Checks TesselationCells.Empty().
	* To force tesselation set forceTesselation to true 
	*/
	virtual void Tesselate(bool forceTesselation=false) = 0;

	//:~
			
	/// Returns the size along every dimension (width, height, ...)
	virtual SizeType GetShapeDimensions();
	
	/// Returns the size along every dimension (width, height, ...)
	virtual void GetShapeDimensions( vnl_vector<double> &vectorDimensions );

	/// Returns the size of the shape (norm2 of centered points)
	virtual double GetShapeSize();



	/**
		* \brief Compare two shapes point by point
		*
		* Compare:
		* - Number of landmarks, polylines and polygons
		* - Position of points, using precision fEpsilon
		* - Points id of each polygon
		*/
	bool Compare( 
					blPDShapeInterface::Pointer		ptrShape2,
					float fEpsilon = 0.0001);
protected:        
	/** \brief constructor */
	blPDShape();

	/** \brief constructor */
	virtual ~blPDShape();
	
	// -----------------------------------------------
	// Definitions
	// -----------------------------------------------

	typename MeshType::Pointer ptMesh;	//!< mesh object with geometry data

	// internal support to polylines

	/**\brief CellIdentifier type*/
	typedef typename MeshType::CellIdentifier CellIdentifier;
	
	/**\brief PolylineIndex type*/
	typedef unsigned int PolylineIndex;

	/**\brief Polyline type. A vector of cell identifiers*/
	typedef std::vector<CellIdentifier> PolylineType;

	/**\brief PolylinesContainer type. A vector of polylines */
	typedef std::vector<PolylineType> PolylinesContainerType;
		
	/**\brief map of pairs <cellId, polylineIndex>*/
	typedef std::map<CellIdentifier, PolylineIndex> LinePolylineLUTType;



	// -----------------------------------------------
	// Member variables
	// -----------------------------------------------

	unsigned int numberDimensions;	
		//!< number of dimensions 2D or 3D

	unsigned int numberLandmarks;	
		//!< number of landmarks in this PDShape

	unsigned int numberLines;		
		//!< number of polylines in this blPDShape

	unsigned int numberPolys;		
		//!< number of polygons in this blPDShape

	bool linksBuilt;			//!< blag to know if links were built

	//added by Costa 20/11/03
	ShapeCellsContainer TesselationCells;
		//!< Container for tesselaton cells, vector of vectors of IDs of points. Vertices in each cell are listed in counterclockwise order (Point IDs) 
		//!< number of vertices per cell = number of dimensions + 1
	
	//:~

	// internal support to polylines

	PolylinesContainerType polylinesContainer;	
		//!< container of 'polylines'

	LinePolylineLUTType polylinesLUT;		
		//!< LookUp Table for line cells <lineCellId, polylineIndex>


	// -----------------------------------------------
	// Methods
	// -----------------------------------------------

	/** \brief creates a copy of the itk::Mesh*/
	MeshTypePointer GetMeshCopy();


	/**\brief clear all cells */
	void InitializeCells();

	// internal support to polylines

	/**\brief returns the polyline index containing 
		the specified line cell*/
	PolylineIndex GetPolylineIndex(CellIdentifier lineCell);

	/**\brief returns the point ids of those points 
		that belongs to the polyline */
	void GetPolylinePoints(unsigned int polylineIndex, 
								PointIdsContainer & polylinePoints) const;

private:
	blPDShape(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented		
};

#include "blPDShape.txx"

#endif
