/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKPolyDataJoiner.h"


#include <time.h>

//VTK
#include <vtkCleanPolyData.h>
#include <vtkFeatureEdges.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkstd/exception>
#include <vtkCleanPolyData.h>
#include <vtkCellArray.h>
#include <vtkBitArray.h>
#include <vtkCellData.h>


//--------------------------------------------------
meVTKPolyDataJoiner::meVTKPolyDataJoiner()
//--------------------------------------------------
{
	// Create input to allow DeepCopy on SetInput function
	this->surrounding = vtkPolyData::New();
	this->patch = vtkPolyData::New();
	this->output = NULL;
	this->cellIdList = NULL;
	this->tag = false;
	this->computationTime = 0.0;
}

//--------------------------------------------------
meVTKPolyDataJoiner::~meVTKPolyDataJoiner()
//--------------------------------------------------
{
	this->surrounding->Delete();
	this->patch->Delete();
	if ( this->output != NULL )
		this->output->Delete();
	if ( this->cellIdList != NULL )
		this->cellIdList->Delete();
}

//--------------------------------------------------
vtkIdList* meVTKPolyDataJoiner::GetCellList()
//--------------------------------------------------
{
	vtkIdList *list = vtkIdList::New();
	list->DeepCopy( this->cellIdList );
	return list;
}

// Find boundary points at patching mesh and find twin boundary points at surrounding mesh.
//--------------------------------------------------
bool meVTKPolyDataJoiner::ComputeTwinBoundaryPoints()
//--------------------------------------------------
{
	bool ok = false;
	double point[3];
	vtkPoints *patchPoints = NULL;
	vtkPoints *surroundingPoints = vtkPoints::New();
	vtkIdType patchId = 0;
	vtkIdType surroundingId = 0;


	// Extract edges, but only boundary edges
	vtkFeatureEdges *extractEdges = vtkFeatureEdges::New();
	extractEdges->SetInput( this->patch );
	extractEdges->BoundaryEdgesOn();
	extractEdges->FeatureEdgesOff();
	extractEdges->ManifoldEdgesOff();
	extractEdges->NonManifoldEdgesOff();

	// Split output, to get boundaries one by one
	// But it should be only one
	vtkPolyDataConnectivityFilter *connectivity = vtkPolyDataConnectivityFilter::New();
	connectivity->SetInput( extractEdges->GetOutput() );
	connectivity->SetExtractionModeToSpecifiedRegions();
	connectivity->InitializeSpecifiedRegionList();
	connectivity->AddSpecifiedRegion( 0 );
	try
    {
		connectivity->Update();
    }
    catch( vtkstd::exception& e )
    {
		extractEdges->Delete();
		connectivity->Delete();
		cout << "meVTKPolyDataFairer::ComputePatchBoundaryPoints() -> ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		throw e;
    }

	unsigned int numHoles = connectivity->GetNumberOfExtractedRegions();


	if ( numHoles == 1 )
	{
		// Get boundary
		vtkPoints *points = connectivity->GetOutput()->GetPoints();

		// Get points from patch polydata
		patchPoints = this->patch->GetPoints();
		surroundingPoints->DeepCopy( this->surrounding->GetPoints() );

		// Creating patching mesh points vector
		this->twinPoint.reserve( patchPoints->GetNumberOfPoints() );
		for ( vtkIdType i = 0; i < patchPoints->GetNumberOfPoints(); i++ )
		{
			// Assign -1 to define this position as non initialized
			this->twinPoint.push_back( -2 );
		}

		// Find point id at patching mesh and surrounding mesh of each boundary point
		for ( vtkIdType i = 0; i < points->GetNumberOfPoints(); i++ )
		{
			// Get coordinates of point in boundary
			points->GetPoint( i, point );

			// Use coordinates to find it at patch polydata
			patchId = this->patch->FindPoint( point );
			
			// Use coordinates to find it at surrounding polydata
			surroundingId = this->surrounding->FindPoint( point );

			// Update twin points vector
			// This point is at boundaries and
			// it is assigned in twin point vector its surrounding id to speed up computation afterwards.
			this->twinPoint[patchId] = surroundingId;
		}

		// Now add all interior patch points to surrounding mesh and store id
		for ( vtkIdType i = 0; i < patchPoints->GetNumberOfPoints(); i++ )
		{
			if ( this->twinPoint[i] == -2 )
			{
				// Point is not at boundaries

				// Add point to surrounding mesh
				patchPoints->GetPoint( i, point );
				this->twinPoint[i] = surroundingPoints->InsertNextPoint( point );
			}
		}

		ok = true;

		// Final check, all points in patch have its corresponding point in surrounding mesh
		for ( vtkIdType i = 0; i < static_cast<vtkIdType>(this->twinPoint.size()) && ok; i++ )
		{
			if ( this->twinPoint[i] < 0 )
				ok = false;
		}

		// Add new points to output polydata if everything goes fine. Otherwise, erase them and write an error message
		if ( ok )
		{
			this->output->SetPoints( surroundingPoints );
		}
		else
		{
			surroundingPoints->Delete();
			cout << "meVTKPolyDataJoiner::ComputeTwinBoundaryPoints() -> There are points at patch mesh without an equivalent at surrounding mesh." << endl;
		}
	}
	else
	{
		cout << "meVTKPolyDataJoiner::ComputeTwinBoundaryPoints() -> Patching mesh has holes." << endl;
	}

	extractEdges->Delete();
	connectivity->Delete();

	return ok;
}

