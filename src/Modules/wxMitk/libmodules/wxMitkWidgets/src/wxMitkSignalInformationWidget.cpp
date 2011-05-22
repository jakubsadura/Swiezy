/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wxMitkSignalInformationWidget.h"

#include "vtkMath.h"

using namespace mitk;


wxMitkSignalInformationWidget::wxMitkSignalInformationWidget(wxWindow* parent, 
																	   int id, 
																	   const wxPoint& pos, 
																	   const wxSize& size, 
																	   long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
  
	 // begin wxGlade: wxMitkSignalInformationWidget::wxMitkSignalInformationWidget
    label_1 = new wxStaticText(this, wxID_ANY, wxT("Number of TimeSteps"));
    m_timesteps = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_2 = new wxStaticText(this, wxID_ANY, wxT("Number of Values"));
    m_values = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_3 = new wxStaticText(this, wxID_ANY, wxT("Min Value"));
	m_minvalue = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_4 = new wxStaticText(this, wxID_ANY, wxT("Max Value"));
    m_maxvalue = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    
    set_properties();
    do_layout();
    // end wxGlade
  
}

void wxMitkSignalInformationWidget::UpdateData(blSignal::Pointer SignalInput)
{

if (SignalInput == NULL )
	{
		this->ResetWidget();
		return;
	}
	
	m_timesteps->SetValue(wxString::Format(wxT("%d"), SignalInput->GetXVector().size()));
	m_values->SetValue(wxString::Format(wxT("%d"), SignalInput->GetYVector().size()));
	m_minvalue->SetValue(wxString::Format(wxT("%.3f"), SignalInput->GetMinYValue()));
	m_maxvalue->SetValue(wxString::Format(wxT("%.3f"), SignalInput->GetMaxYValue()));
}

void wxMitkSignalInformationWidget::ResetWidget()
{		
	m_timesteps->SetValue(wxT(""));
	m_values->SetValue(wxT(""));
	m_minvalue->SetValue(wxT(""));
	m_maxvalue->SetValue(wxT(""));
}


void wxMitkSignalInformationWidget::set_properties()
{
   
}


void wxMitkSignalInformationWidget::do_layout()
{
  
    // begin wxGlade: wxMitkSignalInformationWidget::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    sizer_2->Add(label_1, 1, wxEXPAND, 0);
    sizer_2->Add(m_timesteps, 1, wxALL|wxEXPAND, 0);
    sizer_1->Add(sizer_2, 0, wxEXPAND, 0);
    sizer_3->Add(label_2, 1, wxEXPAND, 0);
    sizer_3->Add(m_values, 1,  wxALL|wxEXPAND, 0);
    sizer_1->Add(sizer_3, 1, wxEXPAND, 0);
	sizer_4->Add(label_3, 1, wxEXPAND, 0);
    sizer_4->Add(m_minvalue, 1,  wxALL|wxEXPAND, 0);
	sizer_1->Add(sizer_4, 2, wxEXPAND, 0);
	sizer_5->Add(label_4, 1, wxEXPAND, 0);
    sizer_5->Add(m_maxvalue, 1,  wxALL|wxEXPAND, 0);
	sizer_1->Add(sizer_5, 3, wxEXPAND, 0);

	SetSizer(sizer_1);
    sizer_1->Fit(this);
    // end wxGlade
  
}

