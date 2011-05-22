// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptMeshStatisticsPanelWidget.h"

ptMeshStatisticsPanelWidget::ptMeshStatisticsPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptMeshStatisticsPanelWidget::ptMeshStatisticsPanelWidget
    sizer1_staticbox = new wxStaticBox(this, -1, wxT("Mesh statistics"));
    ckb_AspectRatio = new wxCheckBox(this, wxID_CHK_ASPECTRATIO, wxT("Aspect Ratio"));
    text_ctrl_1 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    ckb_MinAngle = new wxCheckBox(this, wxID_CHK_MINANGLE, wxT("Min Angle"));
    text_ctrl_1_copy = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    ckb_EgdeRatio = new wxCheckBox(this, wxID_CHK_EDGERATIO, wxT("Edge Ratio"));
    text_ctrl_1_copy_1 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    m_labelNumberofBins = new wxStaticText(this, wxID_ANY, wxT("Number of bins"));
    m_textNumberofBins = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_btnCompute = new wxButton(this, wxID_COMPUTE, wxT("Compute"));

    do_layout();
    // end wxGlade
	
	m_processor = Core::MeshStatisticsProcessor::New();

	UpdateWidget();
		
}

void ptMeshStatisticsPanelWidget::UpdateData()
{	
    m_processor->SetNumberofBins(std::atoi(m_textNumberofBins->GetValue()));
}

void ptMeshStatisticsPanelWidget::UpdateWidget()
{
	// initialize numofbins
	m_textNumberofBins->SetValue(wxString::Format("%d",m_processor->GetNumberofBins()));
	Core::QualityStringMap::iterator it;
	Core::Quality* q ;
	//!TODO create list to go through
	it = m_processor->qualityStringMap.find("Tetra Aspect Ratio");
	q = (*it).second;
	text_ctrl_1->SetValue( wxString::Format(wxT("%.5f"), q->m_averageValue));

	it = m_processor->qualityStringMap.find("Tetra Edge Ratio");
	q = (*it).second;
	text_ctrl_1_copy_1->SetValue( wxString::Format(wxT("%.5f"), q->m_averageValue));

	it = m_processor->qualityStringMap.find("Tetra Min Angle");
	q = (*it).second;
	text_ctrl_1_copy->SetValue( wxString::Format(wxT("%.5f"), q->m_averageValue));


}

BEGIN_EVENT_TABLE(ptMeshStatisticsPanelWidget, wxPanel)
    // begin wxGlade: ptMeshStatisticsPanelWidget::event_table
    EVT_CHECKBOX(wxID_CHK_ASPECTRATIO, ptMeshStatisticsPanelWidget::OnAspectRatioChecked)
    EVT_CHECKBOX(wxID_CHK_MINANGLE, ptMeshStatisticsPanelWidget::OnMinAngleChecked)
    EVT_CHECKBOX(wxID_CHK_EDGERATIO, ptMeshStatisticsPanelWidget::OnEdgeRatioChecked)
    EVT_BUTTON(wxID_COMPUTE, ptMeshStatisticsPanelWidget::OnComputeBtn)
    // end wxGlade
END_EVENT_TABLE();


void ptMeshStatisticsPanelWidget::OnAspectRatioChecked(wxCommandEvent &event)
{
	Core::QualityStringMap::iterator it;
	it = m_processor->qualityStringMap.find("Tetra Aspect Ratio");	
	
	Core::Quality* q = (*it).second;
	q->m_selected = ckb_AspectRatio->GetValue();
}

void ptMeshStatisticsPanelWidget::OnMinAngleChecked(wxCommandEvent &event)
{
   Core::QualityStringMap::iterator it;
   it = m_processor->qualityStringMap.find("Tetra Min Angle");	

   Core::Quality* q = (*it).second;
   q->m_selected = ckb_MinAngle->GetValue();
}

void ptMeshStatisticsPanelWidget::OnEdgeRatioChecked(wxCommandEvent &event)
{
	Core::QualityStringMap::iterator it;
	it = m_processor->qualityStringMap.find("Tetra Edge Ratio");	

	Core::Quality* q = (*it).second;
	q->m_selected =ckb_EgdeRatio->GetValue();
}


void ptMeshStatisticsPanelWidget::OnComputeBtn(wxCommandEvent &event)
{
	UpdateData();
	UpdateProcessor();
	UpdateWidget();
}

void ptMeshStatisticsPanelWidget::do_layout()
{
    // begin wxGlade: ptMeshStatisticsPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer1 = new wxStaticBoxSizer(sizer1_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_2_copy_1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_2_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    sizer_2->Add(ckb_AspectRatio, 1, wxEXPAND, 0);
    sizer_2->Add(text_ctrl_1, 0, 0, 0);
    sizer1->Add(sizer_2, 0, wxEXPAND, 0);
    sizer_2_copy->Add(ckb_MinAngle, 1, wxEXPAND, 0);
    sizer_2_copy->Add(text_ctrl_1_copy, 0, 0, 0);
    sizer1->Add(sizer_2_copy, 0, wxEXPAND, 0);
    sizer_2_copy_1->Add(ckb_EgdeRatio, 1, wxEXPAND, 0);
    sizer_2_copy_1->Add(text_ctrl_1_copy_1, 0, 0, 0);
    sizer1->Add(sizer_2_copy_1, 0, wxEXPAND, 0);
    sizer_1->Add(m_labelNumberofBins, 1, wxEXPAND, 0);
    sizer_1->Add(m_textNumberofBins, 0, 0, 5);
    sizer1->Add(sizer_1, 0, wxEXPAND, 0);
    sizer1->Add(m_btnCompute, 0, wxALL|wxALIGN_RIGHT, 5);
    GlobalSizer->Add(sizer1, 1, wxALL|wxEXPAND, 5);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


bool ptMeshStatisticsPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Mesh Statistics." \
			"\n\nUsage: check the box you want to compute and Run.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("SignalTimePropagationPanelWidget::Enable");

	return bReturn;
}

Core::BaseProcessor::Pointer ptMeshStatisticsPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

void ptMeshStatisticsPanelWidget::OnInit()
{
	GetProcessorOutputObserver(0)->SetHideInput(false);
	GetProcessorOutputObserver(0)->SelectDataEntity(true);
}