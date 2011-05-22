/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wx/wupdlock.h"

#include "coreMultiRenderWindowMITK.h"
#include "coreDataEntity.h"
#include "coreAssert.h"
#include "coreKernel.h"
#include "coreSettings.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreWxMitkCoreMainWindow.h"
#include "coreDataEntityInfoHelper.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreMultiRenderWindowEventHandler.h"

#include "mitkDataTree.h"
#include "mitkDataTreeHelper.h"
#include "mitkRenderingManager.h"
#include "mitkLevelWindowManager.h"

#include "wxMitkDisplayPlaneSubtreeHelper.h"
#include "wxEventHandlerHelper.h"
#include "wxMitkSelectableGLWidget.h"
#include "wxMitkLevelWindowWidget.h"
#include "wxMitkOrthoSliceFactory.h"

#include "vtkCamera.h"

using namespace Core::Widgets;

//!
MultiRenderWindowMITK::MultiRenderWindowMITK(
	wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name)
: wxMitkMultiRenderWindow(parent, id, pos, size, style, name)
{
	// Setup the logo
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	coreAssertMacro(settings.IsNotNull() && "The Settings Manager must have been initialized");
	
	// Assign the logo
	EnableDepartmentLogo(false);
	std::string logoFileFullPath = settings->GetCorpLogoFileName();
	SetDepartmentLogoPath(logoFileFullPath);
	EnableDepartmentLogo(true);

	m_MultiRenderWindowFocus = MultiRenderWindowFocus::New();

	m_MultiRenderWindowOverlay = MultiRenderWindowOverlay::New( );

	Core::RenderingTreeMITK::Pointer tree = Core::RenderingTreeMITK::New();
	SetPrivateRenderingTree( tree.GetPointer() );

	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();
	m_MultiRenderWindowEventHandler = new MultiRenderWindowEventHandler( this );
	gIface->GetMainWindow( )->PushEventHandler( m_MultiRenderWindowEventHandler );

	GetMetadata()->AddTag( "AxisLocked", false );
	GetMetadata()->AddTag( "LinkPlanesEnabled", true );
	GetMetadata()->AddTag( "AnnotationCubeShown", false );
	GetMetadata()->AddTag( "LayoutType", int( mitk::Default2x2 ) );
	GetMetadata()->AddTag( "SlicesPlanesEnabled", true );
	GetMetadata()->AddTag( "CornerAnnotationsShown", false );
	GetMetadata()->AddTag( "LevelWindowInteractor", false );

	m_vtkCameraCallback = vtkSmartPointer<vtkCameraCallback>::New();
	m_vtkCameraCallback->m_MultiRenderWindowMITK = this;
}

//!
MultiRenderWindowMITK::~MultiRenderWindowMITK(void)
{
	SetPrivateRenderingTree( NULL );

	m_MultiRenderWindowFocus = NULL;
	m_MultiRenderWindowOverlay = NULL;

	if ( m_MultiRenderWindowEventHandler )
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
		gIface = Core::Runtime::Kernel::GetGraphicalInterface();
		wxPopEventHandler( gIface->GetMainWindow( ), m_MultiRenderWindowEventHandler );
		delete m_MultiRenderWindowEventHandler;
	}
}

//!
void MultiRenderWindowMITK::Init(
	Core::DataEntityHolder::Pointer selectedDataEntity,
	bool enablePlanes)
{
	wxMitkMultiRenderWindow::SetDataStorage( m_RenderingTree->GetDataStorage() );

	// Prepare planes for the first rendered object
	GetMetadata( )->AddTag( "SlicesPlanesEnabled", enablePlanes );
	SetPlaneRotationLinked( GetMetadata( )->GetTagValue<bool>( "LinkPlanesEnabled" ) );
	EnableAxis( !GetMetadata( )->GetTagValue<bool>( "AxisLocked" ) );
	EnableAnnotatedCube( GetMetadata( )->GetTagValue<bool>( "AnnotationCubeShown" ) );

	m_MultiRenderWindowFocus->Init( this, selectedDataEntity );

	m_MultiRenderWindowOverlay->Init( this, selectedDataEntity );

	// Get camera of the first 3D render window
	if ( !Get3D( ) )
	{
		throw Core::Exceptions::Exception( 
			"MultiRenderWindowMITK::Init", 
			"MITK render window cannot be initialized without a 3D view" );
	}

	vtkCamera* camera = Get3D( )->GetRenderer( )->GetVtkRenderer()->GetActiveCamera(  );
	camera->AddObserver( vtkCommand::ModifiedEvent, m_vtkCameraCallback);

	camera->Azimuth( -20 );
	camera->Elevation( -20 );
	camera->OrthogonalizeViewUp();
}

