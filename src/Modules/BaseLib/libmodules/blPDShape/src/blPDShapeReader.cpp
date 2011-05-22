/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
//This class allow to create a new PDShape from a VTK file

#pragma warning (disable : 4786) 

#include <itkLineCell.h>
#include <itkTriangleCell.h>
#include <itkQuadrilateralCell.h>
#include <itkPolygonCell.h>
#include <itkExceptionObject.h>

#include "blPDShapeReader.h"
///#include "blDebug.h"
#include "blStringTokenizer.h"




/** 
 *	Constructor
 */
//--------------------------------------------------
blPDShapeReader::blPDShapeReader()
//--------------------------------------------------
{
	this->numberDimensions = 3;	//by default 3D
	this->numberLines = 0;
	this->numberPoints = 0;
	this->numberPolys = 0;
	this->pdShape = 0;
	
	this->inDataArray = false;
	this->inDataArrayConnectivity = false;
	this->inDataArrayOffset = false;
	this->inLines = false;
	this->inPoints = false;
	this->inPolys = false;

	numberOfDimension_found = false;
}



/** 
 *	Destructor
 */
//--------------------------------------------------
blPDShapeReader::~blPDShapeReader()
//--------------------------------------------------
{
}



/** 
 *	This method initializes all the member variables 
 *	before a new reading starts.
 */
//--------------------------------------------------
void blPDShapeReader::Initialize()
//--------------------------------------------------
{
	// blags indicating where the parse is currently
	this->inDataArray = false;
	this->inDataArrayConnectivity = false;
	this->inDataArrayOffset = false;

	this->inLines = false;
	this->inPoints = false;
	this->inPolys = false;


	// Data obtained from the file
	// ----------------------------------------------
	this->numberDimensions = 3;	//by default 3D
	this->numberLines = 0;
	this->numberPoints = 0;
	this->numberPolys = 0;
	this->pdShape = 0;


	// vectors of coordinates and ids
	this->points.clear();

	this->linesPointsIds.clear();
	this->linesPointsOffsets.clear();

	this->polysPointsIds.clear();
	this->polysPointsOffsets.clear();
}



/** 
 *	This method reads from the specified file and creates a new
 *	blPDShape instance.
 *
 *	The blPDShape will be 2D or 3D depending on the data read from the file.
 * 
 *	If the file cannot be read, the PDShape will be null.
 */
//--------------------------------------------------		
void blPDShapeReader::Update()
//--------------------------------------------------
{

	this->Initialize();	// first initialize all variables


// read from a .vtp file 
//=======================================================================

	try 
	{
		this->GenerateOutputInformation();
	}
	catch (itk::ExceptionObject ex)
	{
		ex.Print(std::cerr);
		return;
	}
	

	// copy point coordinates in a buffer
	// -----------------------------
	double * pointsBuffer = new double[this->numberPoints * this->numberDimensions];

	size_t i;
	int pointPosition = 0;
	for (i = 0; i < this->numberPoints; i++)
	{	
		size_t d;
		for (d = 0; d < this->numberDimensions; d++)
		{
			pointsBuffer[pointPosition + d] = this->points[i * 3 + d];
		}
		
		pointPosition += this->numberDimensions; 
	}


	if (this->numberDimensions == 2)
	{
		blPDShape2D::Pointer shape = blPDShape2D::New();

		// initialize points
		shape->InitializeFromPoints(pointsBuffer, this->numberPoints);

		// build polygons and polylines
		// --------------------------------
		blPDShapeInterface::ShapeCellsContainer polygons, polylines;
		this->BuildPolygons(polygons);
		this->BuildPolylines(polylines);

		// initialize cells in shape 
		shape->InitializeCells(polygons, polylines);	

		this->pdShape = shape.GetPointer();

	}
	else if (this->numberDimensions == 3)
	{
		blPDShape3D::Pointer shape = blPDShape3D::New();


		// initialize points
		shape->InitializeFromPoints(pointsBuffer, this->numberPoints);

		// build polygons and polylines
		// --------------------------------
		blPDShapeInterface::ShapeCellsContainer polygons, polylines;
		this->BuildPolygons(polygons);
		this->BuildPolylines(polylines);

		// initialize cells in shape 
		shape->InitializeCells(polygons, polylines);	

		this->pdShape = shape.GetPointer();
	}

	delete [] pointsBuffer;	// free memory
}





