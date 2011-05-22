// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


#include "ptEdgeSwappingPanelWidget.h"


ptEdgeSwappingPanelWidget::ptEdgeSwappingPanelWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptEdgeSwappingPanelWidget::ptEdgeSwappingPanelWidget
    m_EdgeSwapping_staticbox = new wxStaticBox(this, -1, wxT("Edge swapping"));
    m_labelEdgeSwappingAngle = new wxStaticText(this, wxID_ANY, wxT("Angle"));
    m_textEdgeSwappingAngle = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_labelEdgeSwapingMinimumEdgesSwapped = new wxStaticText(this, wxID_ANY, wxT("Minimum edges swapped"));
    m_textEdgeSwapingMinimumEdgesSwapped = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    btnEdgeSwapping = new wxButton(this, wxID_btnEdgeSwapping, wxT("Apply"));

	m_processor = ptEdgeSwappingPanelWidget::EdgeSwappingProcessor::New();
	m_processor->SetName( "EdgeSwappingProcessor" );
	m_processor->GetOutputPort( 0 )->SetDataEntityName( "Edge Swapping" );

	do_layout();
	UpdateWidget();
}


BEGIN_EVENT_TABLE(ptEdgeSwappingPanelWidget, wxPanel)
    // begin wxGlade: ptEdgeSwappingPanelWidget::event_table
    EVT_BUTTON(wxID_btnEdgeSwapping, ptEdgeSwappingPanelWidget::OnButtonEdgeSwapping)
    // end wxGlade
END_EVENT_TABLE();


void ptEdgeSwappingPanelWidget::OnButtonEdgeSwapping(wxCommandEvent &event)
{
	UpdateData();

	UpdateProcessor( );
}

void ptEdgeSwappingPanelWidget::UpdateData()
{
	meMeshEdgeSwappingParamsPtr meshEdgeSwappingParams =
		meMeshEdgeSwappingParamsPtr( new meMeshEdgeSwappingParams );
	
	meshEdgeSwappingParams->m_Angle = std::atof(
					m_textEdgeSwappingAngle->GetValue());
	meshEdgeSwappingParams->m_MinimumEdgesSwapped = std::atof(
					m_textEdgeSwapingMinimumEdgesSwapped->GetValue());

	m_processor->GetFilter( )->SetParams(meshEdgeSwappingParams);
}

void ptEdgeSwappingPanelWidget::UpdateWidget()
{
	meMeshEdgeSwappingParamsPtr meshEdgeSwappingParams =
		m_processor->GetFilter( )->GetParams( );

	m_textEdgeSwappingAngle->SetValue(wxString::Format("%f",
		meshEdgeSwappingParams->m_Angle));
	m_textEdgeSwapingMinimumEdgesSwapped->SetValue(wxString::Format("%f", 
		meshEdgeSwappingParams->m_MinimumEdgesSwapped) );
} 

void ptEdgeSwappingPanelWidget::do_layout()
{
    // begin wxGlade: ptEdgeSwappingPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* m_EdgeSwapping = new wxStaticBoxSizer(m_EdgeSwapping_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_5_copy_2_copy_copy_copy_1_copy_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5_copy_2_copy_copy_1_copy_copy = new wxBoxSizer(wxHORIZONTAL);
    sizer_5_copy_2_copy_copy_1_copy_copy->Add(m_labelEdgeSwappingAngle, 1, 0, 0);
    sizer_5_copy_2_copy_copy_1_copy_copy->Add(m_textEdgeSwappingAngle, 0, 0, 0);
    m_EdgeSwapping->Add(sizer_5_copy_2_copy_copy_1_copy_copy, 0, wxALL|wxEXPAND, 2);
    sizer_5_copy_2_copy_copy_copy_1_copy_copy->Add(m_labelEdgeSwapingMinimumEdgesSwapped, 1, 0, 0);
    sizer_5_copy_2_copy_copy_copy_1_copy_copy->Add(m_textEdgeSwapingMinimumEdgesSwapped, 0, 0, 0);
    m_EdgeSwapping->Add(sizer_5_copy_2_copy_copy_copy_1_copy_copy, 0, wxALL|wxEXPAND, 2);
    m_EdgeSwapping->Add(btnEdgeSwapping, 0, wxALL|wxALIGN_RIGHT, 2);
    GlobalSizer->Add(m_EdgeSwapping, 0, wxEXPAND, 0);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptEdgeSwappingPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptEdgeSwappingPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Edge Swapping: swap the edges of the surface triangles." \
			"\n\nUsage: Select an angle and/or the minimum edges" \
			"\nand run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptEdgeSwappingPanelWidget::Enable");

	return bReturn;
}
