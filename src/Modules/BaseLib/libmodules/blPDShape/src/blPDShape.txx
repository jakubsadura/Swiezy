#ifndef __blPDShape__TXX
#define __blPDShape__TXX

/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <itkCellInterface.h>
#include <itkLineCell.h>
#include <itkTriangleCell.h>
#include <itkQuadrilateralCell.h>
#include <itkPolygonCell.h>
#include <itkSmartPointer.h>
#include <iostream>
#include "blPDShape.h"
#include "blNumericUtils.h"

/** 
 *	This constructor creates a new blPDShape with no points.
 */
//--------------------------------------------------
template<unsigned int Dimension> 
blPDShape<Dimension>::blPDShape()
//--------------------------------------------------
{
	this->numberDimensions = Dimension;
	this->numberLandmarks = 0;
	this->ptMesh = MeshType::New();
	
	this->numberLines = 0;
	this->numberPolys = 0;

	this->linksBuilt = false;
}




/** 
 *
 *	This method initializes a blPDShape object with the specified points.
 *	It will remove all existing cells.
 *	
 * It COPIES the points from the array 'points'.
 *
 * \param points point coordinates for this blPDShape in the following format:
 * 
 *		xy[z]xy[z]...
 *
 * \param numberOfPoints number of POINTS (not elements) in the array points
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::InitializeFromPoints(const double * points, 
												unsigned int numberOfPoints)
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << "blPDShape::InitializeFromPoints(): numberOfPoints = " 
				<< numberOfPoints << std::endl;
#endif

	// initilize member variables
	this->numberLandmarks = numberOfPoints;


	// remove all existing cells
	this->InitializeCells();

	//clear tesselation information, because it is no more valid - Costa
	TesselationCells.clear();

	
	// typedefs
	typedef typename MeshType::PointsContainer MeshPointsContainer;
	typedef typename MeshType::PointType PointType;
	
	// init PointsContainer with numberOfPoints elements	
	typename MeshPointsContainer::Pointer meshPoints = this->ptMesh->GetPoints();
	meshPoints->Initialize();
	meshPoints->Reserve(numberOfPoints);

	// iterators
	typename MeshPointsContainer::Iterator meshPointIterator = 
										meshPoints->Begin();

	typename MeshPointsContainer::Iterator meshPointIteratorEnd = 
										meshPoints->End();


	// copy point coordinates to meshPoints container
	// ------------------------------------------------

	// current position in the array 'points'
	unsigned int position = 0;	

#ifdef DEBUG_MESSAGES_PDSHAPE	
	int pointCounter = 0;
#endif

	// for each point
	for (meshPointIterator = meshPoints->Begin(); 
			meshPointIterator != meshPointIteratorEnd; 
			meshPointIterator++)
	{
		PointType & point = meshPointIterator.Value();	

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << "blPDShape::InitializeFromPoints(): Point " 
					<< pointCounter << " -->";
#endif		

		// for each dimension
		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{
			point[dim] = points[position];
			position++;

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << point[dim] << " " ;
#endif		

		}

#ifdef DEBUG_MESSAGES_PDSHAPE
		std::cout << std::endl;
		pointCounter++;
#endif

	}		
}



/**
 *	Clear all existing cells in the shape
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::InitializeCells()
//--------------------------------------------------
{

	// typedefs
	// -------------------------------------
	typedef typename MeshType::CellType CellType;
	typedef typename CellType::CellAutoPointer CellAutoPointer;
	typedef typename MeshType::CellsContainer CellsContainer;
	typedef typename MeshType::CellsContainerPointer CellsContainerPointer;
	typedef typename MeshType::CellLinksContainerPointer CellLinksContainerPointer;

	typedef itk::LineCell<CellType> LineCellType;
	typedef itk::TriangleCell<CellType> TriangleCellType;	
   	typedef itk::QuadrilateralCell<CellType> QuadrilateralCellType;	
   	typedef itk::PolygonCell<CellType> PolygonCellType;	



	//------------------------------------
	// Cells
	//------------------------------------
	
	// internal variables 
	this->numberPolys = 0;	
	this->numberLines = 0;


#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << "blPDShape::InitializeCells(): Remove existing cells" 
				<< std::endl;
#endif


	// remove all existing cells
	// ------------------------------
	CellsContainerPointer cellsContainerPt = this->ptMesh->GetCells();

	typename CellsContainer::Iterator cellPtIterator, cellPtIteratorEnd;
	cellPtIterator = cellsContainerPt->Begin();
	cellPtIteratorEnd = cellsContainerPt->End();
	

	while (cellPtIterator != cellPtIteratorEnd)
	{
		CellType * cellPt = cellPtIterator.Value();
		delete cellPt;

		cellPtIterator++;
	}

	// remove all components in cell container
	// ------------------------------------------
	cellsContainerPt->Initialize(); 


	// remove all polylines data
	// ------------------------------------------
	this->polylinesContainer.clear();
	this->polylinesLUT.clear();


	// remove cell links
	// ------------------------------------------
	CellLinksContainerPointer cellLinksContPt = this->ptMesh->GetCellLinks();
	cellLinksContPt->Initialize();

	this->linksBuilt = false;

}
	


/**
 *	Defines cells for this shape.
 *
 *	\param polygons definition of polygon cells
 *	\param polylines definition of polyline cells
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::InitializeCells(
								const ShapeCellsContainer & polygons, 
								const ShapeCellsContainer & polylines)
//--------------------------------------------------
{

	// typedefs
	// -------------------------------------
	typedef typename MeshType::CellType CellType;
	typedef typename CellType::CellAutoPointer CellAutoPointer;
	typedef typename MeshType::CellsContainer CellsContainer;
	typedef typename MeshType::CellsContainerPointer CellsContainerPointer;

	typedef itk::LineCell<CellType> LineCellType;
	typedef itk::TriangleCell<CellType> TriangleCellType;	
   	typedef itk::QuadrilateralCell<CellType> QuadrilateralCellType;	
   	typedef itk::PolygonCell<CellType> PolygonCellType;	


	// build first polygons, then polylines


	//------------------------------------
	// Cells
	//------------------------------------


#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << "blPDShape::InitializeCells(): number of polygons = " 
				<< polygons.size() << ", number of polylines = " 
				<< polylines.size() << std::endl;
#endif



	// remove all existing cells
	this->InitializeCells();


	// cellId counter
  	unsigned int cellId = 0;

	

	// Polygons
   	//----------------------------------------------  	

	this->numberPolys = 0;	// init to 0
  
	ShapeCellsContainer::const_iterator cellIter = polygons.begin();
	ShapeCellsContainer::const_iterator cellIterEnd = polygons.end();

	PointIdsContainer::const_iterator pointIdIter, pointIdIterEnd;
  
	while (cellIter != cellIterEnd)
	{    
	 	const PointIdsContainer & pointIds = (*cellIter);

		CellAutoPointer cellPt;

	 	// different types of polygon:
		// ---------------------------------
		unsigned int numberPoints = pointIds.size();

		// if it has at least 3 points --> new polygon
		if (numberPoints >= 3)
		{
	 		switch (numberPoints)
	 		{
		 		case 3: cellPt.TakeOwnership(new TriangleCellType);
		 				break;
		 				
		 		case 4: cellPt.TakeOwnership(new QuadrilateralCellType);
		 				break;
			 
		 		default:cellPt.TakeOwnership(new PolygonCellType);
		 				break;
	 		}	 		
	 		
			

#ifdef DEBUG_MESSAGES_PDSHAPE
		std::cout << "InitializeCells: Polygon: ";
#endif


			// for each point id in polygon
			// -----------------------------------
			int pointId = 0;

			pointIdIter = pointIds.begin();
			pointIdIterEnd = pointIds.end();
			
			while (pointIdIter != pointIdIterEnd)
	 		{

#ifdef DEBUG_MESSAGES_PDSHAPE
			std::cout << *pointIdIter << " ";
#endif

				cellPt->SetPointId(pointId, *pointIdIter);
				pointId++;
				pointIdIter++;
 			}


#ifdef DEBUG_MESSAGES_PDSHAPE
		std::cout << std::endl;
#endif

 			// add the cell to the Mesh	and update cellId	  	
	   		this->ptMesh->SetCell(cellId, cellPt);
	   		cellId++;

			this->numberPolys++;	// update numberPolys
		}


		cellIter++;	// update cell iterator
   	}   				


  	  			
   	// Polylines: Lines: 2-point lines
   	//----------------------------------------------

	// internal data structures are cleared previously

	this->numberLines = 0;	// init to 0

	PolylineType currentPolyline;
	unsigned int polylineIndex = 0;
		 
	cellIter = polylines.begin();	// init cell iterators
	cellIterEnd = polylines.end();

	// for each polyline
   	while(cellIter != cellIterEnd)
   	{	 				
		currentPolyline.clear();

#ifdef DEBUG_MESSAGES_PDSHAPE	
		std::cout << "InitializeCells: Polyline: " << std::endl;
#endif

		// current polyline point ids
		// -----------------------------------

		const PointIdsContainer & pointIds = (*cellIter);

		if (pointIds.size() >= 2) // more than one point
		{
			pointIdIter = pointIds.begin();
			pointIdIterEnd = pointIds.end();
			pointIdIterEnd--; 
			// pointer to last point id, because each time we get i and i+1

			// iterate over point ids
   			while(pointIdIter != pointIdIterEnd)
   			{
				CellAutoPointer cellPt;				
			
				// new cell of type Line
	 			cellPt.TakeOwnership(new LineCellType);
	 	 				 		
		 		// add 2 points to the new LineCell
	 			cellPt->SetPointId(0, *pointIdIter);
		 		cellPt->SetPointId(1, *(pointIdIter + 1));
				
		 		// add the cell to the Mesh	 	  	
	   			this->ptMesh->SetCell(cellId, cellPt); 			 	

#ifdef DEBUG_MESSAGES_PDSHAPE
		std::cout << "SetCell: " << cellId << " --> " 
					<< *pointIdIter << "," << *(pointIdIter + 1) << std::endl;
#endif

				// add polyline information
				currentPolyline.push_back(cellId);			// new line cell Id
				this->polylinesLUT[cellId] = polylineIndex;	// new line cell in map

	   			// update cellId counter
	   			cellId++;
	   		
				// update iterator
				pointIdIter++;				
			}
		}

#ifdef DEBUG_MESSAGES_PDSHAPE
		std::cout << std::endl;
#endif

		// add new polyline into container
		this->polylinesContainer.push_back(currentPolyline);
		polylineIndex++;
		this->numberLines++;	// update counter
		
		cellIter++;	// update iterator
   	}

}






/** 
 *	The destructor.
 */
