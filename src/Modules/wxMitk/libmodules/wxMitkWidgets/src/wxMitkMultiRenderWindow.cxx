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

#include "wx/utils.h"

#include "wxMitkMultiRenderWindow.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkSelectableGLWidget.h"
#include "wxMitkLevelWindowWidget.h"
#include "wxMitkOrthoSliceFactory.h"
#include "wxMitkDisplayPlaneSubtreeHelper.h"

#include <mitkSliceNavigationController.h>
#include <mitkDisplayInteractor.h>
#include <mitkDisplayVectorInteractor.h>
#include <mitkLogoRendering.h>
#include <mitkVtkPropRenderer.h>
#include <mitkGlobalInteraction.h>
#include <mitkSlicesSwiveller.h>
#include <mitkLine.h>
#include "mitkSlicesRotatorMovement.h"

#include <vtkRenderLargeImage.h>
#include <vtkPNGWriter.h>
#include <vtkTIFFWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkTransform.h>
#include "mitkVtkLayerController.h"
#include "mitkDisplayVectorInteractorLevelWindow.h"


#include "blMitkAnnotatedCube.h"
#include "blMitkCornerAnnotation.h"

using namespace mitk;


BEGIN_EVENT_TABLE(wxMitkMultiRenderWindow, wxPanel)
	EVT_LEFT_DCLICK(wxMitkMultiRenderWindow::OnDblClick)
END_EVENT_TABLE()

/**
\brief PImpl pattern
\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 17 Oct 2007
*/
class wxMitkMultiRenderWindowImpl
{
public:
	mitk::SlicesRotatorMovement::Pointer m_SlicesRotator;
	mitk::SlicesSwiveller::Pointer m_SlicesSwiveller;
	
	//!
	mitk::SliceNavigationController::Pointer m_TimeNavigationController;

	//!
	std::map<mitk::SliceNavigationController::Pointer, unsigned long> m_TimeNavigationObservers;

	mitk::DisplayVectorInteractor::Pointer m_MoveAndZoomInteractor;
	mitk::LogoRendering::Pointer m_LogoRendering;
	blMitkAnnotatedCube::Pointer m_AnnotatedCubeActor;
	mitk::wxMitkMultiRenderWindowLayout* m_Layout;
	mitk::DisplayVectorInteractorLevelWindow::Pointer m_DisplayVectorInteractorLevelWindow;
};


//!
wxMitkMultiRenderWindow::wxMitkMultiRenderWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name),
m_PlaneMode(wxMitkMultiRenderWindow::RotatePlanes)
{
	m_pImpl = new wxMitkMultiRenderWindowImpl;
	m_pImpl->m_Layout = new wxMitkMultiRenderWindowLayout( );
	SetSizer( m_pImpl->m_Layout );

	// Layout them as default
	SetAutoLayout(true);

	// Add the LevelWindowWidget
	m_LevelWindowWidget = new wxMitkLevelWindowWidget(this, wxNewId());

	// setup the department logo rendering
	m_pImpl->m_LogoRendering = mitk::LogoRendering::New();
	m_pImpl->m_LogoRendering->SetOpacity(1.0);
	m_pImpl->m_LogoRendering->Enable();

	m_bKeep2DViewPosition = false;

}

//!
wxMitkMultiRenderWindow::~wxMitkMultiRenderWindow(void)
{
	delete m_pImpl;
	m_pImpl = NULL;
}

