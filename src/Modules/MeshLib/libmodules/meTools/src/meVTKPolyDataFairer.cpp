/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: $
  Purpose   : meVTKPolyDataFairer implementation.
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker:  $
			  $Log: $


=========================================================================*/

#include "meVTKPolyDataFairer.h"


#include <time.h>

//VTK
#include <vtkFeatureEdges.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkstd/exception>
#include <vtkCleanPolyData.h>
#include <vtkIdList.h>
#include <vtkCleanPolyData.h>
#include <vtkMath.h>


//--------------------------------------------------
meVTKPolyDataFairer::meVTKPolyDataFairer()
//--------------------------------------------------
{
	// Create input to allow DeepCopy on SetInput function
	this->surrounding = vtkPolyData::New();
	this->patch = vtkPolyData::New();
	this->output = NULL;
	this->order = 2;
	this->numberOfIterations = 0;
	this->percentage = 0.0001;
	this->computationTime = 0.0;
	this->m_Verbose = false;
}

//--------------------------------------------------
meVTKPolyDataFairer::~meVTKPolyDataFairer()
//--------------------------------------------------
{
	this->surrounding->Delete();
	this->patch->Delete();
	if ( this->output != NULL )
		this->output->Delete();
}

//--------------------------------------------------
void meVTKPolyDataFairer::PrintPolyDataInfo( vtkPolyData *polydata )
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


// Find boundary points in patching mesh. This will be used later to compute U and U^2 in a different way at those
// points using surrounding mesh, too.
//--------------------------------------------------
bool meVTKPolyDataFairer::ComputePatchBoundaryPoints()
//--------------------------------------------------
{
	bool ok = false;
	double point[3];
	vtkPoints *patchPoints = NULL;
	vtkPoints *surroundingPoints = NULL;
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
		surroundingPoints = this->surrounding->GetPoints();

		// Creating patching mesh points vector
		this->patchPoint.reserve( patchPoints->GetNumberOfPoints() );
		for ( vtkIdType i = 0; i < patchPoints->GetNumberOfPoints(); i++ )
		{
			// Assign -1 to define this position as non boundary and
			// it is not a boundary point that has "equivalent" at surrounding mesh 
			this->patchPoint.push_back( -1 );
		}

		// Creating patching mesh number of neighbours of one point vector
		this->neighbourPoints.reserve( patchPoints->GetNumberOfPoints() );
		for ( vtkIdType i = 0; i < patchPoints->GetNumberOfPoints(); i++ )
		{
			// Assign -1 as non initialized
			this->neighbourPoints.push_back( -1 );
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

			// Update patch points vector
			// This point is at boundaries and
			// it is assigned in patch point vectors its surrounding id to speed up computation afterwards.
			this->patchPoint[patchId] = surroundingId;
		}

		ok = true;

	}
	else
	{
		cout << "meVTKPolyDataFairer::ComputePatchBoundaryPoints() -> Patching mesh has holes." << endl;
	}

	extractEdges->Delete();
	connectivity->Delete();

	return ok;
}

// Find neighbour points of ptId.
// If ignoreBorder = true then points at boundaries will be ignored and not included. IMPORTANT: This will work only
// at patch mesh because there is a vector that marks those points. If this flag is used on surrounding mesh algorithm
// will crash.
//--------------------------------------------------
void meVTKPolyDataFairer::FindPointNeighbours( vtkIdType ptId, vtkPolyData *mesh, bool ignoreBorder, vtkIdList *pointIds )
//--------------------------------------------------
{
	vtkIdType npts = 0;
	vtkIdType *pts = NULL;
	vtkIdList *cellIds = vtkIdList::New();

	// Get cells sharing this point
	mesh->GetPointCells( ptId, cellIds );

	// For each cell
	for ( vtkIdType i = 0; i < cellIds->GetNumberOfIds(); i++ )
	{
		// Get points from cell
		mesh->GetCellPoints( cellIds->GetId( i ), npts, pts );
		
		// Now npts = number of cells points, pts = list of point id
		// For each point check if it is part of an adjacent edge
		for ( vtkIdType j = 0; j < npts; j++ )
		{
			// Avoid compare ptId with itself
			if ( ptId != pts[j] )
			{
				// Check if the point is part of an adjacent edge
				// With triangles this is always true, anyway it is checked
				if ( mesh->IsEdge( ptId, pts[j] ) )
				{
					// If ignoreBorder == false => include point
					// If ignoreBorder == true => if point is NOT at boundaries ==> include point
					if ( !ignoreBorder || this->patchPoint[pts[j]] == -1 )
					{
						// Insert only if it not already inserted
						pointIds->InsertUniqueId( pts[j] );
					}
				}
				else
					cout << "Error: meVTKPolyDataFairer::FindPointNeighbours -> input polydata has no triangular cells" << endl;
			}
		}
	}

	cellIds->Delete();
}

