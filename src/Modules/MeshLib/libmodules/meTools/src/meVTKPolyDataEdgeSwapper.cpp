/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: $
  Purpose   : meVTKPolyDataEdgeSwapper implementation.
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker:  $
			  $Log: $


=========================================================================*/

#include "meVTKPolyDataEdgeSwapper.h"


#include <time.h>

//VTK
#include <vtkIdList.h>
#include <vtkMath.h>
#include <math.h>
#include <vtkTypeTraits.h>

#include <vtkPolyDataWriter.h>


//--------------------------------------------------
meVTKPolyDataEdgeSwapper::meVTKPolyDataEdgeSwapper()
//--------------------------------------------------
{
	this->angle = 0.0;
	this->epsilonAngle = 1.0e-03; // This epsilon should be set in a "better" way
	this->epsilonDeterminant = 1.0e-03; // This epsilon should be set in a "better" way
	this->minimumEdgesSwapped = 10;
	this->computationTime = 0.0;
}

//--------------------------------------------------
meVTKPolyDataEdgeSwapper::~meVTKPolyDataEdgeSwapper()
//--------------------------------------------------
{
}

//--------------------------------------------------
void meVTKPolyDataEdgeSwapper::PrintPolyDataInfo( vtkPolyData *polydata )
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

// Find the point in a triangle not being part of the edge defined by pt1Id, pt2Id
//--------------------------------------------------
bool meVTKPolyDataEdgeSwapper::FindOppositePoint( vtkIdType pt1Id, vtkIdType pt2Id, vtkIdType polyId, vtkIdType &pt3Id )
//--------------------------------------------------
{
	vtkIdType npts;
	vtkIdType *pts;
	vtkIdType i = 0;
	bool found = false;

	this->input->GetCellPoints( polyId, npts, pts );

	i = 0;
	while( i < npts && !found )
	{
		found = ( pt1Id != pts[i] ) && ( pt2Id != pts[i] );
		i++;
	}

	if ( found )
	{
		// IMPORTANT: use i-1 because "i" has been increased just before leave the loop
		pt3Id = pts[i-1];
	}
	else
	{
		// This is impossible, but anyway it is reported
		cout << "Error: meVTKPolyDataEdgeSwapper::FindOppositePoint -> edge requested has no third point in triangle!" << endl;
	}

	return found;
}

// Return true if edge swapping is needed between two coplanar triangles.
//--------------------------------------------------
bool meVTKPolyDataEdgeSwapper::SwapEdgeCoplanarCriterion( double *AB, double *AC, double *B, double *C, double *D )
//--------------------------------------------------
{
	bool edgeSwapping = false;
	double DB[3], DC[3];
	double alpha = 0.0, beta = 0.0;

	//      B
	//   /     \      1 original plane
	// A  1 | 2  D
	//   \     /      2 second plane
	//      C

	//                 __  __
	// Compute vectors DB, DC
	// DB = B - D = ( xb-xd, yb-yd, zb-zd )
	// DC = C - D = ( xc-xd, yc-yd, zc-zd )
	DB[0] = B[0]-D[0];
	DB[1] = B[1]-D[1];
	DB[2] = B[2]-D[2];

	DC[0] = C[0]-D[0];
	DC[1] = C[1]-D[1];
	DC[2] = C[2]-D[2];

	//              __ __
	// cos alpha = (AB�AC) / (|AB|*|AC|)
	alpha = vtkMath::Dot( AB, AC ) / ( vtkMath::Norm( AB ) * vtkMath::Norm( AC ) );

	//             __ __
	// cos beta = (DB�DC) / (|DB|*|DC|)
	beta = vtkMath::Dot( DB, DC ) / ( vtkMath::Norm( DB ) * vtkMath::Norm( DC ) );

	// cos alpha + cos beta < 0 to swap edge BC to AD
	if ( ( alpha + beta ) < 0.0 )
		edgeSwapping = true;

	return edgeSwapping;
}

