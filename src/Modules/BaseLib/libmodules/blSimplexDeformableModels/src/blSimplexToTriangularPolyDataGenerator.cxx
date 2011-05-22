/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "blSimplexToTriangularPolyDataGenerator.h"

// VTK
#include <vtkPoints.h>
#include <vtkCellData.h>
#include <vtkCharArray.h>
#include <vtkTriangle.h>

//--------------------------------------------------
blSimplexToTriangularPolyDataGenerator::blSimplexToTriangularPolyDataGenerator()
//--------------------------------------------------
{
	this->vtkMeshRepresentation = NULL;
	this->iteration = -1;
}

//--------------------------------------------------
blSimplexToTriangularPolyDataGenerator::~blSimplexToTriangularPolyDataGenerator()
//--------------------------------------------------
{
	if ( this->vtkMeshRepresentation != NULL )
		this->vtkMeshRepresentation->Delete();
}

//--------------------------------------------------
void blSimplexToTriangularPolyDataGenerator::Update()
//--------------------------------------------------
{
	unsigned long numPoints = 0;

	// Free previous output object
	if ( this->vtkMeshRepresentation != NULL )
		this->vtkMeshRepresentation->Delete();

	typedef std::map<unsigned long, unsigned long>  SimplexCellToTrianglePointMapType;
	typedef std::pair<unsigned long, unsigned long> IdPairType;
	typedef itk::Point<double,3>					PointType;

	SimplexMeshType::GeometryMapPointer simplexGeom = itkMesh->GetGeometryData();
	SimplexMeshType::GeometryMapIterator simplexGeomIt;

	SimplexCellToTrianglePointMapType simplexCellToTrianglePointMap;
	SimplexCellsContainerPointer simplexCellContainer;
	SimplexCellsContainerIterator simplexCellIterator;

	// Allocate new output object
	this->vtkMeshRepresentation = vtkPolyData::New();

	// In the triangular mesh we will have one point for each cell.
	numPoints =  itkMesh->GetLastCellId();
	SimplexCellAutoPointer auxCell;

	PointType auxPoint, avgPoint;
	unsigned long newPointId;
	SimplexPointsContainerPointer myPoints = this->itkMesh->GetPoints();
	vtkPoints *vtkPts = vtkPoints::New();

	blSimplexMeshGeometry *data = itkMesh->GetGeometryData(0);
	if (this->iteration < 0)
	{
		this->iteration = data->trajectory.size()-1;
		//std::cout << "data->trajectory.size() : " << data->trajectory.size() << std::endl;
	}

	simplexCellContainer = itkMesh->GetCells();
	double vpoint[3];
	// NumPoints is the number of points in the triangular mesh = number of cells in the simplex mesh
	unsigned long auxId = 0;
	// Generate t point for each simplex hexahedral cell.
	for (simplexCellIterator = simplexCellContainer->Begin(); simplexCellIterator != simplexCellContainer->End(); simplexCellIterator++){
		blSimplexMeshGeometry *data;
		const unsigned long *tp;
		unsigned long pointsInCell = simplexCellIterator->Value()->GetNumberOfPoints();
		auxId = simplexCellIterator->Index();
		tp = simplexCellIterator->Value()->GetPointIds();
		// only take into account hexagonal simplex cells.
		if (pointsInCell==6) {
			avgPoint.Fill(0.0);
			for (unsigned int ii = 0;ii<pointsInCell;ii++) {
				data = itkMesh->GetGeometryData(tp[ii]);
				auxPoint = data->trajectory[this->iteration];
				avgPoint[0]+=auxPoint[0];
				avgPoint[1]+=auxPoint[1];
				avgPoint[2]+=auxPoint[2];
			}
			avgPoint[0]/=pointsInCell;
			avgPoint[1]/=pointsInCell;
			avgPoint[2]/=pointsInCell;

			vpoint[0]= avgPoint[0];
			vpoint[1]= avgPoint[1];
			vpoint[2]= avgPoint[2];
			newPointId = vtkPts->InsertNextPoint(vpoint);
			simplexCellToTrianglePointMap.insert(IdPairType(auxId,newPointId));
		}
	}

	//Allocate the same memory for the polydata
	this->vtkMeshRepresentation->Allocate(vtkPts->GetNumberOfPoints());
	this->vtkMeshRepresentation->SetPoints(vtkPts);

	//////////////////////////////////////////////////////////////////////////
	vtkTriangle *triangle;
	blSimplexMeshGeometry *auxGeom;
	unsigned long simplexPointIds[3];
	unsigned long triIds[3];
	blSimplexMeshGeometry::NeghborCellsArray::iterator cellIt;
	unsigned long trianglesCount = 0;

	// Using the neighboring cell of every simplex point, we recover the triangular representation of the mesh.
	for (simplexGeomIt = simplexGeom->Begin(); simplexGeomIt != simplexGeom->End(); simplexGeomIt++){
		unsigned int numCells = 0;
		auxGeom = simplexGeomIt.Value();
		// The first point has more neighboring cells.
		//std::cout << "simplexPointIds : ";
		for (cellIt = auxGeom->neighborCells.begin(); cellIt!= auxGeom->neighborCells.end() && numCells<3; cellIt++)
		{
			simplexPointIds[numCells] = *cellIt;
			//std::cout << simplexPointIds[numCells] << ", ";
			numCells++;
		}
		//std::cout << "numCells : " << numCells << std::endl;
		if (numCells == 3 && cellIt==auxGeom->neighborCells.end()){
			trianglesCount++;
			triangle = vtkTriangle::New();
			triIds[0] = simplexCellToTrianglePointMap.find(simplexPointIds[0])->second;
			triIds[1] = simplexCellToTrianglePointMap.find(simplexPointIds[1])->second;
			triIds[2] = simplexCellToTrianglePointMap.find(simplexPointIds[2])->second;
			triangle->GetPointIds()->SetId(0,triIds[0]);
			triangle->GetPointIds()->SetId(1,triIds[1]);
			triangle->GetPointIds()->SetId(2,triIds[2]);
			this->vtkMeshRepresentation->InsertNextCell(triangle->GetCellType(),triangle->GetPointIds());
			triangle->Delete();
		}
	}

	vtkCharArray *fixedCellsArray = vtkCharArray::New();
	fixedCellsArray->SetNumberOfComponents(1);
	fixedCellsArray->SetNumberOfTuples(trianglesCount);
	fixedCellsArray->SetName("FixedCells");
	fixedCellsArray->Allocate(trianglesCount);

	unsigned long i = 0;
	// Have to repeat the loop to extract the information of the fixed cells.
	for (simplexGeomIt = simplexGeom->Begin(); simplexGeomIt != simplexGeom->End(); simplexGeomIt++){
		unsigned int numCells = 0;
		auxGeom = simplexGeomIt.Value();
		for (cellIt = auxGeom->neighborCells.begin(); cellIt!= auxGeom->neighborCells.end() && numCells<3; cellIt++)
		{
			simplexPointIds[numCells] = *cellIt;
			numCells++;
		}
		if (numCells == 3 && cellIt==auxGeom->neighborCells.end()){
			if (auxGeom->isFixed){
				fixedCellsArray->InsertTuple1(i,0);
			}else{
				fixedCellsArray->InsertTuple1(i,1);
			}
			i++;
		}
	}
	this->vtkMeshRepresentation->GetCellData()->AddArray(fixedCellsArray);
}