//--------------------------------------------------
template <unsigned int Dimension>
blPDShape<Dimension>::~blPDShape()
//--------------------------------------------------
{	
	
}	



/** 
 *	This methods makes a copy of the mesh object referenced by an blPDShape.
 *
 *	\return a SmartPointer to the copy mesh
 */
//--------------------------------------------------
template <unsigned int Dimension>
typename blPDShape<Dimension>::MeshTypePointer blPDShape<Dimension>::GetMeshCopy()
//--------------------------------------------------
{	
	
	// create a new Mesh
	typename MeshType::Pointer newMeshPt = MeshType::New();


	// Points
	//------------------------------------
	typedef typename MeshType::PointType PointType;
	typedef typename MeshType::PointsContainer PointsContainer;
	
	// new points container
	typename PointsContainer::Pointer newMeshPoints = PointsContainer::New();
	newMeshPoints->Reserve(this->numberLandmarks);


	// this PDShape's points container
	typename PointsContainer::Pointer meshPoints = this->ptMesh->GetPoints();


	// copy points to pointsContainer
	unsigned long i;
	for (i = 0; i < this->numberLandmarks; i++)
	{
		newMeshPoints->SetElement(i, meshPoints->GetElement(i));
	}

	// add the points to the mesh
	newMeshPt->SetPoints(newMeshPoints.GetPointer());



	// Cells
	//------------------------------------

	typedef typename MeshType::CellsContainer CellsContainer;	
	typedef typename MeshType::CellsContainerPointer CellsContainerPointer;	
	
	typedef typename MeshType::CellType CellType;
	typedef typename MeshType::CellIdentifier CellIdentifier;

	typedef itk::LineCell<CellType> LineCellType;
	typedef itk::TriangleCell<CellType> TriangleCellType;
	typedef itk::QuadrilateralCell<CellType> QuadrilateralCellType;
	typedef itk::PolygonCell<CellType> PolygonCellType;

	typedef typename CellType::CellAutoPointer CellAutoPointer;

	
  	// get cells
	CellsContainerPointer cellsContPt = this->ptMesh->GetCells();

	// iterators
	typename MeshType::CellsContainerIterator cellsIter, cellsIterEnd;
	cellsIter = cellsContPt->Begin();
	cellsIterEnd = cellsContPt->End();

  	  		
	// define the new CellsContainer
	CellsContainerPointer newCellsContPt = CellsContainer::New();
	newCellsContPt->Reserve(cellsContPt->Size());


	// go through the cells
	while(cellsIter != cellsIterEnd)
	{
		CellIdentifier cellId = cellsIter.Index();
		CellType * cellPt = cellsIter.Value();

		CellAutoPointer newCellPointer;


//		The copy does not work with PolygonCells !!!!
//		so, we have to do it copying points ids

//		cellPt->MakeCopy(newCellPointer);

		switch( cellPt->GetType())
		{
			case CellType::LINE_CELL:
			{
				newCellPointer.TakeOwnership(new LineCellType());
			}
			break;

			case CellType::TRIANGLE_CELL:
			{
				newCellPointer.TakeOwnership(new TriangleCellType());
			}
			break;

			case CellType::QUADRILATERAL_CELL:
			{
				newCellPointer.TakeOwnership(new QuadrilateralCellType());
			}
			break;

			case CellType::POLYGON_CELL:
			{
				newCellPointer.TakeOwnership(new PolygonCellType());
			}
			break;
			default:
			break;
		}

		// define iterators 
		typename CellType::PointIdConstIterator idIter, idIterEnd;
		idIter = cellPt->PointIdsBegin();
		idIterEnd = cellPt->PointIdsEnd();

		// go through the point ids
		unsigned int localPointId = 0;

		while (idIter != idIterEnd)
		{
			newCellPointer->SetPointId(localPointId, *idIter);

			idIter++;
			localPointId++;
		}

		// add cell to new cells container
		newCellsContPt->SetElement(cellId, newCellPointer.ReleaseOwnership());

		cellsIter++;
	}


	// add cells to the new mesh
	newMeshPt->SetCells(newCellsContPt.GetPointer());

	return newMeshPt;
}



