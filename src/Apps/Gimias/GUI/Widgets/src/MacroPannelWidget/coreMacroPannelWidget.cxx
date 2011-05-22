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

#include <wxUnicode.h>

#include "coreMacroPannelWidget.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreAssert.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"

using namespace Core::Widgets;

//!
MacroPannelWidget::MacroPannelWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
	: wxTextCtrl(parent, id, wxT(""), pos, size, style, validator, name)
{
	try
	{
		// [JAM] By now you cannot edit it, but in the future you will enter macros and it will no longer be readOnly 
		this->SetEditable(false);
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull());
		Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
		coreAssertMacro(style.IsNotNull());
		style->ConfigureConsoleWindow(this);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(MacroPannelWidget::MacroPannelWidget)
}

//!
MacroPannelWidget::~MacroPannelWidget(void)
{
}

/** 
Reports and formats accordingly the text of the information message to be displayed 
*/
void MacroPannelWidget::ReportMessage(const std::string& message)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull());
		std::string line = "# " + message + "\n";
		Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
		coreAssertMacro(style.IsNotNull());

		this->SetDefaultStyle(wxTextAttr(style->GetBasicTextColor()));
		this->AppendText(_U(line.c_str()));
	}
	coreCatchExceptionsAddTraceAndThrowMacro(MacroPannelWidget::ReportMessage)
}

/** Reports and formats accordingly the text of the warning message to be displayed */
void MacroPannelWidget::ReportWarning(const std::string& message)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull());
		std::string line = "# Warning: " + message + "\n";
		Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
		coreAssertMacro(style.IsNotNull());

		this->SetDefaultStyle(wxTextAttr(style->GetWarningTextColor()));
		this->AppendText(_U(line.c_str()));
	}
	coreCatchExceptionsAddTraceAndThrowMacro(MacroPannelWidget::ReportWarning)
}

/** Reports and formats accordingly the text of the warning message to be displayed */
void MacroPannelWidget::ReportError(const std::string& message)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull());
		std::string line = "# Error: " + message + "\n";
		Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
		coreAssertMacro(style.IsNotNull());

		this->SetDefaultStyle(wxTextAttr(style->GetErrorTextColor()));
		this->AppendText(_U(line.c_str()));
	}
	coreCatchExceptionsAddTraceAndThrowMacro(MacroPannelWidget::ReportError)
}

/** Reports and formats accordingly the text of the warning message to be displayed */
void MacroPannelWidget::WriteVerbatim(const std::string& message)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(graphicalIface.IsNotNull());
		Core::Runtime::StyleManager::Pointer style = graphicalIface->GetStyleManager();
		coreAssertMacro(style.IsNotNull());

		this->SetDefaultStyle(wxTextAttr(style->GetBasicTextColor()));
		this->AppendText(_U(message.c_str()));
	}
	coreCatchExceptionsAddTraceAndThrowMacro(MacroPannelWidget::ReportError)
}

