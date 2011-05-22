// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blPolyDataDistanceTransformFilter.h"
#include "CILabAssertMacros.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkNeighborhoodConnectedImageFilter.h"
#include "boost/assign.hpp"
#include "itkImageToVTKImageFilter.h"
#include "itkImageFileWriter.h"
#include <set>
#include <iostream>

template< class ImageType >
bool WriteImage(typename ImageType::Pointer image)
{
	typedef itk::ImageFileWriter< ImageType > WriterType;
	typename WriterType::Pointer writer = WriterType::New();
	writer->SetFileName( "G:\\Users\\Maarten\\Projects\\Gimias\\InputData\\GarGac\\debug.vtk" );
	writer->SetInput( image );
	try
	{
		writer->Update();
	}
	catch ( itk::ExceptionObject &err)
	{
		std::cerr << "ExceptionObject caught !" << std::endl; 
		std::cerr << err << std::endl; 
		return false;   
	}
	return true;
}

//--------------------------------------------------
blPolyDataDistanceTransformFilter::blPolyDataDistanceTransformFilter()
//--------------------------------------------------
{
	this->input = vtkPolyData::New();
	this->output = NULL;

	this->spacing[0] = 1.0;
	this->spacing[1] = 1.0;
	this->spacing[2] = 1.0;

	this->offset[0] = 0.0;
	this->offset[1] = 0.0;
	this->offset[2] = 0.0;

	this->sign = true;

	this->pointNormals = NULL;
	this->cellNormals = NULL;
}

//--------------------------------------------------
blPolyDataDistanceTransformFilter::~blPolyDataDistanceTransformFilter()
//--------------------------------------------------
{
	this->input->Delete();
	if ( this->output != NULL )
	{
		this->output->Delete();
	}
}

// IMPORTANT: This funcion doesn't work if there are other cells than polys, like vertexes, lines, etc.
//--------------------------------------------------
void blPolyDataDistanceTransformFilter::CellNormalComputation()
//--------------------------------------------------
{
	vtkIdType npts = 0;
	vtkIdType *pts = NULL;
	vtkDataArray *cellNormal = NULL;
	double AB[3], AC[3], A[3], B[3], C[3], N[3];

	cellNormal = this->input->GetCellData()->GetNormals();

	for ( vtkIdType i = 0; i < this->input->GetNumberOfPolys(); i++ )
	{
		// For each cell
		// Get points
		this->input->GetCellPoints( i, npts, pts );

		if ( npts != 3 )
		{
			cout << "blPolyDataDistanceTransformFilter::CellNormalComputation() -> This polydata has cells that are not triangles." << endl;
			return;
		}

		this->input->GetPoint( pts[0], A );
		this->input->GetPoint( pts[1], C );
		this->input->GetPoint( pts[2], B );
		// 
		// 2     1
		//  \   /
		//   `.�
		//    0
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
		vtkMath::Cross( AC, AB, N );

		cellNormal->SetTuple3( i, N[0], N[1], N[2] );
	}
}

//--------------------------------------------------
void blPolyDataDistanceTransformFilter::NormalAverage()
//--------------------------------------------------
{
	//this->CellNormalComputation();

	vtkDataArray *cellNormal = this->input->GetCellData()->GetNormals();
	vtkDataArray *newPointNormals = this->input->GetPointData()->GetNormals();

	vtkIdList *cellIds = vtkIdList::New();
	double *normal, newNormal[3];

	// For each point
	for ( vtkIdType point = 0; point < this->input->GetNumberOfPoints(); point++ )
	{
		// Get point cell ids
		this->input->GetPointCells( point, cellIds );

		for ( vtkIdType i = 0; i < 3; i++ )
			newNormal[i] = 0.0;

		// Normal at one point is the average of adjacent cell normals
		for ( vtkIdType numCell = 0; numCell < cellIds->GetNumberOfIds(); numCell++ )
		{
			vtkIdType cellId = cellIds->GetId( numCell );
			normal = cellNormal->GetTuple( cellId );

			// Add to the total
			for ( vtkIdType i = 0; i < 3; i++ )
				newNormal[i] += normal[i];
		}

		// Compute average
		for ( vtkIdType i = 0; i < 3; i++ )
			newNormal[i] /= cellIds->GetNumberOfIds();

		// Modify structure with new normal
		newPointNormals->SetTuple3( point, newNormal[0], newNormal[1], newNormal[2] );
	}

	this->pointNormals = newPointNormals;

	cellIds->Delete();

	this->CellNormalAverage();
}