/** 
 *	This method returns a pointer to a normal vector at blPDShape point
 *	specified by the landmark id.
 *
 *	\param landmarkId landmark identifier 
 *	\return a const reference to a normal vector
 */
//--------------------------------------------------
template <unsigned int Dimension>
const typename blPDShape<Dimension>::TNormalVector & 
blPDShape<Dimension>::GetNormal(unsigned int landmarkId) const
//--------------------------------------------------
{
	const typename MeshType::PointDataContainer * pointDataContainerPt = 
												this->ptMesh->GetPointData();

	// without boundary CHECKING
	return pointDataContainerPt->ElementAt(landmarkId);	
}



/** 
 *	This method returns an array of double[3] representing a normal vector.
 *
 *	\param landmarkId landmark identifier 
 *	\param normal array where normal coordinates are copied 
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetNormal(unsigned int landmarkId, double normal[])
//--------------------------------------------------
{	
  //typename
  TNormalVector normalVector;
	
	// if normal vector exists for this landmark.. 
	if (this->ptMesh->GetPointData(landmarkId, 
									(TNormalVector *) &normalVector))
	{
		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{
			normal[dim] = normalVector[dim];
		}
	}
	else
	{
		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{
			normal[dim] = 0.0;
		}
	}
}



/** 
 *	This method copies the coordinates of a point in the provided array.
 *
 *	\param landmarkId landmark identifier
 *	\param point double[numberDimensions] array.
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPoint(unsigned int landmarkId, double point[]) const
//--------------------------------------------------
{
	typename MeshType::PointType meshPoint;

	this->ptMesh->GetPoint(landmarkId, &meshPoint);

	for (unsigned long d = 0; d < this->numberDimensions; d++)
	{
		point[d] = meshPoint[d];
	}
}



//added by Costa, 22/11/03
/** 
 *	This method copies the coordinates of a point in the provided array.
 *
 *	\param landmarkId landmark identifier
 *	\param point double[numberDimensions] array.
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPoint(unsigned int landmarkId, 
									vnlVectorDouble& point) const
//--------------------------------------------------
{
	typename MeshType::PointType meshPoint;

	this->ptMesh->GetPoint(landmarkId, &meshPoint);

	unsigned long d;

	if (point.size() < this->numberDimensions) 
	{
		point.set_size(this->numberDimensions);

	}

	for (d = 0; d < this->numberDimensions; d++)
	{
		point[d] = meshPoint[d];
	}
}





/** 
 *	This method returns a copy of the point coordinates.
 *	The coordinates in following format: 
 *		xy[z]..xy[z]
 *
 *	\param points array with size (dim * l), where l is 
 *	the number of landmarks in the shape
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPoints(double points[]) const
//--------------------------------------------------
{
	// typedefs
	typedef typename MeshType::PointsContainer MeshPointsContainer;	
	typedef typename MeshPointsContainer::Iterator PointsIterator;
	typedef typename MeshType::PointType PointType;
	
	typename MeshPointsContainer::Pointer meshPoints = this->ptMesh->GetPoints();
	
	PointsIterator pIterator = meshPoints->Begin();
	PointsIterator pIteratorEnd = meshPoints->End();
	
	// position in the array 'points'
	unsigned int position = 0;	
	
	// iterate 
	while (pIterator != pIteratorEnd)
	{			
		PointType point = pIterator.Value();			
			
		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{
			points[position++] = point[dim];
		}
				
		++pIterator;
	}	
}



/** 
 *	This method returns a copy of the point coordinates.
 *	The coordinates in following format: 
 *		xy[z].xy[z]
 *
 *	\param points vnl_vector<double> with size (dim * l), where l is 
 *	the number of landmarks in the shape
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPoints(vnl_vector<double> &points) const
//--------------------------------------------------
{
	// typedefs
	typedef typename MeshType::PointsContainer PointsContainer;	
	typedef typename PointsContainer::ConstIterator ConstIterator;
	typedef typename MeshType::PointType PointType;
	
	const PointsContainer * meshPointsPt = this->ptMesh->GetPoints();
	
	ConstIterator pIterator = meshPointsPt->Begin();
	ConstIterator pIteratorEnd = meshPointsPt->End();
	
	vnl_vector<double>::iterator pointsIter;
	pointsIter = points.begin();

	// iterate 
	while (pIterator != pIteratorEnd)
	{
		const PointType & point = pIterator.Value();
		
		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{
			///points[position++] = point[dim];
			*pointsIter = point[dim];
			pointsIter++;
		}
				
		++pIterator;
	}	
	
}

/** 
 *	This method returns a copy of the coordinates of those points
 *	that belong to the specified landmark group.
 *	The coordinates in following format: 
 *		xy[z]..xy[z]
 *
 *	\param landmarkGroup the specified landmark group 
 *
 *	\param points array with size (dim * lg), where lg is 
 *	the number of landmarks in the landmark group
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPoints(blLandmarkGroup * landmarkGroup, 
										double points[]) const
//--------------------------------------------------	
{
	// get landmark ids
	const unsigned int * landmarkIds = landmarkGroup->GetLandmarks();
	
	// typedefs
	typedef typename MeshType::PointType PointType;
	
	// position in the array points
	unsigned int position = 0;
	
	for (unsigned long i = 0; i < landmarkGroup->GetNumberOfLandmarks(); i++)	
	{
		PointType point;
		
		this->ptMesh->GetPoint(landmarkIds[i], &point);

		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{				
			points[position++] = point[dim];
		}
	}	
}	



/**
 *	This method gets the points coordinates corresponding to the 
 *	specified landmarks.
 *
 *	\param numberOfLandmarks number of landmarks in the array landmarksIds
 *	\param landmarksIds array of landmarks identifiers
 *	\param points vector where coordinates will be returned
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPoints(unsigned int numberOfLandmarks, 
										const unsigned int landmarksIds[],
										vnl_vector<double> & points) const
//--------------------------------------------------
{

	// typedefs
	typedef typename MeshType::PointType PointType;
	
	// position in the array points
	unsigned int position = 0;
	
	for (unsigned long i = 0; i < numberOfLandmarks; i++)	
	{
		PointType point;
		
		this->ptMesh->GetPoint(landmarksIds[i], &point);

		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{				
			points[position] = point[dim];
			position++;
		}
	}	

}



/**
 *	This method gets the points coordinates corresponding to the 
 *	specified landmarks.
 *
 *	\param numberOfLandmarks number of landmarks in the array landmarksIds
 *	\param landmarksIds array of landmarks identifiers
 *	\param points pointer to the first position of the provided buffer in which 
 *			coordinates will be copied
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPoints(unsigned int numberOfLandmarks, 
										const unsigned int landmarksIds[],
										double * points) const
//--------------------------------------------------
{
	// typedefs
	typedef typename MeshType::PointType PointType;
	
	// position in the array points
	unsigned int position = 0;
	
	for (unsigned long i = 0; i < numberOfLandmarks; i++)	
	{
		PointType point;
		
		this->ptMesh->GetPoint(landmarksIds[i], &point);

		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{				
			points[position++] = point[dim];
		}
	}	

}



/**  
 *	This method builds and returns a data structure with information
 *	about the the polygons in the shape.
 *
 *	\return a pointer to a new ShapeCellsContainer object
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPolygons(ShapeCellsContainer & polygons) const
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << "blPDShape::GetPolygons() " << std::endl;
#endif

	// typedefs
	// --------------------------
	typedef typename MeshType::CellsContainerPointer CellsContainerPointer;
	typedef typename MeshType::CellsContainerConstIterator CellsContainerConstIterator;
	typedef typename MeshType::CellType CellType;
	typedef typename CellType::PointIdIterator PointIdIterator;
	typedef typename CellType::PointIdentifier PointIdentifier;


	polygons.clear();	// remove all elements

	// get cells container
	// --------------------------
	CellsContainerPointer cellsContPt = this->ptMesh->GetCells();

	// iterators
	CellsContainerConstIterator cellsContIt = cellsContPt->Begin();
	CellsContainerConstIterator cellsContItEnd = cellsContPt->End();

	PointIdIterator pointIdIter, pointIdIterEnd;

	CellType * cellPt;	// cell pointer

	while (cellsContIt != cellsContItEnd)
	{
		cellPt = cellsContIt.Value();

		if (cellPt->GetType() == CellType::POLYGON_CELL || 
			cellPt->GetType() == CellType::QUADRILATERAL_CELL ||
			cellPt->GetType() == CellType::TRIANGLE_CELL)
		{
		
#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << "Polygon: " ;
#endif			

			// point ids container
			PointIdsContainer pointIdsCont(cellPt->GetNumberOfPoints());
			PointIdsContainer::iterator pointIdsContainerIter = pointIdsCont.begin();

			// update pointId iterator for this cell
			pointIdIter = cellPt->PointIdsBegin();
			pointIdIterEnd = cellPt->PointIdsEnd();

			while (pointIdIter != pointIdIterEnd)
			{

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << *pointIdIter << " ";
#endif			

				(*pointIdsContainerIter) = (*pointIdIter);
				pointIdIter++;
				pointIdsContainerIter++;
			}
			
#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << std::endl;
#endif			

			// add a new polyline
			polygons.push_back(pointIdsCont);
		}

		cellsContIt++;	//update iterator
	}
}



/**
 *	Get the polyline index in polylinesContainer.
 *
 *	\return -1 if not found, otherwise polylineIndex
 */
