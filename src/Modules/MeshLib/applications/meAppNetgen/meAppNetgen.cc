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


// MeshLib
#include "meMesh.h"
#include "meMeshImplVTK.h"
#include "meMeshImplNetgen.h"
#include "meMeshImplNetgenSTL.h"
#include "meVTKPolyDataSplitter.h"
#include "meVTKPolyDataJoiner.h"

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

using namespace std;
using namespace nglib;
using namespace blShapeUtils;



//--------------------------
// CONSTANTS & STRUCTURES
//--------------------------

// config filename
const char * defaultConfigFilename = "configFile.txt";

	
/** 
 * \brief Type of test processing that will be performed
 * \ingroup MeshLibTest
 */
typedef enum PROCESSING_TYPE{
	/// Call Smooth( ) of type Netgen 
	PROCESSING_TYPE_SMOOTHING_NETGEN,
	/// Call Smooth( ) of type Taubin 
	PROCESSING_TYPE_SMOOTHING_TAUBIN,
 	/// Call Optimize( )
	PROCESSING_TYPE_OPTIMIZATION,
	/// Call Copy( )
	PROCESSING_TYPE_COPY,
	/// Call Refine( )
	PROCESSING_TYPE_REFINEMENT,
	/// Call VolumeClosing( )
	PROCESSING_TYPE_VOLUME_CLOSING,
	/// Alessandro use case
	PROCESSING_TYPE_ALESSANDRO,
	/// Call SelectCells( )
	PROCESSING_TYPE_SELECT_CELLS,
	/// Call EdgeSwapping( )
	PROCESSING_TYPE_EDGE_SWAPPING,
	/// Call LocalRefiner( ) with local selected cells
	PROCESSING_TYPE_LOCAL_REFINER,
	/// Call LoopSubdivisionRefiner( )
	PROCESSING_TYPE_LOOP_SUBDIVISION_REFINER,
	/// Call EdgeSwapping( ) with local selected cells
	PROCESSING_TYPE_EDGE_SWAPPING_LOCAL,
	/// Call Smooth( ) of type Taubin with local selected cells
	PROCESSING_TYPE_SMOOTHING_TAUBIN_LOCAL,
	/// Call Smooth( ) of type Umbrella with local selected cells
	PROCESSING_TYPE_SMOOTHING_UMBRELLA_LOCAL,
	/// Call ExtractMainConnectedSurface( ) 
	PROCESSING_TYPE_EXTRACT_MAIN_CONNECTED_SURFACE,
	/// Call TetraGeneration???
	PROCESSING_TYPE_TETRA_GENERATION,
	/// Load Data (test)
	PROCESSING_TYPE_LOAD,
	/// Check Data
	PROCESSING_TYPE_CHECK,
	/// Compute Mesh Quality (tetrahedrons)
	PROCESSING_TYPE_COMPUTE_QUALITY,
	/// Call TetraGeneration and compute Mesh quality (tetrahedrons)
	PROCESSING_TYPE_RAFA,
	/// Create polydata in order to visualize regionID
	PROCESSING_TYPE_CHIARA

};




/** 
* \brief Class of parameters
* \ingroup meAppNetgen
*/
class InputData
{
public:
	std::string	strInputPath;
	std::string	strFileName;
	std::string	strOutputPath;
	PROCESSING_TYPE m_processingType;

	//smoothing params
	double percentage;
	int	umbrella_order;
	bool use_percentage;

	//optimize params
	double m_fMaxh;
	double	curvature_safety;
	double	segments_edge;

	//volume closing & edge swapping
	double angle;
	int min_edge_swap;
	double density_factor;

	//tetra generation
	double max_volume;
	double aspect_ratio;
	bool export_tetra;
        
	//loop sub refine
	int num_subdivision;

	//save data
	bool m_save_vtk;
        bool m_save_stl;

        //purkinje and fibers
        double pkjresolution;
        double pkjangle;
        double pkjdistance;
        int pkjnclosepoints;
        bool verbose;

	InputData()
	{
		m_fMaxh = 3.0;
		m_processingType = PROCESSING_TYPE_RAFA;
		strInputPath = "";
		strFileName = "";
		strOutputPath = "";
		percentage = 0.0001;
		umbrella_order = 2;
		use_percentage = true;
		curvature_safety = 0.2;
		segments_edge = 0.2;
		angle = 0.0;
		min_edge_swap = 10;
		density_factor = 1.41421;
		max_volume = 0.5;
		aspect_ratio = 2.0;
		export_tetra = true;
		num_subdivision = 1;
		m_save_vtk = true;
		m_save_stl = false;
		pkjresolution = 1.0;
                pkjangle= 0.34;
                pkjdistance = 4.0;
                pkjnclosepoints = 30;
                verbose = true;
	}

};