// Return true if edge swapping is needed between two non-coplanar triangles.
//--------------------------------------------------
bool meVTKPolyDataEdgeSwapper::SwapEdgeCurvatureCriterion( double *AB, double *AC, double *DB, double *DC )
//--------------------------------------------------
{
	bool edgeSwapping = false;
	double alpha = 0.0, beta = 0.0;
	double gamma = 0.0, delta = 0.0;


	// Points
	//      B
	//   /     \      1 original plane
	// A  1 | 2  D
	//   \     /      2 second plane
	//      C

	// Angles
	//         
	// alpha = BAC
	// beta  = BDC
	// gamma = ABD
	// delta = ACD
	// 
	//        gamma
	//       /     \      1 original plane
	// alpha  1 | 2  beta
	//       \     /      2 second plane
	//        delta

	//              __ __
	// cos alpha = (AB�AC) / (|AB|*|AC|)
	alpha = vtkMath::Dot( AB, AC ) / ( vtkMath::Norm( AB ) * vtkMath::Norm( AC ) );

	//             __ __
	// cos beta = (DB�DC) / (|DB|*|DC|)
	beta = vtkMath::Dot( DB, DC ) / ( vtkMath::Norm( DB ) * vtkMath::Norm( DC ) );


	//              __ __
	// cos gamma = (AB�DB) / (|AB|*|DB|)
	gamma = vtkMath::Dot( AB, DB ) / ( vtkMath::Norm( AB ) * vtkMath::Norm( DB ) );

	//              __ __
	// cos delta = (AC�DC) / (|AC|*|DC|)
	delta = vtkMath::Dot( AC, DC ) / ( vtkMath::Norm( AC ) * vtkMath::Norm( DC ) );


	// ( cos alpha + cos beta ) + ( cos gamma + cos delta ) < 0 to swap edge BC to AD
	if ( ( ( alpha + beta ) - ( gamma + delta ) ) < 0.0 )
		edgeSwapping = true;


	return edgeSwapping;
}


// Return angle in degrees between two adjacent triangles, represented by four points.
// It return vectors AB, AC, DB, DC for optimization purposes.
//--------------------------------------------------
double meVTKPolyDataEdgeSwapper::AngleBetweenTriangles( double *B, double *C, double *A, double *D, double *AB, double *AC, double *DB, double *DC )
//--------------------------------------------------
{
	double angle = 0.0;
	//double det = 0;
	double n1[3], n2[3];
	double alpha = 0.0;

	//      B
	//   /     \      1 first triangle
	// A  1 | 2  D
	//   \     /      2 second triangle
	//      C

	//                 __  __
	// Compute vectors AB, AC
	// AB = B - A = ( x1-x3, y1-y3, z1-z3 )
	// AC = C - A = ( x2-x3, y2-y3, z2-z3 )
	AB[0] = B[0]-A[0];
	AB[1] = B[1]-A[1];
	AB[2] = B[2]-A[2];

	AC[0] = C[0]-A[0];
	AC[1] = C[1]-A[1];
	AC[2] = C[2]-A[2];

	//                 __  __
	// Compute vectors DB, DC
	// DB = B - D = ( xb-xd, yb-yd, zb-zd )
	// DC = C - D = ( xc-xd, yc-yd, zc-zd )
	DB[0] = B[0]-D[0];
	DB[1] = B[1]-D[1];
	DB[2] = B[2]-D[2];

	DC[0] = C[0]-D[0];
	DC[1] = C[1]-D[1];
	DC[2] = C[2]-D[2];

	//   uxv
	// u  |  v
	//  \ | /
	//    |
	//
	// The rule which determines the orientation of the cross product uxv.
	// The right-hand rule states that the orientation of the vectors' cross product is determined by
	// placing u and v tail-to-tail, flattening the right hand, extending it in the direction of u, and
	// then curling the fingers in the direction that the angle v makes with u.
	// The thumb then points in the direction of uxv.

	// Use the same direction for both vectors, otherwise the angle obtained will be wrong
	// It doesn't matter if the normals are the real ones or are flipped,
	// the important thing is if they have "the same direction"
	vtkMath::Cross( AB, AC, n1 );
	vtkMath::Cross( DC, DB, n2 );

	// n1 is normal vector to triangle 1
	// n2 is normal vector to triangle 2
	//              __ __
	// cos alpha = (n1�n2) / (|n1|*|n2|)
	alpha = vtkMath::Dot( n1, n2 ) / ( vtkMath::Norm( n1 ) * vtkMath::Norm( n2 ) );

	angle = acos( alpha );

	return angle;
}


