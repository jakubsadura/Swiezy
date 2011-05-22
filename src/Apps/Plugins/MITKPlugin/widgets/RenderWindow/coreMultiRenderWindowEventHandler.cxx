/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMultiRenderWindowEventHandler.h"

#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "coreMainMenu.h"
#include "coreSettings.h"
#include "corePluginTab.h"
#include "coreMultiRenderWindowMITK.h"
#include "coreDataTreeMITKHelper.h"

#include "wxUnicode.h"
#include "wxMitkSelectableGLWidget.h"

using namespace Core::Widgets;

// Event the widget
BEGIN_EVENT_TABLE(MultiRenderWindowEventHandler, wxEvtHandler)
	EVT_MENU	(wxID_LockAxis, MultiRenderWindowEventHandler::OnMenuEnableAxis)
	EVT_MENU	(wxID_ShowAnnotationCubeMenuItem, MultiRenderWindowEventHandler::OnMenuShowAnnotationCubeMenuItem)	
	EVT_MENU	(wxID_ShowCornerAnnotationsMenuItem,MultiRenderWindowEventHandler::OnMenuShowAnnotationMenuItem)
	EVT_MENU	(wxID_ViewLinkPlanes, MultiRenderWindowEventHandler::OnMenuViewLinkPlanesItem)
	EVT_MENU	(wxID_ViewShowLabels, MultiRenderWindowEventHandler::OnMenuViewShowLabelsItem)
	EVT_MENU	(wxID_LevelWindowInteractor, MultiRenderWindowEventHandler::OnMenuEnableLevelWindowInteractor)

	EVT_MENU	(wxID_ViewLayout2DViewsLeft,MultiRenderWindowEventHandler::OnMenuViewLayout)
	EVT_MENU	(wxID_ViewLayoutDefault,MultiRenderWindowEventHandler::OnMenuViewLayout)
	EVT_MENU	(wxID_ViewLayoutOnly2DViews,MultiRenderWindowEventHandler::OnMenuViewLayout)
	EVT_MENU	(wxID_ViewLayoutXView,MultiRenderWindowEventHandler::OnMenuViewLayout)
	EVT_MENU	(wxID_ViewLayoutYView,MultiRenderWindowEventHandler::OnMenuViewLayout)
	EVT_MENU	(wxID_ViewLayoutZView,MultiRenderWindowEventHandler::OnMenuViewLayout)
	EVT_MENU	(wxID_ViewLayoutDefault2x2_YXZ3D,MultiRenderWindowEventHandler::OnMenuViewLayout)
	EVT_MENU	(wxID_ViewLayoutDefault2x2_XYZ3D,MultiRenderWindowEventHandler::OnMenuViewLayout)
	EVT_MENU	(wxID_ViewLayoutBig3DView,MultiRenderWindowEventHandler::OnMenuViewLayout)

END_EVENT_TABLE()

MultiRenderWindowEventHandler::MultiRenderWindowEventHandler( MultiRenderWindowMITK* renderWindow ) : wxEvtHandler()
{
	m_RenderWindow = renderWindow;

	AppendMenuItems();

	UpdateMenu();
}

