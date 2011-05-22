/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: $
  Purpose   : meVTKPolyDataRefiner implementation.
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker:  $
			  $Log: $


=========================================================================*/

#include "meVTKPolyDataRefiner.h"


#include <time.h>

//VTK
#include <vtkIdList.h>
#include <vtkCleanPolyData.h>
#include <vtkCellArray.h>
#include <vtkMath.h>

#include <vtkPolyDataWriter.h>

#include <vtkFeatureEdges.h>
#include <vtkPolyDataConnectivityFilter.h>


//--------------------------------------------------
meVTKPolyDataRefiner::meVTKPolyDataRefiner()
//--------------------------------------------------
{
	// Create input to allow DeepCopy on SetInput function
	this->input = vtkPolyData::New();
	this->output = NULL;
	this->densityFactor = sqrt( 2.0 );
	this->angle = 0.0;
	this->computationTime = 0.0;
	this->epsilonAngle = 1.0e-03; // This epsilon should be set in a "better" way
	this->epsilonDeterminant = 1.0e-03; // This epsilon should be set in a "better" way
	this->minimumEdgesSwapped = 10;
	this->m_Verbose = false;
}

//--------------------------------------------------
meVTKPolyDataRefiner::~meVTKPolyDataRefiner()
//--------------------------------------------------
{
	this->input->Delete();
	if ( this->output != NULL )
		this->output->Delete();
}

//--------------------------------------------------
void meVTKPolyDataRefiner::PrintPolyDataInfo( vtkPolyData *polydata )
//--------------------------------------------------
{
	cout << "Polydata has:\n\t"
		<< polydata->GetNumberOfPoints()  << " Points\n\t"
		<< polydata->GetNumberOfCells ()  << " Cells	 "
		<< "\n\t\tverts : " << polydata->GetNumberOfVerts ()
		<< "\n\t\tlines : " << polydata->GetNumberOfLines ()
		<< "\n\t\tpolys : " << polydata->GetNumberOfPolys ()
		<< "\n\t\tstrips: " << polydata->GetNumberOfStrips()
		<< endl;
}

// Compute average lenght of adjacent edges, but only if they are boundary edges
//--------------------------------------------------
float meVTKPolyDataRefiner::ComputeBoundarySigma( vtkIdType ptId )
//--------------------------------------------------
{
	float sigma = 0.0;
	vtkIdType npts = 0;
	vtkIdType *pts = NULL;
	vtkIdType boundaryEdges = 0;
	float length = 0.0;
	double pt1[3], pt2[3];

	// Find point coordinates
	this->input->GetPoint( ptId, pt1 );

	// Find cells adjacent to point
	vtkIdList *cellIds = vtkIdList::New();
	this->input->GetPointCells( ptId, cellIds );

	// For each cell
	for ( vtkIdType i = 0; i < cellIds->GetNumberOfIds(); i++ )
	{
		// Get points from cell
		this->input->GetCellPoints( cellIds->GetId( i ), npts, pts );
		
		// Now npts = number of cells points, pts = list of point id
		// For each point check if it is part of an adjacent edge
		for ( vtkIdType j = 0; j < npts; j++ )
		{
			// Avoid compare ptId with itself
			if ( ptId != pts[j] )
			{
				// Check if the point is part of an adjacent edge
				// With triangles this is always true, anyway it is checked
				if ( this->input->IsEdge( ptId, pts[j] ) )
				{
					// Get cell neighbours for the edge to check if it is a boundary edge
					vtkIdList *cellNeighbour = vtkIdList::New();
					this->input->GetCellEdgeNeighbors( cellIds->GetId( i ), ptId, pts[j], cellNeighbour );

					// Check if it is a boundary edge
					if ( cellNeighbour->GetNumberOfIds() == 0 )
					{
						this->input->GetPoint( pts[j], pt2 );

						// compute edge length, add it to total length
						length = length + sqrt( vtkMath::Distance2BetweenPoints( pt1, pt2 ) );

						// One more edge to add for average
						boundaryEdges++;
					}

					cellNeighbour->Delete();
				}
				else
					cout << "Error: meVTKPolyDataRefiner::ComputeBoundarySigma -> input polydata has no triangular cells" << endl;
			}
		}
	}

	cellIds->Delete();

	if ( boundaryEdges != 0 )
	{
		sigma = length / boundaryEdges;
	}
	else
	{
		// boundaryEdges == 0 then something wrong happens because it is impossible
		cout << "Error: meVTKPolyDataRefiner::ComputeBoundarySigma -> point requested is not at boundaries" << endl;
		sigma = 0.0;
	}

	return sigma;
}

