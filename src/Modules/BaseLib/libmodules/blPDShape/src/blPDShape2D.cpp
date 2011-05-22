/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
//This class defines a Point Distribution Shape.

///#ifdef _MSC_VER
///#pragma warning ( disable : 4786 )
///#endif

#pragma warning (disable : 4786) 


#include <time.h>
#include "blPDShape2D.h"
#include "blMath.h"
#include "blDelaunay2D.h"


/**
 *	Constructor.
 */
//--------------------------------------------------
blPDShape2D::blPDShape2D() 
//--------------------------------------------------
{
}

 

/**
 *	Destructor
 */
//--------------------------------------------------
blPDShape2D::~blPDShape2D()
//--------------------------------------------------
{
}



//	DON'T REMOVE THIS

/**
 *
 */
///
/*
//--------------------------------------------------
void blPDShape2D::AddPolyline( unsigned int numberOfPoints, 
								unsigned int pointsIds[])
//--------------------------------------------------
{

	// typedefs
	// -------------------------------------
	typedef MeshType::CellType CellType;
	typedef CellType::CellAutoPointer CellAutoPointer;

	typedef itk::LineCell<CellType> LineCellType;

	// cell identifier
	unsigned int cellId = this->ptMesh->GetNumberOfCells();

	
	// Lines: 2-point lines
   	//----------------------------------------------

	// go through the array of ids
	int i;
   	for(i = 0; i < numberOfPoints - 1; i++)
   	{
		CellAutoPointer cellPt;				
	
		// new cell of type Line
	 	cellPt.TakeOwnership(new LineCellType);
	 	 				
		// add 2 points to the new LineCell
	 	cellPt->SetPointId(0, pointsIds[i]);
		cellPt->SetPointId(1, pointsIds[i + 1]);
		
		// add the cell to the Mesh	 	  	
	   	this->ptMesh->SetCell(cellId, cellPt); 			 	

	   	// update cellId counter
	   	cellId++;
	}

	this->numberLines++;

}
*/
///


//	DON'T REMOVE THIS
/**
 *
 */
///
/*
//--------------------------------------------------
void blPDShape2D::AddPolygon(unsigned int numberOfPoints, 
							 unsigned int pointsIds[])
//--------------------------------------------------
{

	// typedefs
	// -------------------------------------	
	typedef MeshType::CellType CellType;
	typedef CellType::CellAutoPointer CellAutoPointer;

	typedef itk::TriangleCell<CellType> TriangleCellType;	
   	typedef itk::QuadrilateralCell<CellType> QuadrilateralCellType;	
   	typedef itk::PolygonCell<CellType> PolygonCellType;	

	CellAutoPointer cellPt;

	// type of polygon
	switch (numberOfPoints)
	{
		case 3: cellPt.TakeOwnership(new TriangleCellType);
		 		break;
		 		
		case 4: cellPt.TakeOwnership(new QuadrilateralCellType);
		 		break;
	 
		default:cellPt.TakeOwnership(new PolygonCellType);
		 		break;
	}	 


	// go through the pointsIds array, and fill the cell with point ids
   	int i;
   	for(i = 0; i < numberOfPoints; i++)
   	{	 				
		cellPt->SetPointId(i, pointsIds[i]);
			
	}

	// set a cell Id
	unsigned int cellId = this->ptMesh->GetNumberOfCells();

 	// add the cell to the Mesh	and update cellId	  	
	this->ptMesh->SetCell(cellId, cellPt);


	this->numberPolys++;

}
*/
///


/**
 */
//--------------------------------------------------
bool blPDShape2D::AreConnected( MeshType::PointIdentifier pointId1, 
								MeshType::PointIdentifier pointId2, 
								const MeshType::CellType * cell)