// Return true is four points (two adjacent triangles) are in the same plane.
// It return vectors AB and AC for optimization purposes.
//--------------------------------------------------
bool meVTKPolyDataEdgeSwapper::CoplanarTriangles( double *B, double *C, double *A, double *D, double *AB, double *AC )
//--------------------------------------------------
{
	bool coplanar = true;
	double det = 0.0;
	double AD[3];


	//      B
	//   /     \      1 original plane
	// A  1 | 2  D
	//   \     /      2 second plane
	//      C

	//                 __  __
	// Compute vectors AB, AC
	// AB = B - A = ( x1-x3, y1-y3, z1-z3 )
	// AC = C - A = ( x2-x3, y2-y3, z2-z3 )
	// AD = D - A = ( xc-x3, yc-y3, zc-z3 )
	AB[0] = B[0]-A[0];
	AB[1] = B[1]-A[1];
	AB[2] = B[2]-A[2];

	AC[0] = C[0]-A[0];
	AC[1] = C[1]-A[1];
	AC[2] = C[2]-A[2];

	AD[0] = D[0]-A[0];
	AD[1] = D[1]-A[1];
	AD[2] = D[2]-A[2];

	vtkMath::Normalize( AB );
	vtkMath::Normalize( AC );
	vtkMath::Normalize( AD );

	// Compute det( AB, AC, AD )
	det = vtkMath::Determinant3x3( AB[0], AB[1], AB[2],
									AC[0], AC[1], AC[2],
									AD[0], AD[1], AD[2] );

	// If determinant is zero means that those vectors are in the same plane
	if ( fabs(det) > this->epsilonDeterminant )
	{
		coplanar = false;
	}

	return coplanar;
}