//--------------------------------------------------
template <unsigned int Dimension>
typename blPDShape<Dimension>::PolylineIndex 
blPDShape<Dimension>::GetPolylineIndex(CellIdentifier lineCellId)
//--------------------------------------------------
{
	typename LinePolylineLUTType::const_iterator pairIter = 
							this->polylinesLUT.find(lineCellId);

	if (pairIter != this->polylinesLUT.end())
	{
		return (*pairIter).second;
	}
	else
	{		
		return -1; // not found
	}	
}





/**  
 *	This method builds and returns a data structure with information
 *	about the the polylines in the shape.
 *
 *	\return a pointer to a new ShapeCellsContainer object
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPolylinePoints(unsigned int polylineIndex, 
											PointIdsContainer & polylinePoints) const
//--------------------------------------------------
{
	typedef typename MeshType::CellsContainerPointer CellsContainerPointer;
	typedef typename MeshType::CellsContainerIterator CellsContainerIterator;
	typedef typename MeshType::CellType CellType;
	typedef typename CellType::PointIdConstIterator PointIdConstIterator;

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << "blPDShape::GetPolylinePoints(): polylineIndex = "
				<< polylineIndex << std::endl;
#endif

	polylinePoints.clear();	// clear the provided vector

	if (polylineIndex >= 0 && 
		polylineIndex < this->polylinesContainer.size()) 
	{

		const PolylineType & polyline = this->polylinesContainer[polylineIndex];

		// get cells container
		CellsContainerPointer cellsContainerPt = this->ptMesh->GetCells();

		// cell pointer
		CellType * cellPt;		
		PointIdConstIterator pointIdIter;
		
		
		// for each line cell in polyline 
		typename PolylineType::const_iterator cellIndexIter = polyline.begin();
		typename PolylineType::const_iterator cellIndexIterEnd = polyline.end();
		

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << "PointIds: ";
#endif

		// get first line cell
		if (cellIndexIter != cellIndexIterEnd)
		{
			// cell pointer
			cellPt = cellsContainerPt->ElementAt(*cellIndexIter);

			pointIdIter = cellPt->PointIdsBegin();

			// 2 points of first line cell
			polylinePoints.push_back(*pointIdIter);

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << *pointIdIter << " ";
#endif

			pointIdIter++;
			polylinePoints.push_back(*pointIdIter);

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << *pointIdIter << " ";
#endif
			
			cellIndexIter++;	// update cell iterator
		}


		// get the rest of line cells
		while  (cellIndexIter != cellIndexIterEnd)
		{
			// cell pointer
			cellPt = cellsContainerPt->ElementAt(*cellIndexIter);

			pointIdIter = cellPt->PointIdsBegin();			

			// 2nd point of line cell			
			pointIdIter++;
			polylinePoints.push_back(*pointIdIter);

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << *pointIdIter << " ";
#endif
			
			cellIndexIter++;	// update cell iterator		
		}


#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << std::endl;
#endif

	}
		
}






/**  
 *	This method builds and returns a data structure with information
 *	about the the polylines in the shape.
 *
 *	\return a pointer to a new ShapeCellsContainer object
 */
 //--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::GetPolylines(ShapeCellsContainer & polylines) const
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_PDSHAPE
	std::cout << "blPDShape::GetPolylines() " << std::endl;
