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

#include "wxMitkScalarsArrayControl.h"
#include "wxWidgetStackControl.h"
#include <wx/spinctrl.h>
#include "mitkRenderingManager.h"
#include <wxUnicode.h>
#include "wxMitkLookupTableWidget.h"

#include "blMITKUtils.h"

using namespace mitk;

#define ID_cmbScalarArray (1 + wxID_HIGHEST)
#define ID_LookupTable (2 + wxID_HIGHEST )
#define ID_btnLUTRange (3 + wxID_HIGHEST )
#define ID_btnAutomaticLUTRange (4 + wxID_HIGHEST )


BEGIN_EVENT_TABLE(wxMitkScalarsArrayControl, wxPanel)
	EVT_CHOICE(ID_cmbScalarArray, wxMitkScalarsArrayControl::OnScalarArraySelected)
	EVT_BUTTON(ID_btnLUTRange, wxMitkScalarsArrayControl::OnSetRangeSelected)
	EVT_BUTTON(ID_btnAutomaticLUTRange, wxMitkScalarsArrayControl::OnSetAutomaticRangeSelected)
END_EVENT_TABLE()


//!
wxMitkScalarsArrayControl::wxMitkScalarsArrayControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name)
{
	// Generate the list of available representations
	wxString items[1];
	items[0] = wxT("");
	
	wxStaticText* lblScalarArray = new wxStaticText(this, wxID_ANY, wxT("Scalar Array"));
	m_cmbScalarArray = new wxChoice(this, ID_cmbScalarArray, wxDefaultPosition, wxDefaultSize, 1, items);
	m_LookupTableWidget = new wxMitkLookupTableWidget(this, ID_LookupTable);
	m_lblMinRange = new wxStaticText(this, wxID_ANY, wxT("Min"));
	m_lblMaxRange = new wxStaticText(this, wxID_ANY, wxT("Max"));
	m_txtMinRange = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_txtMaxRange = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_rangeBtn = new wxButton(this, ID_btnLUTRange, wxT("Set Range"));
	m_automaticRangeBtn = new wxButton(this, ID_btnAutomaticLUTRange, wxT("Automatic Range"));

	m_LookupTableWidget->Enable( true );
	m_LookupTableWidget->EnableUseColor(false );
	

	// Layout items
	wxBoxSizer* boxLayout = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(lblScalarArray);
	sizer->Add(m_cmbScalarArray, 1, wxEXPAND| wxALIGN_CENTER_VERTICAL);
	
	wxBoxSizer* rangesizer1 = new wxBoxSizer(wxHORIZONTAL);
	rangesizer1->Add(m_lblMinRange);
	rangesizer1->Add(m_txtMinRange, 1);
	wxBoxSizer* rangesizer2 = new wxBoxSizer(wxHORIZONTAL);
	rangesizer2->Add(m_lblMaxRange);
	rangesizer2->Add(m_txtMaxRange, 1);
	wxBoxSizer* rangesizer3 = new wxBoxSizer(wxHORIZONTAL);
	rangesizer3->Add(m_rangeBtn, 1);
	
	boxLayout->Add( sizer, 0, wxEXPAND );
	boxLayout->Add( rangesizer1, 0,wxEXPAND );
	boxLayout->Add( rangesizer2, 0,wxEXPAND );
	boxLayout->Add( rangesizer3, 0,wxEXPAND );
	boxLayout->Add ( m_automaticRangeBtn, 0, wxEXPAND);
	boxLayout->Add( m_LookupTableWidget, 0,wxEXPAND | wxALL, 4);

	SetSizer(boxLayout);

	m_node = NULL;

}


//!
wxMitkScalarsArrayControl::~wxMitkScalarsArrayControl(void)
{
}


//!
void wxMitkScalarsArrayControl::SetScalarArrayTypes(
	blShapeUtils::ShapeUtils::ScalarArrayType& scalar,
	bool bAddSolidColorArrayName )
{
	int NumOfScalars = scalar.size();
	m_currentScalarArray.resize(NumOfScalars + 1);
	m_cmbScalarArray->Clear();
	//wxString items;
	blShapeUtils::ShapeUtils::ScalarArrayType::iterator it;
	//wxMitkScalarsArrayControl::ScalarArrayType::iterator curr_it;
	int i = 0;
	for ( it = scalar.begin(); it!= scalar.end(); it++ )
	{
		//items[i] = _U((*it).name);
		m_cmbScalarArray->Append(_U((*it).name));
		m_currentScalarArray[i].name = (*it).name;
		m_currentScalarArray[i].mode = (*it).mode;
		i++;
	}

	if ( bAddSolidColorArrayName )
	{
		//items[i] = wxT("SolidColor");
		m_cmbScalarArray->Append(wxT("SolidColor"));
		m_currentScalarArray[i].name = "SolidColor";
		m_currentScalarArray[i].mode =  blShapeUtils::ShapeUtils::SCALAR_ARRAY_NONE;
	}
	
}