/**
*/
bool wxMitkMultiRenderWindow::UpdateViewControllers()
{
	if ( m_pImpl->m_TimeNavigationController.IsNull( ) || m_DataStorage.IsNull() )
	{
		return false;
	}

	bool boundingBoxInitialized = false;

	// If it was called with an empty iterator, we just update
	mitk::TimeSlicedGeometry::Pointer geometry = m_DataStorage->ComputeVisibleBoundingGeometry3D( NULL, "includeInBoundingBox" );

	if ( geometry.IsNull() )
	{
		m_pImpl->m_TimeNavigationController->GetTime()->SetPos( 0 );
		m_pImpl->m_TimeNavigationController->Update();

		UpdateNavigationControllers( geometry, false, !m_bKeep2DViewPosition );

		RequestUpdate( );
		m_bKeep2DViewPosition = false;
		m_OldGeometry = NULL;
		return false;
	}

	// compare the new geometry with the old one
	if ( m_OldGeometry.IsNotNull() )
	{
		bool update = false;
		if ( m_OldGeometry->GetTimeSteps() != geometry->GetTimeSteps() )
		{
			update = true;
		}

		float diff = fabs(m_OldGeometry->GetDiagonalLength() - geometry->GetDiagonalLength());
		if(diff > (m_OldGeometry->GetDiagonalLength()*0.1))
		{
			m_bKeep2DViewPosition = false;
			update = true;
		}

		if ( !update )
		{
			return false;
		}
	}
	m_OldGeometry = geometry;

	// Check the bounding box
	mitk::BoundingBox* boundixBox = const_cast<mitk::BoundingBox*>(geometry->GetBoundingBox());
	double diagonalLength  = boundixBox->GetDiagonalLength2();
	if ( (diagonalLength < mitk::eps) || (diagonalLength > mitk::large) )
	{
		m_bKeep2DViewPosition = false;
		return false;
	}

	// Get intersection point
	mitk::Point3D axisCenter;
	bool bAxisCenterComputed = GetIntersectionPoint( axisCenter );

	// First update the time because if the last opened image
	// had different time, the mitk::SliceNavigationController
	// will send an event notify that time has changed with the
	// old time
	int iTimePos = m_pImpl->m_TimeNavigationController->GetTime()->GetPos();
	UpdateTimeController( geometry );
	UpdateNavigationControllers( geometry, true, !m_bKeep2DViewPosition );

	// When the 2D views are updated, the time step is set to 0
	// so we need to change it again
	m_pImpl->m_TimeNavigationController->GetTime()->SetPos( iTimePos );


	// Restore position of the slices
	if ( bAxisCenterComputed && m_bKeep2DViewPosition )
	{
		SelectSliceByPoint( axisCenter );
	}

	// Known bug: Use ForceImmediateUpdate instead of RequestUpdate so that Fit() resets the camera
	// according to the new geometry.
	FitScene();
	ForceImmediateUpdate( );

	// Keep position for next call
	m_bKeep2DViewPosition = true;

	return true;
}

/**
Resets the standard view directions of the navigators.
*/
void wxMitkMultiRenderWindow::ReInitializeViewControllers( )
{
	bool boundingBoxInitialized = false;

	// If it was called with an empty iterator, we just update
	mitk::TimeSlicedGeometry::Pointer geometry = m_DataStorage->ComputeVisibleBoundingGeometry3D( NULL, "includeInBoundingBox" );
	if ( geometry.IsNull() )
	{
		m_pImpl->m_TimeNavigationController->Update();
		UpdateNavigationControllers( geometry, false, true );
		RequestUpdate( );

		// Reset time step
		m_pImpl->m_TimeNavigationController->GetTime()->SetPos( 0 );
		m_OldGeometry = NULL;
		return ;
	}

	m_OldGeometry = geometry;

	// Check the bounding box
	mitk::BoundingBox* boundixBox = const_cast<mitk::BoundingBox*>(geometry->GetBoundingBox());
	double diagonalLength  = boundixBox->GetDiagonalLength2();
	if ( (diagonalLength < mitk::eps) || (diagonalLength > mitk::large) )
	{
		return ;
	}

	int iTimePos = m_pImpl->m_TimeNavigationController->GetTime()->GetPos();
	// First update the time because if the last opened image
	// had different time, the mitk::SliceNavigationController
	// will send an event notify that time has changed with the
	// old time
	UpdateTimeController( geometry );
	UpdateNavigationControllers( geometry, false, true );

	// When the 2D views are updated, the time step is set to 0
	// so we need to change it again
	m_pImpl->m_TimeNavigationController->GetTime()->SetPos( iTimePos );

	// Known bug: Use ForceImmediateUpdate instead of RequestUpdate so that Fit() resets the camera
	// according to the new geometry.
	FitScene();
	ForceImmediateUpdate( );

	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		(*it)->GetSliceNavigationController()->SetKeepPlanePosition( true );
	}
}