#endif

	// resize cellsContainer
	polylines.resize(this->polylinesContainer.size());

	// iterators
	typename PolylinesContainerType::const_iterator polylineIter = 
											this->polylinesContainer.begin();

	typename PolylinesContainerType::const_iterator polylineIterEnd = 
											this->polylinesContainer.end();
	
	// for each polyline in polylinesContainer
	for (unsigned long i = 0; i < this->polylinesContainer.size(); i++)
	{		
		PointIdsContainer & polylinePoints = polylines[i];
		this->GetPolylinePoints(i, polylinePoints);	
	}	
}









/**
 *	This method sets the coordinates for a specified point 
 *
 *	\param landmarkId point identifier
 *	\param pointCoordinates coordinates for the point
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::SetPoint(unsigned int landmarkId, 
									const double pointCoordinates[])
//--------------------------------------------------
{
//clear tesselation information, because it is no more valid - Costa
	TesselationCells.clear();

	typename MeshType::PointType meshPoint;

	for (unsigned long d = 0; d < this->numberDimensions; d++)
	{
		meshPoint[d] = pointCoordinates[d];
	}

	this->ptMesh->SetPoint(landmarkId, meshPoint);
}




/** 
 * This method sets the points coordinates.
 * The coordinates in following format: xyxyxy..xy
 *
 * \param points array with size (dim * l), where l is the 
 * number of landmarks in the shape
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::SetPoints(const double points[])
//--------------------------------------------------	
{	
	//clear tesselation information, because it is no more valid - Costa
	TesselationCells.clear();

	// definition of PointType
	typedef typename MeshType::PointType PointType;

	// position in the array points		
	unsigned int position = 0; 
	
	unsigned int i;
	for (i = 0; i < this->numberLandmarks; i++)
	{
		PointType point;
		
		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{
			point[dim] = points[position++];
		}
		
		this->ptMesh->SetPoint(i, point);
	}
}	



/** 
 * This method sets the points coordinates.
 * The coordinates in following format: xy[z]...xy[z]
 *
 * \param points vnl_vector<double> with size (dim * l), where l is the 
 * number of landmarks in the shape
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::SetPoints(const vnl_vector<double> & points)
//--------------------------------------------------	
{
	//clear tesselation information, because it is no more valid - Costa
	TesselationCells.clear();

	// definition of PointType
	typedef typename MeshType::PointType PointType;
		
	// position in the array points
	unsigned int position = 0; 
	
	unsigned int i;
	for (i = 0; i < this->numberLandmarks; i++)
	{
		PointType point;
		
		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{
			point[dim] = points[position++];
		}
			
		this->ptMesh->SetPoint(i, point);
	}
}



/** 
 *	This method sets the coordinates of points that belongs to 
 *	the specified landmark group.
 *	The coordinates in following format: xy[z]..xy[z]
 *
 *	\param landmarkGroup group of landmarks
 *	\param points array with size (dim * lg), where lg is the 
 *	number of landmarks in the landmark group
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::SetPoints(blLandmarkGroup * landmarkGroup, 
										const double points[])
//--------------------------------------------------	
{
	//clear tesselation information, because it is no more valid - Costa
	TesselationCells.clear();

	const unsigned int * landmarkIds = landmarkGroup->GetLandmarks();
	
	// typedefs
	typedef typename MeshType::PointType PointType;
	
	
	// position in the array points
	unsigned int position = 0;
	

	for (unsigned long i = 0; i < landmarkGroup->GetNumberOfLandmarks(); i++)	
	{
		PointType point;
		
		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{
			point[dim] = points[position++];
		}
				
		this->ptMesh->SetPoint(landmarkIds[i], point);
	}		
}




/**
 *	This method sets new coordinates to points corresponding to 
 *	the specified landmarks ids.
 *
 *	\param numberOfLandmarks number of landmarks in the array
 *	\param landmarksIds array of landmarks ids
 *	\param points coordinates for the specified landmarks
 */