//---------------------------------------------------------------------------------------------------------------------------
std::string CreateOutputFileName( 
								 InputData inputparams,
								 std::string	strFuncName,
								 std::string	strExtension )
//---------------------------------------------------------------------------------------------------------------------------
{
	std::string strOutFileName;
	strOutFileName = inputparams.strOutputPath + inputparams.strFileName + ".meshTestNetgen_" + 
		strFuncName + strExtension;
	return strOutFileName;
}

std::string CreateInputFileName( 
								 InputData inputparams )
{
	std::string strInFileName;
	strInFileName =  inputparams.strInputPath + inputparams.strFileName ;
	return strInFileName;	
}

//--------------------------
// TESTING FUNCTIONS
//--------------------------

/**
 * \brief Test function number 9
 * \ingroup meAppNetgen
 *
 * - Load VTK STL mesh
 * - Convert it into Netgen geometry 
 * - Convert back to VTK mesh
 * 
 */

//---------------------------------------------------------------------------------------------------------------------------
void TestLoad( InputData inputparams )
//---------------------------------------------------------------------------------------------------------------------------
{
	std::string	strOutFileName;
	vtkPolyData	*vtkPolyData;
	
	vtkPolyData = ShapeUtils::LoadShapeFromFile( CreateInputFileName(inputparams).c_str() );
	
	strOutFileName = CreateOutputFileName( inputparams, "T9", ".stl" );
	ShapeUtils::SaveShapeToFile( vtkPolyData, strOutFileName.c_str() );

}
void TestChiara( InputData inputparams )
//---------------------------------------------------------------------------------------------------------------------------
{	/*
	std::string	strOutFileName;
	vtkPolyData	*vtkPolyDataIn;
	vtkPolyData	*vtkPolyDataOut;
	vtkPolyDataIn = ShapeUtils::LoadShapeFromFile( CreateInputFileName(inputparams).c_str() );
	vtkPolyDataOut=ShapeUtils::GetShapeRegion(vtkPolyDataIn,0,0,"regionID");
	strOutFileName = CreateOutputFileName( inputparams, "purkinje", ".vtk" );
	ShapeUtils::SaveShapeToFile( vtkPolyDataOut, strOutFileName.c_str() );

	// Clean
	//mr->Delete();
	//vtkUGrid->Delete();
	vtkPolyDataIn->Delete();
	vtkPolyDataOut->Delete();
	*/
	meMesh		*pMeshone = new meMesh;
    meMesh	    *pMeshtwo ;
	meMeshImplVTK		*pMeshIn = new meMeshImplVTK;
	meMeshImplVTK	    *pMeshOut = new meMeshImplVTK;
	meMeshImplVTK	    *pMeshOut2 = new meMeshImplVTK;
	vtkSphere *sphere = vtkSphere::New();
	sphere->SetRadius(30);
	vtkPlane *plane =vtkPlane::New();
	vtkPolyData	*vtkPolyDataIn;
	vtkPolyData	*vtkPolyDataOut = vtkPolyData::New();
	vtkPolyData	*vtkPolyDataSurr = vtkPolyData::New();
	std::string	strOutFileName;
	vtkPolyDataIn = ShapeUtils::LoadShapeFromFile( CreateInputFileName(inputparams).c_str() );
	vtkPolyDataSurr->DeepCopy(vtkPolyDataIn);
	pMeshIn->SetData( vtkPolyDataIn );
	pMeshIn->VolumeClipping(pMeshOut,sphere,true);
	//pMeshIn->VolumeClipping(pMeshOut,plane,true);
	pMeshOut->GetData( vtkPolyDataOut );
	//pMeshIn->VolumeClipping(pMeshOut2,sphere,false);
	//pMeshOut2->GetData( vtkPolyDataSurr );
	meVTKPolyDataJoiner::Pointer joiner = meVTKPolyDataJoiner::New();
	/*joiner->SetInput(vtkPolyDataSurr,vtkPolyDataOut);
	joiner->SetTagging( false );
	joiner->Update();
	
	
	vtkSphereSource *spheresource = vtkSphereSource::New();
	vtkSphereSource *spheresource2 = vtkSphereSource::New();
	spheresource->SetRadius(3);
	spheresource2->SetRadius(50);
	spheresource->SetThetaResolution(30);
	spheresource->SetPhiResolution(30);
	spheresource2->SetThetaResolution(30);
	spheresource2->SetPhiResolution(30);
	spheresource->SetCenter(0,10,47);
	spheresource2->Update();
	spheresource->Update();

	vtkSelectPolyData *selection = vtkSelectPolyData::New();
	selection->SetInput(vtkPolyDataIn);
	selection->SetLoop(spheresource->GetOutput()->GetPoints());
	selection->Update();
	//vtkPolyDataSurr = selection->GetUnselectedOutput();
		vtkRenderWindow* renWin = vtkRenderWindow::New();
	vtkRenderer* ren = vtkRenderer::New();
	renWin->AddRenderer( ren);
	renWin->SetSize( 600, 300);
	ren->SetBackground( 0, 0, 0);
	ren->SetViewport( 0, 0, 1, 1);
	vtkPolyDataMapper *map = vtkPolyDataMapper::New();
	map -> SetInput(vtkPolyDataOut);
	vtkActor *actor = vtkActor::New();
	actor -> SetMapper(map);
	ren->AddActor(actor);
	renWin->SetWindowName("Clip");
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow( renWin);
	renWin->Render();
	

	strOutFileName = CreateOutputFileName( inputparams, "Clip", ".vtk" );
	ShapeUtils::SaveShapeToFile( vtkPolyDataOut, strOutFileName.c_str() );
	//strOutFileName = CreateOutputFileName( inputparams, "Union", ".vtk" );
	//ShapeUtils::SaveShapeToFile(joiner->GetOutput(), "Union.vtk" );
	ShapeUtils::SaveShapeToFile(spheresource2->GetOutput(), "Sphere2.vtk" );
	ShapeUtils::SaveShapeToFile(spheresource->GetOutput(), "Selectionsphere.vtk" );
	ShapeUtils::SaveShapeToFile(selection->GetOutput(),"Selected.vtk");
	*/
	pMeshone->SetData(vtkPolyDataIn);
	pMeshone->SelectCells(sphere);
	pMeshone->GetData(vtkPolyDataSurr);
	ShapeUtils::SaveShapeToFile(vtkPolyDataSurr,"Selected.vtk");

	//delete pMeshone;
	//delete pMeshtwo;

	delete pMeshIn;
	delete pMeshOut;
	delete pMeshOut2;
	vtkPolyDataIn->Delete( );
	vtkPolyDataOut->Delete( );
	vtkPolyDataSurr->Delete( );
	//joiner->Delete();
	sphere->Delete();
	//spheresource->Delete();
	//spheresource2->Delete();
	plane->Delete();
	//selection->Delete();
	
	
}