/**
 * determine whether a file can be opened and read 
 */
//--------------------------------------------------
int blPDShapeReader::CanReadFile(const char* name)
//--------------------------------------------------
{
	return 1;
}



/** 
 *	Callback function -- called from XML parser with start-of-element
 *	information.
 */
//--------------------------------------------------
void blPDShapeReader::StartElement(const char * name,
											 const char **atts)
//--------------------------------------------------
{
	unsigned int temp_k;

	if (strcmp(name,"Piece") == 0)
	{
		// initialize the points vectors
		temp_k = 0;
		while ( STRING_NOCASE_COMPARE(atts[temp_k], "NumberOfPoints") )
		{
			temp_k++;
		}
		numberPoints = atoi(atts[temp_k + 1]);
		points.reserve(numberPoints);
		
		// initialize the lines offsets vectors
		temp_k = 0;
		while ( STRING_NOCASE_COMPARE(atts[temp_k], "NumberOfLines") )
		{
			temp_k++;
		}
		numberLines = atoi(atts[temp_k + 1]);
		linesPointsOffsets.reserve(numberLines);

		// initialize the polygon offsets vector
		temp_k = 0;
		while ( STRING_NOCASE_COMPARE(atts[temp_k], "NumberOfPolys") )
		{
			temp_k++;
		}
		numberPolys = atoi(atts[temp_k + 1]);
		polysPointsOffsets.reserve(numberPolys);
	}

	else if (strcmp(name,"Points") == 0)
	{
		if (atts)
		{
			temp_k = this->findStringInXmlAtts ("NumberOfDimensions", atts);		
			if ( atts[temp_k + 1] )
			{
				this->numberDimensions = atoi(atts[temp_k + 1]);
				numberOfDimension_found = true;
			}

			temp_k = this->findStringInXmlAtts ("NumberOfComponents", atts);		
			if ( atts[temp_k + 1] )
			{
				if (!numberOfDimension_found)
				{
					this->numberDimensions = atoi(atts[temp_k + 1]);
				}				
				inPoints = true;
			}
			
		}

		// Well, evidently there will be some problem if there is no explicit reference to the number of
		// dimension in the vtk file.....but why would that happen?
		//else
		//{
		//	this->numberDimensions = 3;
		//}
		
	}

	else if (strcmp(name,"Lines") == 0)
	{
		inLines = true;
	}

	else if (strcmp(name,"Polys") == 0)
	{
		inPolys = true;
	}

	else if (strcmp(name,"DataArray") == 0)
	{
		if (!inPoints)
		{			
			temp_k = this->findStringInXmlAtts ("NumberOfComponents", atts);		
			if ( atts[temp_k + 1] )
			{
				if (!numberOfDimension_found)
				{
					this->numberDimensions = atoi(atts[temp_k + 1]);
				}				
				inPoints = true;
			}
		}		
		inDataArray = true;
		

		// Connectivity
		temp_k = this->findStringInXmlAtts ("connectivity", atts);		
		if ( atts[temp_k + 1] )
		{
			inDataArrayConnectivity = true;
		}

		// Offsets
		temp_k = this->findStringInXmlAtts ("offsets", atts);		
		if ( atts[temp_k + 1] )
		{
			inDataArrayOffset = true;
		}

/*		if (strcmp (atts[DATAARRAY_POS_ATTR_CONNECTIVITY], 
						"connectivity") == 0)
		{
			inDataArrayConnectivity = true;
		}

		else if (strcmp (atts[DATAARRAY_POS_ATTR_OFFSET], "offsets") == 0)
		{
			inDataArrayOffset = true;
		}*/
	}
}



/** 
 *	Callback function -- called from XML parser when ending tag
 *	encountered
 */