// Relax edge defined by pt1Id - pt2Id in triangle polyId
//--------------------------------------------------
bool meVTKPolyDataEdgeSwapper::RelaxEdge( vtkIdType pt1Id, vtkIdType pt2Id, vtkIdType polyId, vtkEdgeTable *edgeTable )
//--------------------------------------------------
{
	double B[3], C[3], A[3], D[3];
	double AB[3], AC[3], DB[3], DC[3];
	double trianglesAngle = 0.0;

	vtkIdList *cellEdgeNeighbourId = vtkIdList::New();
	vtkIdType pt3Id, ptNeighbourId;
	vtkIdType *pts = new vtkIdType[3];
	vtkIdType *ptsNeighbour = new vtkIdType[3];
	bool edgeSwapped = false;
	bool coplanar = false;
	bool swappable = false;

	if ( this->input == NULL )
	{
		cout << "Error: meVTKPolyDataEdgeSwapper::RelaxEdge() -> called without input mesh" << endl;
		return false;
	}

	// Be sure links are build before calling neighbourhood operations
	this->input->BuildCells();
	this->input->BuildLinks();

	////////////////////////////////////////////////////////////////
	// Compute edge swapping condition
	////////////////////////////////////////////////////////////////


	// Check if selected triangle and its edge pt1Id - pt2Id neighbour are coplanar

	// Get triangle points
	//         pt1Id
	//       /       \                 1 polyId
	// pt3Id   1 | 2   ptNeighbourId
	//       \       /                 2 cellEdgeNeighbourId->GetId(0)
	//         pt2Id
	//
	// It is converted in:
	//
	//      B
	//   /     \      1 polyId
	// A  1 | 2  D
	//   \     /      2 cellEdgeNeighbourId->GetId(0)
	//      C

	this->input->GetPoint( pt1Id, B ); // Point B
	this->input->GetPoint( pt2Id, C ); // Point C
	bool found = this->FindOppositePoint( pt1Id, pt2Id, polyId, pt3Id );
	
	if ( found )
	{
		this->input->GetPoint( pt3Id, A ); // Point A

		// Get neighbour cell
		this->input->GetCellEdgeNeighbors( polyId, pt1Id, pt2Id, cellEdgeNeighbourId );

		// Check edge has only one neighbour, otherwise is non-manifold
		// If cellEdgeNeighbourId->GetNumberOfIds() == 0 is boundary edge so swapping is not possible
		if ( cellEdgeNeighbourId->GetNumberOfIds() == 1 )
		{
			found = this->FindOppositePoint( pt1Id, pt2Id, cellEdgeNeighbourId->GetId(0), ptNeighbourId );

			if ( found )
			{
				this->input->GetPoint( ptNeighbourId, D ); // Point D
				swappable = true;
			}
			else
				return false;
		}
		else
		{
			if ( cellEdgeNeighbourId->GetNumberOfIds() != 0 )
			{
				cout << "Error: meVTKPolyDataEdgeSwapper::RelaxEdge -> edge requested is non-manifold!" << endl;
			}
			// If cellEdgeNeighbourId->GetNumberOfIds() == 0 is boundary edge so swapping is not possible
		}
	}
	else
		return false;


	//      B
	//   /     \      1 polyId
	// A  1 | 2  D
	//   \     /      2 cellEdgeNeighbourId->GetId(0)
	//      C
	if ( swappable )
	{
		if ( this->angle == 0.0 )
		{
			coplanar = this->CoplanarTriangles( B, C, A, D, AB, AC );

			// Use "normal" edge swapping criterion
			if ( coplanar && this->SwapEdgeCoplanarCriterion( AB, AC, B, C, D ) )
			{
				edgeSwapped = true;

				// Update edgeTable if present
				if ( edgeTable != NULL )
				{
					// IMPORTANT: Edge Table can not erase already registered edges.
					// However, p1Id-p2Id edge will be destroyed, and there is no way of calling this function again
					// with this edge. Problem: at the end, edgeTable will contain initial edges plus all new edges.
					edgeTable->InsertEdge( pt3Id, ptNeighbourId );
				}

				// Swapping
				pts[0] = pt1Id;
				pts[1] = ptNeighbourId;
				pts[2] = pt3Id;

				ptsNeighbour[0] = pt2Id;
				ptsNeighbour[1] = pt3Id;
				ptsNeighbour[2] = ptNeighbourId;

				// Replace cells
	//			this->input->RemoveCellReference( polyId );
	//			this->input->ReplaceLinkedCell( polyId, 3, pts );
				this->input->ReplaceCell( polyId, 3, pts );
	//			this->input->RemoveCellReference( cellEdgeNeighbourId->GetId(0) );
	//			this->input->ReplaceLinkedCell( cellEdgeNeighbourId->GetId(0), 3, ptsNeighbour );
				this->input->ReplaceCell( cellEdgeNeighbourId->GetId(0), 3, ptsNeighbour );

				this->input->BuildCells();
				this->input->BuildLinks();
			}
		}
		else
		{
			// Use "curvature" edge swapping criterion
			trianglesAngle = this->AngleBetweenTriangles( B, C, A, D, AB, AC, DB, DC );

			if ( trianglesAngle < this->epsilonAngle )
			{
				// It is considered coplanar
				// Use "normal" edge swapping criteria
				if ( this->SwapEdgeCoplanarCriterion( AB, AC, B, C, D ) )
				{
					edgeSwapped = true;

					// Update edgeTable if present
					if ( edgeTable != NULL )
					{
						// IMPORTANT: Edge Table can not erase already registered edges.
						// However, p1Id-p2Id edge will be destroyed, and there is no way of calling this function again
						// with this edge. Problem: at the end, edgeTable will contain initial edges plus all new edges.
						edgeTable->InsertEdge( pt3Id, ptNeighbourId );
					}

					// Swapping
					pts[0] = pt1Id;
					pts[1] = ptNeighbourId;
					pts[2] = pt3Id;

					ptsNeighbour[0] = pt2Id;
					ptsNeighbour[1] = pt3Id;
					ptsNeighbour[2] = ptNeighbourId;

					// Replace cells
					this->input->ReplaceCell( polyId, 3, pts );
					this->input->ReplaceCell( cellEdgeNeighbourId->GetId(0), 3, ptsNeighbour );

					this->input->BuildCells();
					this->input->BuildLinks();
				}
			}
			else if ( trianglesAngle <= this->angle && this->SwapEdgeCurvatureCriterion( AB, AC, DB, DC ) )
			{
				edgeSwapped = true;

				// Update edgeTable if present
				if ( edgeTable != NULL )
				{
					// IMPORTANT: Edge Table can not erase already registered edges.
					// However, p1Id-p2Id edge will be destroyed, and there is no way of calling this function again
					// with this edge. Problem: at the end, edgeTable will contain initial edges plus all new edges.
					edgeTable->InsertEdge( pt3Id, ptNeighbourId );
				}

				// Swapping
				pts[0] = pt1Id;
				pts[1] = ptNeighbourId;
				pts[2] = pt3Id;

				ptsNeighbour[0] = pt2Id;
				ptsNeighbour[1] = pt3Id;
				ptsNeighbour[2] = ptNeighbourId;

				// Replace cells
				this->input->ReplaceCell( polyId, 3, pts );
				this->input->ReplaceCell( cellEdgeNeighbourId->GetId(0), 3, ptsNeighbour );

				this->input->BuildCells();
				this->input->BuildLinks();
			}
		}
	}

	cellEdgeNeighbourId->Delete();
	delete pts;
	delete ptsNeighbour;

	return edgeSwapped;
}