//--------------------------------------------------
{
	// typedefs
	typedef MeshType::CellType CellType;


	// get points ids from the cell
	MeshType::CellType::PointIdConstIterator pointIdIter;
	pointIdIter = cell->GetPointIds();

	// number of points in the cell
	unsigned int numPoints = cell->GetNumberOfPoints();

	// first and last points in the cell
	unsigned int firstPointIdInCell = *pointIdIter;
	unsigned int lastPointIdInCell = *(pointIdIter + numPoints - 1);
	

	switch (cell->GetType())
	{	
		// ---------------------------------------------------
		// LINE_CELL
		// ---------------------------------------------------
		case CellType::LINE_CELL:
		{					
			if ((firstPointIdInCell == pointId1 && 
					lastPointIdInCell == pointId2) 	||
				(firstPointIdInCell == pointId2 && 
					lastPointIdInCell == pointId1))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		break;

		// ---------------------------------------------------
		// TRIANGLE_CELL, QUADRILATERAL_CELL or POLYGON_CELL
		// ---------------------------------------------------
		case CellType::TRIANGLE_CELL:
		case CellType::QUADRILATERAL_CELL:
		case CellType::POLYGON_CELL:
		{
			// find the position of pointId1 in the polygon
			unsigned int i = 0;
			while (i < numPoints && *pointIdIter != pointId1)
			{
				i++;
				pointIdIter++;
			}
			
			if (*pointIdIter == pointId1)	// if found
			{
				// check if pointId2 is next or previous point
				
				// previous
				// if position == 0
				if (i == 0)
				{
					if (lastPointIdInCell == pointId2)
					{
						return true;
					}						
				}
				else
				{
					if (*(pointIdIter - 1) == pointId2)
					{
						return true;
					}
				}

				// next point
				// if position is last position
				if (i == numPoints - 1)
				{
					if (firstPointIdInCell == pointId2)
					{
						return true;
					}						
				}
				else
				{
					if (*(pointIdIter + 1) == pointId2)
					{
						return true;
					}
				}

				// if not found yet, return false
				return false;
			}
			else	// if not found, they are not connected in this cell
			{
				return false;
			}

		}
		break;

		default:
			return false;
	}
}





/**
 *	This method creates a new shape containing the specified points.
 *
 *	\param landmarksIds array with the landmarks ids
 *	\param numberOfLandmarks number of elements in the array
 *	\return a pointer to the new shape
 */
//--------------------------------------------------
blPDShapeInterface::Pointer blPDShape2D::CreateSubShape(
												unsigned int landmarksIds[],
												 unsigned int numberOfLandmarks)
//--------------------------------------------------
{
	// typedefs
	typedef MeshType::CellsContainerPointer CellsContainerPointer;
	typedef MeshType::CellsContainer CellsContainer;
	typedef MeshType::CellType CellType;
	typedef CellType::CellIdentifier CellIdentifier;
	typedef MeshType::CellAutoPointer CellAutoPointer;
	typedef CellType::PointIdConstIterator PointIdConstIterator;
	typedef MeshType::CellLinksContainerPointer CellLinksContainerPointer;
	
	typedef MeshType::PointType PointType;		
	typedef MeshType::PointIdentifier PointIdentifier;
	typedef MeshType::PointCellLinksContainer PointCellLinksContainer;
	
	typedef itk::LineCell<CellType> LineCellType;
		

	// cell containers where new cells (polygons, polylines) are stored
	blPDShape2D::ShapeCellsContainer newPolygonsContainer;
	blPDShape2D::ShapeCellsContainer newPolylinesContainer;

	blPDShape2D::PointIdsContainer currentCell;	// current cell


	// build links from points back to the cells
	if (!this->linksBuilt)
	{
		this->ptMesh->BuildCellLinks();		
		this->linksBuilt = true;
	}


	// Get the cellLinks container 
	CellLinksContainerPointer cellLinksContPt = 
						this->ptMesh->GetCellLinks();
	
	// set of cell Ids
	PointCellLinksContainer pointCellLinksContainer;

	// pointer to a cell
	CellAutoPointer cellPt;

// TODO::UNUSED VARIABLE	
// 	bool currentCellIsPolygon = false;
////////////
	bool currentCellIsPolyline = false;
					


	unsigned int newShapeLandmarkId = 0;	// landmark id for new shape
	CellIdentifier currentCellId;


	// go through the given landmarks ids
	unsigned int i = 0;
	while (i < numberOfLandmarks)
	{

#ifdef DEBUG_MESSAGES_PDSHAPE2D
		std::cout << "blPDShape2D::CreateSubShape(): LandmarkId " 
					<< landmarksIds[i] << std::endl;
#endif

		// ------------------------------------------------
		// get the cell corresponding to current landmark
		// ------------------------------------------------

		// copy set of cell Ids
		bool landmarkInCell = 
			cellLinksContPt->GetElementIfIndexExists(landmarksIds[i], 
													&pointCellLinksContainer);

		// if current landmark belongs to any cell
		if (landmarkInCell)
		{
			// first cell (supposing only one cell per landmark)
			PointCellLinksContainer::const_iterator cellIdIter = 
											pointCellLinksContainer.begin();	

			// check that set is not empty
			if (cellIdIter != pointCellLinksContainer.end())
			{
				// get cell from mesh
				this->ptMesh->GetCell(*cellIdIter, cellPt);

				// get cellId
				currentCellId = *cellIdIter;

				// ----------------------------
				// cell in a polyline
				// ----------------------------

				if (cellPt->GetType() == CellType::LINE_CELL)
				{						
					// new polyline
					currentCellIsPolyline = true;
					currentCell.clear();

					// get polyline index
					unsigned int polylineIndex = 
							this->GetPolylineIndex(currentCellId);

					// get polyline points
					PointIdsContainer polylinePoints;
					this->GetPolylinePoints(polylineIndex, polylinePoints);
					

					// find current landmark id in polylinePoints
					// ---------------------------------------------
					unsigned int pointPosition = 0;
					bool found = false;
					while (pointPosition < polylinePoints.size() && !found)
					{
						if (polylinePoints[pointPosition] == landmarksIds[i])
						{
							found = true;
						}
						else
						{
							pointPosition++;
						}
					}
					
					// determine direction of cell definition
					// ----------------------------------------

					bool cellPointsForwards = true;
					bool pointAlone = false;

					if (i + 1 < numberOfLandmarks && 
						pointPosition + 1 < polylinePoints.size() && 
							polylinePoints[pointPosition + 1] == landmarksIds[i + 1])
					{
						cellPointsForwards = true;
					}

					else if (i + 1 < numberOfLandmarks && 
								pointPosition - 1 >= 0 && 
								polylinePoints[pointPosition - 1] == landmarksIds[i + 1])
					{
						cellPointsForwards = false;
					}
					else
					{
						// last point of polyline, so no polyline
						pointAlone = true;
					}


					// iterate over points
					// ---------------------------
					if (!pointAlone)
					{
						// add first point to polyline
						currentCell.push_back(newShapeLandmarkId);

						// update indexes for landmark list, new landmark id, 
						//						polyline point position
						i++;
						newShapeLandmarkId++;					
						
						if (cellPointsForwards) pointPosition++;
						else pointPosition--;


						bool inPolyline = true;

						// add points to polyline
						while (i < numberOfLandmarks && 
								pointPosition >= 0 && 
								pointPosition < polylinePoints.size() && inPolyline)
						{						
							if (polylinePoints[pointPosition] == landmarksIds[i])
							{
								// add first point to polyline
								currentCell.push_back(newShapeLandmarkId);

								i++;
								newShapeLandmarkId++;
								
								if (cellPointsForwards) pointPosition++;
								else pointPosition--;
							}
							else
							{
								inPolyline = false;
							}
						}
						

						// if number of landmarks in polyline is at least 2
						if (currentCell.size() > 1)
						{
							newPolylinesContainer.push_back(currentCell);
						}

					}// end of if not point alone

					else	// point is alone
					{
						i++;	// next point in landmarksIds list
					}

				}


				// ----------------------------
				// cell in a polygon
				// ----------------------------
				
				else if (cellPt->GetType() == CellType::POLYGON_CELL ||
							cellPt->GetType() == CellType::QUADRILATERAL_CELL ||
							cellPt->GetType() == CellType::TRIANGLE_CELL)
				{
					// new polyline					
					currentCell.clear();

				
					// find current landmark id in polygonPoints
					// -----------------------------------------										
					bool found = false;					

					PointIdConstIterator polygonPointIterBegin = cellPt->PointIdsBegin();
					PointIdConstIterator polygonPointIterEnd = cellPt->PointIdsEnd();
					PointIdConstIterator polygonPointIter = polygonPointIterBegin;

					while (polygonPointIter != cellPt->PointIdsEnd() && !found)
					{
						if (*polygonPointIter == landmarksIds[i])
						{
							found = true;
						}
						else
						{
							polygonPointIter++;
						}
					}
					

					// determine direction of cell definition
					// ----------------------------------------

					bool cellPointsForwards = true;
					bool pointAlone = false;

					if (i + 1 < numberOfLandmarks && 
						(polygonPointIter + 1) !=  polygonPointIterEnd && 
							*(polygonPointIter + 1) == landmarksIds[i + 1])
					{
						cellPointsForwards = true;
					}

					else if (i + 1 < numberOfLandmarks && 
								polygonPointIter !=  polygonPointIterBegin && 
								*(polygonPointIter - 1) == landmarksIds[i + 1])
					{
						cellPointsForwards = false;
					}
					else
					{
						// last point of polyline, so no polyline
						pointAlone = true;
					}


					// iterate over points
					// ---------------------------
					if (!pointAlone)
					{
						// add first point to polyline
						currentCell.push_back(newShapeLandmarkId);

						// update indexes for landmark list, new landmark id, 
						//						polyline point position
						i++;
						newShapeLandmarkId++;					
						
						if (cellPointsForwards) polygonPointIter++;
						else polygonPointIter--;


						bool inPolygon = true;

						while (i < numberOfLandmarks && inPolygon)
						{
							// circular iteration
							
							if (polygonPointIter == polygonPointIterEnd)
							{
								// forwards
								polygonPointIter = polygonPointIterBegin;
							}
							else if (polygonPointIter == polygonPointIterBegin - 1)
							{
								// backwards
								polygonPointIter = polygonPointIterEnd;
							}

							
							if (*polygonPointIter == landmarksIds[i])
							{
								// avoid repeated points and infitine loops
								if (currentCell.size() < cellPt->GetNumberOfPoints())
								{
									// add point to cell
									currentCell.push_back(newShapeLandmarkId);

									// update indexes and iterator
									i++;
									newShapeLandmarkId++;
									
									if (cellPointsForwards) polygonPointIter++;
									else polygonPointIter--;
								}
								else
								{
									inPolygon = false;
								}
							}
							else
							{
								inPolygon = false;
							}

						} // end of while


						if (currentCell.size() > 1)
						{
							if (currentCell.size() == cellPt->GetNumberOfPoints())
							{
								newPolygonsContainer.push_back(currentCell);
							}
							else
							{
								newPolylinesContainer.push_back(currentCell);
							}
						}

					}
					else	// point alone
					{
						i++; // next landmark id in the list
					}

				}	// if polygon


			} // if set is not empty

		}	// landmark in CellLinksContainer

	}	// for each landmark


	

	// --------------------------------------------------------
	// Build the new shape
	// --------------------------------------------------------

	// create new shape and initialize points and landmarks
	
	// get point coordinates
	vnl_vector<double> points(numberOfLandmarks * this->numberDimensions);
	this->GetPoints(numberOfLandmarks, landmarksIds, points);
	
	blPDShape2D::Pointer shape = blPDShape2D::New();
	shape->InitializeFromPoints(points.data_block(), numberOfLandmarks);
	shape->InitializeCells(newPolygonsContainer, newPolylinesContainer);


/*

	// build links from points back to the cells
	if (!this->linksBuilt)
	{
		this->ptMesh->BuildCellLinks();		
		this->linksBuilt = true;
	}

	// Get the cellLinks container 
	CellLinksContainerPointer cellLinksContPt = 
						this->ptMesh->GetCellLinks();

	// current cell in the shape
	CellAutoPointer currentCell;

	PointCellLinksContainer cellLinksCont;	//std::set<CellIdentifier>
	PointCellLinksContainer::iterator cellIter, cellIterEnd;


	bool currentCellIsPolygon = false;
	bool currentCellIsPolyline = false;


	// go through the landmarks ids
	int i;
	for (i = 0; i < numberOfLandmarks; i++)
	{
	
		
		
		if (currentCell.empty())	
		{
			currentCell.push_back(landmarksIds[i]);
			cellLinksCont = cellLinksContPt->ElementAt(landmarksIds[i]);
		}

		else
		{
			// if no current cell
			if (cellLinksCont.size() == 0)
			{
				newSequencesContainer.push_back(newSequence);

				newSequence.clear();
				newSequence.push_back(landmarksIds[i]);

				// get cells containing this landmark 
				cellLinksCont = cellLinksContPt->ElementAt(landmarksIds[i]);
			}

			else
			{
				cellIter = cellLinksCont.begin();
				cellIterEnd = cellLinksCont.end();

				// get the cell
				this->ptMesh->GetCell(*cellIter, currentCell);


				// ------------------------------------------------
				//	LINE_CELL
				// ------------------------------------------------
				if (currentCell->GetType() == CellType::LINE_CELL)
				{
					// Seq[..] <--> currentLandmark
					if ( (this->AreConnected(newSequence.back(), 
										landmarksIds[i], currentCell.GetPointer())))
					{
						// add landmark to the sequence
						newSequence.push_back(landmarksIds[i]);

						// update cells containing this landmark
						cellLinksCont = 
							cellLinksContPt->ElementAt(landmarksIds[i]);
					}

					else 
					{
						// if a second cell, try in that cell
						cellIter++;
						if (cellIter != cellIterEnd)
						{
							// get the another cell
							this->ptMesh->GetCell(*cellIter, currentCell);

							// Seq[..] <--> currentLandmark
							if (this->AreConnected(newSequence.back(), 
													landmarksIds[i], 
													currentCell.GetPointer()))
							{
								// add landmark to the sequence
								newSequence.push_back(landmarksIds[i]);

								// update cells containing this landmark
								cellLinksCont = 
									cellLinksContPt->ElementAt(
														landmarksIds[i]);
							}

							// landmark is not connected to prev. landmark
							else
							{
								newSequencesContainer.push_back(newSequence);
								newSequence.clear();
								newSequence.push_back(landmarksIds[i]);

								cellLinksCont = 
									cellLinksContPt->ElementAt(
														landmarksIds[i]);
							}
						}

						// no more cells
						// landmark is not connected to prev. landmark
						else
						{
							newSequencesContainer.push_back(newSequence);							

							newSequence.clear();
							newSequence.push_back(landmarksIds[i]);

							cellLinksCont = 
								cellLinksContPt->ElementAt(landmarksIds[i]);
						}		
					}
				} // end of LINE_CELL
				

				// ------------------------------------------------
				//	POLYGON_CELL
				// ------------------------------------------------
				else
				{
					CellType::PointIdIterator pointIter = currentCell->PointIdsEnd();
					pointIter--;

					unsigned int lastPointInCell = *pointIter;
		
					// Seq[..] <--> currentLandmark
					if (this->AreConnected(newSequence.back(), 
											landmarksIds[i], 
											currentCell.GetPointer()))
					{
						// add current landmark to the sequence
						newSequence.push_back(landmarksIds[i]);
		
						if (landmarksIds[i] == lastPointInCell)
						{
							if (this->AreConnected(newSequence.front(), 
													landmarksIds[i], 
													currentCell.GetPointer()))
							{
								newSequence.push_back(newSequence.front());
								// close the polygon
							}
						}
					}

					else
					{
						// start a new sequence with this landmark
						newSequencesContainer.push_back(newSequence);

						newSequence.clear();
						newSequence.push_back(landmarksIds[i]);

						// get cellLinks for the landmark
						cellLinksCont = 
								cellLinksContPt->ElementAt(landmarksIds[i]);						
					}
				}

			}	// end of cellLinksSize == 0
		}	// end of if sequence not empty

	} // end of for

	// add the last sequence
	newSequencesContainer.push_back(newSequence);


	// --------------------------------------------------------
	// Build the new shape
	// --------------------------------------------------------

	

	// get point coordinates
	vnl_vector<double> points(numberOfLandmarks * this->numberDimensions);
	this->GetPoints(numberOfLandmarks, landmarksIds, points);
	
	blPDShape2D::Pointer shape = blPDShape2D::New();
	shape->InitializeFromPoints(points.data_block(), numberOfLandmarks);


//	shape->SetPoints(points);


	// create cells
	blPDShape2D::ShapeCellsContainer::iterator seqIter, seqIterEnd;
	seqIter = newSequencesContainer.begin();
	seqIterEnd = newSequencesContainer.end();

	unsigned int * landmarks;
	unsigned int currentLandmarkId = 0;	// landmark id for new shape

	while (seqIter != seqIterEnd)
	{
		blPDShape2D::PointIdsContainer & sequence = *seqIter;

		if (sequence.size() > 1)
		{
			// polygon, first landmark is repeated at the end
			if (sequence.front() == sequence.back())
			{
				landmarks = new unsigned int[sequence.size() - 1];

				int i;
				for (i = 0; i < sequence.size() - 1; i++)
				{					
					landmarks[i] = currentLandmarkId++;

#ifdef DEBUG_MESSAGES_PDSHAPE2D
					std::cout << landmarks[i] << " ";					
#endif
				}

#ifdef DEBUG_MESSAGES_PDSHAPE2D
				std::cout << std::endl;
#endif

				shape->AddPolygon(sequence.size() - 1, landmarks);

			}
			else	// polyline
			{
				landmarks = new unsigned int[sequence.size()];

				int i;
				for (i = 0; i < sequence.size(); i++)
				{					
					landmarks[i] = currentLandmarkId++;

#ifdef DEBUG_MESSAGES_PDSHAPE2D
					std::cout << landmarks[i] << " ";					
#endif
				}

#ifdef DEBUG_MESSAGES_PDSHAPE2D
				std::cout << std::endl;
#endif

				shape->AddPolyline(sequence.size(), landmarks);
			}

			// release memory
			delete [] landmarks;			
		}

		else
		{
			if (sequence.size() == 1) 
			{
				currentLandmarkId++;
			}
		}

		seqIter++;
	}
*/

	blPDShapeInterface::Pointer shapeInterface = shape.GetPointer();
	return shapeInterface;
}





/**
 *	This method computes normal vectors at line and polygon points.
 *	A perpendicular vector will be considered as the vector obtained by 
 *  rotating a vector 90 counterclockwise.
 *
 *	It is supposed that each point belongs only to one given cell (line or polygon).
 *
 *	A point has at most two neighbor points. That is always true in polygons (closed),
 *	but not when a point is at the end of a polyline (only one neighbor).
 *
 *	For those points with 2 neighbors, normal vector is the mean vector of normals to
 *	both line segments defined by the given point and its neighbors.
 *
 *	For points with 1 neighbors, normal vector is the normal to line segment defined by
 *	the given point and its neighbor.
 *
 *	If polygons are defined in clockwise direction, the normals are outwards.
 *	If counterclockwise direction, inwards.
 */
//--------------------------------------------------
void blPDShape2D::ComputeNormals()
//--------------------------------------------------
{
	
	// typedefs
	typedef MeshType::CellsContainerPointer CellsContainerPointer;
	typedef MeshType::CellsContainer CellsContainer;
	typedef MeshType::CellType CellType;
	typedef CellType::PointIdConstIterator PointIdConstIterator;
	
	typedef MeshType::PointType PointType;		
	typedef MeshType::PointIdentifier PointIdentifier;
	
	typedef MeshType::PointDataContainer PointDataContainer;
	typedef MeshType::PointDataContainerPointer PointDataContainerPointer;

	typedef itk::LineCell<CellType> LineCellType;
	

	// CellsContainer
	// ----------------------------------------------------------------

	// Get the cells
	CellsContainerPointer cellContPt = this->ptMesh->GetCells();
	
	// Define cell iterators
	CellsContainer::Iterator cellIter = cellContPt->Begin();
	CellsContainer::Iterator cellIterEnd = cellContPt->End();
	

	// PointDataContainer
	// ----------------------------------------------------------------

	// get PointDataContainer from internal mesh
	PointDataContainerPointer pointDataContainerPt = 
											this->ptMesh->GetPointData();

	// if null, create a new one
	if (pointDataContainerPt.IsNull())
	{
		pointDataContainerPt = PointDataContainer::New();	
		this->ptMesh->SetPointData(pointDataContainerPt.GetPointer());
	}
	
	// reserve memory for all landmarks
	pointDataContainerPt->Reserve(this->numberLandmarks);

	

	
	// Vars. used in each iteration
	//-----------------------------------------------------------

	// pointId iterator
	PointIdConstIterator pointIdIterBegin;
	PointIdConstIterator pointIdIterEnd;

	// point identifier variables
	PointIdentifier previousPointId;
	PointIdentifier currentPointId;
	PointIdentifier nextPointId;
	
	// point type pointers 
    PointType * currentPointPt = new PointType();		// current point
    PointType * nextPointPt = new PointType();		// next point       
	PointType * previousPointPt = new PointType();	// previous point
	
	// pointer to normal vector computed in previous point
	TNormalVector * previousNormalVectorPt = new TNormalVector();
	TNormalVector * nextNormalVectorPt = new TNormalVector();
	TNormalVector auxNormalVector;


	// Compute normals
	//-----------------------------------------------------------
	
	CellType * cellPt;

	// polygons
	// ------------------------------

	// iterate over cellsContainer and calculate normals at each point 
	// for each polygon cell


	// For each cell C in itk::Mesh	
	while (cellIter != cellIterEnd)
	{	
		cellPt = cellIter.Value();

		if (cellPt->GetType() == CellType::TRIANGLE_CELL ||
			cellPt->GetType() == CellType::QUADRILATERAL_CELL ||
			cellPt->GetType() == CellType::POLYGON_CELL)
		{

			PointIdConstIterator currentPointIter, 
									previousPointIter, nextPointIter;
			// for each point 
				//--> previous and next normal
				//--> mean normal
			// -------------------------------------

			// initialize iterators 
			pointIdIterBegin = cellPt->PointIdsBegin();
			pointIdIterEnd = cellPt->PointIdsEnd();
			
			currentPointIter = pointIdIterBegin; // begin
			previousPointIter = pointIdIterEnd - 1; // end - 1
			nextPointIter = currentPointIter + 1;
		

			// first point
			// ---------------------------------
			TNormalVector lastNormalVector;	// last normal vector

			if (currentPointIter != pointIdIterEnd)
			{
				previousPointId = *previousPointIter;
				currentPointId = *currentPointIter;
				nextPointId = *nextPointIter;
				
				this->ptMesh->GetPoint(previousPointId, previousPointPt);
				this->ptMesh->GetPoint(currentPointId, currentPointPt);
				this->ptMesh->GetPoint(nextPointId, nextPointPt);
					

				// previous normal vector				
				this->ComputeNormalVector((*previousPointPt)[0],
											(*previousPointPt)[1],
					                        (*currentPointPt)[0],
					                        (*currentPointPt)[1],
					                        lastNormalVector);				

				// next normal vector				
				this->ComputeNormalVector((*currentPointPt)[0],
					                        (*currentPointPt)[1],
											(*nextPointPt)[0],
											(*nextPointPt)[1],
					                        *nextNormalVectorPt);

				// get reference to normal vector of current point
				TNormalVector & pointNormalVector = 
					pointDataContainerPt->ElementAt(currentPointId);


				// calculate mean normal vector and normalize				
				this->ComputeMeanNormalVector(lastNormalVector,
												(*nextNormalVectorPt),
												pointNormalVector);
				

				// update iterators
				// ----------------------
				previousPointIter = currentPointIter;
				currentPointIter++;
				nextPointIter++;
			}
		

			// rest of points, except the last one
			while(nextPointIter != pointIdIterEnd)
			{

				// update point ids				
				currentPointId = *currentPointIter;
				nextPointId = *nextPointIter;

				// update pointers
				PointType * pointPt = currentPointPt;
				currentPointPt = nextPointPt;		
				nextPointPt = pointPt;

				// get next point id				
				this->ptMesh->GetPoint(nextPointId, nextPointPt);
				
				// update pointers to normal vectors
				TNormalVector * tempPt = previousNormalVectorPt;
				previousNormalVectorPt = nextNormalVectorPt;
				nextNormalVectorPt = tempPt;

				// next normal vector				
				this->ComputeNormalVector((*currentPointPt)[0],
					                        (*currentPointPt)[1],
											(*nextPointPt)[0],
											(*nextPointPt)[1],
					                        *nextNormalVectorPt);

				// get reference to normal vector of current point
				TNormalVector & pointNormalVector = 
					pointDataContainerPt->ElementAt(currentPointId);

				// calculate mean normal vector and normalize				
				this->ComputeMeanNormalVector((*previousNormalVectorPt),
												(*nextNormalVectorPt),
												pointNormalVector);
			
				// update iterators
				// ----------------------
				previousPointIter = currentPointIter;
				currentPointIter = nextPointIter;
				nextPointIter++;			
			}

			// last point
			// ----------------------------------

			// update point ids				
			currentPointId = *currentPointIter;
			

			// update pointers to normal vectors
			TNormalVector * tempPt = previousNormalVectorPt;
			previousNormalVectorPt = nextNormalVectorPt;
			nextNormalVectorPt = tempPt;


			// get reference to normal vector of current point
			TNormalVector & pointNormalVector = 
				pointDataContainerPt->ElementAt(currentPointId);

			// calculate mean normal vector and normalize				
			this->ComputeMeanNormalVector((*previousNormalVectorPt),
											lastNormalVector,
											pointNormalVector);

		} // if polygon cell

		cellIter++;	// update iterator

	} // for each cell 


	// ------------------------------
	// polylines
	// ------------------------------


	// polyline iterators
	PolylinesContainerType::const_iterator polylineIter = 
										this->polylinesContainer.begin();

	PolylinesContainerType::const_iterator polylineIterEnd = 
										this->polylinesContainer.end();


	// for each polyline
	while(polylineIter != polylineIterEnd)
	{

		const PolylineType & currentPolyline = *polylineIter;


		PolylineType::const_iterator cellIdIter = currentPolyline.begin();
		PolylineType::const_iterator cellIdIterEnd = currentPolyline.end();

		// first line cell
		// ----------------------------

		cellPt = cellContPt->ElementAt(*cellIdIter);

		currentPointId = *(cellPt->PointIdsBegin());
		nextPointId = *(cellPt->PointIdsBegin() + 1);

		this->ptMesh->GetPoint(currentPointId, currentPointPt);
		this->ptMesh->GetPoint(nextPointId, nextPointPt);
		

		// get reference to normal vector of current point
		TNormalVector & pointNormalVector = 
			pointDataContainerPt->ElementAt(currentPointId);


		// previous normal vector				
		this->ComputeNormalVector((*currentPointPt)[0],
									(*currentPointPt)[1],
					                (*nextPointPt)[0],
					                (*nextPointPt)[1],
					                pointNormalVector);		

		// update iterator of line cell Ids
		cellIdIter++;
		(*previousNormalVectorPt)[0] = pointNormalVector[0];
		(*previousNormalVectorPt)[1] = pointNormalVector[1];



		// the rest of line cells 
		// --------------------------------

		while (cellIdIter != cellIdIterEnd)
		{
			// get line cell
			cellPt = cellContPt->ElementAt(*cellIdIter);

			// update point ids and point pointers
			currentPointId = nextPointId;
			nextPointId = *(cellPt->PointIdsBegin() + 1);

			PointType * pointPt = currentPointPt;
			currentPointPt = nextPointPt;
			nextPointPt = pointPt;

			// get next point
			this->ptMesh->GetPoint(nextPointId, nextPointPt);
			

			// previous normal vector				
			this->ComputeNormalVector((*currentPointPt)[0],
										(*currentPointPt)[1],
										(*nextPointPt)[0],
										(*nextPointPt)[1],
										*nextNormalVectorPt);		

			// get reference to normal vector of current point
			TNormalVector & pointNormalVector = 
				pointDataContainerPt->ElementAt(currentPointId);


			// calculate mean normal vector and normalize				
			this->ComputeMeanNormalVector(*previousNormalVectorPt,
											*nextNormalVectorPt,
											pointNormalVector);	

			// update normal pointers
			TNormalVector * normalPt = previousNormalVectorPt;
			previousNormalVectorPt = nextNormalVectorPt;
			nextNormalVectorPt = normalPt;

			cellIdIter++;
		}

		// second point of last line cell
		// ------------------------------------------
		
		// set normal vector to second point of last line cell
		pointDataContainerPt->SetElement(nextPointId, *previousNormalVectorPt);


		polylineIter++;	// update polyline iterator
	}


	// release memory
	delete previousNormalVectorPt;
	delete nextNormalVectorPt;

	delete previousPointPt;
	delete currentPointPt;
    delete nextPointPt;

}



/* 
 * This method computes the coordinates of a normal vector to 
 * the vector (x1-x0, y1-y0)
 *
 *	\param x0 x coordinate of point 0
 *	\param y0 y coordinate of point 0
 *	\param x1 x coordinate of point 1
 *	\param y1 y coordinate of point 1
 *	\param output output normal vector
 *
 */
//--------------------------------------------------
void blPDShape2D::ComputeNormalVector(double x0, double y0,
									  double x1, double y1,
									  TNormalVector & output)
//--------------------------------------------------	
{
    // given v=(x,y), the normal vector will be n=(-y,x)/|n|
            
    output[0] = y0-y1;
    output[1] = x1-x0;
    
    TNormalVector::ValueType norm = output.GetNorm();
    
    if (norm == 0)
    {
        output[0] = 0;
        output[1] = 0;
    }
    else
    {
        output[0] /= norm;
        output[1] /= norm;
    }
}



/* 
 * This method, given 2 vectors, compute the mean normalized vector
 *
 */
//--------------------------------------------------
void blPDShape2D::ComputeMeanNormalVector(const TNormalVector &vector1, 
											const TNormalVector &vector2,
											TNormalVector &output)
//--------------------------------------------------	
{
	output = vector1 + vector2;

    TNormalVector::ValueType norm = output.GetNorm();
    
    if (norm == 0)
    {
        output[0] = 0;
        output[1] = 0;
    }
    else
    {       
        output /= norm;
    }
}



/** 
 *	This method returns the cell IDs containing the specified landmark.
 *
 *	\param landmarkId landmark ID
 *	\return cell containing the landmark
 */
//--------------------------------------------------
const unsigned long blPDShape2D::GetCellId(unsigned int landmarkId)
//--------------------------------------------------
{
	if (! this->linksBuilt) 
	{
		this->ptMesh->BuildCellLinks();		
		this->linksBuilt = true;
	}


	// typedefs
	typedef MeshType::CellLinksContainerPointer CellLinksContainerPointer;	
	typedef MeshType::PointIdentifier PointIdentifier;
	typedef MeshType::PointCellLinksContainer PointCellLinksContainer;
	

	// Get the cellLinks container 
	CellLinksContainerPointer cellLinksContPt = 
						this->ptMesh->GetCellLinks();


	// search for cells containing landmark
	PointCellLinksContainer & pointCellLinksCont = 
								cellLinksContPt->ElementAt(landmarkId);


	return (*(pointCellLinksCont.begin()));	// return the first cellId
}



/** 
 *	This method creates a new copy from this PDShape.
 *	All the points, polylines and polygons are copied.
 *
 *	\return a pointer to the new shape
 */
//--------------------------------------------------
blPDShapeInterface::Pointer blPDShape2D::GetCopy()
//--------------------------------------------------
{

	// get points
	vnl_vector<double> points(this->numberDimensions * this->numberLandmarks);
	this->GetPoints(points);

	// get polygons, polylines
	ShapeCellsContainer polylines, polygons;
	this->GetPolylines(polylines);
	this->GetPolygons(polygons);

	// new shape
	blPDShape2D::Pointer newShape = blPDShape2D::New();
	newShape->InitializeFromPoints(points.begin(), this->numberLandmarks);
	newShape->InitializeCells(polygons, polylines);


	blPDShapeInterface::Pointer shapeInterface = newShape.GetPointer();
	return shapeInterface;
}





/** 
 *	
 */
//--------------------------------------------------
///void blPDShape2D::InitializeFromMesh(MeshType::Pointer mesh, 
///										unsigned int numberOfLines, 
///										unsigned int numberOfPolygons)
//--------------------------------------------------
///{
///	blPDShape<2>::InitializeFromMesh(mesh, numberOfLines, numberOfPolygons);
///	this->InitPolylinesDataStructure();
///}







/** 
 *	This method determines if the given point is inside the specified cell.
 *
 *	\param point given point 
 *	\param cellId cell identifier
 *	\return -2 if cell not found, -1 if point outside, 
 *			0 if on boundary, 1 if inside
 */
//--------------------------------------------------
int blPDShape2D::PointInCell(const double point[2], unsigned int cellId)
//--------------------------------------------------
{

	typedef MeshType::CellIdentifier CellIdentifierType;
	typedef MeshType::CellAutoPointer CellAutoPointerType;
	typedef MeshType::CellType CellType;
	typedef MeshType::PointType PointType;

	CellIdentifierType cellID = cellId;
	CellAutoPointerType cellPt;

	PointType meshPoint;

	bool cellFound = this->ptMesh->GetCell(cellID, cellPt);


	// vector to store point coordinates of polygon / polyline
	vnl_vector<double> pointCoordinates;
	
	int numberOfPoints = 0;


	if (cellFound)
	{
		switch(cellPt->GetType())
		{

			// Cell is a POLYGON
			// ------------------------------

			case CellType::TRIANGLE_CELL:
			case CellType::QUADRILATERAL_CELL:
			case CellType::POLYGON_CELL:
			{
				CellType::PointIdConstIterator pointIdIter = 
													cellPt->PointIdsBegin();

				CellType::PointIdConstIterator pointIdIterEnd = 
													cellPt->PointIdsEnd();

				
				// set_size vector of point coordinates
				numberOfPoints = cellPt->GetNumberOfPoints();
				pointCoordinates.set_size(numberOfPoints * this->numberDimensions);

				int pointCoordinatesIndex = 0;

				while (pointIdIter != pointIdIterEnd)
				{
//TODO::UNUSED VARIABLE
// 					bool pointFound = this->ptMesh->GetPoint(*pointIdIter, &meshPoint);
////////////
 					this->ptMesh->GetPoint(*pointIdIter, &meshPoint);

					// fill vector with coordinates
					pointCoordinates[pointCoordinatesIndex] = meshPoint[0];
					pointCoordinatesIndex++;

					pointCoordinates[pointCoordinatesIndex] = meshPoint[1];
					pointCoordinatesIndex++;

					pointIdIter++;			
				}

				
			}
			break;


			// Cell is a POLYLINE
			// ------------------------------

			case CellType::LINE_CELL:
			{
				// get polyline index
				unsigned int polylineIndex = this->GetPolylineIndex(cellId);

				
				if (polylineIndex == -1) 
				{
					return -2; // cell not found				
				}
				else
				{
					// get point Ids
					// ------------------------

					PolylineType & polyline = 
									this->polylinesContainer[polylineIndex];

					// vector with point Ids = num line cells + 1
					std::vector<unsigned int> polylineIds(polyline.size() + 1);

					std::vector<unsigned int>::iterator polylineIdsIter = 
														polylineIds.begin();

					PolylineType::const_iterator lineCellIdIter = 
														polyline.begin();
					PolylineType::const_iterator lineCellIdIterEnd = 
														polyline.end();

					if (lineCellIdIter != lineCellIdIterEnd)
					{																	
						// first and second points of first line cell						
						this->ptMesh->GetCell(*lineCellIdIter, cellPt);

						*polylineIdsIter = *(cellPt->PointIdsBegin());	// first
						polylineIdsIter++;

						*polylineIdsIter = *(cellPt->PointIdsBegin() + 1);	// second


						// second point of each other line cell
						
						while(lineCellIdIter != lineCellIdIterEnd)
						{							
							this->ptMesh->GetCell(*lineCellIdIter, cellPt);

							*polylineIdsIter = *(cellPt->PointIdsBegin() + 1);	// second

							polylineIdsIter++;
							lineCellIdIter++;
						}						
					}
					else
					{
						return -2; // no cells in this polyline
					}


					// get point coordinates
					// ------------------------

					// set_size vector of point coordinates
					numberOfPoints = polylineIds.size();
					pointCoordinates.set_size(numberOfPoints * this->numberDimensions);

					int pointCoordinatesIndex = 0;

					polylineIdsIter = polylineIds.begin();

					while (polylineIdsIter != polylineIds.end())
					{															
						
						this->ptMesh->GetPoint(*polylineIdsIter, &meshPoint);

						// fill vector with coordinates
						pointCoordinates[pointCoordinatesIndex] = meshPoint[0];
						pointCoordinatesIndex++;

						pointCoordinates[pointCoordinatesIndex] = meshPoint[1];
						pointCoordinatesIndex++;

						polylineIdsIter++;
					}			
				}									
			}
			break;


			default:
				return -2; // cell not found
		}

		int inside = blPDShape2D::PointInPolygon(point, pointCoordinates.begin(), 
																numberOfPoints);

		return inside;

	}

	else
	{
		return -2; // cell not found
	}
}



/**
 *	This method determines if a Point is inside a 2D Polygon.	
 *
 *	\param point point coordinates
 *	\param polygon polygon coordinates xyxyx..
 *	\param numberOfPoints number of points in the polygon
 *	\return -1 is point is outside, 0 if on boundary, 1 if inside
 */
//--------------------------------------------------
int blPDShape2D::PointInPolygon(const double point[], 
								const double polygon[], int numberOfPoints)
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_PDSHAPE2D
	std::cout << "blPDShape2D::PointInPolygon(): point--> " 
				<< point[0] << " " << point[1] << std::endl;
	std::cout << "blPDShape2D::PointInPolygon(): number of points in polygon " 
				<< numberOfPoints << std::endl;
#endif


	int crossings = 0;
	
	//double t;
	double cy;

	// pointers to current, next, previous point
	const double * currentPoint;
	const double * nextPoint;
	const double * previousPoint;


	int i;

	// for each poin of the polygon

	for (i = 0; i < numberOfPoints; i++)
	{	
		// update pointers
		// ------------------------------
		if (i == 0)
		{
			currentPoint = polygon;
			previousPoint = polygon + 2 * (numberOfPoints - 1);
		}
		else
		{
			previousPoint = currentPoint;
			currentPoint += 2;
		}

		if (i + 1 == numberOfPoints)
		{
			nextPoint = polygon;
		}
		else
		{
			nextPoint = currentPoint + 2;
		}


		// x coordinate between currentPoint.x and nextPoint.x
		if ( (currentPoint[0] < point[0] && point[0] < nextPoint[0]) || 
			 (currentPoint[0] > point[0] && point[0] > nextPoint[0]))
		{
//            t = (point[0] - nextPoint[0]) / (currentPoint[0] - nextPoint[0]);
//            cy = t * currentPoint[1] + (1-t) * nextPoint[1];

			cy = ((point[0] - nextPoint[0]) / (currentPoint[0] - nextPoint[0])) *
					(currentPoint[1] - nextPoint[1]) + nextPoint[1];

			if (point[1] == cy) 
			{
				return 0; //(on boundary)
			}
			else if (point[1] > cy) 
			{
				crossings++;
			}
		}

		// currentPoint is on the vertical ray
		if (currentPoint[0] == point[0] && currentPoint[1] <= point[1]) 
		{
			// point is on currentPoint position
			if (currentPoint[1] == point[1]) 
			{
				return 0; //(on boundary);
			}

			// currentPoint and nextPoint are on the vertical ray
			if (nextPoint[0] == point[0])
			{
				// if point.y is between currentPoint.y and nextPoint.y, 
				// point is on boundary
				if ((currentPoint[1] <= point[1] && point[1] <= nextPoint[1]) || 
					(currentPoint[1] >= point[1] && point[1] >= nextPoint[1]))
					return 0;//(on boundary);
			} 

			// if nextPoint.x > point.x --> crossing++
			else if (nextPoint[0] > point[0]) 
			{
				crossings++;
			}

			// if previousPoint.x > 
			if (previousPoint[0] > point[0]) 
			{
				crossings++;
			}
		}
	}

	//if (crossings is odd)
	if (crossings % 2 != 0)
	{
		return 1; // inside;
	}
	else 
	{
		return -1; //(outside);
	}

}


