// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


//--------------------------
// HEADERS
//--------------------------

#include "cxxtest/TestSuite.h"
#include "CISTIBToolkit.h"

#include <iostream>
#include <fstream>

#include "meMesh.h"
#include "meMeshImplVTK.h"
#include "meMeshImplNetgen.h"
#include "meMeshImplNetgenSTL.h"
#include "meVTKPolyDataSplitter.h"

// BaseLib
#include "blShapeUtils.h"
#include "blClock.h"
#include "blTestParams.h"
#include "blTestParamsFolderInfo.h"

using namespace std;
using namespace blShapeUtils;

//--------------------------
// CONSTANTS & STRUCTURES
//--------------------------

/** 
 * \brief Type of test processing that will be performed
 * \ingroup MeshLibTest
 */
typedef enum PROCESSING_TYPE{
	/// Call Smooth( ) of type Netgen 
	PROCESSING_TYPE_SMOOTHING_NETGEN,
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
	PROCESSING_TYPE_EXTRACT_MAIN_CONNECTED_SURFACE
};


/** 
 * \brief Class of parameters for each test
 * \ingroup meTestMesh
 */
class meTestMeshParams : public blTestParams
{
	public:
		float		m_fMaxh;
		PROCESSING_TYPE m_processingType;

		meTestMeshParams(
			int iNumTest = -1,
			bool bEnableP = false,
			const char* strFileNameP = NULL, 
			const char* strExtension = NULL, 
			float fmaxh = 3,
			PROCESSING_TYPE processingType = PROCESSING_TYPE_SMOOTHING_NETGEN ) : 
		blTestParams( iNumTest, bEnableP, strFileNameP, strExtension )
		{
			m_fMaxh = fmaxh;
			m_processingType = processingType;
		}

};



//--------------------------
// GLOBAL INPUT DATA
//--------------------------

#define TOOLKIT_DATA_ROOT CISTIB_TOOLKIT_FOLDER "/Data/"
#define STR_PATH_TEMPORARY CISTIB_TOOLKIT_BUILD_FOLDER "/Temporary/"
#define STR_PATH_CORRECT_DATA_MESHLIB TOOLKIT_DATA_ROOT "/Tests/meshLib"

const char* STR_PATH_INPUT_DATA = TOOLKIT_DATA_ROOT;
const char* STR_LV_MY_O_OPT = "Cardio/Meshes/LV_Myo_opt.stl";
const char* STR_ANGIO_ANEU1_CUT = "Angio/Aneurisms/aneu_1_Cut.vtk";
const char* STR_CUBE = "Tests/MeshLib/Cube.vtk";
const char* STR_SPHERE = "Tests/MeshLib/Sphere.vtk";
const char* STR_SPHERE_WITH_TEXT = "Tests/MeshLib/SphereWithText.vtk";

const int MAX_INPUT_DATA = 128;
/** 
 * \brief Define the parameters that will be used for each test
 * \ingroup meTestMesh
 */
meTestMeshParams inputDataArray[ MAX_INPUT_DATA ] = {

	// T0
	meTestMeshParams( 0, 1, STR_LV_MY_O_OPT, ".vtk", 3, PROCESSING_TYPE_OPTIMIZATION ),

	// T1
	meTestMeshParams( 1, 1, STR_LV_MY_O_OPT, ".vtk", 3, PROCESSING_TYPE_SMOOTHING_NETGEN ),

	// T2
	meTestMeshParams( 2, 1, STR_LV_MY_O_OPT, ".vtk", 3, PROCESSING_TYPE_COPY ),

	// T3
	meTestMeshParams( 3, 1, STR_CUBE, ".vtk", 3, PROCESSING_TYPE_REFINEMENT ),

	// T4
	meTestMeshParams( 4, 1, STR_LV_MY_O_OPT, ".vtk", 3, PROCESSING_TYPE_ALESSANDRO ),

	// T5
	meTestMeshParams( 5, 1, STR_ANGIO_ANEU1_CUT, ".vtk", 3, PROCESSING_TYPE_VOLUME_CLOSING ),

	// T6
	meTestMeshParams( 6, 1, STR_LV_MY_O_OPT, ".vtk", 3, PROCESSING_TYPE_SELECT_CELLS ),

	// T7
	meTestMeshParams( 7, 1, STR_SPHERE, ".vtk", 3, PROCESSING_TYPE_EDGE_SWAPPING ),

	// T8
	meTestMeshParams( 8, 1, STR_SPHERE, ".vtk", 3, PROCESSING_TYPE_LOCAL_REFINER ),

	// T9
	meTestMeshParams( 9, 1, STR_SPHERE, ".vtk", 3, PROCESSING_TYPE_LOOP_SUBDIVISION_REFINER ),

	// T10
	meTestMeshParams( 10, 1, STR_SPHERE, ".vtk", 3, PROCESSING_TYPE_EDGE_SWAPPING_LOCAL ),

	// T11
	meTestMeshParams( 11, 1, STR_SPHERE, ".vtk", 3, PROCESSING_TYPE_SMOOTHING_TAUBIN_LOCAL ),
	
	// T12
	meTestMeshParams( 12, 1, STR_SPHERE, ".vtk", 3, PROCESSING_TYPE_SMOOTHING_UMBRELLA_LOCAL ),
	
	// T13
	meTestMeshParams( 13, 1, STR_SPHERE_WITH_TEXT, ".vtk", 3, PROCESSING_TYPE_EXTRACT_MAIN_CONNECTED_SURFACE ),

};




