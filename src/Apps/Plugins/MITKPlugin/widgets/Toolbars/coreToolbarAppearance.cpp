/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreToolbarAppearance.h"
#include "corePluginTab.h"
#include "coreDataEntityListBrowser.h"
#include "coreMultiRenderWindowMITK.h"
#include "mitkProperties.h"
#include "blMitkScalarBar.h"
#include "wxMitkSelectableGLWidget.h"
#include "wxMitkRenderWindow.h"

//#include ".xpm"
#include "LUT.xpm"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::ToolbarAppearance, wxToolBar)
  EVT_COMBOBOX(wxID_toolSelectLUT, Core::Widgets::ToolbarAppearance::OnSelectLUT)
  EVT_TOOL(wxID_toolShowLUT, Core::Widgets::ToolbarAppearance::OnShowLUT)
  EVT_COMMAND_SCROLL(wxID_toolOpacity, Core::Widgets::ToolbarAppearance::OnOpacity)
END_EVENT_TABLE()


Core::Widgets::ToolbarAppearance::ToolbarAppearance(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxToolBar(parent, id, pos, size, style, name)
{
	wxBitmap bitmap;

	bitmap = wxBitmap( lut_xpm );
	AddTool(wxID_toolShowLUT, _T("Show/hide LUT"),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		_T("Show/hide LUT"), _T("Show/hide LUT"));

	// Init m_ComboLUT
	wxString m_ComboLUT_choices[ blLookupTables::LUT_TYPE_MAX ];
	for ( int i = 0 ; i < blLookupTables::GetNumberOfLuts(); i++ )
	{
		blLookupTables::LUT_TYPE lutType = blLookupTables::LUT_TYPE( i );
		m_ComboLUT_choices[ i ] = wxT( blLookupTables::GetLUTName( lutType ) );
	}
	m_ComboLUT = new wxComboBox(
		this, 
		wxID_toolSelectLUT, 
		wxT(""), 
		wxDefaultPosition, 
		wxDefaultSize, 
		blLookupTables::LUT_TYPE_MAX, 
		m_ComboLUT_choices, 
		wxCB_DROPDOWN|wxCB_SIMPLE|wxCB_DROPDOWN|wxCB_READONLY);
	m_ComboLUT->SetSelection( 0 );
	AddControl( m_ComboLUT );

	// Opacity
	m_OpacitySlider = new wxSlider(this, wxID_toolOpacity, 100, 0, 100, 
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_AUTOTICKS);
	m_OpacitySlider->SetTickFreq( 25, 1 );
	AddControl( m_OpacitySlider );

	Realize();

	m_NodeTagModified = 0;
	m_RenderingNode = NULL;

	Enable( GetMultiRenderWindowMITK() );
}

Core::Widgets::ToolbarAppearance::~ToolbarAppearance( )
{
	if ( m_RenderingNode )
	{
		m_RenderingNode->GetPropertyList()->RemoveObserver( m_NodeTagModified );
	}
}

Core::BaseProcessor::Pointer Core::Widgets::ToolbarAppearance::GetProcessor()
{
	return NULL;
}


void Core::Widgets::ToolbarAppearance::UpdateState()
{
	if ( m_RenderingNode == NULL )
	{
		return;
	}

	// Get selected lookup table
	mitk::IntProperty::Pointer blLookupTablesTypeProperty;
	blLookupTablesTypeProperty = static_cast<mitk::IntProperty*>(m_RenderingNode->GetProperty("blLookupTablesType"));
	if ( blLookupTablesTypeProperty.IsNotNull() )
	{
		m_ComboLUT->SetSelection( blLookupTablesTypeProperty->GetValue() );
	}

	// Opacity
	float opacity;
	m_RenderingNode->GetOpacity(opacity, NULL);
	m_OpacitySlider->SetValue(opacity*100);

	// Show / Hide LUT
	if ( GetMultiRenderWindow() )
	{
		blMitkScalarBar::Pointer scalarBar;
		scalarBar = GetMultiRenderWindowMITK()->Get3D( )->GetMitkRenderWindow( )->GetScalarBar();
		bool enabled = scalarBar->IsEnabled( );
		mitk::DataTreeNode* node = scalarBar->GetDataTreeNode( );
		enabled = enabled && ( node == m_RenderingNode );
		ToggleTool( wxID_toolShowLUT, enabled );
	}
}

void Core::Widgets::ToolbarAppearance::OnSelectLUT( wxCommandEvent &event )
{
	//Different lookup tables in the combo
	blLookupTables::LUT_TYPE lutType;
	lutType = blLookupTables::LUT_TYPE( m_ComboLUT->GetCurrentSelection() );

	blShapeUtils::ShapeUtils::VTKScalarType scalarType;
	scalarType.name = "";
	scalarType.mode = blShapeUtils::ShapeUtils::SCALAR_ARRAY_NONE;
	blMITKUtils::ApplyLookupTable( true, m_RenderingNode, lutType, scalarType);

	mitk::RenderingManager::GetInstance( )->RequestUpdateAll();
}

void Core::Widgets::ToolbarAppearance::OnShowLUT( wxCommandEvent &event )
{
	if ( GetMultiRenderWindow() && m_RenderingNode != NULL )
	{
		GetMultiRenderWindowMITK()->Get3D( )->GetMitkRenderWindow( )->EnableColorLegend( 
			event.IsChecked(), "", m_RenderingNode );
	}
}

void Core::Widgets::ToolbarAppearance::OnOpacity( wxScrollEvent &event )
{
	if ( m_RenderingNode == NULL )
	{
		return;
	}

	float sliderValue = static_cast<float>(m_OpacitySlider->GetValue());
	m_RenderingNode->SetOpacity(sliderValue/100);
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void Core::Widgets::ToolbarAppearance::SetListBrowser( Core::Widgets::DataEntityListBrowser* val )
{
	BaseWindow::SetListBrowser( val );

	if ( GetListBrowser( ) )
	{
		GetListBrowser( )->GetSelectedDataEntityHolder( )->AddObserver( 
			this,
			&ToolbarAppearance::OnSelectedModified );
	}
}

void Core::Widgets::ToolbarAppearance::OnSelectedModified()
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = GetListBrowser( )->GetSelectedDataEntityHolder( )->GetSubject();

	mitk::DataTreeNode::Pointer renderingNode;
	if ( dataEntity.IsNotNull() && GetListBrowser( )->GetRenderingTree().IsNotNull( ) )
	{
		boost::any anydata = GetListBrowser( )->GetRenderingTree()->GetNode( dataEntity );
		Core::CastAnyProcessingData( anydata, renderingNode );
	}
	SetRenderingNode( renderingNode );

	UpdateState();
}

void Core::Widgets::ToolbarAppearance::SetRenderingNode( mitk::DataTreeNode* renderingNode )
{
	if ( m_RenderingNode )
	{
		m_RenderingNode->GetPropertyList()->RemoveObserver( m_NodeTagModified );
	}

	m_RenderingNode = renderingNode;

	if ( m_RenderingNode )
	{
		mitk::BaseData* data = dynamic_cast<mitk::BaseData*> ( m_RenderingNode->GetData() );
		if ( data == NULL )
		{
			m_ComboLUT->Enable( false );
		}

		mitk::Image* image = dynamic_cast<mitk::Image*> ( data );
		if ( image == NULL )
		{
			m_ComboLUT->Enable( false );
			return;
		}

		m_ComboLUT->Enable( true );

		itk::SimpleMemberCommand<ToolbarAppearance>::Pointer command;

		// Add an observer to the node, to remove the actor when the node is deleted
		command = itk::SimpleMemberCommand<ToolbarAppearance>::New();
		command->SetCallbackFunction(
			this,
			&ToolbarAppearance::OnNodeModified );
		m_NodeTagModified = m_RenderingNode->GetPropertyList()->AddObserver(
			itk::ModifiedEvent(), command);
	}
}

void Core::Widgets::ToolbarAppearance::OnNodeModified()
{
	UpdateState();
}

Core::Widgets::MultiRenderWindowMITK* Core::Widgets::ToolbarAppearance::GetMultiRenderWindowMITK()
{
	return dynamic_cast<MultiRenderWindowMITK*> ( GetMultiRenderWindow() );
}

void Core::Widgets::ToolbarAppearance::SetMultiRenderWindow( Core::Widgets::RenderWindowBase* val )
{
	BaseWindow::SetMultiRenderWindow( val );
	Enable( GetMultiRenderWindowMITK() );
}