/**
 *
 */
//--------------------------------------------------
///
/*
void blPDShape2D::InitializeCells(const ShapeCellsContainer & polygons, 
							const ShapeCellsContainer & polylines)
//--------------------------------------------------
{

	// typedefs
	// -------------------------------------
	typedef MeshType::CellType CellType;
	typedef CellType::CellAutoPointer CellAutoPointer;
	typedef MeshType::CellsContainerPointer CellsContainerPointer;

	typedef itk::LineCell<CellType> LineCellType;
	typedef itk::TriangleCell<CellType> TriangleCellType;	
   	typedef itk::QuadrilateralCell<CellType> QuadrilateralCellType;	
   	typedef itk::PolygonCell<CellType> PolygonCellType;	


	// build first polygons, then polylines


	//------------------------------------
	// Cells
	//------------------------------------

  	// cellId counter
  	unsigned int cellId = 0;
  

	// remove all existing cells
	CellsContainerPointer cellsContainerPt = this->ptMesh->GetCells();
	cellsContainerPt->Initialize(); 

	this->linksBuilt = false;



	// Polygons
   	//----------------------------------------------  	
  
	ShapeCellsContainer::const_iterator cellIter = polygons.begin();
	ShapeCellsContainer::const_iterator cellIterEnd = polygons.end();

	PointIdsContainer::const_iterator pointIdIter, pointIdIterEnd;
  
	while (cellIter != cellIterEnd)
	{    
	 	const PointIdsContainer & pointIds = (*cellIter);

		pointIdIter = pointIds.begin();
		pointIdIterEnd = pointIds.end();


		CellAutoPointer cellPt;

	 	// different types of polygon:
		// ---------------------------------
		unsigned int numberPoints = pointIds.size();

	 	switch (numberPoints)
	 	{
		 	case 3: cellPt.TakeOwnership(new TriangleCellType);
		 			break;
		 			
		 	case 4: cellPt.TakeOwnership(new QuadrilateralCellType);
		 			break;
		 
		 	default:cellPt.TakeOwnership(new PolygonCellType);
		 			break;
	 	}	 		
	 	
		int pointId = 0;

#ifdef DEBUG_MESSAGES_PDSHAPE2D
		std::cout << "InitializeCells: Polygon: ";
#endif

		// for each point id in polygon
		while (pointIdIter != pointIdIterEnd)
	 	{

#ifdef DEBUG_MESSAGES_PDSHAPE2D
			std::cout << *pointIdIter << " ";
#endif

			cellPt->SetPointId(pointId, *pointIdIter);
			pointId++;
			pointIdIter++;
 		}

#ifdef DEBUG_MESSAGES_PDSHAPE2D
		std::cout << std::endl;
#endif

 		// add the cell to the Mesh	and update cellId	  	
	   	this->ptMesh->SetCell(cellId, cellPt);
	   	cellId++;


		cellIter++;	// update cell iterator
   	}   				


  	  			
   	// Polylines: Lines: 2-point lines
   	//----------------------------------------------

	this->polylinesContainer.clear();	// remove polylines	
	this->polylinesLUT.clear();			// remove all entries in map


	PolylineType currentPolyline;
	unsigned int polylineIndex = 0;
		 
	cellIter = polylines.begin();
	cellIterEnd = polylines.end();


	// for each polyline
   	while(cellIter != cellIterEnd)
   	{	 				
		currentPolyline.clear();

#ifdef DEBUG_MESSAGES_PDSHAPE2D		
		std::cout << "InitializeCells: Polyline: " ;
#endif

		const PointIdsContainer & pointIds = (*cellIter);


		if (pointIds.size() > 1) // more than one point
		{
			pointIdIter = pointIds.begin();
			pointIdIterEnd = pointIds.end();
			pointIdIterEnd--; // pointer to last point id

			// go through the vector of ids
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

				// add polyline information
				currentPolyline.push_back(cellId);			// new line cell Id
				this->polylinesLUT[cellId] = polylineIndex;	// new line cell in map

#ifdef DEBUG_MESSAGES_PDSHAPE2D
				std::cout << *pointIdIter << " ";
#endif

	   			// update cellId counter
	   			cellId++;
	   		
				// update iterator
				pointIdIter++;
			}

		}

#ifdef DEBUG_MESSAGES_PDSHAPE2D
		std::cout << std::endl;
#endif

		// add new polyline into container
		this->polylinesContainer.push_back(currentPolyline);
		polylineIndex++;

		
		cellIter++;	// update iterator
   	}

}
*/
///