// Compute first order umbrella operator.
// First order umbrella: U(p) = 1/n * Sum( pi - p ), where i=[0..n-1] and n=number of neighbour points at p
//--------------------------------------------------
void meVTKPolyDataFairer::FirstOrderUmbrellaOperator( vtkIdType ptId, vtkPolyData *mesh, bool ignoreBorder, vector<double> &movement, unsigned int &numNeighbours )
//--------------------------------------------------
{
	double centralPoint[3], adjacentPoint[3];
	vtkIdList *pointIds = vtkIdList::New();


	// Find point coordinates
	mesh->GetPoint( ptId, centralPoint );

	// Find neighbour points
	this->FindPointNeighbours( ptId, mesh, ignoreBorder, pointIds );
	numNeighbours = pointIds->GetNumberOfIds();

	// For each neighbour point
	for ( unsigned int i = 0; i < numNeighbours; i++ )
	{
		// Get coordinates
		mesh->GetPoint( pointIds->GetId(i), adjacentPoint );

		for ( unsigned int j = 0; j < 3; j++ )
		{
			movement[j] = movement[j] + ( adjacentPoint[j] - centralPoint[j] ) ;
		}
	}

	pointIds->Delete();
}


// Compute first order umbrella function for each point at patching mesh.
// First order umbrella: U(p) = 1/n * Sum( pi - p ), where i=[0..n-1] and n=number of neighbour points at p
//--------------------------------------------------
void meVTKPolyDataFairer::ComputeFirstOrderUmbrellaFunction()
//--------------------------------------------------
{
	vector<double> patchAccumulated( 3, 0.0 ), surroundingAccumulated( 3, 0.0 );
	unsigned int numNeighboursPatch = 0, numNeighboursSurrounding = 0;


	// Compute first order umbrella operator for each point at patching mesh
	for ( vtkIdType i = 0; i < this->patch->GetNumberOfPoints(); i++ )
	{
		// Check if point is at boundaries to use both meshes
		if ( this->patchPoint[i] != -1 )
		{
			// Point is at boundaries
			this->FirstOrderUmbrellaOperator( i, this->patch, true, patchAccumulated, numNeighboursPatch );
			this->FirstOrderUmbrellaOperator( this->patchPoint[i], this->surrounding, false, surroundingAccumulated, numNeighboursSurrounding );

			// Initialize number of neighbours if needed
			if ( this->neighbourPoints[i] == -1 )
			{
				this->neighbourPoints[i] = numNeighboursPatch + numNeighboursSurrounding;
			}

			// Compute first order umbrella value
			for ( unsigned int j = 0; j < 3; j++ )
			{
				this->firstOrderUmbrella[i][j] = ( patchAccumulated[j] + surroundingAccumulated[j] ) / ( this->neighbourPoints[i] );
			}

			// Initialize patchAccumulated and surroundingAccumulated
			for ( unsigned int j = 0; j < 3; j++ )
			{
				patchAccumulated[j] = 0.0;
				surroundingAccumulated[j] = 0.0;
			}
		}
		else
		{
			// Point is not at boundaries
			this->FirstOrderUmbrellaOperator( i, this->patch, false, patchAccumulated, numNeighboursPatch );

			// Initialize number of neighbours if needed
			if ( this->neighbourPoints[i] == -1 )
			{
				this->neighbourPoints[i] = numNeighboursPatch;
			}

			// Compute first order umbrella value
			for ( unsigned int j = 0; j < 3; j++ )
			{
				this->firstOrderUmbrella[i][j] = patchAccumulated[j] / this->neighbourPoints[i];
			}

			// Initialize patchAccumulated
			for ( unsigned int j = 0; j < 3; j++ )
			{
				patchAccumulated[j] = 0.0;
			}
		}
	}
}


