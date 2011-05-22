/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meNGTetraFilter.h"
#include "vtkPoints.h"
#include "blShapeUtils.h"
#include "nglib.h"

meNGTetraFilter::meNGTetraFilter()
{
	m_param = meMeshParamsOptimizePtr( new meMeshParamsOptimize );
	m_params = meMeshTetraGenerationParamsPtr( new meMeshTetraGenerationParams);

	m_volume = vtkSmartPointer<vtkUnstructuredGrid>::New();
	nglib::Ng_Init();
}


meNGTetraFilter::~meNGTetraFilter() 
{
	nglib::Ng_Exit();
}

void meNGTetraFilter::SetOptParams( meMeshParamsOptimizePtr params )
{
	m_param = params;
}

void meNGTetraFilter::Update()
{
	try
	{

		nglib::Ng_Meshing_Parameters* netgenParam = new nglib::Ng_Meshing_Parameters() ;
 		netgenParam->maxh = m_param->m_maxh;
		netgenParam->curvaturesafety = m_param->m_curvaturesafety;
		netgenParam->segmentsperedge = m_param->m_segmentsperedge;
		GenerateSurfaceMesh( netgenParam );

		// Copy results to output mesh
		nglib::Ng_GenerateVolumeMesh(GetInput()->GetGeom(), GetInput()->GetNGMesh(),netgenParam);
		nglib::Ng_MeshRefinement(GetInput()->GetGeom(), GetInput()->GetNGMesh());
	}
	catch(...)
	{
		throw;
	}
}

meMeshParamsOptimizePtr meNGTetraFilter::GetOptParams()
{
	return m_param;
}

vtkSmartPointer<vtkUnstructuredGrid> meNGTetraFilter::GetVTK()
{
	int numOfPoints = nglib::Ng_GetNP(GetInput()->GetNGMesh());
	int numOfVolumeElemets = nglib::Ng_GetNE(GetInput()->GetNGMesh());

	m_volume = vtkSmartPointer<vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->SetNumberOfPoints(numOfPoints);
	double point[3];
	point[0]=point[1]=point[2]=0.0;

	for (size_t i = 0; i < numOfPoints; i++)
	{
		
		nglib::Ng_GetPoint(GetInput()->GetNGMesh(), i+1, point);
		points->InsertPoint(i,point );
	}

	m_volume->SetPoints(points);

	vtkSmartPointer<vtkIdList> Vtkpi = vtkSmartPointer<vtkIdList>::New();
	int* pi = new int[10];
	for (size_t j = 0; j < numOfVolumeElemets; j++)
	{
		nglib::Ng_Volume_Element_Type type;
		type = nglib::Ng_GetVolumeElement(GetInput()->GetNGMesh(), j+1, pi);
		for(size_t k=0; k< 4; k++)
			Vtkpi->InsertId(k, pi[k]-1);
		m_volume->InsertNextCell(VTK_TETRA, Vtkpi);
		
	}

	//blShapeUtils::ShapeUtils::SaveVolumeToFile(volume, "TestNetgen.vtk");
	//volume->Delete();

	return m_volume;
}

meMeshTetraGenerationParamsPtr meNGTetraFilter::GetParams()
{
	return m_params;
}

void meNGTetraFilter::SetParams( meMeshTetraGenerationParamsPtr params )
{
	m_params = params;
	m_param->m_maxh = m_params->m_MaxVolume;
	m_param->m_curvaturesafety = 0.2;
	m_param->m_segmentsperedge = 0.2;
}