//--------------------------------------------------
void blPDShapeReader::EndElement(const char *name)
//--------------------------------------------------
{
	
	if (strcmp(name,"VTKFile") == 0)
	{
		
	}

	else if (strcmp(name,"PolyData") == 0)
	{
		
	}

	else if (strcmp(name,"Piece") == 0)
	{
	
	}

	else if (strcmp(name,"Points") == 0)
	{
		inPoints = false;

#ifdef DEBUG_MSG_2		
		
		std::vector<double>::iterator iter, iterEnd;

		iter = this->points.begin();
		iterEnd = this->points.end();


		std::cout << "\t Points *****************" << points.size() << std::endl;

		while (iter != iterEnd)
		{
		
			std::cout << "\t" << *iter << " " ;
			iter++;
			std::cout << *iter << " " ;
			iter++;
			std::cout << *iter << " " << std::endl;
			iter++;
		}
#endif

	}

	else if (strcmp(name,"Lines") == 0)
	{
		inLines = false;

#ifdef DEBUG_MSG_2
		std::vector<unsigned int>::iterator iter, iterEnd;

		iter = this->linesPointsIds.begin();
		iterEnd = this->linesPointsIds.end();


		std::cout << "\t Lines:Points Ids " << 
					this->linesPointsIds.size() << std::endl;

		std::cout << "\t" << " " ;
		while (iter != iterEnd)
		{
			std::cout << *iter << " " ;
			iter++;
		}

		std::cout << std::endl;

		iter = this->linesPointsOffsets.begin();
		iterEnd = this->linesPointsOffsets.end();


		std::cout << "\t Lines: Points Offsets " << 
					this->linesPointsOffsets.size() << std::endl;

		std::cout << "\t" << " " ;
		while (iter != iterEnd)
		{
			std::cout << *iter << " " ;
			iter++;
		}

		std::cout << std::endl;
#endif

	}

	else if (strcmp(name,"Polys") == 0)
	{
		inPolys = false;

#ifdef DEBUG_MSG_2
		std::vector<unsigned int>::iterator iter, iterEnd;

		iter = this->polysPointsIds.begin();
		iterEnd = this->polysPointsIds.end();


		std::cout << "\t Polygons: Points Ids " << 
					this->polysPointsIds.size() << std::endl;

		std::cout << "\t" << " " ;
		while (iter != iterEnd)
		{
			std::cout << *iter << " " ;
			iter++;
		}

		std::cout << std::endl;


		iter = this->polysPointsOffsets.begin();
		iterEnd = this->polysPointsOffsets.end();


		std::cout << "\t Polygons: Points Offsets " << 
					this->polysPointsOffsets.size() << std::endl;

		std::cout << "\t" << " " ;
		while (iter != iterEnd)
		{
			std::cout << *iter << " " ;
			iter++;
		}

		std::cout << std::endl;
#endif
	}

	else if (strcmp(name,"DataArray") == 0)
	{
		inDataArray = false;
		inDataArrayConnectivity = false;
		inDataArrayOffset = false;
	}
}



/** 
 *	Callback function -- called from XML parser with the character data
 *	for an XML element
 */
//--------------------------------------------------
void blPDShapeReader::CharacterDataHandler(const char *inData, 
											int inLength)
//--------------------------------------------------
{

	// Get the indexes of tokens in the string inData

	this->tokenizer.Tokenize(inData, inLength);


	// DataArray in tag Points
	//-----------------------------------------------------------------

	if (inPoints && inDataArray)
	{
		double pointCoord;

		while (tokenizer.HasMoreTokens())
		{
			pointCoord = atof(tokenizer.NextToken());
			this->points.push_back(pointCoord);
		}
	}


	// DataArray in tag Lines
	//-----------------------------------------------------------------
	else if (this->inLines && this->inDataArray)
	{
		int pointId;

		// DataArray of Connectivity
		if (this->inDataArrayConnectivity)
		{
			while (this->tokenizer.HasMoreTokens())
			{
				pointId = atoi(this->tokenizer.NextToken());
				this->linesPointsIds.push_back(pointId);
			}
		}

		// DataArray of Offsets
		else if (this->inDataArrayOffset)
		{
			while (this->tokenizer.HasMoreTokens())
			{
				pointId = atoi(this->tokenizer.NextToken());
				this->linesPointsOffsets.push_back(pointId);
			}
		}
	}


	// DataArray in tag Polys
	//-----------------------------------------------------------------
	else if (this->inPolys && this->inDataArray)
	{
		int pointId;

		// DataArray of Connectivity
		if (this->inDataArrayConnectivity)
		{
			while (this->tokenizer.HasMoreTokens())
			{
				pointId = atoi(this->tokenizer.NextToken());
				this->polysPointsIds.push_back(pointId);
			}
		}
		
		// DataArray of Offsets
		else if (this->inDataArrayOffset)
		{
			while (this->tokenizer.HasMoreTokens())
			{
				pointId = atoi(this->tokenizer.NextToken());
				this->polysPointsOffsets.push_back(pointId);
			}
		}
	}

}



