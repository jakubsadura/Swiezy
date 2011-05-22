/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/



//--------------------------
// HEADERS
//--------------------------


#include <iostream>
#include <fstream>

// Netgen
#include "nglib.h"


// VTK
#include "vtkPolyData.h"

// BaseLib
#include "blShapeUtils.h"
#include "meNGToVtkPolydataFilter.h"


using namespace std;

using namespace blShapeUtils;

void usage()
{
	cerr << "Usage: meAppNetgenToVTK [input][output] " 				<< endl;
	cerr << "The input is a .vol mesh (tetra or surface)" 			<< endl;
	cerr << "The output is a .vtk mesh (unstructuredgrid or polyData)" 			<< endl;

	exit(1); 
}




/**
 * \brief Main function for converting Netgen to vtkObjects
 * 
 */


//---------------------------------------------------------------------------------------------------------------------------
int main (int argc, char ** argv)
//---------------------------------------------------------------------------------------------------------------------------
{
	
	if (argc < 3){
		usage();
	}

	const char * input_name = NULL, * output_name=NULL;    
	input_name = argv[1];
	argc--;
	argv++;
	output_name = argv[1];
	argc--;
	argv++;
	
	nglib::Ng_Mesh* mesh = nglib::Ng_LoadFileMesh(input_name);
	nglib::Ng_STL_Geometry* geom = nglib::Ng_STL_NewGeometry();
	// Create new Netgen Mesh starting from mesh
	meNetgenMesh::Pointer netMesh = meNetgenMesh::New();
	netMesh->CopyGeom(geom);
	netMesh->CopyMesh(mesh);
 
	meNGToVtkPolydataFilter::Pointer filter  = meNGToVtkPolydataFilter::New();
	filter->SetInput(netMesh);
	filter->Update();
	
	blShapeUtils::ShapeUtils::SaveShapeToFile(filter->GetPolyData(),output_name);

	
	return 0;
}
