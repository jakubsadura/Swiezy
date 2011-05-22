/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meNGSmoothFilter.h"

meNGSmoothFilter::meNGSmoothFilter()
{
	m_param = new nglib::Ng_Meshing_Parameters();
}


meNGSmoothFilter::~meNGSmoothFilter() 
{
}

void meNGSmoothFilter::SetParams( meMeshSmoothingParamsPtr params )
{
// 	m_param.maxh = 3;
// 	m_param.fineness =  0.2;
// 	m_param.segmentsperedge =  0.2;
}

void meNGSmoothFilter::Update()
{
	try
	{
		m_param = new Ng_Meshing_Parameters();
		if (nglib::Ng_GetNP(GetInput()->GetNGMesh( )) ==  0)
		{
			GenerateSurfaceMesh( m_param );
		}
		else
		{
			Ng_STL_MakeEdges(GetInput()->GetGeom( ), GetInput()->GetNGMesh( ), m_param );
		}

		m_param->optimize2d = "m";
		Ng_STL_OptimizeSurfaceMesh( GetInput()->GetGeom( ), GetInput()->GetNGMesh( ), m_param );
	}
	catch (...)
	{
		throw;
	}
	
}