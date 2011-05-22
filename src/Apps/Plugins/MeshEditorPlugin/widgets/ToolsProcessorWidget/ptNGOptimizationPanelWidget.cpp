// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptNGOptimizationPanelWidget.h"


ptNGOptimizationPanelWidget::ptNGOptimizationPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptNGOptimizationPanelWidget::ptNGOptimizationPanelWidget
    m_GlobalOptimization_staticbox = new wxStaticBox(this, -1, wxT("Mesh Optimization"));
    m_labelMaxMeshSize = new wxStaticText(this, wxID_ANY, wxT("Max Mesh size"));
    m_textMaxMeshSize = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_labelElementsPerCurvatureRadius = new wxStaticText(this, wxID_ANY, wxT("Elements Per Curvature Radius"));
    m_textElementsPerCurvatureRadius = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_labelElementsPerEdge = new wxStaticText(this, wxID_ANY, wxT("Elements Per Edge"));
    m_textElementsPerEdge = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    //btnNetgenGlobalRefinement = new wxButton(this, wxID_btnNetgenGlobalRefinement, wxT("Netgen global refinement"));
    //btnNetgenGlobalSmoothing = new wxButton(this, wxID_btnNetgenGlobalSmoothing, wxT("Netgen global smoothing"));
    btnNetgenGlobalOptimization = new wxButton(this, wxID_btnNetgenGlobalOptimization, wxT("Netgen global optimization"));

    //m_processorNGSmooth =;
	//m_processorNGRefine =;
	m_processor = Core::NetgenProcessor::New();

    do_layout();
    UpdateWidget();
}


BEGIN_EVENT_TABLE(ptNGOptimizationPanelWidget, wxPanel)
    // begin wxGlade: ptNGOptimizationPanelWidget::event_table
    EVT_BUTTON(wxID_btnNetgenGlobalRefinement, ptNGOptimizationPanelWidget::OnButtonNetgenGlobalRefinement)
    EVT_BUTTON(wxID_btnNetgenGlobalSmoothing, ptNGOptimizationPanelWidget::OnButtonNetgenGlobalSmoothing)
    EVT_BUTTON(wxID_btnNetgenGlobalOptimization, ptNGOptimizationPanelWidget::OnButtonNetgenGlobalOptimization)
    // end wxGlade
END_EVENT_TABLE();


void ptNGOptimizationPanelWidget::OnButtonNetgenGlobalRefinement(wxCommandEvent &event)
{
	UpdateData();
	//m_processorNGRefine->Update();
}


void ptNGOptimizationPanelWidget::OnButtonNetgenGlobalSmoothing(wxCommandEvent &event)
{
	UpdateData();
	//m_processorNGSmooth->Update();
}


void ptNGOptimizationPanelWidget::OnButtonNetgenGlobalOptimization(wxCommandEvent &event)
{
	UpdateData();
	UpdateProcessor( );
}


void ptNGOptimizationPanelWidget::UpdateData()
{
	meMeshParamsOptimizePtr param = 
		meMeshParamsOptimizePtr( new meMeshParamsOptimize );

	m_textElementsPerCurvatureRadius->GetValue( ).ToDouble( &param->m_curvaturesafety );
	m_textMaxMeshSize->GetValue().ToDouble( &param->m_maxh );
	m_textElementsPerEdge->GetValue().ToDouble( &param->m_segmentsperedge );

	m_processor->SetParams(param);

}

void ptNGOptimizationPanelWidget::UpdateWidget()
{
	meMeshParamsOptimizePtr params =
		m_processor->GetParams();

	m_textElementsPerCurvatureRadius->SetValue(wxString::Format("%f", 
		params->m_curvaturesafety) );
	m_textMaxMeshSize->SetValue(wxString::Format("%f", 
		params->m_maxh) );
	m_textElementsPerEdge->SetValue(wxString::Format("%f", 
		params->m_segmentsperedge) );
}


void ptNGOptimizationPanelWidget::do_layout()
{
    // begin wxGlade: ptNGOptimizationPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* m_GlobalOptimization = new wxStaticBoxSizer(m_GlobalOptimization_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_5_copy_1_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5_copy_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5_copy_3 = new wxBoxSizer(wxHORIZONTAL);
    sizer_5_copy_3->Add(m_labelMaxMeshSize, 1, 0, 0);
    sizer_5_copy_3->Add(m_textMaxMeshSize, 0, 0, 0);
    m_GlobalOptimization->Add(sizer_5_copy_3, 0, wxALL|wxEXPAND, 2);
    sizer_5_copy_copy->Add(m_labelElementsPerCurvatureRadius, 1, 0, 0);
    sizer_5_copy_copy->Add(m_textElementsPerCurvatureRadius, 0, 0, 0);
    m_GlobalOptimization->Add(sizer_5_copy_copy, 0, wxALL|wxEXPAND, 2);
    sizer_5_copy_1_copy->Add(m_labelElementsPerEdge, 1, 0, 0);
    sizer_5_copy_1_copy->Add(m_textElementsPerEdge, 0, 0, 0);
    m_GlobalOptimization->Add(sizer_5_copy_1_copy, 0, wxALL|wxEXPAND, 2);
    //m_GlobalOptimization->Add(btnNetgenGlobalRefinement, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    //m_GlobalOptimization->Add(btnNetgenGlobalSmoothing, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    m_GlobalOptimization->Add(btnNetgenGlobalOptimization, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
    GlobalSizer->Add(m_GlobalOptimization, 0, wxEXPAND, 0);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptNGOptimizationPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptNGOptimizationPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Netgen Tools: refine, optimize or smooth the surface" \
			"\nusing Netgen." \
			"\n\nUsage: select the maximum mesh size, the elements" \
			"\nper curvature and the elements per edge" \
			"\nand run the appropriate tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptNGOptimizationPanelWidget::Enable");

	return bReturn;
}