// Apply umbrella function to each point at patching mesh. Skipping boundary points.
// First order umbrella: p = p + U(p)
// Second order umbrella: p = p - ( 1/v * U^2(p) )
//--------------------------------------------------
double meVTKPolyDataFairer::ApplyUmbrellaFunction()
//--------------------------------------------------
{
	vtkPoints *points = NULL;
	double pt[3], movement[3];
	double v = 0.0, scale = 0.0;
	double averageMovement = 0.0;
	unsigned int numInteriorPoints = 0;


	// Get patch points
	points =  this->patch->GetPoints();

	// For each point in patch mesh
	for ( vtkIdType i = 0; i < points->GetNumberOfPoints(); i++ )
	{
		// Get point coordinates
		points->GetPoint( i, pt );

		// Look if it is interior. Boundary points can not be moved, otherwise boundaries of patching
		// mesh and surrounding mesh will not match.
		if ( this->patchPoint[i] == -1 )
		{

			switch ( this->order )
			{
				case 1:
					// Modify coordinates using first order umbrella operator
					for ( unsigned int j = 0; j < 3; j++ )
					{
						pt[j] = pt[j] + this->firstOrderUmbrella[i][j];
					}

					averageMovement = averageMovement + vtkMath::Norm( &(this->firstOrderUmbrella[i][0]), 3 );
					break;

				case 2:
					// If second order umbrella operator is applied an scale factor is needed
					v = this->ComputeDiagonalElement( i, this->patch );
					if ( v != 0.0 )
					{
						scale = 1.0 / v;
					}

					// Modify coordinates using second order umbrella operator
					// IMPORTANT: First order "add", but second order "substract" and needs an scale factor
					for ( unsigned int j = 0; j < 3; j++ )
					{
						movement[j] = scale * this->secondOrderUmbrella[i][j];
						pt[j] = pt[j] - ( movement[j] );
					}

					averageMovement = averageMovement + vtkMath::Norm( movement );
					break;

				default:
					cout << "meVTKPolyDataFairer::ApplyUmbrellaFunction() -> Order " << this->order << " not implemented!" << endl;
				break;

			}
		}

		// Set new coordinates for point
		points->SetPoint( i, pt );

		// Another interior point treated
		numInteriorPoints++;
	}

	if ( numInteriorPoints > 0 )
	{
		averageMovement = averageMovement / numInteriorPoints;
	}
	else
	{
		cout << "meVTKPolyDataFairer::ApplyUmbrellaFunction() -> number of interior points at patch mesh is 0!!!" << endl;
		averageMovement = 0.0;
	}

	// Add again points to patching mesh
	this->patch->SetPoints( points );

	return averageMovement;
}

// Compute second order umbrella operator.
// Second order umbrella: U^2(p) = 1/n * Sum( U(pi) - U(p) ), where i=[0..n-1] and n=number of neighbour points at p
//--------------------------------------------------
void meVTKPolyDataFairer::SecondOrderUmbrellaOperator( vtkIdType ptId, vtkPolyData *mesh, vector<double>& umbrella )
//--------------------------------------------------
{
	vtkIdList *pointIds = vtkIdList::New();


	// Find neighbour points
	this->FindPointNeighbours( ptId, mesh, false, pointIds );

	// Security check
	if ( this->neighbourPoints[ptId] != pointIds->GetNumberOfIds() )
		cout << "meVTKPolyDataFairer::SecondOrderUmbrellaOperator() -> Number of neighbours is dynamic!!" << endl;

	// For each neighbour point
	for ( vtkIdType i = 0; i < pointIds->GetNumberOfIds(); i++ )
	{
		// Compute second order umbrella value
		for ( unsigned int j = 0; j < 3; j++ )
		{
			umbrella[j] = umbrella[j] + ( this->firstOrderUmbrella[pointIds->GetId(i)][j] - this->firstOrderUmbrella[ptId][j] );
		}
	}

	pointIds->Delete();
}

// Computes diagonal element.
// v = 1 + 1/n Sum( 1 / ni ) where n=number of neighbour points and i=[0..n] being ni=number of neighbour points
// of each neighbour point of original point
//--------------------------------------------------
double meVTKPolyDataFairer::ComputeDiagonalElement( vtkIdType ptId, vtkPolyData *mesh )
//--------------------------------------------------
{
	double v = 0.0;
	vtkIdList *pointIds = vtkIdList::New();


	// Find neighbour points
	this->FindPointNeighbours( ptId, mesh, false, pointIds );

	// For each neighbour point
	for ( vtkIdType i = 0; i < pointIds->GetNumberOfIds(); i++ )
	{
		// Find number of neighbours of neighbour point
		v = v + ( 1.0 / this->neighbourPoints[pointIds->GetId(i)] );
	}

	// Weigh with number of neighbour points of original point
	v = 1.0 + ( ( 1.0 / this->neighbourPoints[ptId] ) * v );

	pointIds->Delete();

	return v;
}