void Core::Widgets::MultiRenderWindowEventHandler::AppendMenuItems()
{
	// edit GIMIAS menu: add MultiRenderWindow options...
	wxMenu* menuView = GetViewMenu( );
	
	wxMenuItemList list = menuView->GetMenuItems();
	int pos = 0;
	while ( pos < list.size( ) && !list[pos]->IsSeparator( ) )
	{
		pos++;
	}
	
	if ( menuView->FindItem( wxID_CurrentMultiView ) == NULL )
	{
		wxMenu *subMenu = new wxMenu();
		menuView->Insert(pos,wxID_CurrentMultiView, wxT("&MultiRenderWindow"), subMenu );
		subMenu->Append( wxID_LockAxis, wxT("Lock &Axis\tCtrl+I"), 
			wxT("Enables or disables the axis"), wxITEM_CHECK );
		subMenu->Append( wxID_ShowAnnotationCubeMenuItem, wxT("Show A&nnotation Cube\tCtrl+A"), 
			wxT("Show annotation cube"), wxITEM_CHECK );
		subMenu->Append( wxID_ShowCornerAnnotationsMenuItem, wxT("Show Corner Ann&otations\tCtrl+N"), 
			wxT("Show Corner Annotations"), wxITEM_CHECK );
		subMenu->Append( wxID_ViewLinkPlanes, wxT("Link planes"), 
			wxT("Link &planes"), wxITEM_CHECK);
		subMenu->Append(wxID_ViewShowLabels, wxT("Show landmarks labels"), 
			wxT("Show &landmarks labels"), wxITEM_CHECK);
		subMenu->Append(wxID_LevelWindowInteractor, wxT("Level window interaction\tCtrl+W"), 
			wxT("Level &window interaction"), wxITEM_CHECK);
	}


	if ( menuView->FindItem( wxID_ViewLayout ) == NULL )
	{
		wxMenu *subMenu = new wxMenu();
		menuView->Insert(pos,wxID_ViewLayout, wxT("&Layout"), subMenu );
		subMenu->Append(wxID_ViewLayout2DViewsLeft, wxT("2D Views Left"), 
			wxT("2D Views Left"), wxITEM_RADIO);
		subMenu->Append(wxID_ViewLayoutDefault, wxT("ZXY View"), 
			wxT("XYZ View"), wxITEM_RADIO);
		subMenu->Append(wxID_ViewLayoutOnly2DViews, wxT("2D Views only"), 
			wxT("2D Views only"), wxITEM_RADIO);
		subMenu->Append(wxID_ViewLayoutXView, wxT("X View"), 
			wxT("X View"), wxITEM_RADIO);
		subMenu->Append(wxID_ViewLayoutYView, wxT("Y View"), 
			wxT("Y View"), wxITEM_RADIO);
		subMenu->Append(wxID_ViewLayoutZView, wxT("Z View"), 
			wxT("Z View"), wxITEM_RADIO);
		subMenu->Append(wxID_ViewLayoutBig3DView, wxT("3D View"), 
			wxT("3D View"), wxITEM_RADIO);
		subMenu->Append(wxID_ViewLayoutDefault2x2_YXZ3D, wxT("YXZ View"), 
			wxT("YXZ View"), wxITEM_RADIO);
		subMenu->Append(wxID_ViewLayoutDefault2x2_XYZ3D, wxT("XYZ View"), 
			wxT("XYZ View"), wxITEM_RADIO);
	}

}

void Core::Widgets::MultiRenderWindowEventHandler::OnMenuEnableAxis( wxCommandEvent& event )
{
	try
	{
		m_RenderWindow->EnableAxis( event.IsChecked() );
		UpdateMenu();
	}
	coreCatchExceptionsReportAndNoThrowMacro(MultiRenderWindowEventHandler::OnMenuEnableAxis)
}

void Core::Widgets::MultiRenderWindowEventHandler::OnMenuShowAnnotationCubeMenuItem( wxCommandEvent& event )
{
	try
	{
		// Change the state
		m_RenderWindow->EnableAnnotatedCube( 
			!m_RenderWindow->GetMetadata( )->GetTagValue<bool>( "AnnotationCubeShown" ) );

		UpdateMenu();
	}
	coreCatchExceptionsReportAndNoThrowMacro(MultiRenderWindowEventHandler::OnMenuShowAnnotationCubeMenuItem)
}


void Core::Widgets::MultiRenderWindowEventHandler::OnMenuShowAnnotationMenuItem( 
	wxCommandEvent& event )
{
	try
	{
		m_RenderWindow->EnableCornerAnnotations( event.GetInt() );
		UpdateMenu();
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		MultiRenderWindowEventHandler::OnMenuShowAnnotationMenuItem)
}

void Core::Widgets::MultiRenderWindowEventHandler::OnMenuEnableLevelWindowInteractor( 
	wxCommandEvent& event )
{
	try
	{
		m_RenderWindow->EnableWindowLevelInteractor( event.GetInt() );
		UpdateMenu();
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		MultiRenderWindowEventHandler::OnMenuEnableLevelWindowInteractor)
}

