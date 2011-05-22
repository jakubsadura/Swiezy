/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "vtkSphereSource.h"

#include "wxMitkTestRenderingTreeWidget.h"
#include "wxMitkMultiRenderWindow.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkDisplayPlaneSubtreeHelper.h"
#include "wxMitkTransferFunctionWidget.h"
#include "wxMitkMaterialEditorWidget.h"
#include "wxMitkSphereViewCanvas.h"
#include "wxMitkMultiRenderWindowConfig.h"
#include "wxMitkOrthoSliceFactory.h"
#include "wxMitkMultiSliceFactory.h"

#include "mitkDataTreeNodeFactory.h"
#include "mitkRenderingManager.h"
#include "mitkProperties.h"
#include "mitkGlobalInteraction.h"
#include "mitkSliceNavigationController.h"
#include "mitkCameraRotationController.h"
#include "mitkGeometry2DDataMapper2D.h"
#include "mitkDataStorage.h"
#include "mitkDataTree.h"
#include "mitkStateMachineFactory.h"
#include "mitkUnstructuredGrid.h"
#include "mitkGridRepresentationProperty.h"
#include "mitkGridVolumeMapperProperty.h"

#include <string>

#include "wx/combobox.h"
#include "wx/notebook.h"
#include "wx/font.h"

#include "blShapeUtils.h"

#include "vtkStringArray.h"
#include "vtkPointData.h"

#include <itkImage.h>

//"E:/CILAB/Data/SegmentationUserStory/InputData/demo-CT-00.vtk";//
//"E:/CILAB/gimias_os_svn/thirdParty/ITK-3.8/Insight/Testing/Data/Input/HeadMRVolumeWithDirection.mhd"
//"E:/CILAB/gimias_os_svn/thirdParty/MITK-0.9/mitk/Testing/Data/Png2D-bw.png";
//"dcha250pre_crop2.vtk";
const std::string INPUT_FILENAME_IMAGE = "I:/Data/OrientationCube.vtk";
const std::string INPUT_FILENAME_MESH = "/Data/Tests/CardiacSegmentationPlugin/CardiacCorrections.T00.Out_00.vtk";
const std::string INPUT_FILENAME_VOL_MESH = "/Data/Misc/Meshes/tetgenoutcubeScalars.vtk";
//const std::string INPUT_FILENAME_VOL_MESH = "C:/Temp/temp.vtk";

const std::string STR_LABEL_VECTOR_NAME = "NAMES";
#define ID_wxMitkMultiRenderWindow (01 + wxID_HIGHEST)

using namespace mitk;

/**
*/
wxMitkTestRenderingTreeWidget::wxMitkTestRenderingTreeWidget(
	wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name)
	:wxPanel(parent, id, pos, size, style, name)
{
	m_renderWindow = NULL;
	m_transferFunctionWidget = NULL;
	m_sphereViewCanvas = NULL;
	//m_wxMitkDataTreeListView = NULL;
	m_wxMitkMaterialEditorWidget = NULL;
	m_MeshForInteractor = NULL;
	m_sphere = NULL;
	m_viewConfiguration = NULL;
}

/**
*/
wxMitkTestRenderingTreeWidget::~wxMitkTestRenderingTreeWidget( )
{
	// We need to detach the tree 
	//m_wxMitkDataTreeListView->SetDataTree( (mitk::DataTree*) NULL );

	//m_renderingTree = NULL;
	m_ImageNode = NULL;
	m_SurfaceNode = NULL;
	m_PointSetNode = NULL;
	m_VolumeMeshNode = NULL;
	if ( m_sphere )
	{
		m_sphere->Delete();
		m_sphere = NULL;
	}

	mitk::GlobalInteraction::GetInstance()->RemoveInteractor( 
		m_blPointSelectInteractor );
	m_blPointSelectInteractor = NULL;

	if ( m_MeshForInteractor ){
		m_MeshForInteractor->Delete();
		m_MeshForInteractor = NULL;
	}
	m_PointSet = NULL;


	// When the main window is closed, all child windows will be 
	// destroyed automatically
	m_renderWindow = NULL;
	m_transferFunctionWidget = NULL;
	m_sphereViewCanvas = NULL;
	//m_wxMitkDataTreeListView = NULL;
}

/**
 */
