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

#include "wxMitkLookupTableWidget.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkColorSelectorControl.h"

#include <mitkDataTreeNode.h>
#include <mitkRenderingManager.h>

#include "vtkSmartPointer.h"

#include "wxID.h"

#include "blMITKUtils.h"

using namespace mitk;

#define wxID_wxMitkLookupTableWidget_COM_COLOR wxID("wxID_wxMitkLookupTableWidget_COM_COLOR")
#define wxID_wxMitkLookupTableWidget_USE_COLOR wxID("wxID_wxMitkLookupTableWidget_USE_COLOR")
#define wxID_wxMitkLookupTableWidget_SHOW_LEGEND wxID("wxID_wxMitkLookupTableWidget_SHOW_LEGEND")
#define wxID_wxMitkLookupTableWidget_ColorSelector wxID("wxID_wxMitkLookupTableWidget_ColorSelector")
#define wxID_wxMitkLookupTableWidget_BackgroundColorSelector wxID("wxID_wxMitkLookupTableWidget_BackgroundColorSelector")

// Declare events to process
BEGIN_EVENT_TABLE(wxMitkLookupTableWidget, wxPanel)
	EVT_CHECKBOX	(wxID_wxMitkLookupTableWidget_USE_COLOR, wxMitkLookupTableWidget::OnUseColor)
	EVT_COMBOBOX	(wxID_wxMitkLookupTableWidget_COM_COLOR,wxMitkLookupTableWidget::OnComboLUTModified )
	EVT_CHECKBOX	(wxID_wxMitkLookupTableWidget_SHOW_LEGEND, wxMitkLookupTableWidget::OnShowLegend)
	EVT_COLOURPICKER_CHANGED	(wxID_wxMitkLookupTableWidget_ColorSelector, wxMitkLookupTableWidget::OnColorChanged)
	EVT_COLOURPICKER_CHANGED	(wxID_wxMitkLookupTableWidget_BackgroundColorSelector, wxMitkLookupTableWidget::OnBackgroundColorChanged)
END_EVENT_TABLE()

//!
wxMitkLookupTableWidget::wxMitkLookupTableWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) 
: wxPanel(parent, id, pos, size, style, name)
{
	m_bEnableUserColor = true;
	m_renderWindow = NULL;
	m_node = NULL;
	m_NodeTagModified = 0;

	wxString m_ComboLUT_choices[ blLookupTables::LUT_TYPE_MAX ];
	for ( int i = 0 ; i < blLookupTables::GetNumberOfLuts(); i++ )
	{
		blLookupTables::LUT_TYPE lutType = blLookupTables::LUT_TYPE( i );
		m_ComboLUT_choices[ i ] = wxT( blLookupTables::GetLUTName( lutType ) );
	}
	m_chkUseColor = new wxCheckBox(
		this, 
		wxID_wxMitkLookupTableWidget_USE_COLOR, 
		wxT("Use color"));

	m_ColorSelector = new wxMitkColorSelectorControl(
		this, 
		wxID_wxMitkLookupTableWidget_ColorSelector);


	wxColour blk(0,0,0);
	m_BackgroundColorSelector = new wxMitkColorSelectorControl(
		this, 
		wxID_wxMitkLookupTableWidget_BackgroundColorSelector,blk);

	// Don't sort the list
	m_ComboLUT = new wxComboBox(
		this, 
		wxID_wxMitkLookupTableWidget_COM_COLOR, 
		wxT(""), 
		wxDefaultPosition, 
		wxDefaultSize, 
		blLookupTables::LUT_TYPE_MAX, 
		m_ComboLUT_choices, 
		wxCB_DROPDOWN|wxCB_SIMPLE|wxCB_DROPDOWN|wxCB_READONLY);
	m_ComboLUT->SetSelection( 0 );
	m_chkShowLegend= new wxCheckBox(
		this, 
		wxID_wxMitkLookupTableWidget_SHOW_LEGEND, 
		wxT("Show legend"));

	// Set default state
	m_ColorSelector->Enable( true );
	m_BackgroundColorSelector->Enable(true);
	m_chkUseColor->SetValue( true );
	m_ComboLUT->Enable( false );
	m_chkShowLegend->Enable( false );

	// Layout the canvases
	wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
	layout->Add( m_chkUseColor, 0, wxEXPAND );
	layout->Add( m_ColorSelector, 0, wxEXPAND );

	wxStaticText *backgroundColorLabel = new wxStaticText(this, -1, wxT("Background Color"));
	layout->AddSpacer(8);
	layout->Add( backgroundColorLabel, 0, wxEXPAND );
	layout->Add( m_BackgroundColorSelector, 0, wxEXPAND );
	layout->AddSpacer(8);
	wxStaticText *ComboLut = new wxStaticText(this, -1, wxT("Lookup Tables"));
	layout->Add( ComboLut, 0, wxEXPAND );
	layout->Add(m_ComboLUT, 0, wxEXPAND);
	layout->AddSpacer(8);
	layout->Add(m_chkShowLegend, 0, wxEXPAND);
	SetSizer(layout);

	CleanWidgets();

}