/** 
 *	
 */
///
/*
//--------------------------------------------------
template <typename TMesh>
void blPDShapeReader::BuildMesh(TMesh * meshPt)
//--------------------------------------------------
{

	// typedefs
	// -------------------------------------
	typedef TMesh MeshType;
	typedef MeshType::PointType PointType;
	typedef MeshType::PointsContainer PointsContainer;

	typedef MeshType::CellType CellType;
	typedef CellType::CellAutoPointer CellAutoPointer;

	typedef itk::LineCell<CellType> LineCellType;
	typedef itk::TriangleCell<CellType> TriangleCellType;	
   	typedef itk::QuadrilateralCell<CellType> QuadrilateralCellType;	
   	typedef itk::PolygonCell<CellType> PolygonCellType;	

	//------------------------------------
	// Points
	//------------------------------------
	
	// new points container
	PointsContainer::Pointer meshPoints = PointsContainer::New();
	meshPoints->Reserve(this->numberPoints);

		
	// copy points to pointsContainer
	double * doublePoint = new double[this->numberDimensions];

	int i;
	for (i = 0; i < this->numberPoints; i++)
	{
		int dim;
		for (dim = 0; dim < this->numberDimensions; dim++)
		{
			doublePoint[dim] = this->points[i * 3 + dim];
		}
	  			
		meshPoints->SetElement(i, PointType(doublePoint));
	}

	// add the points to the mesh
	meshPt->SetPoints(meshPoints.GetPointer());

	// free memory
	delete [] doublePoint;


	//------------------------------------
	// Cells
	//------------------------------------

  	// cellId counter
  	unsigned int cellId = 0;
  
  	  			
   	// Lines: 2-point lines
   	//----------------------------------------------

	// define iterators for offsets and ids
	std::vector<unsigned int>::iterator offsetIter, offsetIterEnd;
   	std::vector<unsigned int>::iterator idIter, idIterBegin, idIterEnd;

	unsigned int currentOffset;

   	offsetIter = this->linesPointsOffsets.begin();
	offsetIterEnd = this->linesPointsOffsets.end();
	
	idIterBegin = this->linesPointsIds.begin();
	idIter = idIterBegin;

	// go through the vector of offsets
   	while(offsetIter != offsetIterEnd)
   	{	 				

		currentOffset = *offsetIter;


//std::cout << "PDShapeReader::BuildMesh()--> New offset: " << currentOffset << std::endl;


		// idIterEnd points to the last element in id list
		idIterEnd = idIterBegin + currentOffset - 1;


//std::cout << "PDShapeReader::BuildMesh()--> " 
//			<< *idIter << " " << *idIterEnd << std::endl;


		// go through the vector of ids, for a each offset
   		while(idIter != idIterEnd)
   		{
			CellAutoPointer cellPt;				
		
			// new cell of type Line
	 		cellPt.TakeOwnership(new LineCellType);
	 	 				 	
		 	// add 2 points to the new LineCell
	 		cellPt->SetPointId(0,*idIter);
		 	cellPt->SetPointId(1,*(idIter + 1));
			
		 	// add the cell to the Mesh	 	  	
	   		meshPt->SetCell(cellId, cellPt); 			 	


//std::cout << "PDShapeReader::BuildMesh()--> New Line Cell: " << cellId ;
//std::cout << " , pointIds: " << *idIter << " " << *(idIter + 1) << std::endl;

	   		// update cellId counter
	   		cellId++;
	   	
			// update iterator
			idIter++;
		}

		// update idIter to point to the first element of next line 
		idIter++;

		offsetIter++;
   	}


	// Polygons
   	//----------------------------------------------  	
   	
   	offsetIter = this->polysPointsOffsets.begin();
	offsetIterEnd = this->polysPointsOffsets.end();

	idIterBegin = this->polysPointsIds.begin();
	idIter = idIterBegin;

	
    // go through the offsets vector
   	while(offsetIter != offsetIterEnd)
   	{
	 	CellAutoPointer cellPt;

	 	// different types of polygon:

		idIterEnd = idIterBegin + *offsetIter;

		unsigned int numberPoints = idIterEnd - idIter;

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


		// for each point id in polygon
		while (idIter != idIterEnd)
	 	{
			cellPt->SetPointId(pointId,*idIter);
			pointId++;
			idIter++;
 		}

 		// add the cell to the Mesh	and update cellId	  	
	   	meshPt->SetCell(cellId, cellPt);
	   	cellId++;

		offsetIter++;
   	}   				

}

*/
///