bool wxMitkTestRenderingTreeWidget::Initialize( 
	wxMitkTestRenderingTreeWidgetParams &params )
{
	if ( params.m_bEnableMultiRenderWindow )
	{
		CreatewxMitkMultiRenderWindow( params );
	}

	CreateRenderingTree( );

	if ( params.m_inputType & wxMitkTestRenderingTreeWidgetParams::INPUT_TYPE_IMAGE )
	{
		CreateImageNode( params.m_strDataPath + INPUT_FILENAME_IMAGE );

		if ( params.m_bEnableVolumeRendering )
		{
			EnableVolumeRendering( );
		}
	}

	if ( params.m_inputType & wxMitkTestRenderingTreeWidgetParams::INPUT_TYPE_MESH )
	{
		CreateSurfaceNode( params.m_strDataPath + INPUT_FILENAME_MESH );

	}

	if ( params.m_inputType & wxMitkTestRenderingTreeWidgetParams::INPUT_TYPE_VOLUME_MESH )
	{
		CreateVolumeMeshNode( params.m_strDataPath + INPUT_FILENAME_VOL_MESH );
	}

	CreateInteractor( params );

	UpdateRenderWindowTree( );

	CreateViewConfiguration( );

	ComposeWindows( );

	return true;
}

/**
 */
void wxMitkTestRenderingTreeWidget::CreatewxMitkMultiRenderWindow( 
	wxMitkTestRenderingTreeWidgetParams &params )
{

	// Test 1: Create a renderwindow 
	//m_renderWindow = new mitk::wxMitkMultiRenderWindow(
	//	this, ID_wxMitkMultiRenderWindow, wxDefaultPosition, wxSize(400, 400));

	//m_renderWindow = wxMitkOrthoSliceFactory::Build( 
	//	this, ID_wxMitkMultiRenderWindow, wxDefaultPosition, wxSize(400, 400));

	m_renderWindow = wxMitkMultiSliceFactory::Build( 
		this, 
		ID_wxMitkMultiRenderWindow, 
		mitk::SliceNavigationController::Sagittal,
		8,
		wxDefaultPosition, 
		wxSize(400, 400));

	m_renderWindow->EnableNavigationController(true);
	m_renderWindow->EnableDepartmentLogo(false);
	m_renderWindow->EnableAnnotatedCube(false);
	m_renderWindow->SetCurrentLayout( params.m_layoutConfiguration );

	//// Test 2: Create a wxMitkSelectableGLWidget
	//wxMitkSelectableGLWidget* rwinX = 
	//	new mitk::wxMitkSelectableGLWidget(mainWindow, ID_wxMitkMultiRenderWindow, wxDefaultPosition, wxSize(200, 200));
	//
	//// Test 3: Create a wxMitkRenderWindow
	//wxMitkRenderWindow* internalRenderWindow = 
	//	new mitk::wxMitkRenderWindow(mainWindow, ID_wxMitkMultiRenderWindow, NULL, wxDefaultPosition, wxSize(200, 200));

	//// Test 4: Create vtkRenderWindow and mitk::VtkPropRenderer
	//vtkRenderWindow* win = vtkRenderWindow::New();
	//mitk::VtkPropRenderer * renderer = new mitk::VtkPropRenderer("ContourRenderer", win );
	//win->Delete();
	//renderer->Delete();

	//// Test 5: Create mitk::GlobalInteraction
	//mitk::GlobalInteraction::GetInstance();

	//// Test 6: Create mitk::StateMachineFactory and call LoadStandardBehavior( )
	//mitk::StateMachineFactory::LoadStandardBehavior();

}

/**
 */
void wxMitkTestRenderingTreeWidget::CreateRenderingTree( )
{
    try
    {
		// Basic initialization: create a tree
		//m_renderingTree = mitk::DataTree::New();

		// We need it to use LevelWindow
		m_dataStorage = mitk::StandaloneDataStorage::New();

		//m_wxMitkDataTreeListView = new 
		//	mitk::wxMitkDataTreeListView( m_renderingTree, this, wxID_ANY );
		//m_wxMitkDataTreeListView->SetSize( 200, 200 );

	}
    catch ( itk::ExceptionObject & ex )
    {
      std::cerr << "Exception: " << ex << "[FAILED]" << std::endl;
      return;
    }
}

/**
 */
void wxMitkTestRenderingTreeWidget::CreateImageNode( const std::string &inputFilename )
{
    try
    {

		// Create some data by reading a file
		mitk::DataTreeNodeFactory::Pointer nodeReader = mitk::DataTreeNodeFactory::New();
		nodeReader->SetFileName(inputFilename.c_str());
		nodeReader->Update();
		if(nodeReader->GetNumberOfOutputs()<1)
		{
			std::cerr<<"File could not be loaded [FAILED]"<<std::endl;
			return;
		}

		m_ImageNode = nodeReader->GetOutput();
		mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(m_ImageNode->GetData());
		if(image.IsNull())
		{
			std::cout<<"File not an image - test will not be applied [PASSED]"<<std::endl;
			std::cout<<"[TEST DONE]"<<std::endl;
			return;
		}

		//m_ImageNode->SetProperty("use color", new mitk::BoolProperty(false));

		// create an iterator on the tree
		m_dataStorage->Add( m_ImageNode );

	}
    catch ( itk::ExceptionObject & ex )
    {
      std::cerr << "Exception: " << ex << "[FAILED]" << std::endl;
      return;
    }
}


