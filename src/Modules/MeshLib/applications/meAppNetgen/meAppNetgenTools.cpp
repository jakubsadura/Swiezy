/*=========================================================================

  Module    : meshTest
  File      : $RCSfile: meAppNetgen.cc,v $
  Copyright : (C)opyright (University of Pompeu Fabra) 2006++
              See COPYRIGHT statement in top level directory.
  Authors   : Xavi Planes
  Modified  : $Author: criccobene $
  Purpose   : 
  Date      : $Date: 2008-07-22 10:48:03 $
  Version   : $Revision: 1.4 $
  Changes   : $Locker:  $


=========================================================================*/



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

#include "meMeshTypes.h"
#include "meNGOptimizeFilter.h"
#include "meNGRefineFilter.h"
#include "meNGSmoothFilter.h"

using namespace std;

using namespace blShapeUtils;

void usage()
{
	cerr << "Usage: meAppNetgenTools [input][output] <option>" 				<< endl;
	cerr << "The input is a .stl mesh of triangles" 			<< endl;
	cerr << "The input is a .vol mesh of triangles" 			<< endl;
	cerr << "Options:" 								<< endl;
	cerr << " -filter <int>:\n" 							<< endl;
	cerr << "\t 0: Optimize." 				<< endl;
	cerr << "\t 1: Smooth." 	<< endl;
	cerr << "\t 2: Refine." 				<< endl;
	cerr << "\t 3: optimize+refine." 		<< endl;
	exit(1); 
}




/**
 * \brief Main function for testing Netgen Filters
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
	bool ok;
	unsigned int filter;
	while (argc > 1)
	{
		ok = false;   
		//Option:
		if (!ok && strcmp(argv[1], "-filter")==0)
		{
			argc--;
			argv++;
			if(argv[1] == NULL)
			{
				cout << "ERROR:: filter IS NULL"<< endl;
				usage();
			}
			filter = atoi(argv[1]);
			argc--;
			argv++;
			ok = true;
		}
		if (!ok)
		{
			cerr << "Can not parse argument " << argv[1] << endl;
			usage();
		}
	} 
	meMeshParamsOptimizePtr param = meMeshParamsOptimizePtr (new meMeshParamsOptimize); 
	nglib::Ng_STL_Geometry* geom = nglib::Ng_STL_LoadGeometry(input_name);
	nglib::Ng_Mesh* mesh = nglib::Ng_NewMesh()/*nglib::Ng_LoadFileMesh(input_name_vol)*/;
	nglib::Ng_Meshing_Parameters* mp = new nglib::Ng_Meshing_Parameters(); 
	param->m_curvaturesafety = 0.6;
	param->m_segmentsperedge = 0.6;
	
	std::cout<< "mp->curvaturesafety : "<<mp->curvaturesafety <<std::endl;
	std::cout<< "mp->segmentsperedge : "<<mp->segmentsperedge <<std::endl;
	std::cout<< "mp->maxh : "<<mp->maxh <<std::endl;
	std::cout<< "mp->optimize2d : "<<mp->optimize2d <<std::endl;
	
	// Create new Netgen Mesh starting from geometry
	meNetgenMesh::Pointer netMesh = meNetgenMesh::New();
	netMesh->CopyGeom(geom);
	netMesh->CopyMesh(mesh);
 
	switch(filter)
	{
		case 0: //optimize
			{
				meNGOptimizeFilter::Pointer optimize = meNGOptimizeFilter::New();
				optimize->SetInput(netMesh);
				optimize->SetParams(param);
				optimize->Update();

				nglib::Ng_SaveMesh(optimize->GetInput()->GetNGMesh(),output_name );
				break;
			}
		case 1: //smooth
			{
				meNGSmoothFilter::Pointer smooth = meNGSmoothFilter::New();
				smooth->SetInput(netMesh);
				smooth->Update();
				nglib::Ng_SaveMesh(smooth->GetInput()->GetNGMesh(),output_name );
				break;
			}
		case 2: //refine
			{
				meNGRefineFilter::Pointer refine = meNGRefineFilter::New();
				refine->SetInput(netMesh);
				refine->Update();
				nglib::Ng_SaveMesh(refine->GetInput()->GetNGMesh(),output_name );
				break;
			}
		case 3: //optimize+refine
			{
				meNGOptimizeFilter::Pointer optimize = meNGOptimizeFilter::New();
				optimize->SetInput(netMesh);
				optimize->SetParams(param);
				optimize->Update();
				meNGRefineFilter::Pointer refine = meNGRefineFilter::New();
				refine->SetInput(optimize->GetInput());
				refine->Update();
				nglib::Ng_SaveMesh(refine->GetInput()->GetNGMesh(),output_name );
				break;
			}
		default:
			break;
			

	}
  	
	return 0;
}