//!
void wxMitkMultiRenderWindow::SetDepartmentLogoPath(const std::string& fileNameFullPath)
{
	m_pImpl->m_LogoRendering->ForceMBILogoVisible(false);
	m_pImpl->m_LogoRendering->SetLogoSource(fileNameFullPath.c_str());
}

//!
mitk::SlicesRotator* wxMitkMultiRenderWindow::GetSlicesRotator(void) const
{
	return m_pImpl->m_SlicesRotator;
}

//!
mitk::SlicesSwiveller* wxMitkMultiRenderWindow::GetSlicesSwiveller(void) const
{
	return m_pImpl->m_SlicesSwiveller;
}

//!
mitk::SliceNavigationController* wxMitkMultiRenderWindow::GetTimeNavigationController (void) const
{
	return m_pImpl->m_TimeNavigationController.GetPointer();
}

//!
mitk::DisplayVectorInteractor* wxMitkMultiRenderWindow::GetMoveAndZoomInteractor(void) const
{
	return m_pImpl->m_MoveAndZoomInteractor.GetPointer();
}

/**
*/
void wxMitkMultiRenderWindow::EnableNavigationController(bool enable)
{
	// Let NavigationControllers listen to GlobalInteraction
	mitk::GlobalInteraction* globalInteraction = mitk::GlobalInteraction::GetInstance();

	switch (m_PlaneMode)
	{
	default:
	case SlicePlanes:
		{
			WidgetListType::iterator it;
			for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
			{
				if(enable)
				{
					globalInteraction->AddListener( (*it)->GetSliceNavigationController());
				}
				else
				{
					globalInteraction->RemoveListener( (*it)->GetSliceNavigationController());
				}
			}
		}

		break;

	case RotatePlanes:
		if(enable)
			globalInteraction->AddListener(m_pImpl->m_SlicesRotator);
		else
			globalInteraction->RemoveListener(m_pImpl->m_SlicesRotator);
		break;

	case SwivelPlanes:
		if(enable)
			globalInteraction->AddListener(m_pImpl->m_SlicesSwiveller);
		else
			globalInteraction->RemoveListener(m_pImpl->m_SlicesSwiveller);
		break;
	}
	if(enable)
		globalInteraction->AddListener(m_pImpl->m_TimeNavigationController);
	else
		globalInteraction->AddListener(m_pImpl->m_TimeNavigationController);
}

/**
It enables or disables the Slice navigation for the user, so as to change it to other interactors, such as
the plane rotation.
*/
void wxMitkMultiRenderWindow::EnableMoveAndZoomInteractor(bool enable)
{
	mitk::GlobalInteraction* globalInteraction = mitk::GlobalInteraction::GetInstance();
	if(enable)
		globalInteraction->AddListener(m_pImpl->m_MoveAndZoomInteractor);
	else
		globalInteraction->RemoveListener(m_pImpl->m_MoveAndZoomInteractor);
}

void wxMitkMultiRenderWindow::EnableWindowLevelInteractor(bool enable)
{
	mitk::GlobalInteraction* globalInteraction = mitk::GlobalInteraction::GetInstance();
	if(enable)
		globalInteraction->AddListener(m_pImpl->m_DisplayVectorInteractorLevelWindow);
	else
		globalInteraction->RemoveListener(m_pImpl->m_DisplayVectorInteractorLevelWindow);
}

