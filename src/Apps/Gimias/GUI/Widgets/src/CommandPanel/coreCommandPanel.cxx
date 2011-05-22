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

#include "coreCommandPanel.h"
#include "coreKernel.h"
#include "coreAssert.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreStyleManager.h"
#include "Panel.xpm"

using namespace Core::Widgets;


Core::Widgets::CommandPanelItem::~CommandPanelItem( void )
{
}

//!
CommandPanel::CommandPanel(
	wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, bool exclusive)
		: wxToolBoxControl(parent, id, exclusive, pos, size, style, name)
{
	//SetBitmap( panel_xpm );

	//this->SetAutoLayout(true);
	this->Hide();

	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();
	Core::Runtime::StyleManager::Pointer styleManager = gIface->GetStyleManager();
	styleManager->ConfigureCommandPanel(this);
}

//!
CommandPanel::~CommandPanel(void)
{
}

//!
void CommandPanel::Add(CommandPanelItem* item)
{
	coreAssertMacro(item != NULL);
	// Set the look&feel to the item
	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface = Core::Runtime::Kernel::GetGraphicalInterface();
	coreAssertMacro(gIface.IsNotNull() && "The Graphical interface must have been initialized");
	Core::Runtime::StyleManager::Pointer styleManager = gIface->GetStyleManager();
	coreAssertMacro(styleManager.IsNotNull() && "The Style manager must be running");
	styleManager->ConfigureCommandPanelItem(item);

	// If it was hidden, show it
	this->Show();

	//item->SetMinSize(wxSize(this->GetMinSize().GetWidth(), -1));
	//item->SetMaxSize(wxSize(this->GetMinSize().GetWidth(), -1));
	this->wxToolBoxControl::Add(item);
	
	// Resize this
	// Cast a resize event
	wxSizeEvent resEvent(this->GetBestSize(), this->GetId());
	resEvent.SetEventObject(this);
	this->GetEventHandler()->ProcessEvent(resEvent);
}

//!
void CommandPanel::Remove(CommandPanelItem* item)
{
	wxToolBoxControl::Remove(item);

	// If it is empty, hide it
	if(this->GetNumberOfItems() == 0)
		this->Hide();
}

Core::BaseProcessor::Pointer Core::Widgets::CommandPanel::GetProcessor()
{
	return NULL;
}