// Relax all interior edges of the mesh
// IMPORTANT: this operation should be done on the mesh directly without copying, to the contrary of meVTKPolyDataRefining::Subdivision().
// The reason is simple, if two triangles are swap, neighbours should see this change. If a copy is used,
// it will have this change but not the original one. Thus, when a neighbour of those two triangles ask for swap,
// vertexs in original and copy meshes are different, and it will give wrong results and probably crash.
//--------------------------------------------------
void meVTKPolyDataEdgeSwapper::RelaxAllInteriorEdges()
//--------------------------------------------------
{
	vtkIdType npts = 0;
	vtkIdType *pts = NULL;
	vtkEdgeTable *edgeTable = vtkEdgeTable::New();
	//bool finished = false;
	vtkIdType edgesSwapped = vtkTypeTraits< vtkIdType >::Max();


	while ( edgesSwapped > minimumEdgesSwapped )
	{
		edgesSwapped = 0;

		// An edge table is created for an efficient walking along the edges, not repeating any of them
		edgeTable->Initialize();
		edgeTable->InitEdgeInsertion( this->input->GetNumberOfPoints() );

		// Iterate over all edges

		// IMPORTANT: Do not access to cells through vtkCellArray from this->input->GetPolys()
		// because vtkCellArray access is based on low level vector access, while vtkPolyData cell access
		// is based in cell ids.
		// Example: Supose there are 10 (triangles) cells, vtkCellArray has 40 positions:
		//      n, id1, ..., idn (n = number of ids per cell, in this case 3) * 10 cells = 40
		//		3 id0 id1 id2 3 id0 id1 id2  ...  3 id0 id1 id2
		//      |   cell 0   |    cell 1   | ... |    cell n   |
		// If it is asked to a polydata for cell 2 using:
		//      this->input->GetCellPoints( 2, npts, pts ); // it will return the right information
		// On the contrary, if it is asked to the vtkCellArray using:
		//      this->input->GetPolys()->GetCell( 2, npts, pts ); // in this case concept is wrong
		//      It will go to position 2 in vector and return the numbers there (id1,id2,n2)
		//		3 id0 id1 id2 3 id0 id1 id2  ...  3 id0 id1 id2
		//           | returned |
		//      the right call using this access method is:
		//      this->input->GetPolys()->GetCell( 2 * 4, npts, pts ); // being 4 the number of elements to define a cell
		//                                                             // 3 id0 id1 id2

		for ( vtkIdType polyId = 0; polyId < this->input->GetNumberOfPolys(); polyId++ )
		{
			this->input->GetCellPoints( polyId, npts, pts );

			// Iterate over cell edges
			for ( vtkIdType i = 0; i < npts; i++ )
			{
				// Check this edge has not been treated before
				if ( edgeTable->IsEdge( pts[i], pts[((i+1)%npts)] ) == -1 )
				{
					// Prior to insert edge, it must be check it is not already inserted
					edgeTable->InsertEdge( pts[i], pts[((i+1)%npts)] );

					// Relax edge
					// IMPORTANT: when doing thinks like this with booleans remember to place boolean variable at the end,
					// otherwise function is never called. Because C++ execute from left to right.
					// In this case, if first member make condition false, second one will never be executed

					if ( this->RelaxEdge( pts[i], pts[((i+1)%npts)], polyId, edgeTable ) )
					{
						edgesSwapped++;
					}
				}
			}
		}
	}

	edgeTable->Delete();
}

//--------------------------------------------------
void meVTKPolyDataEdgeSwapper::Update()
//--------------------------------------------------
{
	clock_t start, finish;
	start = clock();


	if ( this->input != NULL )
	{
		this->input->BuildCells();
		this->input->BuildLinks();
		this->input->Update();

		this->RelaxAllInteriorEdges();
	}

	finish = clock();
	this->computationTime = (double) ( finish - start ) / (double) CLOCKS_PER_SEC;
}