bool wxMitkMultiRenderWindow::IsDepartmentLogoEnabled()
{
	return m_pImpl->m_LogoRendering->IsEnabled();
}

//!
void wxMitkMultiRenderWindow::EnableDepartmentLogo(bool enable)
{
	if(enable)
		m_pImpl->m_LogoRendering->Enable();
	else
		m_pImpl->m_LogoRendering->Disable();
}

//!
void wxMitkMultiRenderWindow::EnableAnnotatedCube(bool enable)
{
	if ( m_pImpl->m_AnnotatedCubeActor )
	{
		m_pImpl->m_AnnotatedCubeActor->Enable( enable );
	}
	EnableOrientationAnnotations( enable );
}

//!
void wxMitkMultiRenderWindow::EnableCornerAnnotations(bool enable)
{
	WidgetListType sliceViews = GetLayout()->GetSliceViews();
	WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		(*it)->EnableCornerAnnotations( enable );
	}
	RequestUpdate();
}

//!
void wxMitkMultiRenderWindow::EnableOrientationAnnotations(bool enable)
{
	WidgetListType sliceViews = GetLayout()->GetSliceViews();
	WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		(*it)->EnableOrientationAnnotations( enable );
	}
	RequestUpdate();
}

//!
bool wxMitkMultiRenderWindow::GetShowCornerAnnotations()
{
	if ( GetX() )
	{
		return GetX()->GetCornerAnnotationsEnabled();
	}
	return false;
}

/**
Displays or hides the m_LevelWindowWidget, as it is disabled by default. 
It must be called always after a successful call to SetData() because it needs a valid 2D image data set
in order to compute the LevelWindow.
*/
void wxMitkMultiRenderWindow::EnableStandardLevelWindow(bool enable)
{
	GetLayout()->ShowLevelWindow( enable );

	// Force recalc layout
	wxSizeEvent resEvent(GetBestSize(), GetId());
	resEvent.SetEventObject(this);
	GetEventHandler()->ProcessEvent(resEvent);
}

void wxMitkMultiRenderWindow::SetPlaneRotationLinked(bool link)
{
	if ( m_pImpl->m_SlicesRotator )
	{
		m_pImpl->m_SlicesRotator->SetLinkPlanes(link);
	}
	if ( m_pImpl->m_SlicesSwiveller )
	{
		m_pImpl->m_SlicesSwiveller->SetLinkPlanes(link);
	}
}
/**
Sets the plane interaction mode to either SlicePlanes, RotatePlanes or SwivelPlanes
*/
void wxMitkMultiRenderWindow::SetNavigationInteractionMode(wxMitkMultiRenderWindow::PlaneInteractionMode mode)
{
	// Do nothing if mode didn't change
	if(m_PlaneMode == mode)
		return;

	EnableNavigationController( false );

	// Set new mode and add corresponding listener to GlobalInteraction
	m_PlaneMode = mode;

	EnableNavigationController( true );

}



void mitk::wxMitkMultiRenderWindow::SetCurrentLayout( 
	LayoutConfiguration layout,
	wxMitkSelectableGLWidget* widget )
{
	Hide( );

	GetLayout( )->SetCurrentLayout( layout, widget );

	// Force recalc layout
	wxSizeEvent resEvent(GetBestSize(), GetId());
	resEvent.SetEventObject(this);
	GetEventHandler()->ProcessEvent(resEvent);
	//GetSizer( )->RecalcSizes();

	Show( );
	this->Refresh();
}

void mitk::wxMitkMultiRenderWindow::SelectSliceByPoint( mitk::Point3D point )
{
	WidgetListType sliceViews = GetLayout()->GetSliceViews();
	WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		if ( (*it)->GetSliceNavigationController()->GetCreatedWorldGeometry( ) )
		{
			(*it)->GetSliceNavigationController()->SelectSliceByPoint( point );
		}
	}
}

