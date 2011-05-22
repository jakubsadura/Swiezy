/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "ptExecuteCommandPanelWidget.h"

// begin wxGlade: ::extracode
#define wxID_ExecuteCommand wxID_HIGHEST + 1
// end wxGlade

ptExecuteCommandPanelWidget::ptExecuteCommandPanelWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptExecuteCommandPanelWidget::ptExecuteCommandPanelWidget
   m_ExecuteCommand_staticbox = new wxStaticBox(this, -1, wxT("Execute Command"));
	label_0 = new wxStaticText(this, wxID_ANY, wxT("Input name"));
	text_ctrl_0 = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	label_1 = new wxStaticText(this, wxID_ANY, wxT("Executable"));
    text_ctrl_1 = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_2 = new wxStaticText(this, wxID_ANY, wxT("Options"));
    text_ctrl_2 = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    button_1 = new wxButton(this, wxID_ExecuteCommand, wxT("Execute Command"));

    do_layout();

	m_processor = Core::ExecuteCommandProcessor::New();
	UpdateWidget();
    // end wxGlade
}


BEGIN_EVENT_TABLE(ptExecuteCommandPanelWidget, wxPanel)
    // begin wxGlade: ptExecuteCommandPanelWidget::event_table
    EVT_BUTTON(wxID_ExecuteCommand, ptExecuteCommandPanelWidget::OnExecuteButton)
    // end wxGlade
END_EVENT_TABLE();


void ptExecuteCommandPanelWidget::OnExecuteButton(wxCommandEvent &event)
{
	UpdateData();
	UpdateProcessor();
}

Core::BaseProcessor::Pointer ptExecuteCommandPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptExecuteCommandPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Execute Command: launch an executable from GIMIAS." \
			"\n\nUsage: Select an input name, an executable" \
			"\nand command line options and then run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptExecuteCommandPanelWidget::Enable");

	return bReturn;
}

void ptExecuteCommandPanelWidget::do_layout()
{
    // begin wxGlade: ptExecuteCommandPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* m_ExecuteCommand = new wxStaticBoxSizer(m_ExecuteCommand_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_0 = new wxBoxSizer(wxHORIZONTAL);
    sizer_0->Add(label_0, 1, 0, 0);
    sizer_0->Add(text_ctrl_0, 1, 0, 0);
    m_ExecuteCommand->Add(sizer_0, 0, wxALL|wxEXPAND, 2);
    sizer_1->Add(label_1, 1, 0, 0);
    sizer_1->Add(text_ctrl_1, 1, 0, 0);
    m_ExecuteCommand->Add(sizer_1, 0, wxALL|wxEXPAND, 2);
    sizer_2->Add(label_2, 1, 0, 0);
    sizer_2->Add(text_ctrl_2, 1, 0, 0);
    m_ExecuteCommand->Add(sizer_2, 0, wxALL|wxEXPAND, 2);
    m_ExecuteCommand->Add(button_1, 0, wxALL|wxALIGN_RIGHT, 2);
    GlobalSizer->Add(m_ExecuteCommand, 0, wxEXPAND, 0);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


void ptExecuteCommandPanelWidget::UpdateData()
{
	m_processor->SetInputName(text_ctrl_0->GetValue());
	m_processor->SetExecutableName(text_ctrl_1->GetValue());
	m_processor->SetExecutableOptions(text_ctrl_2->GetValue());
}

void ptExecuteCommandPanelWidget::UpdateWidget()
{
	text_ctrl_0->SetValue(m_processor->GetInputName());
	text_ctrl_1->SetValue(m_processor->GetExecutableName());
	text_ctrl_2->SetValue(m_processor->GetExecutableOptions());
}