//--------------------------------------------------
template <unsigned int Dimension>
void blPDShape<Dimension>::SetPoints(unsigned int numberOfLandmarks, 
										const unsigned int landmarksIds[],
										const vnl_vector<double> & points)
//--------------------------------------------------
{
	//clear tesselation information, because it is no more valid - Costa
	TesselationCells.clear();
	
	// typedefs
	typedef typename MeshType::PointType PointType;
	
	
	// position in the array points
	unsigned int position = 0;

	for (unsigned long i = 0; i < numberOfLandmarks; i++)	
	{
		PointType point;
		for (unsigned long dim = 0; dim < this->numberDimensions; dim++)
		{
			point[dim] = points[position++];
		}
				
		this->ptMesh->SetPoint(landmarksIds[i], point);
	}			

}


//returns the size along every dimension (width, height, ...)
template <unsigned int Dimension>
typename blPDShape<Dimension>::SizeType blPDShape<Dimension>::GetShapeDimensions()
{
	const BoundingBoxType* BoundingBox = GetMeshBoundingBox();
	typename BoundingBoxType::BoundsArrayType Bounds = BoundingBox->GetBounds(); 

	SizeType result;
	for( unsigned long k=0; k<Dimension; k++ ) 
	{
		//this returns double, NEVER EVER use ceil in here!!!
		result[k] = (Bounds[k*2+1]-Bounds[k*2]); //get shape height and width
	}
	
	return result;
}



