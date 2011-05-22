// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


#include "ptLocalRefinerPanelWidget.h"

ptLocalRefinerPanelWidget::ptLocalRefinerPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptLocalRefinerPanelWidget::ptLocalRefinerPanelWidget
    m_LocalRefiner_staticbox = new wxStaticBox(this, -1, wxT("Local Refiner"));
    m_labelLocalRefinerDensityFactor = new wxStaticText(this, wxID_ANY, wxT("Density factor"));
    m_textLocalRefinerDensityFactor = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_labelLocalRefinerAngle = new wxStaticText(this, wxID_ANY, wxT("Angle"));
    m_textLocalRefinerAngle = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_labelLocalRefinerMinimumEdgesSwapped = new wxStaticText(this, wxID_ANY, wxT("Minimum edges swapped"));
    m_textLocalRefinerMinimumEdgesSwapped = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    btnLocalRefiner = new wxButton(this, wxID_btnLocalRefiner, wxT(" Refine "));

    m_processor = LocalRefinerProcessor::New();
	m_processor->GetOutputPort( 0 )->SetDataEntityName( "Local Refined" );
	m_processor->SetName( "LocalRefinerProcessor" );

	do_layout();
	UpdateWidget();
}


BEGIN_EVENT_TABLE(ptLocalRefinerPanelWidget, wxPanel)
    // begin wxGlade: ptLocalRefinerPanelWidget::event_table
    EVT_BUTTON(wxID_btnLocalRefiner, ptLocalRefinerPanelWidget::OnButtonLocalRefiner)
    // end wxGlade
END_EVENT_TABLE();


void ptLocalRefinerPanelWidget::OnButtonLocalRefiner(wxCommandEvent &event)
{
	UpdateData();
	UpdateProcessor( );
}

void ptLocalRefinerPanelWidget::UpdateWidget()
{
	meMeshLocalRefinerParamsPtr meshLocalRefinerParams = 
		m_processor->GetFilter( )->GetParams();

	m_textLocalRefinerDensityFactor->SetValue(wxString::Format("%f",
		meshLocalRefinerParams->m_DensityFactor));
	m_textLocalRefinerAngle->SetValue(wxString::Format("%f", 
		meshLocalRefinerParams->m_Angle));
	m_textLocalRefinerMinimumEdgesSwapped->SetValue(wxString::Format("%f",
		meshLocalRefinerParams->m_MinimumEdgesSwapped));
}
void ptLocalRefinerPanelWidget::UpdateData()
{
	meMeshLocalRefinerParamsPtr meshLocalRefinerParams = 
		meMeshLocalRefinerParamsPtr (new meMeshLocalRefinerParams );

	meshLocalRefinerParams->m_DensityFactor = std::atof(
		m_textLocalRefinerDensityFactor->GetValue());
	meshLocalRefinerParams->m_Angle = std::atof(
		m_textLocalRefinerAngle->GetValue());
	meshLocalRefinerParams->m_MinimumEdgesSwapped = std::atof(
		m_textLocalRefinerMinimumEdgesSwapped->GetValue());

	m_processor->GetFilter( )->SetParams(meshLocalRefinerParams);
}

void ptLocalRefinerPanelWidget::do_layout()
{
    // begin wxGlade: ptLocalRefinerPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* m_LocalRefiner = new wxStaticBoxSizer(m_LocalRefiner_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_5_copy_2_copy_copy_copy_1_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5_copy_2_copy_copy_1_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5_copy_2_copy_copy_copy_copy_1_copy = new wxBoxSizer(wxHORIZONTAL);
    sizer_5_copy_2_copy_copy_copy_copy_1_copy->Add(m_labelLocalRefinerDensityFactor, 1, 0, 0);
    sizer_5_copy_2_copy_copy_copy_copy_1_copy->Add(m_textLocalRefinerDensityFactor, 0, 0, 0);
    m_LocalRefiner->Add(sizer_5_copy_2_copy_copy_copy_copy_1_copy, 0, wxALL|wxEXPAND, 2);
    sizer_5_copy_2_copy_copy_1_copy->Add(m_labelLocalRefinerAngle, 1, 0, 0);
    sizer_5_copy_2_copy_copy_1_copy->Add(m_textLocalRefinerAngle, 0, 0, 0);
    m_LocalRefiner->Add(sizer_5_copy_2_copy_copy_1_copy, 0, wxALL|wxEXPAND, 2);
    sizer_5_copy_2_copy_copy_copy_1_copy->Add(m_labelLocalRefinerMinimumEdgesSwapped, 1, 0, 0);
    sizer_5_copy_2_copy_copy_copy_1_copy->Add(m_textLocalRefinerMinimumEdgesSwapped, 0, 0, 0);
    m_LocalRefiner->Add(sizer_5_copy_2_copy_copy_copy_1_copy, 0, wxALL|wxEXPAND, 2);
    m_LocalRefiner->Add(btnLocalRefiner, 0, wxALL|wxALIGN_RIGHT, 2);
    GlobalSizer->Add(m_LocalRefiner, 0, wxEXPAND, 0);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptLocalRefinerPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptLocalRefinerPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Local Refiner: locally refine a surface (work in progress)." \
			"\n\nUsage: SHIFT + left click to select the region,." \
			"\nselect the density, angle and minimum edges" \
			"\nand then run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptLocalRefinerPanelWidget::Enable");

	return bReturn;
}
