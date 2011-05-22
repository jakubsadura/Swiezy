/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meNGRefineFilter.h"

meNGRefineFilter::meNGRefineFilter()
{

}


meNGRefineFilter::~meNGRefineFilter() 
{
}

void meNGRefineFilter::Update()
{
	try
	{
		nglib::Ng_Meshing_Parameters* mp = new nglib::Ng_Meshing_Parameters();

		if (nglib::Ng_GetNP(GetInput()->GetNGMesh( )) ==  0)
		{
			GenerateSurfaceMesh( mp );
			nglib::Ng_STL_OptimizeSurfaceMesh( GetInput()->GetGeom( ), GetInput()->GetNGMesh( ), mp );
		}
		else
		{
			nglib::Ng_STL_MakeEdges(GetInput()->GetGeom( ), GetInput()->GetNGMesh( ), mp );
		}

		nglib::Ng_MeshRefinement( GetInput()->GetGeom( ), GetInput()->GetNGMesh( ) );
	}
	catch (...)
	{
		throw;
	}
	
}