/**
Returns the currents selected representation type.
If nothing was selected, returns NONE.
*/

blShapeUtils::ShapeUtils::VTKScalarType wxMitkScalarsArrayControl::GetCurrentSelectedScalarType()
{
	return m_selectedScalarType;
}

//!
int wxMitkScalarsArrayControl::GetScalarArrayIndex(void) const
{
	return m_cmbScalarArray->GetSelection();
}


//!
void wxMitkScalarsArrayControl::OnScalarArraySelected(wxCommandEvent& WXUNUSED(event))
{ 
	int sel = m_cmbScalarArray->GetSelection();
	if ( m_currentScalarArray.size() == 0 )
	{
		return;
	}

	m_selectedScalarType.name = m_currentScalarArray[sel].name;
	m_selectedScalarType.mode = m_currentScalarArray[sel].mode;
	m_LookupTableWidget->SetScalarType( m_selectedScalarType );
	SetAutomaticRange();
	// Create a command event and configure it
	wxMitkWidgetUpdateEvent eventUpdate(GetId());
	eventUpdate.SetEventObject(this);
	// Cast the event
	GetEventHandler()->ProcessEvent(eventUpdate);
}

void wxMitkScalarsArrayControl::OnSetRangeSelected(wxCommandEvent& WXUNUSED(event))
{ 
	double range[2];
	m_txtMinRange->GetValue().ToDouble( &range[0] );
	m_txtMaxRange->GetValue().ToDouble( &range[1] );

	blMITKUtils::SetVtkLutScalarRangeForMesh(m_node,range);
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( );
}

void wxMitkScalarsArrayControl::OnSetAutomaticRangeSelected(wxCommandEvent& WXUNUSED(event))
{ 
	SetAutomaticRange();
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( );
}

bool mitk::wxMitkScalarsArrayControl::SetSelectedArray( const char *arrayName )
{
	bool bFound = false;

	for ( int i = 0 ; i < m_currentScalarArray.size() ; i++ )
	{
		if ( m_currentScalarArray[ i ].name == std::string( arrayName ) )
		{
			m_selectedScalarType = m_currentScalarArray[ i ];
			m_cmbScalarArray->SetSelection( i );
			bFound = true;
		}
	}

	return bFound;
}

void mitk::wxMitkScalarsArrayControl::SetAutomaticRange( )
{
	if (m_node.IsNull()) return;

	blMITKUtils::SetScalarMode(
		m_node,
		GetCurrentSelectedScalarType().name,
		GetCurrentSelectedScalarType().mode);

	float range[2];
	m_node->GetPropertyValue("ScalarsRangeMaximum", range[1]);
	m_node->GetPropertyValue("ScalarsRangeMinimum", range[0]);
	m_txtMaxRange->SetValue( wxString::Format(wxT("%.2f"), range[1] ) );
	m_txtMinRange->SetValue( wxString::Format(wxT("%.2f"), range[0] ) );

}

void mitk::wxMitkScalarsArrayControl::SetDataTreeNode( mitk::DataTreeNode::Pointer node )
{
	m_node = node;
	m_LookupTableWidget->SetDataTreeNode(node);
}

void mitk::wxMitkScalarsArrayControl::SetMitkRenderWindow( mitk::wxMitkRenderWindow* renderWindow )
{
	m_LookupTableWidget->SetMitkRenderWindow( renderWindow );
}

void mitk::wxMitkScalarsArrayControl::ShowAdvancedControls( bool show )
{
	GetSizer( )->Show( m_rangeBtn, show, true );
	GetSizer( )->Show( m_automaticRangeBtn, show, true );
	GetSizer( )->Show( m_lblMinRange, show, true );
	GetSizer( )->Show( m_txtMinRange, show, true );
	GetSizer( )->Show( m_lblMaxRange, show, true );
	GetSizer( )->Show( m_txtMaxRange, show, true );
	GetSizer( )->Show( m_LookupTableWidget, show, true );
}