//returns the size along every dimension (width, height, ...)
template <unsigned int Dimension>
void blPDShape<Dimension>::GetShapeDimensions( 
						vnl_vector<double> &vectorDimensions )
{
	SizeType result;

	result = GetShapeDimensions( );
	vectorDimensions.set_size( Dimension );
	for( unsigned long  k=0; k<Dimension; k++ ) 
	{
		vectorDimensions[ k ] = result[ k ];
	}

}



//returns the size of the shape (norm2 of centered points)
template <unsigned int Dimension>
double blPDShape<Dimension>::GetShapeSize()
{
	vnl_vector<double> shapePoints(numberLandmarks*numberDimensions);
	GetPoints(shapePoints);
	
	vnl_vector<double> center(numberDimensions);
	center.fill(0);

	//shift shape to the centroid 
	//vector is arranged as x1 y1 x2 y2 x3 y3 x4 y4 ...
	for(unsigned long j=0; j<shapePoints.size(); )
	{
		for(unsigned long i=0; i<numberDimensions; i++)
			center[i] += shapePoints[j++];
	}
	center /= numberLandmarks;
	
	for(unsigned long j=0; j<shapePoints.size(); )
	{
		for(unsigned long i=0; i<numberDimensions; i++)
			shapePoints[j++] -= center[i];
	}

	return sqrt( shapePoints.squared_magnitude() );
	
}