//!
wxMitkLookupTableWidget::~wxMitkLookupTableWidget(void)
{
	if ( m_node )
	{
		m_node->GetPropertyList()->RemoveObserver( m_NodeTagModified );
	}
}

//!
void wxMitkLookupTableWidget::ResetWidget(void)
{
	SetDataTreeNode( NULL );
}

/**
This is the call for making the widget work with an image (in fact, a DataTreeNode that contains an image)
Manages a DataTreeNode, getting the image inside and computing its scalar histogram and gradient 
histogram. It also creates the TransferFunctionProperty for the given node and assigns it.
Also transfers all the computed histograms and data to the children canvases.
\note Design decision: all work of analyzing the image is done here, since it implies a lot of computations and we want this widget to be efficient and not to repeat code (like it would happen if we rely this work to the children canvases)
*/
void wxMitkLookupTableWidget::SetDataTreeNode(mitk::DataTreeNode* node)
{
	if ( m_node == node && m_node != NULL )
	{
		return;
	}

	if ( m_node )
	{
		m_node->GetPropertyList()->RemoveObserver( m_NodeTagModified );
	}

	m_node = node;

	if ( m_node )
	{
		itk::SimpleMemberCommand<wxMitkLookupTableWidget>::Pointer command;

		// Add an observer to the node, to remove the actor when the node is deleted
		command = itk::SimpleMemberCommand<wxMitkLookupTableWidget>::New();
		command->SetCallbackFunction(
			this,
			&wxMitkLookupTableWidget::UpdateWidgets );
		m_NodeTagModified = m_node->GetPropertyList()->AddObserver(
			itk::ModifiedEvent(), command);
	}

	UpdateWidgets( );

}

void wxMitkLookupTableWidget::OnComboLUTModified(wxCommandEvent& event)
{
	// Update the LUT
	UpdateData( );
}


void mitk::wxMitkLookupTableWidget::OnUseColor( wxCommandEvent& event )
{
	// Update lookup table
	UpdateData();

	UpdateWidgets();
}


void mitk::wxMitkLookupTableWidget::OnShowLegend( wxCommandEvent& event )
{
	//UpdateData();

	if ( m_renderWindow)/*  && m_bEnableUserColor)*/
	{
		m_renderWindow->EnableColorLegend( m_chkShowLegend->GetValue(), "", m_node );
	}

	UpdateWidgets();
}

void mitk::wxMitkLookupTableWidget::OnColorChanged( wxColourPickerEvent& WXUNUSED(event) )
{
	UpdateData( );

	UpdateWidgets();
}

void mitk::wxMitkLookupTableWidget::OnBackgroundColorChanged( wxColourPickerEvent& WXUNUSED(event) )
{
	
	//todo: use the rendermanager to change colour to all the renderers
	if ( m_renderWindow == NULL)
		return;

	vtkRenderWindow *renWin=this-> m_renderWindow->GetRenderWindow();
	vtkSmartPointer<vtkRenderer> vtkrenderer;
	vtkrenderer = mitk::BaseRenderer::GetInstance(renWin)->GetVtkRenderer();
	double rgb[3];
	this->m_BackgroundColorSelector->GetRGBColour(rgb);
	vtkrenderer->SetBackground(rgb[0],rgb[1],rgb[2]);

	UpdateData( );
	UpdateWidgets();
}


void mitk::wxMitkLookupTableWidget::SetMitkRenderWindow( mitk::wxMitkRenderWindow* renderWindow )
{
	m_renderWindow = renderWindow;
}

void mitk::wxMitkLookupTableWidget::EnableUseColor( bool bVal )
{
	m_bEnableUserColor = bVal;
	ResetWidget();
}