/**
 *	Fill the provided container with point ids defining polygons
 */
//--------------------------------------------------
void blPDShapeReader::BuildPolygons(
								blPDShape2D::ShapeCellsContainer & polygons)
//--------------------------------------------------
{
	this->BuildCells(this->polysPointsOffsets, this->polysPointsIds, polygons);
}




/**
 *	Fill the provided container with point ids defining polygons
 */
//--------------------------------------------------
void blPDShapeReader::BuildPolylines(
								blPDShape2D::ShapeCellsContainer & polylines)
//--------------------------------------------------
{
	this->BuildCells(this->linesPointsOffsets, 
						this->linesPointsIds, polylines);
}




/**
 *	Fill the provided container with point ids defining cells
 */
//--------------------------------------------------
void blPDShapeReader::BuildCells(const PointsIdsVector & offsets, 
								const PointsIdsVector & ids, 
								blPDShape2D::ShapeCellsContainer & cells)
//--------------------------------------------------
{
	// cells
	cells.resize(offsets.size());
	blPDShape2D::ShapeCellsContainer::iterator cellIter = cells.begin();


	// offset iterators
	PointsIdsVector::const_iterator offsetIter = offsets.begin();
	PointsIdsVector::const_iterator offsetIterEnd = offsets.end();

	// ids iterators
	PointsIdsVector::const_iterator idsIterBegin, idsIter, idsIterEnd;
	idsIterBegin = ids.begin();


	unsigned int previousOffset = 0;
	unsigned int currentOffset = 0;

	// for each cell
	while (offsetIter != offsetIterEnd)
	{
		previousOffset = currentOffset;
		currentOffset = *offsetIter;
		
#ifdef DEBUG_MESSAGES_PDSHAPE_READER
		std::cout <<"blPDShapeReader::BuildCells: New cell: ";
#endif

		// resize current ids container
		blPDShape2D::PointIdsContainer & currentCell = *cellIter;
		currentCell.resize(currentOffset - previousOffset);

		// iterator over cell ids
		blPDShape2D::PointIdsContainer::iterator cellIdsIter = currentCell.begin();

		// update iterators over list of ids
		idsIter = idsIterBegin + previousOffset;
		idsIterEnd = idsIterBegin + currentOffset;

		while (idsIter != idsIterEnd)
		{
#ifdef DEBUG_MESSAGES_PDSHAPE_READER
			std::cout << *idsIter << " ";
#endif
			(*cellIdsIter) = (*idsIter);

			idsIter++;
			cellIdsIter++;
		}

#ifdef DEBUG_MESSAGES_PDSHAPE_READER
		std::cout << std::endl;
#endif

		offsetIter++;	// update offset iterator and cell iterator
		cellIter++;
	}
}

/**
*	Returns the position right before of ATTS at which theString was found
*/
//--------------------------------------------------
unsigned int blPDShapeReader::findStringInXmlAtts(
	const char* theString, const char **atts)
//--------------------------------------------------
{
	unsigned int temp_k = 0;
	while (!atts[temp_k])
	{
		temp_k++;
	}
	while ( STRING_NOCASE_COMPARE(atts[temp_k], theString) )
	{
		temp_k++;
		if (!atts[temp_k])
		{
			temp_k--;
			break;
		}
	}
	return temp_k;
}