//---------------------------------------------------------------------------------------------------------------------------
void TestAlessandro( InputData inputparams )
//---------------------------------------------------------------------------------------------------------------------------
{

	meMesh				*pMeshIn = new meMesh;
	meMesh				*pMeshOut = new meMesh;
	vtkPolyData*		vtkPolyDataIn;
	vtkPolyData*		vtkPolyDataOut;
	std::string			strOutFileName;


	vtkPolyDataOut		= vtkPolyData::New();

	// Load VTK shape
	vtkPolyDataIn = ShapeUtils::LoadShapeFromFile( 
		CreateInputFileName(inputparams).c_str() );

	// Insert VTK mesh data into Netgen mesh
	pMeshIn->SetData( vtkPolyDataIn );

	// Call optimize in VTK mesh
	meMeshSmoothingParams params;
	params.m_Type = MESH_SMOOTHING_TYPE_TAUBIN;
	pMeshIn->Smooth( params, *pMeshOut );

	pMeshOut->Copy( *pMeshIn );
	pMeshIn->ExtractMainConnectedSurface( *pMeshOut );

	pMeshOut->Copy( *pMeshIn );
	pMeshIn->Smooth( params, *pMeshOut );

	//pMeshOut->Copy( *pMeshIn );
	//meMeshParamsOptimize	paramsOptimize;
	//pMeshIn->Optimize( paramsOptimize, *pMeshOut );

	
	// Get the result mesh in a VTK shape
	pMeshOut->GetData( vtkPolyDataOut );
	strOutFileName = CreateOutputFileName( inputparams, "AleOut", ".vtk" );
	ShapeUtils::SaveShapeToFile( vtkPolyDataOut, strOutFileName.c_str() );


	// Delete memory
	delete pMeshIn;
	delete pMeshOut;
	vtkPolyDataIn->Delete( );
	vtkPolyDataOut->Delete( );

}

