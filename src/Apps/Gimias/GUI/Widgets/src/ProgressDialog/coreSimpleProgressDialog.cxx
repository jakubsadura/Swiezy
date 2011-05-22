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

#include "coreSimpleProgressDialog.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreAssert.h"
#include "coreReportExceptionMacros.h"
#include <wx/listctrl.h>

#define wxID_ListControl	(1 + wxID_HIGHEST)

using namespace Core::Widgets;

//!
SimpleProgressDialog::SimpleProgressDialog(
	wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: coreSimpleProgressFrameUI(parent, id, title, pos, size, style)
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
	graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
	coreAssertMacro(graphicalIface.IsNotNull());
	Core::Runtime::StyleManager::Pointer styleManager = graphicalIface->GetStyleManager();
	coreAssertMacro(styleManager.IsNotNull());
	styleManager->ConfigureProgressPanel(this);
}

//!
SimpleProgressDialog::~SimpleProgressDialog(void)
{

}