void mitk::wxMitkMultiRenderWindow::SaveImage3DWindow( std::string imageName )
{
	vtkRenderer* vtkrenderer;
	wxColour paleWhite(241,244,250);
	vtkrenderer = Get3D()->GetRenderer()->GetVtkRenderer();
	vtkrenderer->SetBackground(1,1,1);
	//Get3D()->GetMitkRenderWindow()->SetOwnBackgroundColour(paleWhite);
	//Get3D()->GetMitkRenderWindow()->SetOwnForegroundColour(paleWhite);
	vtkRenderLargeImage* renderLarge = vtkRenderLargeImage::New();
	renderLarge->SetInput(vtkrenderer);
//	vtkJPEGWriter* writer=vtkJPEGWriter::New();
	vtkPNGWriter* writer=vtkPNGWriter::New();
	//vtkTIFFWriter* writer=vtkTIFFWriter::New();
	writer->SetInputConnection(renderLarge->GetOutputPort());
	writer->SetFileName(imageName.c_str()) ;
	writer->Write();
	vtkrenderer->SetBackground(0,0,0);

}

void mitk::wxMitkMultiRenderWindow::OnDblClick( wxMouseEvent& event )
{
	//static int a = 0;
	//SetCurrentLayout( LayoutConfiguration( ( a++ ) % (Only2D + 1) ) );

	if ( GetLayout()->RestoreLayout( ) )
	{
		SetCurrentLayout( GetLayout()->GetCurrentLayout() );
	}
	else
	{
		wxPoint mousePosition = event.GetPosition();

		GetLayout()->BackupLayout();

		WidgetListType::iterator it;
		for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
		{
			if ( (*it)->GetRect().Contains( mousePosition ) )
			{
				SetCurrentLayout( Single, (*it) );
			}
		}
	}

}

void mitk::wxMitkMultiRenderWindow::EnableMouseWheel( bool enable /*= true */ )
{
	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		(*it)->GetMitkRenderWindow( )->EnableMouseWheel( enable );
	}
}

wxMitkLevelWindowWidget*  mitk::wxMitkMultiRenderWindow::GetLevelWindowWidget()
{
	return this->m_LevelWindowWidget;
}

bool mitk::wxMitkMultiRenderWindow::Enable( bool enable /* = true */ )
{
	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		(*it)->Enable( enable );
	}

	EnableNavigationController( enable );
	EnableMoveAndZoomInteractor( enable );
	//EnableWindowLevelInteractor( enable );

	return wxPanel::Enable( enable );
}

bool mitk::wxMitkMultiRenderWindow::GetIntersectionPoint(
	mitk::Point3D &axisCenter )
{
	if ( !GetX() || !GetY() || !GetZ() )
	{
		return false;
	}

	bool bAxisCenterComputed = false;
	mitk::SliceNavigationController* sliceNavigatorTransversal = GetX()->GetSliceNavigationController();
	mitk::SliceNavigationController* sliceNavigatorSagittal = GetY()->GetSliceNavigationController();
	mitk::SliceNavigationController* sliceNavigatorFrontal = GetZ()->GetSliceNavigationController();
	const PlaneGeometry* planeGeometry  = sliceNavigatorTransversal->GetCurrentPlaneGeometry( );
	const PlaneGeometry* planeGeometry1 = sliceNavigatorSagittal->GetCurrentPlaneGeometry( );
	const PlaneGeometry* planeGeometry2 = sliceNavigatorFrontal->GetCurrentPlaneGeometry( );
	if ( planeGeometry && planeGeometry1 && planeGeometry2) 
	{
		Line3D intersection;
		bAxisCenterComputed |= planeGeometry->IntersectionLine( planeGeometry1, intersection );
		bAxisCenterComputed |= planeGeometry2->IntersectionPoint(intersection, axisCenter);
	}
	return bAxisCenterComputed;
}

