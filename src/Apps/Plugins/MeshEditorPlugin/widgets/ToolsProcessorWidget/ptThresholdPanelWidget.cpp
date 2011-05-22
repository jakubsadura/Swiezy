// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "ptThresholdPanelWidget.h"
#include "blShapeUtils.h"

#define wxID_scalarArray wxID( "wxID_scalarArray" )

ptThresholdPanelWidget::ptThresholdPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
	wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptThresholdPanelWidget::ptThresholdPanelWidget
    m_Threshold_staticbox = new wxStaticBox(this, -1, wxT("Surface Threshold"));
    m_labelLowerThreshold = new wxStaticText(this, wxID_ANY, wxT("Lower threshold"));
    m_LowerThreshold = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_llabelUpperThreshold = new wxStaticText(this, wxID_ANY, wxT("Upper threshold"));
    m_UpperThreshold = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_btnThreshold = new wxButton(this, wxID_MESH_THRESHOLD_BTN_APPLY, wxT("Apply Threshold"));

	m_processor = Core::ThresholdProcessor::New();

	m_ScalarArrayControl = new mitk::wxMitkScalarsArrayControl(this, wxID_scalarArray);
	m_ScalarArrayControl->ShowAdvancedControls( false );
	
	m_processor->GetInputDataEntityHolder( 0 )->AddObserver(
		this,
		&ptThresholdPanelWidget::OnModifiedInput );

	do_layout();
	UpdateWidget();
}

BEGIN_EVENT_TABLE(ptThresholdPanelWidget, wxPanel)
    // begin wxGlade: ptThresholdPanelWidget::event_table
    EVT_BUTTON(wxID_MESH_THRESHOLD_BTN_APPLY, ptThresholdPanelWidget::OnButtonThreshold)
    // end wxGlade
END_EVENT_TABLE();


void ptThresholdPanelWidget::OnButtonThreshold(wxCommandEvent &event)
{
	UpdateData();
	UpdateProcessor( );
}

void ptThresholdPanelWidget::UpdateData()
{
	// Update each parameter 
	m_processor->SetLowerThreshold( std::atof( m_LowerThreshold->GetValue()));
	m_processor->SetUpperThreshold( std::atof( m_UpperThreshold ->GetValue()));

	blShapeUtils::ShapeUtils::VTKScalarType scalarSelected;
	scalarSelected = m_ScalarArrayControl->GetCurrentSelectedScalarType( );
	m_processor->SetScalarsArrayName( scalarSelected.name.c_str() );
}

void ptThresholdPanelWidget::UpdateWidget()
{
	m_LowerThreshold->SetValue(wxString::Format("%f",m_processor->GetLowerThreshold()) );
	m_UpperThreshold->SetValue(wxString::Format("%f", m_processor->GetUpperThreshold()) );

	m_ScalarArrayControl->SetSelectedArray( m_processor->GetScalarsArrayName() );

} 

void ptThresholdPanelWidget::do_layout()
{
    // begin wxGlade: ptThresholdPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* m_Threshold = new wxStaticBoxSizer(m_Threshold_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    sizer_1->Add(m_labelLowerThreshold, 1, 0, 0);
    sizer_1->Add(m_LowerThreshold, 0, 0, 0);
    m_Threshold->Add(sizer_1, 0, wxALL|wxEXPAND, 2);
    sizer_2->Add(m_llabelUpperThreshold, 1, 0, 0);
    sizer_2->Add(m_UpperThreshold, 0, 0, 0);
    m_Threshold->Add(sizer_2, 0, wxALL|wxEXPAND, 2);
    m_Threshold->Add(m_btnThreshold, 0, wxALL|wxALIGN_RIGHT, 2);
    GlobalSizer->Add(m_Threshold, 0, wxEXPAND, 0);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptThresholdPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

void ptThresholdPanelWidget::OnModifiedInput()
{
	try
	{
		Core::vtkPolyDataPtr mesh;
		m_processor->GetProcessingData( 0, mesh );

		std::vector<blShapeUtils::ShapeUtils::VTKScalarType> scalarnames;
		blShapeUtils::ShapeUtils::GetScalarsVector( mesh, scalarnames );
		m_ScalarArrayControl->SetScalarArrayTypes( scalarnames, false );
	}
	coreCatchExceptionsLogAndNoThrowMacro( ptThresholdPanelWidget::OnModifiedInput )
}

bool ptThresholdPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Threshold Surface: surface thresholding using scalars." \
			"\n\nUsage: select the scalar array, the minimum and" \
			"\nmaximum threshold and then run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptThresholdPanelWidget::Enable");

	return bReturn;
}
