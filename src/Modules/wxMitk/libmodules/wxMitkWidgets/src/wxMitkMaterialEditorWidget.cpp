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

#include "wxMitkMaterialEditorWidget.h"
#include "wxMitkMaterialSettingsPanel.h"
#include "wxMitkSphereViewCanvas.h"
#include "wxMitkSurfaceLightingControl.h"
#include "wxMitkSurfaceRepresentationControl.h"
#include "wxMitkScalarsArrayControl.h"
#include "wxMitkRenderWindow.h"
#include "wxID.h"
#include "mitkRenderingManager.h"
#include "vtkDataSetAttributes.h"
#include "vtkSmartPointer.h"
#include "vtkCellData.h"
#include "vtkPointData.h"
#include "vtkDataSet.h"
#include "vtkPolyData.h"

#include "vtkLookupTable.h"
#include "blLookupTables.h"
#include "blMITKUtils.h"
#include "mitkLookupTable.h"
#include "mitkLookupTableProperty.h"
#include "mitkVtkScalarModeProperty.h"
#include "mitkSurface.h"

using namespace mitk;

#define wxID_scalarArray (5 + wxID_HIGHEST)
#define wxID_chkShowLighting wxID( "wxID_chkShowLighting" )
#define wxID_chkShowScalars wxID( "wxID_chkShowScalars" )
#define wxID_MeshNotebook wxID("wxID_MeshNotebook")


BEGIN_EVENT_TABLE(wxMitkMaterialEditorWidget, wxPanel)
	EVT_COMMAND( wxID_ANY, wxEVT_MATERIALSETTINGS_REPRESENTATION_CHANGED, wxMitkMaterialEditorWidget::OnRepresentationChange )
	EVT_WIDGET_UPDATE(wxID_scalarArray, wxMitkMaterialEditorWidget::OnScalarArrayChange)
END_EVENT_TABLE()

//!
wxMitkMaterialEditorWidget::wxMitkMaterialEditorWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name), 
m_materialProperty(NULL) 
{
	// Create widgets
	SetId(wxID_ANY);
	wxSize parentSize = parent->GetClientSize();
	parentSize.y = -1;
	m_MeshNotebook = new wxNotebook(this, wxID_MeshNotebook, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	m_pMaterialSettingsPane = new wxMitkMaterialSettingsPanel( m_MeshNotebook, wxID_ANY );

	m_pMaterialSettingsPane->Connect( wxMitkMaterialSettingsPanel::wxID_lighting, wxEVT_WIDGET_UPDATE,
	    wxMitkWidgetUpdateEventHandler( wxMitkMaterialEditorWidget::OnLightingChange ), NULL, this );

	m_ScalarsNotebookPane = new wxPanel(m_MeshNotebook, wxID_ANY);

	m_scalarArray = new wxMitkScalarsArrayControl(m_ScalarsNotebookPane, wxID_scalarArray);
	
	m_vLayoutScalar = new wxBoxSizer(wxVERTICAL);
	m_vLayoutScalar->AddSpacer(20);
	m_vLayoutScalar->Add(m_scalarArray, 0, wxEXPAND | wxALL, 4);

	m_MeshNotebook->AddPage( m_pMaterialSettingsPane, "Lighting settings" );
	m_ScalarsNotebookPane->SetSizer(m_vLayoutScalar);
	m_MeshNotebook->AddPage(m_ScalarsNotebookPane,"Scalars settings");

	wxBoxSizer* hLayoutSizer = new wxBoxSizer( wxHORIZONTAL );
	hLayoutSizer->Add( m_MeshNotebook, 1, wxEXPAND | wxALL, 3 );
	SetSizer( hLayoutSizer );
	hLayoutSizer->Fit( this );

	m_node = mitk::DataTreeNode::New();

	m_renderWindow = NULL;
}

//!
wxMitkMaterialEditorWidget::~wxMitkMaterialEditorWidget(void)
{
}

//!
void wxMitkMaterialEditorWidget::SetDataTreeNode(mitk::DataTreeNode::Pointer node)
{
	if ( m_node == node )
	{
		return;
	}

	m_node = node;
	mitk::MaterialProperty::Pointer material = NULL;
	if(node.IsNotNull())
	{
		material = static_cast<mitk::MaterialProperty*>(node->GetProperty("material"));
		SetDefaultScalarArray( );
		m_scalarArray->SetDataTreeNode(node);
	}
	else
	{
		m_scalarArray->SetDataTreeNode(NULL);
		//m_scalarArray->ResetWidget();
	}
	SetMaterial(material);
}

//!
void wxMitkMaterialEditorWidget::SetDefaultScalarArray()
{
	if(m_node.IsNull())
	{
		return;
	}

	mitk::Surface* surfacedata = dynamic_cast<mitk::Surface*>(m_node->GetData());
	vtkDataSet* data = dynamic_cast<vtkDataSet*>(surfacedata->GetVtkPolyData(0));

	std::vector<blShapeUtils::ShapeUtils::VTKScalarType> scalarnames;
	blShapeUtils::ShapeUtils::GetScalarsVector( data, scalarnames );

	m_scalarArray->SetScalarArrayTypes( scalarnames, true );
}


//!
void wxMitkMaterialEditorWidget::SetMaterial(MaterialProperty::Pointer material)
{
	m_materialProperty = material;
	m_pMaterialSettingsPane->SetMaterialProperty( m_materialProperty );
}

//!
MaterialProperty::Pointer wxMitkMaterialEditorWidget::GetMaterial(void) const
{
	return m_materialProperty;
}

//!
void wxMitkMaterialEditorWidget::ApplyMaterialProperty(void)
{
	// we have to APPLY (copy the backup material over the real one)
	if(m_materialProperty.IsNotNull())
	{
		mitk::RenderingManager::GetInstance()->SetNextLOD(0);
		mitk::RenderingManager::GetInstance()->RequestUpdateAll();
	}
}

//!
void wxMitkMaterialEditorWidget::OnLightingChange(wxMitkWidgetUpdateEvent& WXUNUSED(event))
{
	m_pMaterialSettingsPane->UpdateMaterial();
	ApplyMaterialProperty();
}

//!
void wxMitkMaterialEditorWidget::OnRepresentationChange( wxCommandEvent& WXUNUSED(event) )
{
	ApplyMaterialProperty();
}

//!
void wxMitkMaterialEditorWidget::OnScalarArrayChange(wxMitkWidgetUpdateEvent& WXUNUSED(event))
{
	blShapeUtils::ShapeUtils::VTKScalarType type  = m_scalarArray->GetCurrentSelectedScalarType();
	blMITKUtils::ApplyLookupTableToSurface(
		m_node,
		blLookupTables::LUT_TYPE_RAINBOW_HUE_INVERTED,
		type.name,
		type.mode);
	
	
	// Now update the scenes
	ApplyMaterialProperty();
}

void wxMitkMaterialEditorWidget::SetMitkRenderWindow( mitk::wxMitkRenderWindow* renderWindow )
{
	m_renderWindow = renderWindow;
	m_scalarArray->SetMitkRenderWindow( renderWindow );
}

void mitk::wxMitkMaterialEditorWidget::UpdateLayout( )
{
//	GetSizer( )->Show(m_vLayoutLighting, true, true );
//	GetSizer( )->Show(m_vLayoutScalar, true, true );

//	FitInside();
	//if ( GetParent() && GetParent()->GetParent())
	//{
	//	GetParent()->GetParent()->FitInside();
	//}
}
