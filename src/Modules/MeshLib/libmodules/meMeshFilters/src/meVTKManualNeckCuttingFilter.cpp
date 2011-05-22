/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKManualNeckCuttingFilter.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkShortArray.h"

#include "blShapeUtils.h"

const std::string STR_ARRAY_NAME_CELLS_SELECTION = "selection";

vtkStandardNewMacro(meVTKManualNeckCuttingFilter);


meVTKManualNeckCuttingFilter::meVTKManualNeckCuttingFilter()
{
	// two outputs: the input mesh to be cut and the line containin the point along which to cut the neck
	this->SetNumberOfInputPorts(2);
	// two output: the two regions as a result of the cut
	this->SetNumberOfOutputPorts(2);
}


meVTKManualNeckCuttingFilter::~meVTKManualNeckCuttingFilter() 
{
}


int meVTKManualNeckCuttingFilter::RequestData(
	vtkInformation *vtkNotUsed(request),
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{
	try
	{	
	// -------------------------------------------  
	// get the info objects
	vtkInformation *inInfoInputMesh = inputVector[0]->GetInformationObject(0);
	vtkInformation *inInfoLandmarksLine = inputVector[1]->GetInformationObject(0);
	vtkInformation *outInfoMeshComponent1= outputVector->GetInformationObject(0);
	vtkInformation *outInfoMeshComponent2 = outputVector->GetInformationObject(1);

	// get the input and ouptut
	vtkPolyData *inputMesh = vtkPolyData::SafeDownCast(
		inInfoInputMesh->Get(vtkDataObject::DATA_OBJECT()));

	vtkPolyData *inputLandmarksLine = vtkPolyData::SafeDownCast(
		inInfoLandmarksLine->Get(vtkDataObject::DATA_OBJECT()));

	vtkPolyData *outputMeshComponent1 = vtkPolyData::SafeDownCast(
		outInfoMeshComponent1->Get(vtkDataObject::DATA_OBJECT()));

	vtkPolyData *outputMeshComponent2 = vtkPolyData::SafeDownCast(
		outInfoMeshComponent2->Get(vtkDataObject::DATA_OBJECT()));


	//Step 1: orient input mesh
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkPolyDataNormals::New();
	normals->SetInput(inputMesh);
	normals->SplittingOff();

	normals->Update();
	vtkSmartPointer<vtkPolyData> inputMeshOriented = vtkPolyData::New();
	inputMeshOriented->DeepCopy(normals->GetOutput());

	
	//Step 2: Label the cells with 1
	LabelBorderCells( inputLandmarksLine, inputMeshOriented );

	//Step 3: Label the cells with 2
	LabelNeighborCells( inputLandmarksLine, inputMeshOriented );

	//Step 4: threshold and create two surfaces, and get aneurysm
	vtkSmartPointer<vtkPolyData> region1;
	region1 = GetFirstComponent( inputLandmarksLine, inputMeshOriented );
	vtkSmartPointer<vtkPolyData> region2;
	region2 = GetSecondComponent( inputLandmarksLine, inputMeshOriented );
		

	outputMeshComponent1->DeepCopy( region1 );
	outputMeshComponent2->DeepCopy( region2 );

	return 1;
	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return 0;
	}

}



void meVTKManualNeckCuttingFilter::LabelBorderCells( 
	vtkSmartPointer<vtkPolyData> landmarksLine, 
	vtkSmartPointer<vtkPolyData> inputMeshOriented )
{
	//Step 3: add array to all the cells, with value 0;
	vtkSmartPointer<vtkShortArray> label = vtkShortArray::New();
	label->SetName( STR_ARRAY_NAME_CELLS_SELECTION.c_str() );
	for (size_t i =0; i< inputMeshOriented->GetNumberOfCells(); i++)
		label->InsertNextValue(0);
	inputMeshOriented->GetCellData()->AddArray(label);

	//Step 4: for every edge, check the orientation and compare with the two adiacent triangles, 
	//			then take the one that has a different orientation and label currentLineId as border line
	inputMeshOriented->BuildCells();
	inputMeshOriented->BuildLinks();
	//landmarksLine->BuildLinks();

	// Selected cells array
	vtkDataArray *selectedArray;
	selectedArray = inputMeshOriented->GetCellData()->GetArray(STR_ARRAY_NAME_CELLS_SELECTION.c_str());


	// For each line in the landmarks line
	for( vtkIdType currentLineId=0; currentLineId < landmarksLine->GetNumberOfLines(); currentLineId++)
	{

		// Get the 2 points of the line in the input mesh
		vtkIdType pointID1InputMesh;
		vtkIdType pointID2InputMesh;
		GetInputMeshIDs( 
			landmarksLine, inputMeshOriented, currentLineId, 
			pointID1InputMesh,
			pointID2InputMesh );

		//std::cout<< "Looking at edge" << currentLineId << std::endl;
		//std::cout << " cellPoint1 : " << cellPoint1[0] << " , " << cellPoint1[1] << "id : " <<  inputMeshOriented->FindPoint(cellPoint1)<< std::endl;
		//std::cout << " cellPoint1 : " << cellPoint2[0] << " , " << cellPoint2[1] << "id : " <<  inputMeshOriented->FindPoint(cellPoint2)<< std::endl;
		//std::cout << "list0 :" << cellPoints->GetId(0);
		//std::cout << "list1 :" << cellPoints->GetId(1);
		// Check if the line is an edge of the input mesh
		//bool isEdge;
		//isEdge = inputMeshOriented->IsEdge( pointID1InputMesh, pointID2InputMesh );

		// For the point1, Get the cells in the input mesh
		vtkSmartPointer< vtkIdList> cellIdsPoint1 = vtkIdList::New();
		inputMeshOriented->GetPointCells( pointID1InputMesh, cellIdsPoint1);

		// For each cell -> Mark as selected if it is boundary and oriented clockwise
		vtkIdType j=0;
		bool cellFound = false;
		while ( j < cellIdsPoint1->GetNumberOfIds() && !cellFound )
		{
			vtkIdType currentCellID = cellIdsPoint1->GetId(j);

			// Check if the point2 is used by this cell
			bool cellIsUsingTheLine = false;
			cellIsUsingTheLine = inputMeshOriented->IsPointUsedByCell(
				pointID2InputMesh,
				currentCellID );
			
			if ( cellIsUsingTheLine )
			{
				// After normals filter is applied, the points id are ordered 
				// clockwise (or vice versa), then the triangles at one side of
				// the line have the points of the segment in the opposite
				// order than the triangles of the other side
				vtkSmartPointer< vtkIdList> currentCellPtId = vtkIdList::New();
				inputMeshOriented->GetCellPoints( currentCellID, currentCellPtId);

				for (size_t ptId=0; ptId< 3; ptId++)
				{
					//std::cout << "j :" << j << ", l : " << l <<std::endl;
					//std::cout << "id :" << currentCellPtId->GetId(l) << ", id+1 : " << currentCellPtId->GetId((l+1)%3) <<std::endl;
					//std::cout << "id1 :" << inputMeshOriented->FindPoint(cellPoint1) << ", id2 : " << inputMeshOriented->FindPoint(cellPoint2) <<std::endl;

					if (currentCellPtId->GetId( ptId ) == pointID1InputMesh &&
						currentCellPtId->GetId( (ptId+1)%3 ) == pointID2InputMesh)
					{
						selectedArray->SetTuple1( currentCellID, 1);
						cellFound = true;
					}
				}
			}

			j++;
		}

	}
}

void meVTKManualNeckCuttingFilter::GetInputMeshIDs( 
	vtkSmartPointer<vtkPolyData> landmarksLine, 
	vtkSmartPointer<vtkPolyData> inputMeshOriented, 
	vtkIdType currentLineId, 
	vtkIdType &pointID1InputMesh, 
	vtkIdType &pointID2InputMesh )
{
	// Get the 2 points of the line
	vtkSmartPointer< vtkIdList> cellPoints = vtkIdList::New();
	double cellPoint1[ 3 ];
	double cellPoint2[ 3 ];

	landmarksLine->GetCellPoints( currentLineId, cellPoints );
	landmarksLine->GetPoint( cellPoints->GetId(0) ,cellPoint1 );
	landmarksLine->GetPoint( cellPoints->GetId(1) ,cellPoint2 );

	pointID1InputMesh = inputMeshOriented->FindPoint(cellPoint1);
	pointID2InputMesh = inputMeshOriented->FindPoint(cellPoint2);

}

vtkIdType meVTKManualNeckCuttingFilter::GetSelectedCellID( 
	vtkSmartPointer<vtkPolyData>  inputMeshOriented,	
	vtkIdType pointID1InputMesh, 
	vtkIdType pointID2InputMesh )
{
	vtkIdType selectedCellID = -1;

	vtkSmartPointer< vtkIdList> cellIdsPoint1 = vtkIdList::New();
	inputMeshOriented->GetPointCells( pointID1InputMesh, cellIdsPoint1);

	vtkSmartPointer< vtkIdList> cellIdsPoint2 = vtkIdList::New();
	inputMeshOriented->GetPointCells( pointID2InputMesh, cellIdsPoint2);

	vtkSmartPointer<vtkDataArray> selectedArray = 
		inputMeshOriented->GetCellData()->GetArray( 
		STR_ARRAY_NAME_CELLS_SELECTION.c_str() );
	cellIdsPoint1->IntersectWith( *cellIdsPoint2.GetPointer() );

	for ( int i = 0 ; i < cellIdsPoint1->GetNumberOfIds( ) ; i++ )
	{
		vtkIdType cellID = cellIdsPoint1->GetId( i );
		if ( selectedArray->GetTuple1( cellID ) == 1 )
		{
			selectedCellID = cellID;
		}
	}

	return selectedCellID;
}


void meVTKManualNeckCuttingFilter::LabelNeighborCells( 
	vtkSmartPointer<vtkPolyData> landmarksLine, 
	vtkSmartPointer<vtkPolyData> inputMeshOriented )
{
	vtkSmartPointer<vtkDataArray> selectedArray = 
		inputMeshOriented->GetCellData()->GetArray( 
		STR_ARRAY_NAME_CELLS_SELECTION.c_str() );


	// Select the neighbor cells to the selected ones
	for( vtkIdType currentLineId=0; currentLineId < landmarksLine->GetNumberOfLines(); currentLineId++)
	{
		// Get the 2 points of the line in the input mesh
		vtkIdType pointID1InputMesh;
		vtkIdType pointID2InputMesh;
		GetInputMeshIDs( 
			landmarksLine, inputMeshOriented, currentLineId, 
			pointID1InputMesh,
			pointID2InputMesh );

		// Get selected cell of the current line
		vtkIdType primarySelectedCellID = GetSelectedCellID( 
			inputMeshOriented,
			pointID1InputMesh, 
			pointID2InputMesh );

		// The cell should: not use point2, use point1, use the third point of the cell
		vtkIdType nextCellPointIdNotUsed1;
		vtkIdType nextCellPointIdUsed1;
		vtkIdType nextCellPointIdUsed2;
		nextCellPointIdNotUsed1 = pointID2InputMesh;
		nextCellPointIdUsed1 = pointID1InputMesh;
		nextCellPointIdUsed2 = GetThirdPoint( 
			inputMeshOriented, 
			pointID1InputMesh, 
			pointID2InputMesh, 
			primarySelectedCellID );

		// For each cell, mark selected if it matches the criteria
		bool finalCellFound = false;
		vtkSmartPointer< vtkIdList> cellIdsPoint1 = vtkIdList::New();
		inputMeshOriented->GetPointCells( pointID1InputMesh, cellIdsPoint1);
		while( !finalCellFound )
		{

			// Mark the next cell
			vtkIdType k=0;
			bool cellFound = false;
			while( !cellFound && k< cellIdsPoint1->GetNumberOfIds() )
			{
				vtkIdType currentCellID = cellIdsPoint1->GetId(k);

				// Check if the points are used by this cell
				cellFound = inputMeshOriented->IsPointUsedByCell(
					nextCellPointIdUsed1,
					currentCellID );
				cellFound = cellFound && inputMeshOriented->IsPointUsedByCell(
					nextCellPointIdUsed2,
					currentCellID );
				cellFound = cellFound && !inputMeshOriented->IsPointUsedByCell(
					nextCellPointIdNotUsed1,
					currentCellID );

				// If found -> Mark selected
				if ( cellFound )
				{
					if ( selectedArray->GetTuple1( currentCellID ) == 1 )
					{
						finalCellFound = true;
					}
					else
					{
						selectedArray->SetTuple1( currentCellID, 2);

						// Configure next points
						primarySelectedCellID = currentCellID;
						nextCellPointIdNotUsed1 = nextCellPointIdUsed2;
						nextCellPointIdUsed2 = GetThirdPoint( 
							inputMeshOriented,
							nextCellPointIdUsed1, 
							nextCellPointIdUsed2, 
							primarySelectedCellID );
					}
				}

				k++;
			}
		}


	}

}


vtkIdType meVTKManualNeckCuttingFilter::GetThirdPoint( 
	vtkSmartPointer<vtkPolyData> inputMeshOriented,
	vtkIdType pointID1, 
	vtkIdType pointID2, 
	vtkIdType cellID )
{
	try
	{
	vtkSmartPointer< vtkIdList> cellPtsId = vtkSmartPointer< vtkIdList>::New();
	inputMeshOriented->GetCellPoints( cellID, cellPtsId);

	vtkIdType pointID3 = -1;
	for ( int i = 0 ; i < 3 ; i++ )
	{
		vtkIdType currentId = cellPtsId->GetId( i );
		if ( currentId != pointID1 && currentId != pointID2 )
		{
			pointID3 = currentId;
		}
	}

	return pointID3;
	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		throw e;
		return 0;
	}
}


vtkSmartPointer<vtkPolyData> meVTKManualNeckCuttingFilter::GetFirstComponent( 
	vtkSmartPointer<vtkPolyData> landmarksLine, 
	vtkSmartPointer<vtkPolyData> inputMeshOriented )
{
	vtkSmartPointer<vtkPolyData> dividedMesh;
	dividedMesh = blShapeUtils::ShapeUtils::GetShapeRegion(
		inputMeshOriented,
		0,
		0,
		STR_ARRAY_NAME_CELLS_SELECTION.c_str());

	if ( dividedMesh != NULL )
	{
		dividedMesh->GetCellData()->RemoveArray(STR_ARRAY_NAME_CELLS_SELECTION.c_str());
	}

	
	// Step 5: Get first component 
	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivity;
	connectivity = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivity->SetInput( dividedMesh );
	connectivity->SetExtractionModeToClosestPointRegion();
	connectivity->SetClosestPoint(landmarksLine->GetPoint(0));
	connectivity->Update( );

	vtkSmartPointer<vtkPolyData> region = vtkPolyData::New();
	connectivity->AddSpecifiedRegion( 0 );
	connectivity->Update();
	region->DeepCopy(connectivity->GetOutput());

	return region;
}


vtkSmartPointer<vtkPolyData> meVTKManualNeckCuttingFilter::GetSecondComponent( 
	vtkSmartPointer<vtkPolyData> landmarksLine, 
	vtkSmartPointer<vtkPolyData> inputMeshOriented )
{
	vtkSmartPointer<vtkPolyData> ring;
	ring = blShapeUtils::ShapeUtils::GetShapeRegion(
		inputMeshOriented,
		1,
		10,
		STR_ARRAY_NAME_CELLS_SELECTION.c_str());
	if ( ring != NULL )
	{
		ring->GetCellData()->RemoveArray(STR_ARRAY_NAME_CELLS_SELECTION.c_str());
	}

	vtkSmartPointer<vtkPolyData> dividedMesh;
	dividedMesh = blShapeUtils::ShapeUtils::GetShapeRegion(
		inputMeshOriented,
		0,
		0,
		STR_ARRAY_NAME_CELLS_SELECTION.c_str());

	if ( dividedMesh != NULL )
	{
		dividedMesh->GetCellData()->RemoveArray(STR_ARRAY_NAME_CELLS_SELECTION.c_str());
	}



	// Get the cell of the ring that contains the line 0
	vtkIdType pointID1, pointID2;
	GetInputMeshIDs( landmarksLine, ring, 0, pointID1, pointID2 );
	vtkSmartPointer<vtkIdList> cellList1 = vtkIdList::New();
	vtkSmartPointer<vtkIdList> cellList2 = vtkIdList::New();
	ring->GetPointCells(pointID1, cellList1);
	ring->GetPointCells(pointID2, cellList2);
	cellList1->IntersectWith( *cellList2.GetPointer() );

	// Get the point on the other side of the ring line
	vtkIdType point;
	point = GetThirdPoint( 
		ring,
		pointID1, 
		pointID2, 
		cellList1->GetId( 0 ) );

	
	// Step 6: Get second component 
	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivity;
	connectivity = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivity->SetInput( dividedMesh );
	connectivity->SetExtractionModeToClosestPointRegion();
	connectivity->SetClosestPoint(ring->GetPoint(point));
	connectivity->AddSpecifiedRegion( 0 );
	connectivity->Update();

	vtkSmartPointer<vtkAppendPolyData> append;
	append = vtkSmartPointer<vtkAppendPolyData>::New();
	append->AddInput( ring );
	append->AddInput( connectivity->GetOutput());
	append->Update( );

	return append->GetOutput();
}