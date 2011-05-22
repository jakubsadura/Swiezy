// Copyright 2010 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptShapeScalePanelWidget.h"

ptShapeScalePanelWidget::ptShapeScalePanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
     wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptShapeScalePanelWidget::ptShapeScalePanelWidget
    m_ShapeScale_staticbox = new wxStaticBox(this, -1, wxT("Scale shape"));
    m_labelScale = new wxStaticText(this, wxID_ANY, wxT("Scale"));
    m_textScale = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_btnScale = new wxButton(this, wxID_BTN_SCALE, wxT("Scale"));

	m_processor = Core::ShapeScaleProcessor::New();


    do_layout();
	UpdateWidget();
}


BEGIN_EVENT_TABLE(ptShapeScalePanelWidget, wxPanel)
    // begin wxGlade: ptShapeScalePanelWidget::event_table
    EVT_BUTTON(wxID_BTN_SCALE, ptShapeScalePanelWidget::OnScaleClick)
    // end wxGlade
END_EVENT_TABLE();


void ptShapeScalePanelWidget::OnScaleClick(wxCommandEvent &event)
{
	UpdateData();
	UpdateProcessor( );
}

void ptShapeScalePanelWidget::do_layout()
{
    // begin wxGlade: ptShapeScalePanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* m_ShapeScale = new wxStaticBoxSizer(m_ShapeScale_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    sizer_1->Add(m_labelScale, 1, 0, 0);
    sizer_1->Add(m_textScale, 0, 0, 0);
    m_ShapeScale->Add(sizer_1, 0, wxEXPAND, 0);
    m_ShapeScale->Add(m_btnScale, 0, wxALL|wxALIGN_RIGHT, 5);
    GlobalSizer->Add(m_ShapeScale, 1, wxALL|wxEXPAND, 5);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptShapeScalePanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

void ptShapeScalePanelWidget::OnInit()
{

}

void ptShapeScalePanelWidget::UpdateWidget()
{
	Core::ShapeScaleProcessor::ParametersHolder::Pointer params;
	params = m_processor->GetParametersHolder();

	m_textScale->SetValue(wxString::Format("%f",params->GetSubject( )));
}

void ptShapeScalePanelWidget::UpdateData()
{
	Core::ShapeScaleProcessor::ParametersHolder::Pointer params;
	params = m_processor->GetParametersHolder();

	params->SetSubject(std::atof(m_textScale->GetValue()));


}