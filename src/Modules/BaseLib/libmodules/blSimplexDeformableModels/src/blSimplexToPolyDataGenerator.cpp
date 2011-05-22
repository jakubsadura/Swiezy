/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "blSimplexToPolyDataGenerator.h"

// VTK

#include <vtkLine.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGridWriter.h>

//--------------------------------------------------
blSimplexToPolyDataGenerator::blSimplexToPolyDataGenerator()
//--------------------------------------------------
{
	this->vtkMeshRepresentation = NULL;
	this->iteration = -1;
}

//--------------------------------------------------
blSimplexToPolyDataGenerator::~blSimplexToPolyDataGenerator()
//--------------------------------------------------
{
	if ( this->vtkMeshRepresentation != NULL )
		this->vtkMeshRepresentation->Delete();
}

//--------------------------------------------------
void blSimplexToPolyDataGenerator::Update()
//--------------------------------------------------
{
	unsigned long numPoints = 0;

	// Free previous output object
	if ( this->vtkMeshRepresentation != NULL )
		this->vtkMeshRepresentation->Delete();

	SimplexMeshType::GeometryMapPointer simplexGeom = itkMesh->GetGeometryData();
	SimplexMeshType::GeometryMapIterator simplexGeomIt= simplexGeom->Begin();
	
	typedef std::map<unsigned long, unsigned long>  IdMapType;
	typedef std::pair<unsigned long, unsigned long> IdPairType;
	typedef itk::Point<double,3>					PointType;

	IdMapType idMap;

	// Allocate new output object
	this->vtkMeshRepresentation = vtkPolyData::New();

	numPoints =  itkMesh->GetNumberOfPoints();

	blSimplexMeshGeometry *data = simplexGeomIt->Value();
	int size = data->trajectory.size();
	if (this->iteration == -1 || this->iteration > size)
	{
		this->iteration = data->trajectory.size()-1;
	}

	vtkPoints *vtkPts = vtkPoints::New();
	
	blSimplexMeshGeometry::IndexArray::Iterator neighIt;
	vtkLine *line;
	double vpoint[3];
	SimplexPointsContainerPointer myPoints = itkMesh->GetPoints();
	
	simplexGeomIt = simplexGeom->Begin();

	vtkIdType i = 0;
	
	while (simplexGeomIt != simplexGeom->End()){
		
		unsigned long pointId = simplexGeomIt->Index();
		unsigned long newPointId;
		PointType pointAux;

		data = simplexGeomIt->Value();
		pointAux = data->trajectory[this->iteration];
		vpoint[0]= pointAux[0];
		vpoint[1]= pointAux[1];
		vpoint[2]= pointAux[2];
		
		newPointId = vtkPts->InsertNextPoint(vpoint);
		
		idMap.insert(IdPairType(pointId,newPointId));		
		
		simplexGeomIt++;
		i++;
	}

	simplexGeomIt = simplexGeom->Begin();
	
	this->vtkMeshRepresentation->Allocate(vtkPts->GetNumberOfPoints());
	this->vtkMeshRepresentation->SetPoints(vtkPts);

	while (simplexGeomIt != simplexGeom->End()){
		
		unsigned long pointId = simplexGeomIt->Index();
		data = simplexGeomIt->Value();
		simplexGeomIt++;
		neighIt = data->neighborIndices.Begin();
		while (neighIt != data->neighborIndices.End())
		{
			unsigned long neighId = *neighIt++;
			line = vtkLine::New();
			long ptId = idMap.find(pointId)->second;
			long ngId = idMap.find(neighId)->second;
			
			line->GetPointIds()->SetId(0,ptId);
			line->GetPointIds()->SetId(1,ngId);
			
			if ((ngId!=-1) && (ptId!=-1)) {
				this->vtkMeshRepresentation->InsertNextCell(line->GetCellType(),line->GetPointIds());
			}
			line->Delete();
		}
	}
}
	