/**
 */
template <unsigned int Dimension>
bool blPDShape<Dimension>::Compare( 
				blPDShapeInterface::Pointer		ptrShape2,
				float fEpsilon)
{
	bool bRes = true;
	blPDShapeInterface::Pointer		ptrShape1;
	ptrShape1 = this;

	if ( ptrShape2.IsNull() )
	{
		return false;
	}

	bRes &=	ptrShape1->GetNumberOfLandmarks( ) == 
			ptrShape2->GetNumberOfLandmarks( );
	
	bRes &=	ptrShape1->GetNumberOfPolylines( ) == 
			ptrShape2->GetNumberOfPolylines( );

	bRes &=	ptrShape1->GetNumberOfPolygons( ) == 
			ptrShape2->GetNumberOfPolygons( );

	if ( !bRes )
	{
		return false;
	}


	// Points
	double point1[ 2 ];
	double point2[ 2 ];
	for ( unsigned long i = 0 ; i < ptrShape1->GetNumberOfLandmarks( ) ; i++ )
	{
		ptrShape1->GetPoint( i, point1 );
		ptrShape2->GetPoint( i, point2 );

		for ( int iCoord = 0 ; iCoord < 2 ; iCoord++ )
		{
			bRes &= FLOAT_EQ( point1[ iCoord ], point2[ iCoord ], fEpsilon );
		}
	}
	

	// Polygons
	blPDShapeInterface::ShapeCellsContainer polygons1;
	blPDShapeInterface::ShapeCellsContainer polygons2;
	blPDShapeInterface::PointIdsContainer pointsId1;
	blPDShapeInterface::PointIdsContainer pointsId2;
	ptrShape1->GetPolygons( polygons1 );
	ptrShape2->GetPolygons( polygons2 );
	for ( unsigned long iPoly = 0 ; iPoly < polygons1.size() ; iPoly++ )
	{
		pointsId1 = polygons1[ iPoly ];
		pointsId2 = polygons2[ iPoly ];
		bRes &=	pointsId1.size() == pointsId2.size();

		if ( bRes )
		{
			for ( unsigned long iPoint = 0 ; iPoint < pointsId1.size() ; iPoint++ )
			{
				bRes &= pointsId1[ iPoint ] == pointsId2[ iPoint ];
			}
		}
	}

	return bRes;
}


#endif