// For each vertex on the hole boundary, compute average length of the edges
// that are adjacent to it in the surrounding mesh.
//--------------------------------------------------
bool meVTKPolyDataRefiner::BoundarySigmaInitialization()
//--------------------------------------------------
{
	vtkIdType numPoints = 0;
	unsigned int numBoundaries = 0;
	bool end = false;


	// Extract edges, but only boundary edges
	vtkFeatureEdges *extractEdges = vtkFeatureEdges::New();
	extractEdges->SetInput( this->input );
	extractEdges->BoundaryEdgesOn();
	extractEdges->FeatureEdgesOff();
	extractEdges->ManifoldEdgesOff();
	extractEdges->NonManifoldEdgesOff();

	// Split output, to get boundaries one by one
	vtkPolyDataConnectivityFilter *connectivity = vtkPolyDataConnectivityFilter::New();
	connectivity->SetInput( extractEdges->GetOutput() );
	connectivity->SetExtractionModeToSpecifiedRegions();
	connectivity->Update();

	numBoundaries = connectivity->GetNumberOfExtractedRegions();

	if ( numBoundaries != 1 )
	{
		cout << "meVTKPolyDataRefiner::BoundarySigmaInitialization -> Error only one boundary allowed for refining. Number of boundaries: " << numBoundaries << endl;
		end = true;
	}
	else
	{
		// Only one boundary allowed.
		connectivity->InitializeSpecifiedRegionList();
		connectivity->AddSpecifiedRegion( 0 );
		connectivity->Update();

		vtkPolyData *boundary = connectivity->GetOutput();

		vtkPoints *points = boundary->GetPoints();
		double point[3];
		vtkIdType originalID = 0;

		// Initialize boundary sigma vector with -1. No point is boundary.
		numPoints = this->input->GetNumberOfPoints();
		this->boundarySigma.reserve( numPoints );

		for ( vtkIdType i = 0; i < numPoints; i++ )
		{
			this->boundarySigma.push_back( -1.0 );
		}

		// Search boundary points and compute sigma
		for ( vtkIdType i = 0; i < boundary->GetNumberOfPoints() && !end; i++  )
		{
			boundary->GetPoint( i, point );
			originalID = this->input->FindPoint( point );

			if ( originalID != -1 )
			{
				this->boundarySigma[ originalID ] = this->ComputeBoundarySigma( originalID );
			}
			else
			{
				cout << "meVTKPolyDataRefiner::BoundarySigmaInitialization() -> Error: boundary point not found." << endl;
				end = true;
			}
		}

		extractEdges->Delete();
		connectivity->Delete();
	}

	return end;
}

// Compute sigma for a point. Sigma is the average length of the edges
// that are adjacent to it in the patching mesh.
//--------------------------------------------------
float meVTKPolyDataRefiner::ComputeSigma( vtkIdType ptId )
//--------------------------------------------------
{
	float sigma = 0.0;
	vtkIdType npts = 0;
	vtkIdType *pts = NULL;
	double pt1[3], pt2[3];
	vtkIdType numEdges = 0;
	float length = 0.0;


	// Check if point is at boundaries to use precomputed boundary sigmas
	if ( ptId < (vtkIdType) this->boundarySigma.size() && this->boundarySigma[ptId] >= 0.0 )
	{
		sigma = this->boundarySigma[ptId];
	}
	else
	{
		// Point is not at boundaries so computing normal sigma

		// Find point coordinates
		this->output->GetPoint( ptId, pt1 );

		// Find cells adjacent to point
		vtkIdList *cellIds = vtkIdList::New();
		this->output->GetPointCells( ptId, cellIds );

		// For each cell
		for ( vtkIdType i = 0; i < cellIds->GetNumberOfIds(); i++ )
		{
			// Get points from cell
			this->output->GetCellPoints( cellIds->GetId( i ), npts, pts );
			
			// Now npts = number of cells points, pts = list of point id
			// For each point check if it is part of an adjacent edge
			for ( vtkIdType j = 0; j < npts; j++ )
			{
				// Avoid compare ptId with itself
				if ( ptId != pts[j] )
				{
					// Check if the point is part of an adjacent edge
					// With triangles this is always true, anyway it is checked
					if ( this->output->IsEdge( ptId, pts[j] ) )
					{
						this->output->GetPoint( pts[j], pt2 );

						// compute edge length, add it to total length
						length = length + sqrt( vtkMath::Distance2BetweenPoints( pt1, pt2 ) );

						// One more edge to add for average
						numEdges++;
					}
					else
						cout << "Error: meVTKPolyDataRefiner::ComputeSigma -> input polydata has no triangular cells" << endl;
				}
			}
		}

		cellIds->Delete();


		if ( numEdges != 0 )
		{
			sigma = length / numEdges;
		}
		else
		{
			// numEdges == 0 then something wrong happens because it is impossible
			cout << "Error: meVTKPolyDataRefiner::ComputeSigma -> point requested has no edges" << endl;
			sigma = 0.0;
		}
	}

	return sigma;
}