//---------------------------------------------------
// CLASS DEFINITION
//---------------------------------------------------
/** 
 * \ingroup meTestMesh
 */
class meTestMesh : public CxxTest::TestSuite 
{
public:

	/** 
	 */
	meTestMesh ( )
	{
		blTestParamsFolderInfo::Instance( )->Initialize(
				STR_PATH_INPUT_DATA,
				STR_PATH_TEMPORARY,
				STR_PATH_CORRECT_DATA_MESHLIB,
				"meTestMesh" );
	};

	void Test0( ){MeshProcessing( inputDataArray[ 0 ] );}
	void Test1( ){MeshProcessing( inputDataArray[ 1 ] );}
	void Test2( ){MeshProcessing( inputDataArray[ 2 ] );}
	void Test3( ){MeshProcessing( inputDataArray[ 3 ] );}
	void Test4( ){MeshProcessing( inputDataArray[ 4 ] );}
	void Test5( ){MeshProcessing( inputDataArray[ 5 ] );}
	void Test6( ){MeshProcessing( inputDataArray[ 6 ] );}
	void Test7( ){MeshProcessing( inputDataArray[ 7 ] );}
	void Test8( ){MeshProcessing( inputDataArray[ 8 ] );}
	void Test9( ){MeshProcessing( inputDataArray[ 9 ] );}
	void Test10( ){MeshProcessing( inputDataArray[ 10 ] );}
	void Test11( ){MeshProcessing( inputDataArray[ 11 ] );}
	void Test12( ){MeshProcessing( inputDataArray[ 12 ] );}
	void Test13( ){MeshProcessing( inputDataArray[ 13 ] );}

	void MeshProcessing( meTestMeshParams	inputData )
	{
		if ( !inputData.GetEnable( ) )
			return;

		switch ( inputData.m_processingType )
		{
		case PROCESSING_TYPE_COPY:			TestCopy( inputData ); break;
		case PROCESSING_TYPE_ALESSANDRO:	TestAlessandro( inputData ); break;
		default:					MeshSingleProcessing( inputData ); break;
		}
	}




	/**
	 * \brief Mesh processing test
	 * \ingroup meshTest
	 *
	 * - Load VTK shape
	 * - Taubin smooth with VTK 
	 * - Save VTK shape
	 *
	 */
	void TestAlessandro(	
							meTestMeshParams	inputData )
	{

		meMesh				*pMeshIn = new meMesh;
		meMesh				*pMeshOut = new meMesh;
		vtkPolyData*		vtkPolyDataIn;
		vtkPolyData*		vtkPolyDataOut;
		std::string			strOutFileName;


		vtkPolyDataOut		= vtkPolyData::New();

		// Load VTK shape
		vtkPolyDataIn = ShapeUtils::LoadShapeFromFile( 
							inputData.GetInputFileName().c_str() );

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
		strOutFileName = inputData.GetWorkingFileName( 0, "Alessandro" );
		ShapeUtils::SaveShapeToFile( vtkPolyDataOut, strOutFileName.c_str() );

		// Check results
		std::string strCorrectFileName;
		strCorrectFileName = inputData.GetCorrectFileName( 0, "Alessandro" );
		TS_ASSERT( ShapeUtils::CompareShapes(	
					strOutFileName.c_str(),
					strCorrectFileName.c_str( ) ) );


		// Delete memory
		delete pMeshIn;
		delete pMeshOut;
		vtkPolyDataIn->Delete( );
		vtkPolyDataOut->Delete( );

	}