/**
*/
void wxMitkTestRenderingTreeWidget::EnableVolumeRendering( )
{
	try
	{
		// set the property "volumerendering" to the Boolean value "true"
		m_ImageNode->SetProperty("volumerendering", mitk::BoolProperty::New(true));
		m_transferFunctionWidget = new mitk::wxMitkTransferFunctionWidget( this, wxID_ANY );
		m_transferFunctionWidget->SetDataTreeNode( m_ImageNode );
		m_transferFunctionWidget->SetMitkRenderWindow( 
			m_renderWindow->Get3D()->GetMitkRenderWindow() );

	}
	catch ( itk::ExceptionObject & ex )
	{
		std::cerr << "Exception: " << ex << "[FAILED]" << std::endl;
		return;
	}
}

/**
 */
void wxMitkTestRenderingTreeWidget::CreateAdditional3DView( )
{
    try
    {
		// Test adding another 3D view
		m_sphereViewCanvas = new mitk::wxMitkSphereViewCanvas( this, wxID_ANY );

	}
    catch ( itk::ExceptionObject & ex )
    {
      std::cerr << "Exception: " << ex << "[FAILED]" << std::endl;
      return;
    }
}

/**
 */
void wxMitkTestRenderingTreeWidget::CreateSurfaceNode( const std::string &inputFilename )
{
    try
    {

		RemoveSurfaceNode();

		// Create some data by reading a file
		mitk::DataTreeNodeFactory::Pointer nodeReader = mitk::DataTreeNodeFactory::New();
		nodeReader->SetFileName(inputFilename.c_str());
		nodeReader->Update();
		if(nodeReader->GetNumberOfOutputs()<1)
		{
			std::cerr<<"File could not be loaded [FAILED]"<<std::endl;
			return;
		}

		m_SurfaceNode = nodeReader->GetOutput();
		mitk::Surface::Pointer surface = dynamic_cast<mitk::Surface*>(m_SurfaceNode->GetData());
		if(surface.IsNull())
		{
			std::cout<<"File not a surface - test will not be applied [PASSED]"<<std::endl;
			std::cout<<"[TEST DONE]"<<std::endl;
			return;
		}

		// 
		m_dataStorage->Add( m_SurfaceNode );
		

		if ( m_wxMitkMaterialEditorWidget == NULL )
		{
			m_wxMitkMaterialEditorWidget = new mitk::wxMitkMaterialEditorWidget( this, wxID_ANY );
		}
		m_wxMitkMaterialEditorWidget->SetMitkRenderWindow( m_renderWindow->Get3D()->GetMitkRenderWindow( ) );
		m_wxMitkMaterialEditorWidget->SetDataTreeNode( m_SurfaceNode );
		m_wxMitkMaterialEditorWidget->SetSize( 200, 200 );
	}
    catch ( itk::ExceptionObject & ex )
    {
      std::cerr << "Exception: " << ex << "[FAILED]" << std::endl;
      return;
    }
}

void mitk::wxMitkTestRenderingTreeWidget::RemoveSurfaceNode()
{
	if ( m_SurfaceNode.IsNotNull() )
	{
		m_dataStorage->Remove( m_SurfaceNode );
		m_wxMitkMaterialEditorWidget->SetDataTreeNode( NULL );
	}

	m_SurfaceNode = NULL;
}


/**
 */
void wxMitkTestRenderingTreeWidget::ComposeWindows( )
{
	// Add all windows into the main window and show it
	wxBoxSizer* layout = new wxBoxSizer(wxHORIZONTAL);
	if ( m_renderWindow )
	{
		m_renderWindow->SetMinSize( wxSize( 400, 400 ) );
		layout->Add( m_renderWindow, 1, wxEXPAND);
	}
	if ( m_transferFunctionWidget )
	{
		layout->Add(m_transferFunctionWidget);
	}
	if ( m_wxMitkMaterialEditorWidget )
	{
		layout->Add(m_wxMitkMaterialEditorWidget);
	}
	//if ( m_wxMitkDataTreeListView )
	//{
	//	layout->Add(m_wxMitkDataTreeListView);
	//}
	if ( m_viewConfiguration )
	{
		layout->Add(m_viewConfiguration, 0, wxEXPAND);
	}
	SetSizer(layout);
	SetSize( 1024, 600 );
}

