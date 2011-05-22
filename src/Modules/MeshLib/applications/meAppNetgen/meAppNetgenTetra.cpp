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
#include "vtkCell.h"
#include "vtkCellArray.h"
#include "vtkFeatureEdges.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkSelectPolyData.h"
#include "vtkSphereSource.h"
#include "vtkSphere.h"
#include "vtkPlane.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkClipPolyData.h"

// BaseLib
#include "blShapeUtils.h"
#include "blClock.h"
#include "blTestParams.h"
#include "blTestParamsFolderInfo.h"
#include <blConfigFileReader.h>

#include "meMeshTypes.h"
#include "meNGTetraFilter.h"

using namespace std;

using namespace blShapeUtils;



//--------------------------
// CONSTANTS & STRUCTURES
//--------------------------

// config filename
const char * defaultinputFilename = "input.vtk";


/**
 * \brief Main function for testing
 * 
 * Selects the input meshes
 * For each mesh, perform the tests and put the results into the 
 * WorkingFolder
 */


//---------------------------------------------------------------------------------------------------------------------------
int main (int argc, char ** argv)
//---------------------------------------------------------------------------------------------------------------------------
{
	//optimize params
	meMeshParamsOptimizePtr params = meMeshParamsOptimizePtr( new meMeshParamsOptimize);

	nglib::Ng_STL_Geometry* geom = nglib::Ng_STL_LoadGeometry("G:/Netgen/input.stl");
	meNGTetraFilter::Pointer m_Filter = meNGTetraFilter::New();
	meNetgenMesh::Pointer netgenMesh = meNetgenMesh::New();
	netgenMesh->SetGeom(geom);

	vtkUnstructuredGrid* volume;

	m_Filter->SetInput( netgenMesh );
	m_Filter->SetOptParams(params);
	try
	{
		m_Filter->Update();

		volume = m_Filter->GetVTK();
	}
	catch(...)
	{
		throw;
	}
	
	ShapeUtils::SaveVolumeToFile( volume, "G:/Netgen/volumetest3.vtk" );

	  return 0;
}