//added by Costa 20/11/03


/* 
 * This method performs 2D Delaunay triangulation of the mesh
 * points are retrieved by GetPoints, result is stroed in TesselationCells
 * Function was ported with slight changes from WildMagic Library
 *
 */
//--------------------------------------------------
void blPDShape2D::Tesselate(bool forceTesselation)
//--------------------------------------------------
{
	//force tesselation
	if(forceTesselation) TesselationCells.clear(); 

	//Tesselation was already performed
	if( !TesselationCells.empty() ) return;

	// The extra three slots are required for temporary storage -- WildMagic stuff
	vnlVectorDouble akVertex(numberLandmarks*numberDimensions);
	GetPoints(akVertex); //get all the landmarks (not IDs, but real points)

	const blPDShape2D::MeshType::BoundingBoxType* BoundingBox = 
													ptMesh->GetBoundingBox();
	itk::FixedArray<double,4> Bounds = BoundingBox->GetBounds();
	
	BoundingBox2D box;

    box.m_fXMin = Bounds[0];
    box.m_fXMax = Bounds[1];
    box.m_fYMin = Bounds[2];
    box.m_fYMax = Bounds[3];

	blDelaunay2D::blDelaunay2D(akVertex,TesselationCells, &box);
}
//:~





/** 
 * 
 *
 */