/**
 */
void wxMitkTestRenderingTreeWidget::UpdateRenderWindowTree( )
{
	try {

		// Update render render window
		if ( m_renderWindow )
		{
			m_renderWindow->SetDataStorage(m_dataStorage);
			m_renderWindow->UpdateViewControllers( );

			mitk::wxMitkDisplayPlaneSubtreeHelper::AddDisplayPlaneSubTree( m_renderWindow );

			if ( m_ImageNode.IsNotNull() )
			{
				m_renderWindow->EnableStandardLevelWindow(true);
			}
		}
	}
	catch ( ... )
	{
		std::cerr << "[wxMitkTestRenderingTreeWidget::UpdateRenderWindowTree] "
			<< "Exception caught"
			<< std::endl;
	}

}

/**
*/
void wxMitkTestRenderingTreeWidget::TestTextOutput()
{
	// Create a sphere and its associated mapper and actor.
	m_sphere = vtkSphereSource::New();
	m_sphere->Update();
	vtkPolyData	*sphere = m_sphere->GetOutput();

	vtkStringArray *stringData = vtkStringArray::New();
	stringData->SetName( STR_LABEL_VECTOR_NAME.c_str() );
	for ( int i = 0 ; i < sphere->GetNumberOfPoints() ; i++ )
	{
		char cName[ 16 ];
		sprintf( cName, "Point %d", i );
		stringData->InsertValue( i, cName );
	}
	sphere->GetPointData()->AddArray(stringData);

}

/**
*/
void wxMitkTestRenderingTreeWidget::CreateInteractor( 
	wxMitkTestRenderingTreeWidgetParams &params )
{
	if ( params.m_pointInteractorType == 
		wxMitkTestRenderingTreeWidgetParams::POINT_INTERACTOR_TYPE_NONE )
	{
		return;
	}


	try
	{
		// Create a node to put the selected point
		m_PointSet = mitk::PointSet::New();
		m_PointSetNode = mitk::DataTreeNode::New( );
		m_PointSetNode->SetData( m_PointSet );
		m_PointSetNode->SetName("Selected point");
		//m_PointSetNode->SetProperty( "label", new mitk::StringProperty("Point") );


		// Add to the tree
		m_dataStorage->Add( m_PointSetNode );

		// Create the interactor
		switch ( params.m_pointInteractorType )
		{
		case wxMitkTestRenderingTreeWidgetParams::POINT_INTERACTOR_TYPE_SELECTED_AT_RUNTIME:
		{
			// Read a vtkPolyData
			mitk::Interactor* rawPtr = dynamic_cast<mitk::Interactor*>(
				m_CreatePointInteractorCallback(m_PointSetNode).GetPointer());
			m_blPointSelectInteractor = itk::SmartPointer<mitk::Interactor>(rawPtr);
			mitk::GlobalInteraction::GetInstance()->AddInteractor(m_blPointSelectInteractor );
		}
		break;
		case wxMitkTestRenderingTreeWidgetParams::POINT_INTERACTOR_TYPE_SELECT:
			{
				mitk::Surface::Pointer surface = mitk::Surface::New();
				surface->SetVtkPolyData( m_MeshForInteractor );
				mitk::DataTreeNode::Pointer node = mitk::DataTreeNode::New();
				node->SetData( surface );
				// Read a vtkPolyData
				m_blPointSelectInteractor = blMitkPointSelectInteractor::New(
					"PointSelectInteractor", m_PointSetNode, node );
				mitk::GlobalInteraction::GetInstance()->AddInteractor( m_blPointSelectInteractor );
			}
			break;
		case wxMitkTestRenderingTreeWidgetParams::POINT_INTERACTOR_TYPE_SET:
			{
				m_blPointSetInteractor = blMitkPointSetInteractor::New(
					"pointsetinteractorOneClick", m_PointSetNode, -1 );
				mitk::Surface::Pointer surface = mitk::Surface::New();
				surface->SetVtkPolyData( m_MeshForInteractor );
				mitk::DataTreeNode::Pointer node = mitk::DataTreeNode::New();
				node->SetData( surface );
				m_blPointSetInteractor->SetRestrictAddPointPosition( true, node );
				m_blPointSetInteractor->SetIncrementalAdd( true );
				mitk::GlobalInteraction::GetInstance()->AddInteractor( m_blPointSetInteractor );
			}
			break;
		}

		// Add an observer to test it
		itk::SimpleMemberCommand<wxMitkTestRenderingTreeWidget>::Pointer command;
		command = itk::SimpleMemberCommand<wxMitkTestRenderingTreeWidget>::New();
		command->SetCallbackFunction(this, &wxMitkTestRenderingTreeWidget::ModifiedInteractorPoints);
		m_PointSet->AddObserver( mitk::PointSetAddEvent(), command );
	}
	catch ( itk::ExceptionObject & ex )
	{
		std::cerr << "Exception: " << ex << "[FAILED]" << std::endl;
		return;
	}
}