// For each triangle compute "centroid" and use sigma criterion to decide
// if it is added, then create new triangles and swap edges if needed.
//--------------------------------------------------
bool meVTKPolyDataRefiner::ComputeSubdivisionAndSwapping( vtkIdType polyId, vtkPolyData *newMesh )
//--------------------------------------------------
{
	vtkIdType npts = 0;
	vtkIdType *pts = NULL;
	double pt[3], centroid[3] = { 0.0, 0.0, 0.0 };
	float sigmaPoints[3], sigma = 0.0;
	bool triangleSplit = false;


	////////////////////////////////////////////////////////////////
	// Phase 1: Compute "centroid" and its sigma
	////////////////////////////////////////////////////////////////

	// Get points from cell
	this->output->GetCellPoints( polyId, npts, pts );

	// Iterate over triangle points
	for ( vtkIdType i = 0; i < npts; i++ )
	{
		this->output->GetPoint( pts[i], pt );

		// Add each point to the centroid
		for ( unsigned int j = 0; j < 3; j++ )
		{
			centroid[j] = centroid[j] + pt[j];
		}

		// Add each point sigma to the centroid sigma
		sigmaPoints[i] = this->ComputeSigma( pts[i] );
	}

	// Divide by the number of points to get triangle centroid and centroid sigma
	for ( unsigned int j = 0; j < 3; j++ )
	{
		centroid[j] = centroid[j] / 3;
		sigma = sigma + sigmaPoints[j];
	}
	
	sigma = sigma / 3;


	////////////////////////////////////////////////////////////////
	// Phase 2: Check sigma condition.
	////////////////////////////////////////////////////////////////

	double vector[3], norm[3];
	bool sigmaCondition = true;

	// For each vm, with m = i, j, k (triangle vertexs)
	for ( vtkIdType i = 0; i < npts; i++ )
	{
		this->output->GetPoint( pts[i], pt );

		// vc - vm
		for ( unsigned int j = 0; j < 3; j++ )
		{
			vector[j] = centroid[j] - pt[j];
		}

		// || vc - vm ||
		norm[i] = vtkMath::Norm( vector );
	}

	// ( alpha * || vc - vm || > sigma(vc) ) && ( alpha * || vc - vm || > sigma(vm) )
	// with m = i, j, k
	for ( vtkIdType i = 0; i < npts && sigmaCondition; i++ )
	{
		sigmaCondition = ( (this->densityFactor * norm[i]) > sigma ) && ( (this->densityFactor * norm[i]) > sigmaPoints[i] );
	}


	////////////////////////////////////////////////////////////////
	// Subdivision
	////////////////////////////////////////////////////////////////

	if ( sigmaCondition )
	{
		triangleSplit = true;

		// If sigmaCondition is true, erase former triangle and add 3 new triangles
		vtkIdType centroidId;

		// Insert centroid point in mesh and keep its id
		vtkPoints *points = newMesh->GetPoints();
		centroidId = points->InsertNextPoint( centroid );

		// Get poligons and create new array to insert point id for new triangles
		vtkCellArray *polys = newMesh->GetPolys();
		vtkIdType *newTriangle = new vtkIdType[3];
		vtkIdType *triangleId = new vtkIdType[npts];

		// For each point, create a new traingle using the centroid
		// 0-1-c 1-2-c 2-0-c
		// counterclockwise
		for ( vtkIdType pos = 0; pos < npts; pos++ )
		{
			newTriangle[0] = pts[pos];
			newTriangle[1] = pts[((pos+1)%npts)];
			newTriangle[2] = centroidId;
			triangleId[pos] = polys->InsertNextCell( npts, newTriangle );
		}
		delete [] newTriangle;

		// Be sure to update structure
		//newMesh->Update();
		newMesh->BuildCells();
		newMesh->BuildLinks();


		////////////////////////////////////////////////////////////////
		// Phase 3: relax edges (i,j), (j,k), (i,k)
		////////////////////////////////////////////////////////////////

		for ( vtkIdType pos = 0; pos < npts; pos++ )
		{
			this->edgeSwapper->SetInput( newMesh );
			this->edgeSwapper->RelaxEdge( pts[pos], pts[((pos+1)%npts)], triangleId[pos] );
		}

		delete [] triangleId;
	}
	else
	{
		// If not, add the former triangle. It is not necessary to subdivide this traingle anymore.
		vtkCellArray *polys = newMesh->GetPolys();
		polys->InsertNextCell( npts, pts );

		newMesh->BuildCells();
		newMesh->BuildLinks();
	}

	return triangleSplit;
}