// Add cells from patch mesh to surrounding mesh, and fill cell id list.
// Points were already inserted at ComputeTwinBoundaryPoints() and translation vector twinPoint can be used
// to map patch mesh point ids to surroinding mesh point ids.
//--------------------------------------------------
bool meVTKPolyDataJoiner::AddCells()
//--------------------------------------------------
{
	vtkIdType npts = 0;
	vtkIdType *pts = NULL;
	vtkCellArray *cells = vtkCellArray::New();

	// Copy surrounding cells to output prior to insert new ones
	cells->DeepCopy( this->surrounding->GetPolys() );

	// Find cells type
	// IMPORTANT: It is assumed that all cells has the same type

	// Add all patch cells to output mesh
	for ( vtkIdType i = 0; i < this->patch->GetPolys()->GetNumberOfCells(); i++ )
	{
		// Get points from cell
		this->patch->GetCellPoints( i, npts, pts );

		// Translate id from patch to surrounding
		for ( vtkIdType j = 0; j < npts; j++ )
		{
			pts[j] = this->twinPoint[pts[j]];
		}

		vtkIdType id = cells->InsertNextCell( npts, pts );

		if ( id >= 0 )
		{
			this->cellIdList->InsertNextId( id );
		}
		else
		{
			cout << "Unable to add cell " << i << " from patch to output. Memory problems?" << endl;
			return false;
		}
	}

	this->output->SetPolys( cells );

	return true;
}

//--------------------------------------------------
void meVTKPolyDataJoiner::Update()
//--------------------------------------------------
{
	bool step1 = false, step2 = false;
	clock_t start, finish;
	start = clock();

	if ( this->output != NULL )
		this->output->Delete();

	if ( this->cellIdList != NULL )
		this->cellIdList->Delete();

	// Allocate ouput objects
	this->output = vtkPolyData::New();
	this->cellIdList = vtkIdList::New();

	// Cleaning inputs
	vtkCleanPolyData *clean = vtkCleanPolyData::New();
	clean->SetInput( this->surrounding );
	clean->Update();
	this->surrounding->DeepCopy( clean->GetOutput() );
	clean->Delete();

	clean = vtkCleanPolyData::New();
	clean->SetInput( this->patch );
	clean->Update();
	this->patch->DeepCopy( clean->GetOutput() );
	clean->Delete();

	// Build structures to random access
	this->surrounding->BuildCells();
	this->surrounding->BuildLinks();
	this->surrounding->Update();

	this->patch->BuildCells();
	this->patch->BuildLinks();
	this->patch->Update();

	//////////////////////////////////////////////////////////
	// Find boundary equivalent points and add interior ones
	//////////////////////////////////////////////////////////
	step1 = this->ComputeTwinBoundaryPoints();

	if ( !step1 )
		return;

	// Add cells from patch mesh to surrounding mesh
	step2 = this->AddCells();

	if ( step2 )
	{
		if ( this->tag )
		{
			// Tag added cells by setting 1 a their position
			vtkBitArray *bit = vtkBitArray::New();
			vtkIdType size = this->output->GetNumberOfPolys();
			
			bit->SetNumberOfValues( size );
			bit->SetName( "Sticky" );

			// Initialize tag bit vector to zero
			for ( vtkIdType i = 0; i < size; i++ )
			{
				bit->SetValue( i, 0 );
			}

			// Set 1 at new cell ids positions
			for ( vtkIdType i = 0; i < this->cellIdList->GetNumberOfIds(); i++ )
			{
				bit->SetValue( this->cellIdList->GetId(i), 1 );
			}

			//cout << "Tag size: " << size << endl;
			//for ( vtkIdType i = 0; i < size; i++ )
			//{
			//	cout << bit->GetValue(i);
			//}
			//cout << endl;

			// Add data array to cells
			this->output->GetCellData()->AddArray( bit );
		}
	}
	else
	{
		// If new cells could not be added, erase output
		this->output->Initialize();
	}


	finish = clock();
	this->computationTime = (double) ( finish - start ) / (double) CLOCKS_PER_SEC;

   cout << "meVTKPolyDataJoiner::Computation time: " << this->computationTime << endl;
}