	/**
	 * \brief Mesh processing test
	 * \ingroup meshTest
	 *
	 * - Load VTK shape
	 * - Optimize or smooth
	 * - Save VTK shape
	 *
	 */
	void MeshSingleProcessing(	meTestMeshParams	inputData )
	{

		meMesh				*pMeshIn = new meMesh;
		meMesh				*pMeshOut = new meMesh;
		vtkPolyData*		vtkPolyDataIn;
		vtkPolyData*		vtkPolyDataOut;
		std::string			strOutFileName;
		std::string			strModuleStep;

		vtkPolyDataOut		= vtkPolyData::New();


		//------------------------------------------
		// Load VTK shape
		vtkPolyDataIn = ShapeUtils::LoadShapeFromFile( 
							inputData.GetInputFileName( ).c_str() );

		// Insert VTK mesh data into Netgen mesh
		pMeshIn->SetData( vtkPolyDataIn );



		//------------------------------------------
		// Processing test
		switch ( inputData.m_processingType )
		{
		
		//------------------------------------------
		case PROCESSING_TYPE_OPTIMIZATION:
			{
			meMeshParamsOptimize	paramsOptimize;
			pMeshIn->Optimize( paramsOptimize, *pMeshOut );
			strModuleStep = "Optimize";
			}
			break;

		//------------------------------------------
		case PROCESSING_TYPE_SMOOTHING_NETGEN:
			{
			meMeshSmoothingParams params;
			params.m_Type = MESH_SMOOTHING_TYPE_NETGEN;
			pMeshIn->Smooth( params, *pMeshOut );
			strModuleStep = "Smooth";
			}
			break;

		//------------------------------------------
		case PROCESSING_TYPE_REFINEMENT:
			pMeshIn->Refine( *pMeshOut );
			strModuleStep = "Refine";
			break;

		//------------------------------------------
		case PROCESSING_TYPE_VOLUME_CLOSING:
			{
			meMeshVolumeClosingParams params;
			params.m_DensityFactor = 1.7;
			params.m_Order = 1;
			pMeshIn->VolumeClosing( params, *pMeshOut );
			strModuleStep = "VolumeClosing";
			}
			break;

		//------------------------------------------
		case PROCESSING_TYPE_SELECT_CELLS:
			{
			// Select cells
			pMeshIn->SelectCells( -100, 100, -100, 100, 0, 100 );
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

			strModuleStep = "SelectCells";
			}
			break;

		//------------------------------------------
		case PROCESSING_TYPE_EDGE_SWAPPING:
			{
			meMeshEdgeSwappingParams	params;
			pMeshIn->EdgeSwapping( params, *pMeshOut );
			strModuleStep = "EdgeSwapping";
			}
			break;

		//------------------------------------------
		case PROCESSING_TYPE_LOCAL_REFINER:
			{
			pMeshIn->SelectCells( -100, 100, -100, 100, 0, 100 );
			meMeshLocalRefinerParams	params;
			params.m_DensityFactor = 2.5;
			pMeshIn->LocalRefiner( params, *pMeshOut );
			strModuleStep = "LocalRefiner";
			}
			break;

		//------------------------------------------
		case PROCESSING_TYPE_LOOP_SUBDIVISION_REFINER:
			{
			meMeshLoopSubdivisionParams	params;
			pMeshIn->LoopSubdivisionRefiner( params, *pMeshOut );
			strModuleStep = "LoopSubdivisionRefiner";
			}
			break;

		//------------------------------------------
		case PROCESSING_TYPE_EDGE_SWAPPING_LOCAL: 
			{
			pMeshIn->SelectCells( -100, 100, -100, 100, 0, 100 );
			meMeshEdgeSwappingParams	params;
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
		}


		//------------------------------------------
		// Get the result mesh in a VTK shape
		pMeshOut->GetData( vtkPolyDataOut );
		strOutFileName = inputData.GetWorkingFileName( 0, strModuleStep.c_str() );
		ShapeUtils::SaveShapeToFile( vtkPolyDataOut, strOutFileName.c_str() );


		//------------------------------------------
		// Check results
		std::string strCorrectFileName;
		strCorrectFileName = inputData.GetCorrectFileName( 0, strModuleStep.c_str() );
		TS_ASSERT( ShapeUtils::CompareShapes(	
					strOutFileName.c_str(),
					strCorrectFileName.c_str( ) ) );



		//------------------------------------------
		// Delete memory
		delete pMeshIn;
		delete pMeshOut;
		vtkPolyDataIn->Delete( );
		vtkPolyDataOut->Delete( );

	}








	/**
	 * \brief Copy from source to dest. and save to a VTK shape file
	 */
	void CopyMesh(			meMeshImpl		*pMeshSrc, 
							meMeshImpl		*pMeshDst, 
							meTestMeshParams		inputData,
							std::string		strOperation )
	{

		vtkPolyData*		vtkPolyDataOut;
		vtkPolyDataOut		= vtkPolyData::New();
		blClock				clock;
		meMeshImpl			*pMeshVTK = new meMeshImplVTK; 

		clock.Start( );
		pMeshDst->Copy( *pMeshSrc );
		clock.Finish( );

		pMeshVTK->Copy( *pMeshDst );
		pMeshVTK->GetData( vtkPolyDataOut );
		std::string strOutFileName;
		strOutFileName = inputData.GetWorkingFileName( 0, strOperation.c_str() );
		ShapeUtils::SaveShapeToFile( vtkPolyDataOut, strOutFileName.c_str() );


		// Check results
		std::string strCorrectFileName;
		strCorrectFileName = inputData.GetCorrectFileName( 0, strOperation.c_str() );
		TS_ASSERT( ShapeUtils::CompareShapes(	
					strOutFileName.c_str(),
					strCorrectFileName.c_str( ) ) );


		vtkPolyDataOut->Delete( );

		cout.precision( 3 );
	}



	/**
	 * \brief Test function number 3
	 * \ingroup meshTest
	 *
	 * Copy each implementation of mesh to the other and save to a VTK shape 
	 * file
	 *
	 * Verify results: Each VTK shape file must be the same size
	 *
	 */
	void TestCopy( meTestMeshParams	inputData )
	{
		meMeshImpl			*pMeshVtk = new meMeshImplVTK;
		meMeshImpl			*pMeshNetgen = new meMeshImplNetgen;
		meMeshImpl			*pMeshNetgenSTL = new meMeshImplNetgenSTL;
		vtkPolyData*		vtkPolyDataIn;


		// Load VTK shape
		vtkPolyDataIn = ShapeUtils::LoadShapeFromFile( inputData.GetInputFileName().c_str() );


		// VTK
		pMeshVtk->SetData( vtkPolyDataIn );
		CopyMesh( pMeshVtk, pMeshVtk, inputData, "_VTK_VTK" );
		CopyMesh( pMeshVtk, pMeshNetgen, inputData, "_VTK_Netgen" );
		CopyMesh( pMeshVtk, pMeshNetgenSTL, inputData, "_VTK_NetgenSTL" );



		// Netgen
		CopyMesh( pMeshNetgen, pMeshVtk, inputData, "_Netgen_VTK" );
		CopyMesh( pMeshNetgen, pMeshNetgen, inputData, "_Netgen_Netgen" );
		CopyMesh( pMeshNetgen, pMeshNetgenSTL, inputData, "_Netgen_NetgenSTL" );



		// Netgen STL
		CopyMesh( pMeshNetgenSTL, pMeshVtk, inputData, "_NetgenSTL_VTK" );
		CopyMesh( pMeshNetgenSTL, pMeshNetgen, inputData, "_NetgenSTL_Netgen" );
		CopyMesh( pMeshNetgenSTL, pMeshNetgenSTL, inputData, "_NetgenSTL_NetgenSTL" );


		// Delete memory
		delete pMeshVtk;
		delete pMeshNetgen;
		delete pMeshNetgenSTL;
		vtkPolyDataIn->Delete( );
	}


};