//--------------------------------------------------
///
/*
void blPDShape2D::InitPolylinesDataStructure()
//--------------------------------------------------
{

	if (! this->linksBuilt) 
	{
		this->ptMesh->BuildCellLinks();		
		this->linksBuilt = true;
	}


	// go through the mesh and fill the polydata structure with info


	MeshType::CellsContainerPointer cellsContainerPt = 
										this->ptMesh->GetCells();



	MeshType::CellsContainer::ConstIterator cellIter = 
												cellsContainerPt->Begin();

	MeshType::CellsContainer::ConstIterator cellIterEnd = 
												cellsContainerPt->End();


	typedef MeshType::CellType CellType;
	
	const CellType * cellPt;
	const CellType * previousCellPt;
	CellIdentifier cellId;

	bool firstLineCell = true;
	PolylineType polyline;
	CellIdentifier polylineIndex = 0;	// first index --> 0


	// iterate over cells
	while (cellIter != cellIterEnd)
	{
		previousCellPt = cellPt;	// update previous pt

		cellId = cellIter.Index();
		cellPt = cellIter.Value();
		
		// LINE_CELL

		// supposed that the lines that make up a polyline and ordered,
		// so first line cell will go first in cell container

		if (cellPt->GetType() == CellType::LINE_CELL)
		{			
			if (firstLineCell)
			{
				firstLineCell = false;

				polyline.clear();		// new polyline, polylineId = first line ID	
			}

			else
			{
				// check current cell is conected to previous

				CellType::PointIdConstIterator previousPointId = previousCellPt->PointIdsBegin();
				previousPointId++; // 2nd point of line

				CellType::PointIdConstIterator pointId = cellPt->PointIdsBegin();

				// connected
				if (*previousPointId == *pointId)
				{
					// nothing
				}
				else
				{
					// store current polyline
					this->polylinesContainer.push_back(polyline);

					// new polyline					
					polyline.clear();
					polylineIndex++;
				}

			}

			// add line cell ID to current polyline
			polyline.push_back(cellId);

			// add new pair to LUT
///			LinePolylinePairType pair(cellId, polylineId);
			this->polylinesLUT[cellId] = polylineIndex;

		}
		
		// if cell type != LINE_CELL
		else
		{
			// end of polyline
			// add current polyline to polyline container

			// the next line will be the first of a new sequence
			firstLineCell = true;	

			this->polylinesContainer.push_back(polyline);		
		}

		cellIter++;	// update cell iterator
	}


	if (polyline.size() > 0)
	{
		// insert last polyline into polylinesContainer
		this->polylinesContainer.push_back(polyline);
	}



	// polylinesContainer

#ifdef DEBUG_MESSAGES_PDSHAPE2D
	std::vector<PolylineType>::const_iterator polyIter = 
										this->polylinesContainer.begin();

	std::vector<PolylineType>::const_iterator polyIterEnd = 
										this->polylinesContainer.end();

	while (polyIter != polyIterEnd)
	{
		std::cout << "New polyline: ";

		const PolylineType & polyline = *polyIter;

		PolylineType::const_iterator cellIdIter = polyline.begin();
		PolylineType::const_iterator cellIdIterEnd = polyline.end();

		while(cellIdIter != cellIdIterEnd)
		{
			std::cout << *cellIdIter << " ";

			cellIdIter++;
		}

		std::cout << std::endl;

		polyIter++;
	}




	// LUT

///	std::vector<LinePolylinePairType>::const_iterator pairIter = 
///												this->polylinesLUT.begin();

///	std::vector<LinePolylinePairType>::const_iterator pairIterEnd = 
///												this->polylinesLUT.end();
	
	LinePolylineLUTType::const_iterator pairIter = this->polylinesLUT.begin();
	LinePolylineLUTType::const_iterator pairIterEnd = this->polylinesLUT.end();

	while(pairIter != pairIterEnd)
	{

		std::cout << "line cell ID:" << (*pairIter).first 
					<< ", polylineIndex: " << (*pairIter).second << std::endl;


		pairIter++;
	}
#endif


}
*/
///


/**
 *	Get the polyline index in polylinesContainer.
 *
 *	\return -1 if not found, otherwise polylineIndex
 */
///
/*
blPDShape2D::CellIdentifier blPDShape2D::GetPolylineIndex(CellIdentifier lineCellId)
{

	LinePolylineLUTType::const_iterator pairIter = 
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

*/
///
