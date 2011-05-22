/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKPolyDataSplitter.h"


#include <time.h>

//VTK
#include <vtkCleanPolyData.h>
#include <vtkPoints.h>
//#include <vtkstd/exception>
#include <vtkCellArray.h>
#include <vtkBitArray.h>
#include <vtkCellData.h>


//--------------------------------------------------
meVTKPolyDataSplitter::meVTKPolyDataSplitter()
//--------------------------------------------------
{
	// Create input to allow DeepCopy on SetInput function
	this->input = vtkPolyData::New();
	this->cellIdList = vtkIdList::New();
	this->surrounding = NULL;
	this->patch = NULL;
	this->tag = false;
	this->computationTime = 0.0;
}

//--------------------------------------------------
meVTKPolyDataSplitter::~meVTKPolyDataSplitter()
//--------------------------------------------------
{
	this->input->Delete();
	this->cellIdList->Delete();
	if ( this->surrounding != NULL )
		this->surrounding->Delete();
	if ( this->patch != NULL )
		this->patch->Delete();
}

// Check cell ids list.
// Checked: Not empty, not full (extract the whole input), and not out of range values.
// NOT checked: if there are cell id repeated.
//--------------------------------------------------
bool meVTKPolyDataSplitter::CheckCellIdList()
//--------------------------------------------------
{
	bool ok = false;

	if ( this->cellIdList->GetNumberOfIds() == 0 )
	{
		cout << "meVTKPolyDataSplitter::CheckCellIdList() -> No cells requested!" << endl;
	}
	else if ( this->cellIdList->GetNumberOfIds() == this->input->GetNumberOfPolys() )
	{
		cout << "meVTKPolyDataSplitter::CheckCellIdList() -> Requested all cells! No need to split, use whole input instead" << endl;
	}
	else
	{
		// Check that all cell ids are valid
		vtkIdType maxCells = this->input->GetNumberOfPolys();

		ok = true;

		for ( vtkIdType i = 0; i < this->cellIdList->GetNumberOfIds() && ok ; i++ )
		{
			// All cell ids should be in input range
			if ( this->cellIdList->GetId(i) < 0 || this->cellIdList->GetId(i) >= maxCells )
				ok = false;
		}

		if ( !ok )
			cout << "meVTKPolyDataSplitter::CheckCellIdList() -> Cell id outside of range requested" << endl;
	}

	return ok;
}


//--------------------------------------------------
void meVTKPolyDataSplitter::InsertCell( vtkIdType cellId, vtkPoints *points, vtkCellArray *cells, vtkIdList *pointsAdded )
//--------------------------------------------------
{
	vtkIdType npts = 0;
	vtkIdType *pts = NULL;
	vtkIdType pointId = -1;


	// Get cell point ids
	this->input->GetCellPoints( cellId, npts, pts );
	vtkIdType listPosition = 0;
	vtkIdType *meshId = new vtkIdType[npts];

	// For each point
	for ( vtkIdType j = 0; j < npts; j++ )
	{
		// Check if point was already inserted
		listPosition = pointsAdded->IsId( pts[j] );
		if ( listPosition == -1 )
		{
			// Insert point in point set, and store position in set for adding cell later
			meshId[j] = points->InsertNextPoint( this->input->GetPoint( pts[j] ) );

			// Insert mesh point id in id list
			pointsAdded->InsertNextId( pts[j] );
		}
		else
		{
			meshId[j] = listPosition;
		}
	}

	// Add new cell
	cells->InsertNextCell( npts, meshId );

	delete [] meshId;
}

// Divides input mesh in surrounding and patching meshes using cell id list
//--------------------------------------------------
void meVTKPolyDataSplitter::ComputeMeshes()
//--------------------------------------------------
{
	vtkPoints *patchPoints = vtkPoints::New();
	vtkPoints *surroundingPoints = vtkPoints::New();
	vtkCellArray *patchCells = vtkCellArray::New();
	vtkCellArray *surroundingCells = vtkCellArray::New();
	vtkIdType currentCellId;
	vtkIdList *surroundingPointList = vtkIdList::New();
	vtkIdList *patchPointList = vtkIdList::New();


	// For each cell id at input
	for ( vtkIdType i = 0; i < this->input->GetNumberOfPolys(); i++ )
	{
		currentCellId = this->cellIdList->IsId( i );
		if ( currentCellId == -1 )
		{
			// Cell is not at the list, add to surrounding
			this->InsertCell( i, surroundingPoints, surroundingCells, surroundingPointList );
		}
		else
		{
			// Cell is at the list, add to patch
			this->InsertCell( i, patchPoints, patchCells, patchPointList );
		}
	}

	surroundingPointList->Delete();
	patchPointList->Delete();

	// Creating outputs
	this->patch->SetPoints( patchPoints );
	this->patch->SetPolys( patchCells );

	this->surrounding->SetPoints( surroundingPoints );
	this->surrounding->SetPolys( surroundingCells );
}

//--------------------------------------------------
void meVTKPolyDataSplitter::Update()
//--------------------------------------------------
{
	bool step1 = false, step2 = false;
	double average = VTK_DOUBLE_MAX, initialAverage = 0.0;

	clock_t start, finish;
	start = clock();


	if ( this->surrounding != NULL )
		this->surrounding->Delete();

	if ( this->patch != NULL )
		this->patch->Delete();


	// Allocate ouput objects
	this->surrounding = vtkPolyData::New();
	this->patch = vtkPolyData::New();


	// Cleaning input
	vtkCleanPolyData *clean = vtkCleanPolyData::New();
	clean->SetInput( this->input );
	clean->Update();
	this->input->DeepCopy( clean->GetOutput() );
	clean->Delete();

	// Build structures to random access
	this->input->BuildCells();
	this->input->BuildLinks();
	this->input->Update();

	if ( this->tag )
	{
		// Get tagged cells
		vtkBitArray *bit = NULL;

		this->input->GetCellData()->SetActiveAttribute( "Sticky", vtkDataSetAttributes::SCALARS );
		bit = dynamic_cast<vtkBitArray *>( this->input->GetCellData()->GetScalars( "Sticky" ) );

		// Dynamic cast returns NULL when fails
		if ( bit != NULL )
		{
			// Reset cell id list
			if ( this->cellIdList != NULL )
				this->cellIdList->Initialize();

			// Iterate over bit array
			for ( vtkIdType i = 0; i < bit->GetSize(); i++ )
			{
				// Bit is 0 or 1
				if ( bit->GetValue( i ) )
				{
					// This cell is marked, add to cell id list
					this->cellIdList->InsertNextId( i );
				}
			}
		}
	}

	step1 = this->CheckCellIdList();

	if ( !step1 )
		return;

	this->ComputeMeshes();


	finish = clock();
	this->computationTime = (double) ( finish - start ) / (double) CLOCKS_PER_SEC;

cout << "meVTKPolyDataSplitter::Computation time: " << this->computationTime << endl;
}
