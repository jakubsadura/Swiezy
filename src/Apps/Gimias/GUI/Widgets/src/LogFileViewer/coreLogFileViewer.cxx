/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "coreLogFileViewer.h"
#include "coreLogger.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreReportExceptionMacros.h"
#include "coreAssert.h"
#include <wxUnicode.h>

#include <wx/textctrl.h>

using namespace Core::Widgets;

#define wxID_btnClear (1 + wxID_HIGHEST)
#define wxID_btnClose (2 + wxID_HIGHEST)

BEGIN_EVENT_TABLE(LogFileViewer, wxDialog)
	EVT_BUTTON(wxID_btnClose, LogFileViewer::OnButtonClose)
	EVT_BUTTON(wxID_btnClear, LogFileViewer::OnButtonClear)
END_EVENT_TABLE()

//!
LogFileViewer::LogFileViewer(wxWindow* parent) 
: wxDialog(parent, wxID_ANY, wxT("Log file: "), wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX )
{
	try
	{
		// Get Log file and set title
		Core::Runtime::Logger::Pointer logger =  Core::Runtime::Kernel::GetLogManager();
		coreAssertMacro(logger.IsNotNull());
		std::string title = "Log file: " + logger->GetFileName();
		this->SetTitle(_U(title));

		// Create widgets
		this->txtBox = new wxTextCtrl(
			this, 
			wxID_ANY, 
			wxEmptyString, 
			wxDefaultPosition, 
			wxSize(600, 600), 
			wxTE_READONLY | wxTE_MULTILINE | wxVSCROLL | wxHSCROLL
		);
		
		wxButton* btnClear = new wxButton(this, wxID_btnClear, wxT("Clea&r log"));
		wxButton* btnClose = new wxButton(this, wxID_btnClose, wxT("&Close window"));

		// Apply look & feel
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull());
		Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
		coreAssertMacro(style.IsNotNull());
		style->ConfigureConsoleWindow(this->txtBox);

		// layout
		wxBoxSizer* hBox = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* vLayout = new wxBoxSizer(wxVERTICAL);
		hBox->AddStretchSpacer(3);
		hBox->Add(btnClear, 1, wxEXPAND);
		hBox->Add(btnClose, 1, wxEXPAND);
		vLayout->Add(this->txtBox, 1, wxEXPAND);
		vLayout->Add(hBox, 0, wxEXPAND);
		this->SetSizer(vLayout);

		// Get the log file contents
		this->ReadLogFile();
	}
	coreCatchExceptionsAddTraceAndThrowMacro(LogFileViewer::LogFileViewer);
}

//!
LogFileViewer::~LogFileViewer(void)
{
}

//!
void LogFileViewer::ReadLogFile(void)
{
	try
	{
		Core::Runtime::Logger::Pointer logger = Core::Runtime::Kernel::GetLogManager();
		if(logger.IsNotNull())
			this->txtBox->SetValue(_U(logger->ReadLogFileGetFullText()));
	}
	coreCatchExceptionsAddTraceAndThrowMacro(LogFileViewer::ReadLogFile);
}


//!
void LogFileViewer::ClearLogFile(void)
{
	try
	{
		Core::Runtime::Logger::Pointer logger = Core::Runtime::Kernel::GetLogManager();
		coreAssertMacro(logger.IsNotNull());
		logger->Clear();
		this->ReadLogFile();
	}
	coreCatchExceptionsAddTraceAndThrowMacro(LogFileViewer::ClearLogFile);
}


//!
void LogFileViewer::OnButtonClear(wxCommandEvent& WXUNUSED(event))
{
	try
	{
		this->ClearLogFile();
	}
	coreCatchExceptionsReportAndNoThrowMacro(LogFileViewer::OnButtonClear);
}

//!
void LogFileViewer::OnButtonClose(wxCommandEvent& WXUNUSED(event))
{
	try
	{
		this->Close();
	}
	coreCatchExceptionsReportAndNoThrowMacro(LogFileViewer::OnButtonClose);
}