// Compute second order umbrella function for each point at patching mesh.
// Second order umbrella: U^2(p) = 1/n * Sum( U(pi) - U(p) ), where i=[0..n-1] and n=number of neighbour points at p
//--------------------------------------------------
void meVTKPolyDataFairer::ComputeSecondOrderUmbrellaFunction()
//--------------------------------------------------
{
	vector<double> umbrella( 3, 0.0 );


	// Compute second order umbrella operator for each point at patching mesh
	for ( vtkIdType i = 0; i < this->patch->GetNumberOfPoints(); i++ )
	{
		// Check if point is NOT at boundaries.
		// IMPORTANT: Second order umbrella operator will not be applied to boundary points.
		// Ths solve a lot of problems because compute second order umbrella operators at bounsdaries is not trivial.
		if ( this->patchPoint[i] == -1 )
		{
			this->SecondOrderUmbrellaOperator( i, this->patch, umbrella );
		}

		// Weight with number of neighbours
		for ( unsigned int j = 0; j < 3; j++ )
		{
			umbrella[j] = umbrella[j] / this->neighbourPoints[i];
		}

		this->secondOrderUmbrella[i] = umbrella;

		// Initialize
		for ( unsigned int j = 0; j < 3; j++ )
		{
			umbrella[j] = 0.0;
		}
	}

}


//--------------------------------------------------
void meVTKPolyDataFairer::Update()
//--------------------------------------------------
{
	bool step1 = false;
	double average = VTK_DOUBLE_MAX, initialAverage = 0.0;

	clock_t start, finish;
	start = clock();


	if ( this->output != NULL )
		this->output->Delete();

	// Allocate ouput object
	this->output = vtkPolyData::New();


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
	// Get patch boundary points
	//////////////////////////////////////////////////////////
	step1 = this->ComputePatchBoundaryPoints();

	if ( !step1 )
		return;

	// Creating patching mesh first order umbrella vector initialized to 0.0
	point3D pt( 3, 0.0 );
	this->firstOrderUmbrella.reserve( this->patch->GetNumberOfPoints() );
	for ( vtkIdType i = 0; i < this->patch->GetNumberOfPoints(); i++ )
	{
		this->firstOrderUmbrella.push_back( pt );
	}

	// Creating patching mesh second order umbrella vector initialized to 0.0
	this->secondOrderUmbrella.reserve( this->patch->GetNumberOfPoints() );
	for ( vtkIdType i = 0; i < this->patch->GetNumberOfPoints(); i++ )
	{
		this->secondOrderUmbrella.push_back( pt );
	}


	//////////////////////////////////////////////////////////
	// Compute umbrella operator
	//////////////////////////////////////////////////////////

	if ( this->numberOfIterations != 0 )
	{
		for ( unsigned int i = 0; i < this->numberOfIterations; i++ )
		{
			switch ( this->order )
			{
				case 0: // No fairing
						this->output->DeepCopy( this->patch	);
						return;
						break;
				case 1: this->ComputeFirstOrderUmbrellaFunction();
						break;
				case 2: this->ComputeFirstOrderUmbrellaFunction();
						this->ComputeSecondOrderUmbrellaFunction();
						break;
				default:
					cout << "meVTKPolyDataFairer::ApplyUmbrellaFunction() -> Order " << this->order << " not implemented!" << endl;
					return;
					break;
			}

			// Step 4: Compute new vertexes position using umbrella operator
			average = this->ApplyUmbrellaFunction();
		}
	}
	else
	{
		switch ( this->order )
		{
			case 0: // No fairing
					this->output->DeepCopy( this->patch	);
					return;
					break;
			case 1: this->ComputeFirstOrderUmbrellaFunction();
					break;
			case 2: this->ComputeFirstOrderUmbrellaFunction();
					this->ComputeSecondOrderUmbrellaFunction();
					break;
			default:
				cout << "meVTKPolyDataFairer::ApplyUmbrellaFunction() -> Order " << this->order << " not implemented!" << endl;
				return;
				break;
		}

		// Compute new vertexes position using umbrella operator
		initialAverage = this->ApplyUmbrellaFunction();
		this->numberOfIterations++;

		while ( average > ( initialAverage * this->percentage ) )
		{
			switch ( this->order )
			{
				case 1: this->ComputeFirstOrderUmbrellaFunction();
						break;
				case 2: this->ComputeFirstOrderUmbrellaFunction();
						this->ComputeSecondOrderUmbrellaFunction();
						break;
			}

			// Compute new vertexes position using umbrella operator
			average = this->ApplyUmbrellaFunction();

			this->numberOfIterations++;
		}
	}

	// Copy patch mesh modified to output
	this->output->DeepCopy( this->patch	);

	finish = clock();
	this->computationTime = (double) ( finish - start ) / (double) CLOCKS_PER_SEC;

	if ( m_Verbose )
	{
		cout 
			<< "Fairer iterations: " << this->numberOfIterations 
			<< " Computation time: " << this->computationTime 
			<< endl;
	}
}

bool meVTKPolyDataFairer::GetVerbose() const
{
	return m_Verbose;
}

void meVTKPolyDataFairer::SetVerbose( bool val )
{
	m_Verbose = val;
}
