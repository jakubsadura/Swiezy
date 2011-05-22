/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meNGOptimizeFilter.h"

meNGOptimizeFilter::meNGOptimizeFilter()
{
	m_param = meMeshParamsOptimizePtr( new meMeshParamsOptimize );
}


meNGOptimizeFilter::~meNGOptimizeFilter() 
{
}

void meNGOptimizeFilter::SetParams( meMeshParamsOptimizePtr params )
{
	m_param = params;
}

void meNGOptimizeFilter::Update()
{
	try
	{
		nglib::Ng_Meshing_Parameters*  netgenParam = new nglib::Ng_Meshing_Parameters(); 
		netgenParam->maxh = m_param->m_maxh;
		netgenParam->curvaturesafety = m_param->m_curvaturesafety;
		netgenParam->segmentsperedge = m_param->m_segmentsperedge;
		//netgenParam->optsteps2d = 3;
		
		// if the input is only the geometry then generate the mesh before using it
		if (nglib::Ng_GetNP(GetInput()->GetNGMesh( )) ==  0)
		{
			GenerateSurfaceMesh( netgenParam );
		}
		else
		{
			nglib::Ng_STL_MakeEdges(GetInput()->GetGeom( ),GetInput()->GetNGMesh( ),netgenParam);
		}

	
		netgenParam->optimize2d = "cmSmSm";
		nglib::Ng_STL_OptimizeSurfaceMesh( GetInput()->GetGeom( ), GetInput()->GetNGMesh( ), netgenParam );
		nglib::Ng_MeshRefinement( GetInput()->GetGeom( ), GetInput()->GetNGMesh( ) );
		netgenParam->optimize2d = "m";
		nglib::Ng_STL_OptimizeSurfaceMesh( GetInput()->GetGeom( ), GetInput()->GetNGMesh( ), netgenParam );

		std::cout<< "optimization done!"<<std::endl;


	}
	catch (...)
	{
		throw;
	}
	
}

meMeshParamsOptimizePtr meNGOptimizeFilter::GetParams()
{
	return m_param;
}
