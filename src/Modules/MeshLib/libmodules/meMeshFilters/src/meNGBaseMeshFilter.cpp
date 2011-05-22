/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meNGBaseMeshFilter.h"

using namespace nglib;

meNGBaseMeshFilter::meNGBaseMeshFilter()
{
}


meNGBaseMeshFilter::~meNGBaseMeshFilter() 
{
}

void meNGBaseMeshFilter::SetInput(meNetgenMesh::Pointer input)
{
	// The problem of the netgen filters is that it uses directly the
	// mesh and it modify it so we have to copy the input in order to
	// have our output intact
	m_NetgenMesh = meNetgenMesh::New();
	m_NetgenMesh->CopyGeom(input->GetGeom());
	m_NetgenMesh->CopyMesh(input->GetNGMesh());
}

meNetgenMesh::Pointer  meNGBaseMeshFilter::GetInput(int id)
{
	return m_NetgenMesh;
}


void meNGBaseMeshFilter::GenerateSurfaceMesh( 
						Ng_Meshing_Parameters* mp )
{
	try
	{
		//Ng_STL_InitSTLGeometry(GetInput()->GetGeom( ));
		Ng_Result result;
		result = Ng_STL_MakeEdges( GetInput()->GetGeom( ), GetInput()->GetNGMesh( ), mp );
		if (result != NG_OK )
		{	
			std::cout << " Ng_result: "<< result << std::endl;
			throw std::runtime_error( "Error executing MakeEdges" );
		}
		std::cout<< "after making edges"<<std::endl;

		result = Ng_STL_GenerateSurfaceMesh( GetInput()->GetGeom( ), GetInput()->GetNGMesh( ), mp );
		if (result != NG_OK ) 
		{	
			std::cout<< " Ng_result: "<< result << std::endl;
			throw std::runtime_error( "Error executing GenerateSurfaceMesh" );
		}
		std::cout<< "after generate surface"<<std::endl;

	}
	catch(...)
	{
		throw;
	}
	
}