void mitk::wxMitkMultiRenderWindow::UpdateTimeController( 
	mitk::TimeSlicedGeometry::Pointer geometry )
{
	int iTimePos = m_pImpl->m_TimeNavigationController->GetTime()->GetPos();
	m_pImpl->m_TimeNavigationController->SetInputWorldGeometry(geometry);
	m_pImpl->m_TimeNavigationController->Update();
	m_pImpl->m_TimeNavigationController->GetTime()->SetPos( iTimePos );

}

void mitk::wxMitkMultiRenderWindow::InitCubeAnnotations()
{	
	m_pImpl->m_AnnotatedCubeActor = blMitkAnnotatedCube::New( );
	m_pImpl->m_AnnotatedCubeActor->SetRenderWindow( Get3D()->GetVtkRenderWindow() );
	m_pImpl->m_AnnotatedCubeActor->Enable( false );

	if ( GetX() && GetY() && GetZ() )
	{
		float colorX[3];
		float colorY[3];
		float colorZ[3];
		GetX( )->GetColourAsFloat( colorX );
		GetY( )->GetColourAsFloat( colorY );
		GetZ( )->GetColourAsFloat( colorZ );
		m_pImpl->m_AnnotatedCubeActor->SetAxisColors( colorX, colorY, colorZ );
	}
}

void mitk::wxMitkMultiRenderWindow::InitInteractors()
{
	// create a slice rotator
	m_pImpl->m_SlicesRotator = mitk::SlicesRotatorMovement::New( "slices-rotator-and-movement" );

	// create a slice swiveller (using the same state-machine as SlicesRotator)
	m_pImpl->m_SlicesSwiveller = mitk::SlicesSwiveller::New( );

	// initialize m_TimeNavigationController: send time via sliceNavigationControllers
	m_pImpl->m_TimeNavigationController = mitk::SliceNavigationController::New(NULL);

	// instantiate display interactor
	m_pImpl->m_MoveAndZoomInteractor = mitk::DisplayVectorInteractor::New(
		"moveNzoom", 
		new mitk::DisplayInteractor());

	m_pImpl->m_DisplayVectorInteractorLevelWindow = 
		mitk::DisplayVectorInteractorLevelWindow::New( "LeftClickLevelWindow" );

	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		ConnectInteractors( (*it) );
	}
}

void mitk::wxMitkMultiRenderWindow::ConnectInteractors( 
	wxMitkSelectableGLWidget* widget )
{
	mitk::SliceNavigationController::Pointer navigationController;
	navigationController = widget->GetSliceNavigationController();

	// Add to slice controlers if is 2D mapper
	if ( navigationController->GetRenderer()->GetMapperID() == mitk::BaseRenderer::Standard2D )
	{
		m_pImpl->m_SlicesRotator->AddSliceController( navigationController );
		m_pImpl->m_SlicesSwiveller->AddSliceController( navigationController );
	}

	// Always Add to time controller
	unsigned long tag;
	tag = m_pImpl->m_TimeNavigationController->ConnectGeometryTimeEvent<mitk::SliceNavigationController>(
		navigationController, false);
	navigationController->ConnectGeometryTimeEvent<mitk::SliceNavigationController>(
		m_pImpl->m_TimeNavigationController, false);

	m_pImpl->m_TimeNavigationObservers[ navigationController ] = tag;
}

void mitk::wxMitkMultiRenderWindow::DisconnectInteractors( 
	wxMitkSelectableGLWidget* widget )
{
	mitk::SliceNavigationController::Pointer navigationController;
	navigationController = widget->GetSliceNavigationController();

	m_pImpl->m_SlicesRotator->RemoveSliceController( navigationController );
	m_pImpl->m_SlicesSwiveller->RemoveSliceController( navigationController );
	m_pImpl->m_TimeNavigationController->RemoveObserver( 
		m_pImpl->m_TimeNavigationObservers[ navigationController ] );
}