//--------------------------------------------------
void blPolyDataDistanceTransformFilter::CellNormalAverage()
//--------------------------------------------------
{
	vtkDataArray *newCellNormals = this->input->GetCellData()->GetNormals();

	vtkIdList *pointIds = vtkIdList::New();
	vtkIdType pointId;
	double *normal, newNormal[3];

	// For each cell
	for ( vtkIdType cell = 0; cell < this->input->GetNumberOfCells(); cell++ )
	{
		// Find cell point ids
		pointIds = this->input->GetCell( cell )->GetPointIds();

		for ( vtkIdType i = 0; i < 3; i++ )
			newNormal[i] = 0.0;

		// Normal at one cell is the average of point normals
		for ( vtkIdType numPoint = 0; numPoint < pointIds->GetNumberOfIds(); numPoint++ )
		{
			pointId = this->input->GetCell( cell )->GetPointId( numPoint );
			normal = this->pointNormals->GetTuple( pointId );

			// Add to the total
			for ( vtkIdType i = 0; i < 3; i++ )
				newNormal[i] += normal[i];
		}

		// Compute average
		for ( vtkIdType i = 0; i < 3; i++ )
			newNormal[i] /= pointIds->GetNumberOfIds();

		// Modify structure with new normal
		newCellNormals->SetTuple3( cell, newNormal[0], newNormal[1], newNormal[2] );
	}

	this->cellNormals = newCellNormals;

	pointIds->Delete();
}

//--------------------------------------------------
void blPolyDataDistanceTransformFilter::Update()
//--------------------------------------------------
{
	vtkIdType pointId = 0, cellId = 0, npts = 0, ident = 0;
	// WARNING:
	// changed subId type from vtkIdType to int to allow it to be passed to FindClosestPoint (with gcc)
	// (ymartelli 07/10/2008)
	int subId = 0;
	int index[3];
	double mdist = 0.0, scprod = 0.0, dist = 0.0;
	double bounds[6], dim[3], origin[3], gridPoint[3], closestPoint[3], *normal;
	vtkVoxelModeller *vm = NULL;
	vtkCellLocator *cellLocator = NULL;
	vtkPointLocator *pointLocator = NULL;

	clock_t start, finish;
	start = clock();


	if ( this->input == NULL )
	{
		cout << "blPolyDataDistanceTransformFilter::Update() -> Error: no input set." << endl;
	}

	if ( this->output != NULL )
	{
		this->output->Delete();
	}
	this->output = vtkImageData::New();


	// Build triangulated surface cells.
	this->input->BuildCells();
	this->input->BuildLinks();
	this->input->Update();

	// Compute the dimensions of the bounding box.
	this->input->GetBounds( bounds );

	dim[0]=bounds[1]-bounds[0];
	dim[1]=bounds[3]-bounds[2];
	dim[2]=bounds[5]-bounds[4];

	// vtkVoxelModeller converts an arbitrary data set to a structured point (i.e., voxel) representation.
	vm = vtkVoxelModeller::New();
	vm->SetInput( this->input );
	vm->SetMaximumDistance( 0.01 );

	// Apply offset to the bounding box. 
	bounds[0] = bounds[0] - this->offset[0];
	bounds[1] = bounds[1] + this->offset[0];
	bounds[2] = bounds[2] - this->offset[1];
	bounds[3] = bounds[3] + this->offset[1];
	bounds[4] = bounds[4] - this->offset[2];
	bounds[5] = bounds[5] + this->offset[2];

	// Get the number of voxels in each dimension.
	// n = ceil(distance between corresponding bounding points divided by the voxel size)
	for( unsigned int i = 0; i < 3; i++ )
	{
		this->dimension[i] = ( unsigned int ) ceil( ( bounds[2*i+1] - bounds[2*i] ) / this->spacing[i] );
	}

	// Set the new bounding box and dimensions into the voxel modeller.
	vm->SetModelBounds( bounds );
	vm->SetSampleDimensions( (int *) this->dimension );
	vm->Update();

	// Generate the distance transform.
	// Set output parameters and allocate scalars.
	vm->GetOutput()->GetSpacing( this->spacing );
	vm->GetOutput()->GetOrigin( origin );

	this->output->SetSpacing( this->spacing );
	this->output->SetOrigin( origin );
	this->output->SetExtent( 0, this->dimension[0]-1, 0, this->dimension[1]-1, 0, this->dimension[2]-1 );
	this->output->AllocateScalars();

	// Creation of cell locator for exact distance computation.
	cellLocator = vtkCellLocator::New();
	cellLocator->SetDataSet( this->input );
	cellLocator->SetNumberOfCellsPerBucket( 8 );
	cellLocator->CacheCellBoundsOn();
	cellLocator->BuildLocator();

	// Creation of point locator.
	pointLocator = vtkPointLocator::New();
	pointLocator->SetDataSet( this->input );
	pointLocator->AutomaticOn();
	pointLocator->SetNumberOfPointsPerBucket( 8 );
	pointLocator->BuildLocator();

	if ( this->sign )
	{
		this->pointNormals = this->input->GetPointData()->GetNormals();
		this->cellNormals = this->input->GetCellData()->GetNormals();
		//this->NormalAverage();
	}


	//Main loop
	for( index[0] = 0; (unsigned int) index[0] < this->dimension[0]; index[0]++  )
	{
		for( index[1] = 0; (unsigned int) index[1] < this->dimension[1]; index[1]++  )
		{
			for( index[2] = 0; (unsigned int) index[2] < this->dimension[2]; index[2]++  )
			{
				// Get point inside voxel { index[0], index[1], index[2] }
				ident = vm->GetOutput()->ComputePointId( index );
				vm->GetOutput()->GetPoint( ident, gridPoint );

				// Find the closest surface cell to the index,
				// Parameter mdist actually contains the squared distance.
				cellLocator->FindClosestPoint( gridPoint, closestPoint, cellId, subId, mdist );


				// Find the closest surface point to the index,
				// Parameter mdist actually contains the squared distance.
				pointId = pointLocator->FindClosestPoint( gridPoint );
				//pointId = this->input->FindPoint( gridPoint );
				dist = sqrt( mdist );
				this->output->SetScalarComponentFromDouble( index[0], index[1], index[2], 0, dist );

				// If signed distance...
				if( this->sign )
				{
					unsigned short ncells = 0;
					vtkIdType *cells = NULL;

					this->input->GetPointCells( pointId, ncells, cells );

					bool negative = true;
					double gridVector[3];

					// For each cell adjacent to the point
					for ( unsigned short i = 0; i < ncells && negative; i++ )
					{
						normal = this->cellNormals->GetTuple( cells[i] );

						for ( vtkIdType j = 0; j < 3 ; j++ )
						{
							gridVector[j] = gridPoint[j] - closestPoint[j];
						}

						// Compute scalar product between the vector (grid point - closest point) and cell normal
						vtkMath::Normalize( normal );
						vtkMath::Normalize( gridVector );
						scprod = vtkMath::Dot( gridVector, normal );

						// If the scalar product is positive, both vectors have opposite directions
						if( scprod > 0.0 )
							negative = false;
					}

					// Change sign only if all triangles adjacent to the nearest point say it is inside
					if( negative )
						this->output->SetScalarComponentFromDouble( index[0], index[1], index[2], 0, -dist );
				}
			}
		}
	}

	// Correct pixels that were giving a wrong sign
	this->CorrectDistances();

	// If vtkVoxelModeller is destroyed, this->input becomes a memory leak and garbage collector crashes
	// Problem seems to be that this->input is registered only by 3 "entities", but there are 4 pointing to it
	// (this pointer + 3 filters). When the last one try to unregister it, it crashes.
	// I don't know how to solve it...
	//vm->Delete();

	cellLocator->FreeSearchStructure();
	cellLocator->Delete();
	pointLocator->FreeSearchStructure();
	pointLocator->Delete();

	finish = clock();
	this->computationTime = (double) ( finish - start ) / (double) CLOCKS_PER_SEC;
}