//---------------------------------------------------------------------------------------------------------------------------
void Check(	InputData inputparams)
//---------------------------------------------------------------------------------------------------------------------------
{
	//Load Data
	vtkPolyData	*vtkPolyData;
	vtkPolyData = ShapeUtils::LoadShapeFromFile( CreateInputFileName(inputparams).c_str() );
	// Extract edges, but only boundary edges
	vtkFeatureEdges *extractEdges = vtkFeatureEdges::New();
	extractEdges->SetInput(vtkPolyData);
	extractEdges->BoundaryEdgesOn();
	extractEdges->FeatureEdgesOff();
	extractEdges->ManifoldEdgesOff();
	extractEdges->NonManifoldEdgesOff();
	
	unsigned int numEdges = 0;

	// Split output, to get boundaries one by one
	vtkPolyDataConnectivityFilter *connectivity = vtkPolyDataConnectivityFilter::New();
	connectivity->SetInput( extractEdges->GetOutput() );
	connectivity->SetExtractionModeToSpecifiedRegions();
	connectivity->Update();

	unsigned int numHoles = connectivity->GetNumberOfExtractedRegions();

	std::cout << "Number of holes: " << numHoles << std::endl;
	std::cout << "Number of edges: " << vtkPolyData->GetNumberOfLines() << std::endl;
	std::cout << "Number of vertices: " << vtkPolyData->GetNumberOfVerts() << std::endl;
	std::cout << "Number of triangles: " << vtkPolyData->GetNumberOfCells() << std::endl;

	// Delete local objects
	connectivity->Delete();
	extractEdges->Delete();
	

}

//---------------------------------------------------------------------------------------------------------------------------
void ComputeQuality(	InputData inputparams)
//---------------------------------------------------------------------------------------------------------------------------
{
	//Load Data
	vtkUnstructuredGridReader* mr = vtkUnstructuredGridReader::New();
	vtkUnstructuredGrid* ug;
       
	//char* fname = vtkTestUtilities::ExpandDataFileName(filename);

	mr->SetFileName( CreateInputFileName(inputparams).c_str() );
	mr->Update();

	ug = mr->GetOutput();
	meMeshImplVTK  *pMeshVTK = new meMeshImplVTK;
	std::cout<< mr->GetFileName();
	pMeshVTK->ComputeMeshQuality(ug);

	// Clean
	delete pMeshVTK;  
	mr->Delete();

}
//---------------------------------------------------------------------------------------------------------------------------
void TestRafa( InputData inputparams )
//---------------------------------------------------------------------------------------------------------------------------
{
        meMesh				*pMeshIn = new meMesh;
	meMesh				*pMeshOut = new meMesh;
	vtkPolyData*		vtkPolyDataIn;
	vtkPolyData*		vtkPolyDataOut;
	std::string			strOutFileName;
	std::string			strModuleStep;
	Ng_STL_Geometry		*geom = new Ng_STL_Geometry;
	
	vtkPolyDataOut		= vtkPolyData::New();


	//------------------------------------------
	// Load VTK shape
	vtkPolyDataIn = ShapeUtils::LoadShapeFromFile( CreateInputFileName(inputparams).c_str() );

	// Insert VTK mesh data into Netgen mesh
	pMeshIn->SetData( vtkPolyDataIn );

	//-----------------------------------------
	// Step 1  Tetra Generation
	//-----------------------------------------
	meMeshTetraGenerationParams params;
	params.m_MaxVolume = inputparams.max_volume;
	params.m_AspectRatio = inputparams.aspect_ratio;
	params.m_ExportTetras = inputparams.export_tetra;
	pMeshIn->TetraGeneration(params, *pMeshOut );
	strModuleStep = "Tetrageneration";
	
		
	//-----------------------------------------
	// Step 2  MeshQuality
	//-----------------------------------------
	//Load Data
	vtkUnstructuredGridReader* mr = vtkUnstructuredGridReader::New();
	vtkUnstructuredGrid* ug;
		mr->SetFileName( "tetgenout.vtk" );
	mr->Update();

	ug = mr->GetOutput();
	meMeshImplVTK  *pMeshVTK = new meMeshImplVTK;
	std::cout<< mr->GetFileName();
	pMeshVTK->ComputeMeshQuality(ug);
	//------------------------------------------
	
	

	//------------------------------------------
	// Delete memory
	delete pMeshIn;
	delete pMeshOut;
	vtkPolyDataIn->Delete( );
	vtkPolyDataOut->Delete( );
	delete pMeshVTK;  
	mr->Delete();
}