void mitk::wxMitkMultiRenderWindow::SetCornerAnotation(
	vtkRenderer* textRenderer, 
	RenderWindowType window)
{
	switch( window )
	{
	case X_Win :
		mitk::VtkLayerController::GetInstance(GetX()->GetVtkRenderWindow())->InsertForegroundRenderer(textRenderer, true);
		break;
	case Y_Win :
		mitk::VtkLayerController::GetInstance(GetY()->GetVtkRenderWindow())->InsertForegroundRenderer(textRenderer, true);
		break;
	case Z_Win :
		mitk::VtkLayerController::GetInstance(GetZ()->GetVtkRenderWindow())->InsertForegroundRenderer(textRenderer, true);
		break;
	case ThreeD_Win :
		mitk::VtkLayerController::GetInstance(Get3D()->GetVtkRenderWindow())->InsertForegroundRenderer(textRenderer, true);
		break;
	default:break;
	}
}

wxMitkSelectableGLWidget* mitk::wxMitkMultiRenderWindow::GetX()
{
	return GetLayout()->GetXView();
}

wxMitkSelectableGLWidget* mitk::wxMitkMultiRenderWindow::GetY()
{
	return GetLayout()->GetYView();
}

wxMitkSelectableGLWidget* mitk::wxMitkMultiRenderWindow::GetZ()
{
	return GetLayout()->GetZView();
}

wxMitkSelectableGLWidget* mitk::wxMitkMultiRenderWindow::Get3D()
{
	return GetLayout()->Get3DView( );
}

wxMitkMultiRenderWindowLayout* mitk::wxMitkMultiRenderWindow::GetLayout()
{
	return m_pImpl->m_Layout;
}

mitk::DataStorage::Pointer mitk::wxMitkMultiRenderWindow::GetDataStorage() const
{
	return m_DataStorage;
}

std::list<wxMitkSelectableGLWidget*> mitk::wxMitkMultiRenderWindow::GetWidgets() const
{
	return m_Widgets;
}

void mitk::wxMitkMultiRenderWindow::SetWidgets( 
	const std::list<wxMitkSelectableGLWidget*> &widgets )
{

	if ( widgets.empty() )
	{
		return;
	}

	RemoveSlicePlanesObervers();

	m_Widgets = widgets;

	m_pImpl->m_Layout->SetViews( m_Widgets );
	m_pImpl->m_Layout->SetLevelWindowWidget( m_LevelWindowWidget );
	m_pImpl->m_Layout->SetCurrentLayout( Default2x2 );

	InitInteractors();

	InitCubeAnnotations();

	m_pImpl->m_LogoRendering->SetRenderWindow( Get3D( )->GetVtkRenderWindow() );

	// For 2D views
	EnableMoveAndZoomInteractor( true );
	EnableNavigationController(true);
	SetPlaneRotationLinked(true);
	LockPlaneRotation( false );
	LockPlaneSlicing( false );

	AddSlicePlanesObervers( );
}

void mitk::wxMitkMultiRenderWindow::AddWidget( wxMitkSelectableGLWidget* widget )
{
	RemoveSlicePlanesObervers();

	widget->GetRenderer()->SetDataStorage( m_DataStorage );

	ConnectInteractors( widget );

	m_Widgets.push_back( widget );

	m_pImpl->m_Layout->SetViews( m_Widgets );
	SetCurrentLayout( m_pImpl->m_Layout->GetCurrentLayout() );

	AddSlicePlanesObervers( );
}

void mitk::wxMitkMultiRenderWindow::RemoveWidget( wxMitkSelectableGLWidget* widget )
{
	RemoveSlicePlanesObervers();

	DisconnectInteractors( widget );

	m_Widgets.remove( widget );

	m_pImpl->m_Layout->SetViews( m_Widgets );
	SetCurrentLayout( m_pImpl->m_Layout->GetCurrentLayout() );

	AddSlicePlanesObervers( );
}