bool blPolyDataDistanceTransformFilter::CorrectDistances()
{
	// Comment: we define pixels on the zero-contour as pixels with a distance whose absolute value is smaller than half a
	// pixel spacing.
	
	// Step 2: Find regions in the distance image, connected to the border of the image, that are not on the contour.
	// These regions must be on the outside of the contour
	unsigned int imageDimensions[3];
	this->GetDimensions( imageDimensions );
	int volumeSize = imageDimensions[0] * imageDimensions[1] * imageDimensions[2]; 
	double* spacing = this->GetSpacing();
	const double spacingMagnitude = sqrt( spacing[0] * spacing[0] + spacing[1] * spacing[1] + spacing[2] * spacing[2] );
	double* distanceImagePtr = (double *) this->GetOutput()->GetScalarPointer();
	const int rowSize = imageDimensions[0];
	const int colSize = imageDimensions[1];
	const int zSize = imageDimensions[2];
	const int planeSize = rowSize * colSize;

	// init trial set
	std::set<int> trialSet;
	for( int iPixel = 0; iPixel < volumeSize; ++iPixel )
	{
		int z = iPixel / planeSize;
		int y = (iPixel - z * planeSize) / rowSize;
		int x = (iPixel - z * planeSize - y * rowSize);
		if( x == 0 || x == rowSize - 1 || y == 0 || y == colSize - 1 || z == 0 || z == zSize - 1 )
		{
			double distance = distanceImagePtr[iPixel];
			bool isOnContour = distance >= -0.5 * spacingMagnitude && distance <= 0.5 * spacingMagnitude;
			if( !isOnContour )
				trialSet.insert(iPixel);
		}
	}

	// generate acceptedSet
	using namespace boost::assign;
	std::vector<char> acceptedSet(volumeSize, 0);
	std::set<int> deltas;
	deltas += 1, -1, rowSize, -rowSize, planeSize, -planeSize;
	while( !trialSet.empty() )
	{
		int iPixel = *trialSet.begin();
		acceptedSet[iPixel] = 1;
		trialSet.erase(trialSet.begin());
	
		for( std::set<int>::iterator itDelta = deltas.begin(); itDelta != deltas.end(); ++itDelta )
		{
			int iNeighbour = iPixel + *itDelta;
			int zN = iNeighbour / planeSize;
			int yN = (iNeighbour - zN * planeSize) / rowSize;
			int xN = (iNeighbour - zN * planeSize - yN * rowSize);
			if( xN < 0 || yN < 0 || zN < 0 || xN >= rowSize || yN >= colSize || zN >=zSize)
				continue;

			double neighbourDistance = distanceImagePtr[iNeighbour];
			bool isNeighbourOnContour = neighbourDistance >= -0.5 * spacingMagnitude && neighbourDistance <= 0.5 * spacingMagnitude;
			if( !isNeighbourOnContour && !acceptedSet[iNeighbour] )
				trialSet.insert(iNeighbour);
		}
	}

	// Iterate over the original distance image. For each pixel, check if it is in:
	// a) the outside region (it is in the acceptedSet),
	// b) the contour region (it lies on a contour), or
	// c) the inside region (the other pixels)
	// In case a and c, correction of the sign in the distance image is trivial.
	// In case b, inspect both the distances and labels (a, b, or c) of the neighbours of the pixel to see if a correction
	// is needed.

	// correct pixels in groups a) and c)
	for( int iPixel = 0; iPixel < volumeSize; ++iPixel )
	{
		double distance = distanceImagePtr[iPixel];
		bool isOnContour = distance >= -0.5 * spacingMagnitude && distance <= 0.5 * spacingMagnitude;

		if( isOnContour )
			continue;

		bool isOutside = acceptedSet[iPixel];
		bool correctTheSign = (isOutside && distance < 0) || (!isOutside && distance > 0);
		if( correctTheSign )
			distanceImagePtr[iPixel] *= -1;
	}

	// correct pixels that are on the contour
	for( int iPixel = 0; iPixel < volumeSize; ++iPixel )
	{
		double distance = distanceImagePtr[iPixel];
		bool isOnContour = distance >= -0.5 * spacingMagnitude && distance <= 0.5 * spacingMagnitude;

		if( isOnContour )
		{
			// compare iPixel to all its neighbours: the difference between the distance image values should not exceed
			// the distance between the pixel centers. If it does exceed, then correct the sign of the distance value for iPixel.
			int z = iPixel / planeSize;
			int y = (iPixel - z * planeSize) / rowSize;
			int x = (iPixel - z * planeSize - y * rowSize);
			bool signIsWrong = false;

			for( int dX = -1; !signIsWrong && dX <= 1; ++dX)
			{
				int nx(x + dX);
				if( !( nx > 0 && nx < rowSize ) )
					continue;

				for( int dY = -1; !signIsWrong && dY <= 1; ++dY)
				{
					int ny(y + dY);
					if( !( ny > 0 && ny < colSize ) )
						continue;

					for( int dZ = -1; !signIsWrong && dZ <= 1; ++dZ)
					{
						int nz(z + dZ);
						if( !( nz > 0 && nz < zSize ) )
							continue;

						if( dX == 0 && dY == 0 && dZ == 0 )
							continue;

						int iNeighbour = nx + ny * rowSize + nz * planeSize;
						double neighbourDistance = distanceImagePtr[iNeighbour];
						bool isNeighbourOnContour = neighbourDistance >= -0.5 * spacingMagnitude && neighbourDistance <= 0.5 * spacingMagnitude;
						if( isNeighbourOnContour )
							continue;

						double pixelDistanceSqr = dX * dX * spacing[0] * spacing[0] + dY * dY * spacing[1] * spacing[1] + dZ * dZ * spacing[2] * spacing[2];
						double pixelDistance = sqrt(pixelDistanceSqr);
						bool signIsWrong = abs(distanceImagePtr[iPixel] - distanceImagePtr[iNeighbour]) > pixelDistance;
						if( signIsWrong )
						{
							distanceImagePtr[iPixel] *= -1;
							cilabAssertMacro( abs(distanceImagePtr[iPixel] - distanceImagePtr[iNeighbour]) <= pixelDistance + 0.00000001);
						}
					}
				}
			}
		}
	}

	return true;
}

//--------------------------------------------------
//--------------------------------------------------

namespace blTests
{

	

} // blTests namespace