void Core::Widgets::MultiRenderWindowEventHandler::UpdateMenu()
{
	GetViewMenu( )->Check( wxID_LockAxis, 
		m_RenderWindow->GetMetadata()->GetTagValue<bool>( "AxisLocked" ) );
	GetViewMenu( )->Check( wxID_ShowAnnotationCubeMenuItem, 
		m_RenderWindow->GetMetadata()->GetTagValue<bool>( "AnnotationCubeShown" ) );
	GetViewMenu( )->Check( wxID_ShowCornerAnnotationsMenuItem, 
		m_RenderWindow->GetShowCornerAnnotations( ) );
	GetViewMenu( )->Check( wxID_ViewLinkPlanes, 
		m_RenderWindow->GetMetadata()->GetTagValue<bool>( "LinkPlanesEnabled" ) );
	GetViewMenu( )->Check( wxID_LevelWindowInteractor, 
		m_RenderWindow->GetMetadata()->GetTagValue<bool>( "LevelWindowInteractor" ) );

	// If it's single, check current window
	int layoutType = m_RenderWindow->GetMetadata()->GetTagValue<int>( "LayoutType" );
	if ( layoutType == mitk::Single )
	{
		mitk::wxMitkMultiRenderWindow::WidgetListType widgets;
		widgets = m_RenderWindow->GetWidgets();
		mitk::wxMitkMultiRenderWindow::WidgetListType::iterator it;
		int count = 0;
		for ( it = widgets.begin() ; it != widgets.end() ; it++ )
		{
			if ( (*it)->IsShown() )
			{
				switch(count)
				{
				case mitk::ThreeD_Win:layoutType = mitk::Single3D;break;
				case mitk::X_Win:layoutType = mitk::Single2D_X;break;
				case mitk::Y_Win:layoutType = mitk::Single2D_Y;break;
				case mitk::Z_Win:layoutType = mitk::Single2D_Z;break;
				}
			}
			count++;
		}
	}

	switch ( layoutType )
	{
	case mitk::NoneLayout:GetViewMenu( )->Check( wxID_ViewLayoutDefault, true );break;
	case mitk::Default2x2:GetViewMenu( )->Check( wxID_ViewLayoutDefault, true );break;
	case mitk::Left2DRight3D:GetViewMenu( )->Check( wxID_ViewLayout2DViewsLeft, true );break;
	case mitk::Single3D:GetViewMenu( )->Check( wxID_ViewLayoutBig3DView, true );break;
	case mitk::Single2D_X:GetViewMenu( )->Check( wxID_ViewLayoutXView, true );break;
	case mitk::Single2D_Y:GetViewMenu( )->Check( wxID_ViewLayoutYView, true );break;
	case mitk::Single2D_Z:GetViewMenu( )->Check( wxID_ViewLayoutZView, true );break;
	case mitk::Only2D:GetViewMenu( )->Check( wxID_ViewLayoutOnly2DViews, true );break;
	case mitk::Default2x2_YXZ3D:GetViewMenu( )->Check( wxID_ViewLayoutDefault2x2_YXZ3D, true );break;
	case mitk::Default2x2_XYZ3D:GetViewMenu( )->Check( wxID_ViewLayoutDefault2x2_XYZ3D, true );break;
	default:break;
	}

}

wxMenu* Core::Widgets::MultiRenderWindowEventHandler::GetViewMenu()
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();
	wxMenuBar* menuBar = gIface->GetMainWindow()->GetMenuBar();

	int menuPos = menuBar->FindMenu(_U("View"));
	if(menuPos == wxNOT_FOUND)
	{
		return NULL;
	}

	wxMenu* menuView = menuBar->GetMenu(menuPos);
	return menuView;
}

void Core::Widgets::MultiRenderWindowEventHandler::OnMenuViewLinkPlanesItem( wxCommandEvent& event )
{
	m_RenderWindow->SetPlaneRotationLinked( event.IsChecked() );
	UpdateMenu();
}

void Core::Widgets::MultiRenderWindowEventHandler::OnMenuViewShowLabelsItem( wxCommandEvent& event )
{
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	Core::DataEntityHolder::Pointer holder;
	holder = dataContainer->GetDataEntityList()->GetSelectedDataEntityHolder();

	Core::DataTreeMITKHelper::ChangeShowLabelsProperty(  
		holder->GetSubject(),
		m_RenderWindow->GetPrivateRenderingTree(),  
		event.IsChecked( ) );

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
	UpdateMenu();
}

void Core::Widgets::MultiRenderWindowEventHandler::OnMenuViewLayout( wxCommandEvent& event )
{
	mitk::LayoutConfiguration layout;

	if ( event.GetId() == wxID_ViewLayout2DViewsLeft )
	{
		layout = mitk::Left2DRight3D;
	}else if ( event.GetId() == wxID_ViewLayoutDefault )
	{
		layout = mitk::Default2x2;
	}else if ( event.GetId() == wxID_ViewLayoutOnly2DViews )
	{
		layout = mitk::Only2D;
	}else if ( event.GetId() == wxID_ViewLayoutXView )
	{
		layout = mitk::Single2D_X;
	}else if ( event.GetId() == wxID_ViewLayoutYView )
	{
		layout = mitk::Single2D_Y;
	}else if ( event.GetId() == wxID_ViewLayoutZView )
	{
		layout = mitk::Single2D_Z;
	}else if ( event.GetId() == wxID_ViewLayoutBig3DView ) 
	{
		layout = mitk::Single3D;
	}else if ( event.GetId( ) == wxID_ViewLayoutDefault2x2_YXZ3D )
	{
		layout = mitk::Default2x2_YXZ3D;
	}else if ( event.GetId( ) == wxID_ViewLayoutDefault2x2_XYZ3D )
	{
		layout = mitk::Default2x2_XYZ3D;
	}

	m_RenderWindow->SetCurrentLayout( layout );

	UpdateMenu();
}

void Core::Widgets::MultiRenderWindowEventHandler::SetEvtHandlerEnabled( bool enabled )
{
	wxEvtHandler::SetEvtHandlerEnabled( enabled );

	if ( enabled )
	{
		UpdateMenu( );
	}
}