/*
//--------------------------------------------------
void meVTKPolyDataRefiner::CleanPolyDataCopy( vtkPolyData *in, vtkPolyData *out )
//--------------------------------------------------
{
	vtkCellArray *inPolys = NULL;
	vtkCellArray *outPolys = vtkCellArray::New();
	vtkIdType npts = 0;
	vtkIdType *pts = NULL;

	out->Initialize();
	out->SetPoints( in->GetPoints() );

	inPolys = in->GetPolys();
	for ( inPolys->InitTraversal(); inPolys->GetNextCell( npts, pts ); )
	{
		outPolys->InsertNextCell( npts, pts );
	}

	out->SetPolys( outPolys );

	out->BuildCells();
	out->BuildLinks();
	out->Update();
}
*/

// For each triangle compute "centroid" and use sigma criterion to decide
// if it is added, then create new triangles and swap edges if needed.
//--------------------------------------------------
bool meVTKPolyDataRefiner::Subdivision()
//--------------------------------------------------
{
	vtkIdType numTriangles = 0;
	bool newTriangles = false;

//cout << "Subdivision" << endl;
//clock_t start, finish;
//start = clock();

	numTriangles = this->output->GetNumberOfPolys();

	// A copy of the original mesh is needed to add sequentially each triangle
	// divided or not. All input points are add.
	// No cells are add becuase they will be created using sigma condition
	vtkPolyData *newMesh = vtkPolyData::New();
	newMesh->SetPoints( this->output->GetPoints() );
	// IMPORTANT to create an empty cell structure for adding later new cells, otherwise when newMesh->GetPolys()
	// is called the pointer point to hyperspace.
	vtkCellArray *polys = vtkCellArray::New();
	newMesh->SetPolys( polys );

	for ( vtkIdType i = 0; i < numTriangles; i++ )
	{
		// IMPORTANT: when doing thinks like this with booleans remember to place boolean variable at the end,
		// otherwise function is never called. Because C++ execute from left to right.
		// In this case, if first member make condition true, second one will never be executed
		newTriangles = ( this->ComputeSubdivisionAndSwapping( i , newMesh ) ) || newTriangles;
	}

	this->output->DeepCopy( newMesh );
	newMesh->Delete();
	polys->Delete( );

//finish = clock();
//cout << "Subdivision time: " << (double) ( finish - start ) / (double) CLOCKS_PER_SEC << endl;

	return newTriangles;
}


//--------------------------------------------------
void meVTKPolyDataRefiner::Update()
//--------------------------------------------------
{
	bool finished = false;
	vtkIdType i = 0;

	clock_t start, finish;
	start = clock();


	if ( this->output != NULL )
		this->output->Delete();

	// Initialize edge swapper parameters
	this->edgeSwapper = meVTKPolyDataEdgeSwapper::New();
	this->edgeSwapper->SetAngle( this->angle );
	this->edgeSwapper->SetEpsilonAngle( this->epsilonAngle );
	this->edgeSwapper->SetMinimumEdgesSwapped( this->minimumEdgesSwapped );
	this->edgeSwapper->SetEpsilonDeterminant( this->epsilonDeterminant );


	// Allocate ouput object
	this->output = vtkPolyData::New();

	vtkCleanPolyData *clean = vtkCleanPolyData::New();
	clean->SetInput( this->input );
	clean->Update();
	this->input->DeepCopy( clean->GetOutput() );
	clean->Delete();

	// Build structures to random access
	this->input->BuildCells();
	this->input->BuildLinks();
	this->input->Update();

	// By default all steps begin getting input from this->output because it contains previous iteration result.
	// This rule only is break at step 1, were this->input is used as input because is a read only step.
	this->output->DeepCopy( this->input );
	this->output->BuildCells();
	this->output->BuildLinks();
	this->output->Update();


	// Step 1: For each vertex on the hole boundary, compute average length of the edges
	// that are adjacent to it in the surrounding mesh.
	this->BoundarySigmaInitialization();
	
	// Steps loop
	while ( !finished )
	{
		// Step 2: For each triangle compute "centroid" and use sigma criterion to decide
		// if it is added, then create new triangles and swap edges if needed.
		finished = !( this->Subdivision() );

		// Step 3: Check if no triangles were created, and algorithm must end
		if ( !finished )
		{
			// Step 4: Relax all interior edges
			// BEWARE: edgeSwapper modify this->output in place.
			this->edgeSwapper->SetInput( this->output );
			this->edgeSwapper->Update();
		}

		i++;
	}

	finish = clock();
	this->computationTime = (double) ( finish - start ) / (double) CLOCKS_PER_SEC;

	if ( m_Verbose )
	{
		cout 
			<< "Refiner iterations: " << i 
			<< " Computation time: " << this->computationTime 
			<< endl;
	}
}

bool meVTKPolyDataRefiner::GetVerbose() const
{
	return m_Verbose;
}

void meVTKPolyDataRefiner::SetVerbose( bool val )
{
	m_Verbose = val;
}