//!
void MultiRenderWindowMITK::RenderScene(void)
{
	wxWindowUpdateLocker locker( this );

	// Disable logo
	if ( wxMitkMultiRenderWindow::IsDepartmentLogoEnabled( ) )
	{
		wxMitkMultiRenderWindow::EnableDepartmentLogo( false );
		// We need to disable it if it was enabled, otherwise it will not work
		wxMitkMultiRenderWindow::EnableAnnotatedCube( false );
	}
	// Enable 3D cube
	wxMitkMultiRenderWindow::EnableAnnotatedCube(
		GetMetadata( )->GetTagValue<bool>( "AnnotationCubeShown" ) );

	// initialize views as transversal, sagittal, coronal (from top-left to bottom)
	// Initialize should be called last, because computes bounding boxes of the full scene and fits it
	wxMitkMultiRenderWindow::UpdateViewControllers( );

	// Enable the display of the level/window bar on the screen.
	// It should be called before AddDisplayPlaneSubTree(), because otherwise a MITK bug is triggered
	bool treeHasVolumeImage = m_RenderingTree->HasDataEntityOfType(Core::ImageTypeId);
	EnableStandardLevelWindow( treeHasVolumeImage );
	// We always display plane subtrees
	if ( GetMetadata( )->GetTagValue<bool>( "SlicesPlanesEnabled" ) )
	{
		EnableDisplayPlaneSubtree( true );
	}

	// Update the rendering
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void MultiRenderWindowMITK::EnableDisplayPlaneSubtree( bool enable )
{
	if( enable != mitk::wxMitkDisplayPlaneSubtreeHelper::HasDisplayPlaneSubTree( 
		m_RenderingTree->GetDataStorage() ) )
	{
		if ( enable )
		{
			mitk::wxMitkDisplayPlaneSubtreeHelper::AddDisplayPlaneSubTree( this );
		}
		else
		{
			mitk::wxMitkDisplayPlaneSubtreeHelper::RemoveDisplayPlaneSubTree( 
				m_RenderingTree->GetDataStorage() );
		}

		// Notify observers that the planes has been added
		//m_RenderingTree->Modified( false );

		// Update the rendering
		mitk::RenderingManager::GetInstance()->RequestUpdateAll();
	}

	if ( enable != GetMetadata( )->GetTagValue<bool>( "SlicesPlanesEnabled" ) )
	{
		GetMetadata( )->AddTag( "SlicesPlanesEnabled", enable );
		GetMetadataHolder( )->NotifyObservers( );
	}
}

void MultiRenderWindowMITK::ChangeColorDisplayPlaneSubtree( bool bEnable )
{
	if ( m_RenderingTree.IsNull( ) )
	{
		return;
	}

	mitk::wxMitkDisplayPlaneSubtreeHelper::ChangeColorToDisplayPlanes( 
		bEnable, this );

	m_RenderingTree->Modified( );
}

void Core::Widgets::MultiRenderWindowMITK::EnableAxis( bool enable )
{
	EnableNavigationController( enable );
	EnableMoveAndZoomInteractor( enable );
	ChangeColorDisplayPlaneSubtree( enable );

	if ( GetMetadata( )->GetTagValue<bool>( "AxisLocked" ) == enable )
	{
		GetMetadata()->AddTag( "AxisLocked", !enable );
		GetMetadataHolder( )->NotifyObservers( );
	}
}

void Core::Widgets::MultiRenderWindowMITK::SetPlaneRotationLinked( bool enable )
{
	mitk::wxMitkMultiRenderWindow::SetPlaneRotationLinked( enable );

	if ( GetMetadata( )->GetTagValue<bool>( "LinkPlanesEnabled" ) != enable )
	{
		GetMetadata()->AddTag( "LinkPlanesEnabled", enable );
		GetMetadataHolder( )->NotifyObservers( );
	}
}

void Core::Widgets::MultiRenderWindowMITK::EnableAnnotatedCube(bool enable)
{
	mitk::wxMitkMultiRenderWindow::EnableAnnotatedCube( enable );

	if ( GetMetadata( )->GetTagValue<bool>( "AnnotationCubeShown" ) != enable )
	{
		GetMetadata()->AddTag( "AnnotationCubeShown", enable );
		GetMetadataHolder( )->NotifyObservers( );
	}
}

void Core::Widgets::MultiRenderWindowMITK::SetCurrentLayout( 
	mitk::LayoutConfiguration layout,
	mitk::wxMitkSelectableGLWidget* widget/* = NULL*/ )
{
	mitk::wxMitkMultiRenderWindow::SetCurrentLayout( layout, widget );
	
	mitk::LayoutConfiguration oldlayout;
	oldlayout = mitk::LayoutConfiguration( GetMetadata( )->GetTagValue<int>( "LayoutType" ) );
	if ( layout != oldlayout )
	{
		GetMetadata()->AddTag( "LayoutType", int( GetLayout()->GetCurrentLayout() ) );
		GetMetadataHolder( )->NotifyObservers();
	}
}

Core::RenderingTree::Pointer Core::Widgets::MultiRenderWindowMITK::GetPrivateRenderingTree() const
{
	return m_RenderingTree.GetPointer();
}

void Core::Widgets::MultiRenderWindowMITK::EnableCornerAnnotations( bool enable )
{
	mitk::wxMitkMultiRenderWindow::EnableCornerAnnotations( enable );

	if ( GetMetadata( )->GetTagValue<bool>( "CornerAnnotationsShown" ) != enable )
	{
		GetMetadata()->AddTag( "CornerAnnotationsShown", enable );
		GetMetadataHolder( )->NotifyObservers( );
	}
}

void Core::Widgets::MultiRenderWindowMITK::EnableWindowLevelInteractor( bool enable /*= true*/ )
{
	wxMitkMultiRenderWindow::EnableWindowLevelInteractor( enable );

	if ( GetMetadata( )->GetTagValue<bool>( "LevelWindowInteractor" ) != enable )
	{
		GetMetadata()->AddTag( "LevelWindowInteractor", enable );
		GetMetadataHolder( )->NotifyObservers( );
	}
}

bool Core::Widgets::MultiRenderWindowMITK::Enable( bool enable /*= true */ )
{
	bool retVal = wxMitkMultiRenderWindow::Enable( enable );

	// Restore state
	if ( enable )
	{
		EnableNavigationController( !GetMetadata( )->GetTagValue<bool>( "AxisLocked" ) );
		EnableMoveAndZoomInteractor( !GetMetadata( )->GetTagValue<bool>( "AxisLocked" ) );
	}

	return retVal;
}

void Core::Widgets::MultiRenderWindowMITK::GetProperties( blTagMap::Pointer tagMap )
{
	std::string windowName = GetName().c_str();
	tagMap->AddTag( "Name", windowName );
	int layout = GetLayout()->GetCurrentLayout();
	tagMap->AddTag( "Layout", layout );
	bool isSlicesPlanesEnabled = GetMetadata( )->GetTagValue<bool>( "SlicesPlanesEnabled" );
	tagMap->AddTag( "EnableDisplayPlaneSubtree", isSlicesPlanesEnabled );
	tagMap->AddTag( "LevelWindowInteractor", 
		GetMetadata( )->GetTagValue<bool>( "LevelWindowInteractor" ) );
	WidgetListType widgets = GetWidgets( );
	tagMap->AddTag( "Size", (unsigned int) ( widgets.size() ) );

	WidgetListType::iterator it;
	int count = 0;
	for ( it = widgets.begin() ; it != widgets.end() ; it++ )
	{
		std::ostringstream stream;
		stream << "SliceView" << count++;

		mitk::SliceNavigationController* controller;
		controller = (*it)->GetSliceNavigationController( );

		std::string name = (*it)->GetName().c_str();
		int mapperID = (*it)->GetRenderer()->GetMapperID();
		int viewDirection = controller->GetViewDirection();
		bool top = controller->GetTop();
		bool frontSide = controller->GetFrontSide();
		bool rotated = controller->GetRotated();

		blTagMap::Pointer tagMapView = blTagMap::New();
		tagMapView->AddTag( "Name", name );
		tagMapView->AddTag( "Mapper", mapperID );
		tagMapView->AddTag( "ViewDirection", viewDirection );
		tagMapView->AddTag( "Top", top );
		tagMapView->AddTag( "FrontSide", frontSide );
		tagMapView->AddTag( "Rotated", rotated );

		tagMap->AddTag( stream.str() , tagMapView );
	}

}

void Core::Widgets::MultiRenderWindowMITK::SetProperties( blTagMap::Pointer tagMap )
{
	blTag::Pointer tag;

	size_t size = 0;
	tag = tagMap->FindTagByName( "Size" );
	if ( tag.IsNotNull() )
	{
		size = tag->GetValueCasted<unsigned int>( );
	}

	if ( size == 0 )
	{
		throw Core::Exceptions::Exception( "MultiRenderWindow::SetProperties", 
			"Cannot create a view with 0 windows" );
	}

	mitk::wxMitkMultiRenderWindow::WidgetListType widgets;
	for ( int count = 0 ; count < size ; count++ )
	{
		mitk::wxMitkSelectableGLWidget *widget;
		widget = new mitk::wxMitkSelectableGLWidget( this, wxNewId( ), "" );

		std::ostringstream stream;
		stream << "SliceView" << count;

		blTagMap::Pointer tagMapView;
		tag = tagMap->FindTagByName( stream.str() );
		tagMapView = tag->GetValueCasted<blTagMap::Pointer>( );

		mitk::SliceNavigationController* controller;
		controller = widget->GetSliceNavigationController( );

		tag = tagMapView->FindTagByName( "Name" );
		widget->SetName( tag->GetValueCasted<std::string>( ) );
		tag = tagMapView->FindTagByName( "Mapper" );
		widget->GetRenderer()->SetMapperID( tag->GetValueCasted<int>( ) );
		tag = tagMapView->FindTagByName( "ViewDirection" );
		controller->SetViewDirection( mitk::SliceNavigationController::ViewDirection( tag->GetValueCasted<int>( ) ) );
		tag = tagMapView->FindTagByName( "Top" );
		controller->SetTop( tag->GetValueCasted<bool>( ) );
		tag = tagMapView->FindTagByName( "FrontSide" );
		controller->SetFrontSide( tag->GetValueCasted<bool>( ) );
		tag = tagMapView->FindTagByName( "Rotated" );
		controller->SetRotated( tag->GetValueCasted<bool>( ) );

		widgets.push_back( widget );
	}
	SetWidgets( widgets );

	tag = tagMap->FindTagByName( "Layout" );
	if ( tag.IsNotNull() )
	{
		GetMetadata()->AddTag( "LayoutType", int( tag->GetValueCasted<int>( ) ) );
	}

	tag = tagMap->FindTagByName( "EnableDisplayPlaneSubtree" );
	if ( tag.IsNotNull() )
	{
		GetMetadata()->AddTag( "SlicesPlanesEnabled", tag->GetValueCasted<bool>( ) );
	}

	tag = tagMap->FindTagByName( "LevelWindowInteractor" );
	if ( tag.IsNotNull() )
	{
		GetMetadata()->AddTag( "LevelWindowInteractor", tag->GetValueCasted<bool>( ) );
	}

	GetMetadataHolder( )->NotifyObservers( );
}

void Core::Widgets::MultiRenderWindowMITK::SetEvtHandlerEnabled( bool enabled )
{
	m_MultiRenderWindowEventHandler->SetEvtHandlerEnabled( enabled );
}

MultiRenderWindowOverlay::Pointer Core::Widgets::MultiRenderWindowMITK::GetMultiRenderWindowOverlay()
{
	return m_MultiRenderWindowOverlay;
}

void Core::Widgets::MultiRenderWindowMITK::SetRenderingTreeInstance( Core::RenderingTree::Pointer val )
{
	m_RenderingTree = dynamic_cast<Core::RenderingTreeMITK*> (val.GetPointer());
}

void Core::Widgets::MultiRenderWindowMITK::SaveImage3DWindow( const std::string& completeFileName )
{
	SetCurrentLayout( mitk::Single3D );
	wxMitkMultiRenderWindow::SaveImage3DWindow( completeFileName );
}

void Core::Widgets::MultiRenderWindowMITK::CenterToPoint( double p[3] )
{
	mitk::Point3D point;
	point[ 0 ] = mitk::ScalarType( p[ 0 ] );
	point[ 1 ] = mitk::ScalarType( p[ 1 ] );
	point[ 2 ] = mitk::ScalarType( p[ 2 ] );
	SelectSliceByPoint( point );
}

std::string Core::Widgets::MultiRenderWindowMITK::GetName()
{
	return mitk::wxMitkMultiRenderWindow::GetName( ).c_str();
}

void Core::Widgets::MultiRenderWindowMITK::OnDataSelected( Core::DataEntity::Pointer dataEntity )
{
	mitk::DataTreeNode::Pointer node;
	m_RenderingTree->GetNode(dataEntity);
	if ( node.IsNotNull() && !(dataEntity->GetType() & Core::ROITypeId) )
	{
		mitk::LevelWindowProperty::Pointer levelWindowProperty;
		levelWindowProperty = dynamic_cast<mitk::LevelWindowProperty*>(node->GetProperty("levelwindow"));
		if ( levelWindowProperty )
		{
			GetLevelWindowWidget()->GetLevelWindowManager()->SetLevelWindowProperty(levelWindowProperty);
		}
	}
}

int Core::Widgets::MultiRenderWindowMITK::GetTime()
{
	return GetTimeNavigationController()->GetTime()->GetPos( );
}

void Core::Widgets::MultiRenderWindowMITK::SetTime( int time )
{
	if ( time != GetTime() )
	{
		GetTimeNavigationController()->GetTime()->SetPos( time );
		ForceImmediateUpdate( );
	}
}

void Core::Widgets::MultiRenderWindowMITK::GetTimeRange( int &min, int &max )
{
	min = GetTimeNavigationController()->GetTime()->GetRangeMin();
	max = GetTimeNavigationController()->GetTime()->GetRangeMax();
}

void Core::Widgets::MultiRenderWindowMITK::SetTimeStep( int time )
{
	Core::BaseWindow::SetTimeStep( time );

	if ( GetEnableTimeUpdates() )
	{
		SetTime( time );
	}
}

void Core::Widgets::MultiRenderWindowMITK::UpdateCamera3D( 
	Core::Camera3D::Pointer cam )
{
	Core::Camera3D::Pointer camera = GetCamera3DHolder( )->GetSubject();
	if ( camera->m_initialized == false || GetLock3DCamera() )
	{
		return;
	}

	vtkCamera* vtkCam = Get3D( )->GetRenderer( )->GetVtkRenderer()->GetActiveCamera(  );
	vtkCam->SetPosition( camera->m_eyex, camera->m_eyey, camera->m_eyez );
	vtkCam->SetViewUp( camera->m_upx, camera->m_upy, camera->m_upz );
	vtkCam->SetFocalPoint( camera->m_lookatx, camera->m_lookaty, camera->m_lookatz );
	vtkCam->SetViewAngle( camera->m_view_angle );
	vtkCam->SetClippingRange( camera->m_near_clip_plane, camera->m_far_clip_plane );

	mitk::RenderingManager::GetInstance()->ForceImmediateUpdate( Get3D()->GetVtkRenderWindow() );
}

void Core::Widgets::MultiRenderWindowMITK::UpdateSlicePlane( SlicePlane::Pointer slicePlane )
{
	if ( slicePlane.IsNull() || GetLockSlicePlanes() )
	{
		return;
	}

	int count = 0;
	WidgetListType widgets = GetWidgets();
	WidgetListType::iterator it;
	for ( it = widgets.begin() ; it != widgets.end() ; it++ )
	{
		if ( count == slicePlane->m_Id )
		{
			(*it)->GetSliceNavigationController( )->GetSlice( )->SetPos( slicePlane->m_Pos );
		}
		count++;
	}
}

void Core::Widgets::MultiRenderWindowMITK::OnModifiedSlicePlane( 
	const itk::Object* o, const itk::EventObject &event )
{
	if ( GetSlicePlaneHolder( ).IsNull() )
	{
		return ;
	}

	SetLockSlicePlanes( true );

	int count = 0;

	WidgetListType widgets = GetWidgets();
	WidgetListType::iterator it;
	for ( it = widgets.begin() ; it != widgets.end() ; it++ )
	{
		mitk::SliceNavigationController::Pointer controller;
		controller = (*it)->GetSliceNavigationController();
		if ( o == controller->GetSlice() )
		{
			SlicePlane::Pointer slicePlane = SlicePlane::New( );
			slicePlane->m_Orientation = SlicePlane::PlaneOrientation( controller->GetViewDirection( ) );
			slicePlane->m_Id = count;
			slicePlane->m_Pos = controller->GetSlice()->GetPos();
			GetSlicePlaneHolder()->SetSubject( slicePlane );
		}

		count++;
	}

	SetLockSlicePlanes( false );
}

void Core::Widgets::MultiRenderWindowMITK::InitDefaultLayout()
{
	mitk::wxMitkOrthoSliceFactory::InitRenderWindows( 
		dynamic_cast<MultiRenderWindowMITK*> (this) );
}

void Core::Widgets::MultiRenderWindowMITK::RequestUpdateAll()
{
	WidgetListType widgets = GetWidgets();
	WidgetListType::iterator it;
	for ( it = widgets.begin() ; it != widgets.end() ; it++ )
	{
		mitk::RenderingManager::GetInstance()->RequestUpdate( (*it)->GetVtkRenderWindow() );
	}
}

void Core::Widgets::MultiRenderWindowMITK::GetSlicePlanesPositions( 
	std::list<SlicePlane::Pointer> &slicePlanesPositions )
{
	int count = 0;

	WidgetListType widgets = GetWidgets();
	WidgetListType::iterator it;
	for ( it = widgets.begin() ; it != widgets.end() ; it++ )
	{
		mitk::SliceNavigationController::Pointer controller;
		controller = (*it)->GetSliceNavigationController();
		SlicePlane::Pointer slicePlane = SlicePlane::New( );
		slicePlane->m_Orientation = SlicePlane::PlaneOrientation( controller->GetViewDirection( ) );
		slicePlane->m_Id = count;
		slicePlane->m_Pos = controller->GetSlice()->GetPos();
		slicePlanesPositions.push_back( slicePlane );

		count++;
	}
}

void Core::Widgets::MultiRenderWindowMITK::OnModifiedMetadata()
{
	SetPlaneRotationLinked( GetMetadata( )->GetTagValue<bool>( "LinkPlanesEnabled" ) );
	EnableAxis( !GetMetadata( )->GetTagValue<bool>( "AxisLocked" ) );
	EnableAnnotatedCube( GetMetadata( )->GetTagValue<bool>( "AnnotationCubeShown" ) );
	mitk::LayoutConfiguration layout;
	layout = mitk::LayoutConfiguration( GetMetadata( )->GetTagValue<int>( "LayoutType" ) );
	SetCurrentLayout( layout );
	EnableDisplayPlaneSubtree( GetMetadata( )->GetTagValue<bool>( "SlicesPlanesEnabled" ) );
	EnableCornerAnnotations( GetMetadata( )->GetTagValue<bool>( "CornerAnnotationsShown" ) );
	EnableWindowLevelInteractor( GetMetadata( )->GetTagValue<bool>( "LevelWindowInteractor" ) );
}

void Core::Widgets::vtkCameraCallback::Execute( 
	vtkObject *caller, 
	unsigned long event, void *vtkNotUsed(callData) )
{
	if ( m_MultiRenderWindowMITK->GetCamera3DHolder( ).IsNull() )
	{
		return ;
	}

	Core::Camera3D::Pointer camera = m_MultiRenderWindowMITK->GetCamera3DHolder( )->GetSubject();
	m_MultiRenderWindowMITK->Lock3DCamera( true );

	camera->m_initialized = true;
	vtkCamera* vtkCam = m_MultiRenderWindowMITK->Get3D( )->GetRenderer( )->GetVtkRenderer()->GetActiveCamera(  );
	vtkCam->GetPosition( camera->m_eyex, camera->m_eyey, camera->m_eyez );
	vtkCam->GetViewUp( camera->m_upx, camera->m_upy, camera->m_upz );
	vtkCam->GetFocalPoint( camera->m_lookatx, camera->m_lookaty, camera->m_lookatz );
	camera->m_view_angle = vtkCam->GetViewAngle();
	vtkCam->GetClippingRange( camera->m_near_clip_plane, camera->m_far_clip_plane );

	m_MultiRenderWindowMITK->GetCamera3DHolder( )->NotifyObservers();

	m_MultiRenderWindowMITK->Lock3DCamera( false );
}