void mitk::wxMitkMultiRenderWindow::AddSlicePlanesObervers()
{
	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		(*it)->AddSlicePosObserver( this, &wxMitkMultiRenderWindow::OnModifiedSlicePlane );
	}	
}

void mitk::wxMitkMultiRenderWindow::RemoveSlicePlanesObervers()
{
	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		(*it)->RemoveSlicePosObserver( );
	}	
}

void mitk::wxMitkMultiRenderWindow::OnModifiedSlicePlane( 
	const itk::Object* o, const itk::EventObject &event )
{
	
}

void wxMitkMultiRenderWindow::SetDataStorage(mitk::DataStorage* storage)
{
	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		(*it)->GetRenderer()->SetDataStorage( storage );
	}
	m_LevelWindowWidget->SetDataStorage( storage );

	m_DataStorage = storage;
}


void wxMitkMultiRenderWindow::RequestUpdate(void)
{
	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		(*it)->RequestUpdate();
	}
}

void wxMitkMultiRenderWindow::ForceImmediateUpdate(void)
{
	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		(*it)->ForceImmediateUpdate();
	}
}

void wxMitkMultiRenderWindow::FitScene(void)
{
	// Disable vtk warnings
	int warnDescriptor = vtkObject::GetGlobalWarningDisplay();
	vtkObject::GlobalWarningDisplayOff();

	vtkRenderer* vtkrenderer;
	WidgetListType::iterator it;
	for ( it = m_Widgets.begin() ; it != m_Widgets.end() ; it++ )
	{
		(*it)->GetRenderer()->GetDisplayGeometry()->Fit();
		vtkrenderer = (*it)->GetRenderer()->GetVtkRenderer();
		if ( vtkrenderer != NULL ) 
		{
			vtkrenderer->ResetCamera();
		}
	}

	// Enable vtk warnings
	vtkObject::SetGlobalWarningDisplay(warnDescriptor);
}


void wxMitkMultiRenderWindow::LockPlaneSlicing(bool locked)
{
	WidgetListType sliceViews = GetLayout()->GetSliceViews();
	WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		(*it)->GetSliceNavigationController()->SetSliceLocked(locked);
	}
}

void wxMitkMultiRenderWindow::LockPlaneRotation(bool locked)
{
	WidgetListType sliceViews = GetLayout()->GetSliceViews();
	WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		(*it)->GetSliceNavigationController()->SetSliceRotationLocked(locked);
	}
}

void wxMitkMultiRenderWindow::UpdateNavigationControllers(
	mitk::TimeSlicedGeometry::Pointer geometry, 
	bool keepOrientation,
	bool centerPos )
{

	int direction;
	for ( direction = mitk::SliceNavigationController::Transversal; 
		  direction < mitk::SliceNavigationController::Original ; 
		  direction++ )
	{
		// Get number of slices views for this direction
		WidgetListType widgets = GetLayout()->GetSliceViews( 
			mitk::SliceNavigationController::ViewDirection( direction ) );
		unsigned int numberOfViews = widgets.size();

		int i = 0;
		WidgetListType::iterator it;
		for ( it = widgets.begin() ; it != widgets.end() ; it++ )
		{
			// Update controller
			(*it)->UpdateController( geometry, keepOrientation );

			unsigned int totalSlices = 0;
			totalSlices = (*it)->GetSliceNavigationController()->GetSlice()->GetSteps();

			// Change position
			if ( centerPos )
			{
				unsigned int slicePos;
				slicePos = float( i + 1 ) / ( numberOfViews + 1) * totalSlices;
				(*it)->GetSliceNavigationController()->GetSlice()->SetPos( slicePos );
			}

			i++;
		}

	}

	// Update 3D view
	GetLayout()->Get3DView()->UpdateController( geometry, keepOrientation );
}