/**
* \brief Copy from source to dest. and save to a VTK shape file
*/
//---------------------------------------------------------------------------------------------------------------------------
void CopyMesh(	meMeshImpl			*pMeshSrc, 
			  meMeshImpl			*pMeshDst, 
			  InputData			inputparams,
			  std::string			strOperation )
//---------------------------------------------------------------------------------------------------------------------------
{

	vtkPolyData*		vtkPolyDataOut;
	vtkPolyDataOut		= vtkPolyData::New();
	blClock				clock;
	meMeshImpl			*pMeshVTK = new meMeshImplVTK; 

	clock.Start( );
	pMeshDst->Copy( *pMeshSrc );
	clock.Finish( );


	/*
	pMeshVTK->Copy( *pMeshDst );
	pMeshVTK->GetData( vtkPolyDataOut );
	std::string strOutFileName;
	strOutFileName = inputData.GetWorkingFileName( 0, strOperation.c_str() );
	ShapeUtils::SaveShapeToFile( vtkPolyDataOut, strOutFileName.c_str() );
	*/

	vtkPolyDataOut->Delete( );

	cout.precision( 3 );
}


/**
* \brief Mesh processing 
* \ingroup meAppNetgen
*
* - Load VTK shape
* - Optimize or smooth
* - Save VTK shape
*
*/
//---------------------------------------------------------------------------------------------------------------------------
void MeshSingleProcessing( InputData inputparams )
//---------------------------------------------------------------------------------------------------------------------------
{

	meMesh				*pMeshIn = new meMesh;
	meMesh				*pMeshOut = new meMesh;
	vtkPolyData*		vtkPolyDataIn;
	vtkPolyData*		vtkPolyDataOut;
	std::string			strOutFileName;
	std::string			strModuleStep;
	Ng_STL_Geometry		*geom = new Ng_STL_Geometry;
	
	vtkPolyDataOut		= vtkPolyData::New();


	//------------------------------------------
	// Load VTK shape
	vtkPolyDataIn = ShapeUtils::LoadShapeFromFile( CreateInputFileName(inputparams).c_str() );

	// Insert VTK mesh data into Netgen mesh
	pMeshIn->SetData( vtkPolyDataIn );



	//------------------------------------------
	// Processing test
	switch (inputparams.m_processingType )
	{

		//------------------------------------------
	case PROCESSING_TYPE_OPTIMIZATION:
		{
			meMeshParamsOptimize	paramsOptimize;
			paramsOptimize.m_curvaturesafety = inputparams.curvature_safety;
			paramsOptimize.m_segmentsperedge = inputparams.segments_edge;
			paramsOptimize.m_maxh = inputparams.m_fMaxh;
			pMeshIn->Optimize( paramsOptimize, *pMeshOut );
			strModuleStep = "Optimize";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_SMOOTHING_NETGEN:
		{
			meMeshSmoothingParams params;
			params.m_Type = MESH_SMOOTHING_TYPE_NETGEN;
			params.m_UmbrellaOrder = inputparams.umbrella_order;
			params.m_Percentage = inputparams.percentage;
			pMeshIn->Smooth( params, *pMeshOut );
			strModuleStep = "Smooth";
		}
		break;
		//------------------------------------------
	case PROCESSING_TYPE_SMOOTHING_TAUBIN:
		{
			meMeshSmoothingParams params;
			params.m_Type = MESH_SMOOTHING_TYPE_TAUBIN;
			params.m_UmbrellaOrder = inputparams.umbrella_order;
			params.m_Percentage = inputparams.percentage;
			pMeshIn->Smooth( params, *pMeshOut );
			strModuleStep = "SmoothT";
		}
		break;
		//------------------------------------------
	case PROCESSING_TYPE_REFINEMENT:
		{
			pMeshIn->Refine( *pMeshOut );
			strModuleStep = "Refine";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_VOLUME_CLOSING:
		{
			meMeshVolumeClosingParams params;
			params.m_DensityFactor = inputparams.density_factor;
			params.m_Order = inputparams.umbrella_order;
			pMeshIn->VolumeClosing( params, *pMeshOut );
			strModuleStep = "VolumeClosing";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_SELECT_CELLS:
		{
			vtkSphere *sphere = vtkSphere::New();
			sphere->SetRadius(30);
			// Select cells
			//pMeshIn->SelectCells( -100, 100, -100, 100, 0, 100 );
			pMeshIn->SelectCells( sphere );
			vtkIdList *pList = pMeshIn->GetSelectedCellList();

			// Split vtkPolyData
			meVTKPolyDataSplitter::Pointer splitter = meVTKPolyDataSplitter::New();
			splitter->SetInput( vtkPolyDataIn );
			splitter->SetCellIdList( pList );
			splitter->SetTagging( false );
			splitter->Update();
			vtkPolyData *outputSurrounding = NULL, *outputPatch = NULL;
			splitter->GetOutput( outputSurrounding, outputPatch );

			vtkPolyDataOut->DeepCopy( outputPatch );
			pMeshOut->SetData( vtkPolyDataOut );

			// Free memory
			pList->Delete();
			sphere->Delete();

			strModuleStep = "SelectCells";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_EDGE_SWAPPING:
		{
			meMeshEdgeSwappingParams	params;
			params.m_Angle = inputparams.angle;
			params.m_MinimumEdgesSwapped = inputparams.min_edge_swap;
			pMeshIn->EdgeSwapping( params, *pMeshOut );
			strModuleStep = "EdgeSwapping";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_LOCAL_REFINER:
		{
			pMeshIn->SelectCells( -100, 100, -100, 100, 0, 100 );
			meMeshLocalRefinerParams	params;
			params.m_DensityFactor = inputparams.density_factor;
			params.m_Angle = inputparams.angle;
			params.m_MinimumEdgesSwapped = inputparams.min_edge_swap;
			pMeshIn->LocalRefiner( params, *pMeshOut );
			strModuleStep = "LocalRefiner";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_LOOP_SUBDIVISION_REFINER:
		{
			meMeshLoopSubdivisionParams	params;
			params.m_NumberOfSubdivisions = inputparams.num_subdivision;
			pMeshIn->LoopSubdivisionRefiner( params, *pMeshOut );
			strModuleStep = "LoopSubdivisionRefiner";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_EDGE_SWAPPING_LOCAL: 
		{
			pMeshIn->SelectCells( -100, 100, -100, 100, 0, 100 );
			meMeshEdgeSwappingParams	params;
			params.m_Angle = inputparams.angle;
			params.m_MinimumEdgesSwapped = inputparams.min_edge_swap;
			pMeshIn->EdgeSwapping( params, *pMeshOut );
			strModuleStep = "EdgeSwapping.Local";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_SMOOTHING_TAUBIN_LOCAL: 
		{
			pMeshIn->SelectCells( -100, 100, -100, 100, 0, 100 );
			meMeshSmoothingParams params;
			params.m_Type = MESH_SMOOTHING_TYPE_TAUBIN;
			params.m_UmbrellaOrder = inputparams.umbrella_order;
			params.m_Percentage = inputparams.percentage;
			pMeshIn->Smooth( params, *pMeshOut );
			strModuleStep = "Smooth.Taubin.Local";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_SMOOTHING_UMBRELLA_LOCAL: 
		{
			pMeshIn->SelectCells( -100, 100, -100, 100, 0, 100 );
			meMeshSmoothingParams params;
			params.m_Type = MESH_SMOOTHING_TYPE_UMBRELLA;
			params.m_UmbrellaOrder = inputparams.umbrella_order;
			params.m_Percentage = inputparams.percentage;
			pMeshIn->Smooth( params, *pMeshOut );
			strModuleStep = "Smooth.Umbrella.Local";
		}
		break;

		//------------------------------------------
	case PROCESSING_TYPE_EXTRACT_MAIN_CONNECTED_SURFACE: 
		{
			pMeshIn->ExtractMainConnectedSurface(*pMeshOut );
			strModuleStep = "ExtractMainConnectedSurface";
		}
		break;
		
		//------------------------------------------
	case PROCESSING_TYPE_TETRA_GENERATION: 
		{
		        meMeshTetraGenerationParams params;
			params.m_MaxVolume = inputparams.max_volume;
			params.m_AspectRatio = inputparams.aspect_ratio;
			params.m_ExportTetras = inputparams.export_tetra;
		        pMeshIn->TetraGeneration(params, *pMeshOut );
			strModuleStep = "Tetrageneration";
		}
		break;
		
	}


	//------------------------------------------
	
	if ( inputparams.m_save_vtk )
	{
	// Get the result mesh in a VTK shape
	pMeshOut->GetData( vtkPolyDataOut );
	strOutFileName = CreateOutputFileName(inputparams ,strModuleStep.c_str(), ".vtk");
	ShapeUtils::SaveShapeToFile( vtkPolyDataOut, strOutFileName.c_str() );
	}

	if ( inputparams.m_save_stl )
	{
	// Get the result mesh in a VTK shape
	strOutFileName = CreateOutputFileName( inputparams , strModuleStep.c_str(), ".stl" );
	ShapeUtils::SaveShapeToFile( vtkPolyDataOut, strOutFileName.c_str() );
	}

	//------------------------------------------
	// Delete memory
	delete pMeshIn;
	delete pMeshOut;
	vtkPolyDataIn->Delete( );
	vtkPolyDataOut->Delete( );

}

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
        
	blConfigFileReader * configFileReader = new blConfigFileReader();
	
	// ---------------------------------------------
	// READ PARAMETERS FROM CONFIG FILE
	// ---------------------------------------------	
	
	// ---------------------------------------------
	// LOOK FOR SPECIFIED CONFIGURATION FILE
	// ---------------------------------------------
	switch ( argc )
	{
	case 1:	// Default config file
		{
			configFileReader->SetFilename(defaultConfigFilename);
			break;
		}
	case 2: // ASM_Battery configFileName
		{
			configFileReader->SetFilename( argv[1] );
			break;
		}	
	default:
		{ 
			return 0;
		}
	}


	configFileReader->Update();
	switch (configFileReader->GetLastError() )
	{
	case 0: break;
	case -1: 
		{
			std::cerr << "Error: File " << 
				configFileReader->GetFilename() << 
				" not found" << std::endl;			
			return -2;	// Unable to open config file
		}
	default:
		{
			std::cerr << "Error: Unexpected error reading " <<
				configFileReader->GetFilename() << std::endl;			
			return -3;	// Errors reading config file
		}
	}

	// ---------------------------------------------
	// INIT PARAMETERS
	// ---------------------------------------------
	
	InputData m_inputdata;
	std::string	process_type;	
	
	m_inputdata.strInputPath = configFileReader->GetValue("INPUT_DATA_FOLDER");
	m_inputdata.strFileName = configFileReader->GetValue("INPUT_DATA_NAME");
	m_inputdata.strOutputPath = configFileReader->GetValue("OUTPUT_DATA_FOLDER");
	process_type = configFileReader->GetValue("PROCESSING_TYPE");
	m_inputdata.m_save_stl = configFileReader->GetValueAsBool("SAVE_STL_FILES");
	m_inputdata.m_save_vtk = configFileReader->GetValueAsBool("SAVE_VTK_FILES");

	// process parameters initialization
	if (process_type == "SMOOTHNETGEN")			
		{	m_inputdata.m_processingType = PROCESSING_TYPE_SMOOTHING_NETGEN;
			m_inputdata.percentage = configFileReader->GetValueAsDouble("PERCENTAGE");
		}	
	else if (process_type == "SMOOTHTAUBIN")			
		{	m_inputdata.m_processingType = PROCESSING_TYPE_SMOOTHING_TAUBIN;
			m_inputdata.percentage = configFileReader->GetValueAsDouble("PERCENTAGE");
		}
	else if (process_type == "OPTIMIZE")		
		{
		m_inputdata.m_processingType = PROCESSING_TYPE_OPTIMIZATION;
		m_inputdata.m_fMaxh = configFileReader->GetValueAsDouble("MAX_H");
		m_inputdata.curvature_safety = configFileReader->GetValueAsDouble("CURVATURE_SAFETY");
		m_inputdata.segments_edge = configFileReader->GetValueAsDouble("SEGMENTS_EDGE");
		}
	else if (process_type == "COPY")			
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_COPY;
		}
	else if (process_type == "REFINE")			
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_REFINEMENT;
		}
	else if (process_type == "VOLUMECLOSING")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_VOLUME_CLOSING;
			m_inputdata.angle = configFileReader->GetValueAsDouble("VC_ANGLE");
			m_inputdata.min_edge_swap = configFileReader->GetValueAsDouble("VC_MIN_EDGE_SWAP");
			m_inputdata.density_factor = configFileReader->GetValueAsDouble("VC_DENSITY_FACTOR");
			m_inputdata.umbrella_order = configFileReader->GetValueAsInt("VC_ORDER");
		}
	else if (process_type == "ALESSANDRO")		
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_ALESSANDRO;
		}
	else if (process_type == "SELECTCELLS")		
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_SELECT_CELLS;
		}
	else if (process_type == "EDGESWAP")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_EDGE_SWAPPING;
			m_inputdata.angle = configFileReader->GetValueAsDouble("ANGLE");
			m_inputdata.min_edge_swap = configFileReader->GetValueAsDouble("MIN_EDGE_SWAP");
		}
	else if (process_type == "LOCALREFINE")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_LOCAL_REFINER;
		}
	else if (process_type == "LOOPSUBREFINE")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_LOOP_SUBDIVISION_REFINER;
			m_inputdata.num_subdivision =configFileReader->GetValueAsInt("NUM_SUBDIVISION");
		}
	else if (process_type == "LOCALEDGESWAP")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_EDGE_SWAPPING_LOCAL;
			m_inputdata.angle = configFileReader->GetValueAsDouble("ANGLE");
			m_inputdata.min_edge_swap = configFileReader->GetValueAsDouble("MIN_EDGE_SWAP");
		}
	else if (process_type == "LOCALSMOOTHTAU")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_SMOOTHING_TAUBIN_LOCAL;
			m_inputdata.percentage = configFileReader->GetValueAsDouble("PERCENTAGE");
		}
	else if (process_type == "LOCALSMOOTHUMB")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_SMOOTHING_UMBRELLA_LOCAL;
			m_inputdata.percentage = configFileReader->GetValueAsDouble("PERCENTAGE");
			m_inputdata.umbrella_order = configFileReader->GetValueAsInt("ORDER");
		}
	else if (process_type == "EXTMAINCONNSURF")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_EXTRACT_MAIN_CONNECTED_SURFACE;
		}
	else if (process_type == "LOAD")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_LOAD;
		}
	else if (process_type == "CHECK")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_CHECK;
		}
	else if (process_type == "TETRA")	
		{
			m_inputdata.m_processingType =	PROCESSING_TYPE_TETRA_GENERATION;
			m_inputdata.max_volume = configFileReader->GetValueAsDouble("MAX_VOLUME");
			m_inputdata.aspect_ratio = configFileReader->GetValueAsDouble("ASPECT_RATIO");
			m_inputdata.export_tetra = configFileReader->GetValueAsBool("EXPORT_TETRA");
	      	}
	else if (process_type == "QUALITY")	
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_COMPUTE_QUALITY;
		}
	else if (process_type == "RAFA")		
		{
			m_inputdata.m_processingType = PROCESSING_TYPE_RAFA;
			m_inputdata.max_volume = configFileReader->GetValueAsDouble("MAX_VOLUME");
			m_inputdata.aspect_ratio = configFileReader->GetValueAsDouble("ASPECT_RATIO");
			m_inputdata.export_tetra = configFileReader->GetValueAsBool("EXPORT_TETRA");
		}
	else if (process_type == "CHIARA")
	{
		m_inputdata.m_processingType = PROCESSING_TYPE_CHIARA;
		
	}


	
	switch ( m_inputdata.m_processingType )
	{
	case PROCESSING_TYPE_CHECK:					Check( m_inputdata ); break;
	case PROCESSING_TYPE_COMPUTE_QUALITY:		ComputeQuality( m_inputdata ); break;
	case PROCESSING_TYPE_LOAD:					TestLoad( m_inputdata );break;
	case PROCESSING_TYPE_ALESSANDRO:			TestAlessandro( m_inputdata ); break;
	case PROCESSING_TYPE_RAFA:			        TestRafa( m_inputdata ); break;
	case PROCESSING_TYPE_CHIARA:			        TestChiara( m_inputdata ); break;
	default:									MeshSingleProcessing( m_inputdata ); break;
	}

  return 0;
}