void mitk::wxMitkLookupTableWidget::UpdateWidgets()
{
	if ( m_node == NULL )
	{
		// If node was given NULL
		// Set default state, disable the widgets
		CleanWidgets();
		m_useColor = NULL;
		return;
	}

	if ( m_bEnableUserColor )
	{	
		// "use color"
		m_useColor = static_cast<mitk::BoolProperty*>(m_node->GetProperty("use color"));
		if(m_useColor.IsNotNull())
		{
			// Check the volume rendering checkbox
			m_chkUseColor->SetValue(m_useColor->GetValue());
		}

		// Update controls
		bool enableColor = m_chkUseColor->GetValue();
		m_ComboLUT->Enable( !enableColor );
		m_chkShowLegend->Enable( !enableColor );
	}

	// Get selected lookup table
	mitk::IntProperty::Pointer blLookupTablesTypeProperty;
	blLookupTablesTypeProperty = static_cast<mitk::IntProperty*>(m_node->GetProperty("blLookupTablesType"));
	if ( blLookupTablesTypeProperty.IsNotNull() )
	{
		m_ComboLUT->SetSelection( blLookupTablesTypeProperty->GetValue() );
	}

	// Update selected color
	float rgbColor[3];
	m_node->GetColor( rgbColor );
	double color[3];
	color[ 0 ] = rgbColor[ 0 ];
	color[ 1 ] = rgbColor[ 1 ];
	color[ 2 ] = rgbColor[ 2 ];
	m_ColorSelector->SetRGBColour( color );

	
	if ( m_renderWindow )
	{
		vtkSmartPointer<vtkRenderer> vtkrenderer;
		vtkrenderer = m_renderWindow->GetRenderer()->GetVtkRenderer();
		vtkrenderer->GetBackground(color);
		this->m_BackgroundColorSelector->SetRGBColour(color);
	}

	// Show / Hide LUT
	if ( m_renderWindow )
	{
		blMitkScalarBar::Pointer scalarBar;
		scalarBar = m_renderWindow->GetScalarBar();
		bool enabled = scalarBar->IsEnabled( );
		mitk::DataTreeNode* node = scalarBar->GetDataTreeNode( );
		enabled = enabled && ( node == m_node );
		m_chkShowLegend->SetValue( enabled );
	}

}

void mitk::wxMitkLookupTableWidget::UpdateData()
{
	UpdateLookupTableProp();
	
	double rgbColor[3];
	m_ColorSelector->GetRGBColour( rgbColor );
	m_node->SetColor( rgbColor[ 0 ], rgbColor[ 1 ], rgbColor[ 2 ] );

	// Disable legend if no LUT
	if ( m_chkUseColor->GetValue() )
	{
		if ( m_renderWindow )
		{
			m_renderWindow->EnableColorLegend( false, "", m_node );
		}
	}
	else
	{
		// Update legend status
		if ( m_renderWindow )
		{
			m_renderWindow->EnableColorLegend( m_chkShowLegend->GetValue(), "", m_node );
		}
	}

	mitk::RenderingManager::GetInstance()->RequestUpdateAll( );
}

void mitk::wxMitkLookupTableWidget::UpdateLookupTableProp()
{
	if (m_bEnableUserColor)
	{
		// "use color"
		m_useColor = static_cast<mitk::BoolProperty*>(m_node->GetProperty("use color"));
		if(m_useColor.IsNull())
		{
			m_useColor = mitk::BoolProperty::New(false);
			m_node->SetProperty("use color", m_useColor);
		}
		// Check the volume rendering checkbox
		m_useColor->SetValue( m_chkUseColor->GetValue( ) );
	}
	//Different lookup tables in the combo
	blLookupTables::LUT_TYPE lutType;
	lutType = blLookupTables::LUT_TYPE( m_ComboLUT->GetCurrentSelection() );
	blMITKUtils::ApplyLookupTable( !m_chkUseColor->GetValue( ), m_node, lutType, GetScalarType());
}

void mitk::wxMitkLookupTableWidget::SetScalarType( blShapeUtils::ShapeUtils::VTKScalarType scalarType )
{
	m_selectedScalarType.name = scalarType.name;
	m_selectedScalarType.mode = scalarType.mode;
}

blShapeUtils::ShapeUtils::VTKScalarType mitk::wxMitkLookupTableWidget::GetScalarType( )
{
	return m_selectedScalarType;
}

void mitk::wxMitkLookupTableWidget::CleanWidgets()
{
	if ( m_bEnableUserColor )
	{
		m_chkUseColor->Show();
		m_chkUseColor->SetValue( true );
		m_ColorSelector->Enable( true );
		m_ColorSelector->Show();
		m_ComboLUT->Enable( false );
		m_chkShowLegend->Enable( false );
	}
	else
	{
		m_chkUseColor->Hide();
		m_chkUseColor->SetValue( false );
		m_ColorSelector->Hide();
		m_ComboLUT->Enable( true );
		m_chkShowLegend->Enable( true );
	}

	if ( m_renderWindow )
	{
		m_renderWindow->EnableColorLegend( false );
	}

	FitInside( );
}