/**
*/
void wxMitkTestRenderingTreeWidget::ModifiedInteractorPoints( )
{
	// Test that the interactor still has a copy of this and
	// will not be destroyed
	if ( m_MeshForInteractor )
	{
		m_MeshForInteractor->Delete();
		m_MeshForInteractor = NULL;
	}
	m_MeshForInteractor = blShapeUtils::ShapeUtils::LoadShapeFromFile( 
		INPUT_FILENAME_MESH.c_str() );
}

void wxMitkTestRenderingTreeWidget::SetCallback( CallbackType val )
{
	this->m_CreatePointInteractorCallback = val;
}

void mitk::wxMitkTestRenderingTreeWidget::CreateVolumeMeshNode( const std::string &inputFilename )
{
	try
	{

		// Create some data by reading a file
		mitk::DataTreeNodeFactory::Pointer nodeReader = mitk::DataTreeNodeFactory::New();
		nodeReader->SetFileName(inputFilename.c_str());
		nodeReader->Update();
		if(nodeReader->GetNumberOfOutputs()<1)
		{
			std::cerr<<"File could not be loaded "<<std::endl;
			return;
		}

		m_VolumeMeshNode = nodeReader->GetOutput();
		mitk::UnstructuredGrid::Pointer volume = dynamic_cast<mitk::UnstructuredGrid*>(m_VolumeMeshNode->GetData());
		if(volume.IsNull())
		{
			std::cout<<"File not a volume "<<std::endl;
			return;
		}

		// SURFACE
		m_VolumeMeshNode->SetProperty( "scalar visibility", mitk::BoolProperty::New(false) );
		m_VolumeMeshNode->SetProperty( "grid representation", 
			mitk::GridRepresentationProperty::New( mitk::GridRepresentationProperty::SURFACE ) );
		m_VolumeMeshNode->SetProperty( "outline polygons", mitk::BoolProperty::New(true) );

		// VOLUME
		//m_VolumeMeshNode->ReplaceProperty( "scalar visibility", new mitk::BoolProperty(true) );
		//mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
		//float m_Min = volume->GetVtkUnstructuredGrid( )->GetScalarRange()[ 0 ];
		//float m_Max = volume->GetVtkUnstructuredGrid( )->GetScalarRange()[ 1 ];
		//tf->GetScalarOpacityFunction()->Initialize();
		//tf->GetScalarOpacityFunction()->AddPoint ( m_Min, 0 );
		//tf->GetScalarOpacityFunction()->AddPoint ( m_Max, 1 );
		//tf->GetColorTransferFunction()->AddRGBPoint(m_Min,1,0,0);
		//tf->GetColorTransferFunction()->AddRGBPoint(m_Max,1,1,0);
		//tf->GetGradientOpacityFunction()->Initialize();
		//tf->GetGradientOpacityFunction()->AddPoint(m_Min,1.0);
		//tf->GetGradientOpacityFunction()->AddPoint(0.0,1.0);
		//tf->GetGradientOpacityFunction()->AddPoint((m_Max*0.25),1.0);
		//tf->GetGradientOpacityFunction()->AddPoint(m_Max,1.0);  
		//m_VolumeMeshNode->SetProperty ( "TransferFunction", new mitk::TransferFunctionProperty ( tf.GetPointer() ) );
		//m_VolumeMeshNode->ReplaceProperty( "grid volume mapper", 
		//	new mitk::GridVolumeMapperProperty( mitk::GridVolumeMapperProperty::RAYCAST ) );
		
		

		// 
		m_dataStorage->Add( m_VolumeMeshNode );
	}
	catch ( itk::ExceptionObject & ex )
	{
		std::cerr << "Exception: " << ex << "[FAILED]" << std::endl;
		return;
	}
}

void mitk::wxMitkTestRenderingTreeWidget::CreateViewConfiguration( )
{
	m_viewConfiguration = new wxMitkMultiRenderWindowConfig( this, wxID_ANY );
	m_viewConfiguration->SetView( m_renderWindow );
	m_viewConfiguration->SetMinSize( wxSize( 250, 600 ) );

}

