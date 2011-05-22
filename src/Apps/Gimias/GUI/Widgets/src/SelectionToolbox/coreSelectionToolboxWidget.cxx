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

#include <wx/wupdlock.h>

// Core
#include "coreSelectionToolboxWidget.h"
#include "coreReportExceptionMacros.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"
#include "corePluginTab.h"

using namespace Core::Widgets;

//!
SelectionToolboxWidget::SelectionToolboxWidget(wxWindow* parent, 
											wxWindowID id, 
											const wxPoint& pos, 
											const wxSize& size, 
											long style, 
											const wxString& name)
: BaseToolboxWidget(parent, id, pos, size, style, name)
{
	m_SelectionToolWidget = NULL;
}

//!
SelectionToolboxWidget::~SelectionToolboxWidget(void)
{
}

void Core::Widgets::SelectionToolboxWidget::Start( )
{
	if ( !m_SelectionToolWidget )
	{
		return;
	}
	m_SelectionToolWidget->StartInteractor();
}

void Core::Widgets::SelectionToolboxWidget::Stop( )
{
	if ( !m_SelectionToolWidget )
	{
		return;
	}
	m_SelectionToolWidget->StopInteraction();
}

bool Core::Widgets::SelectionToolboxWidget::IsSelectionEnabled()
{
	if ( m_SelectionToolWidget == NULL )
	{
		return false;
	}
	return m_SelectionToolWidget->IsSelectionEnabled( );
}

wxWindow* Core::Widgets::SelectionToolboxWidget::GetToolWindow()
{
	if ( m_SelectionToolWidget == NULL )
	{
		return NULL;
	}

	wxWindow* window = dynamic_cast<wxWindow*> (m_SelectionToolWidget);
	return window;
}


SelectionToolWidget* Core::Widgets::SelectionToolboxWidget::GetSelectionToolWidget() const
{
	return m_SelectionToolWidget;
}

void Core::Widgets::SelectionToolboxWidget::SetToolWindow( wxWindow *win )
{
	Core::Widgets::SelectionToolWidget* selectionToolWidget;
	selectionToolWidget = dynamic_cast<SelectionToolWidget*> ( win );
	m_SelectionToolWidget = selectionToolWidget;
}

std::string Core::Widgets::SelectionToolboxWidget::GetNameMenuItem()
{
	return  "Selection";
}

WIDGET_TYPE Core::Widgets::SelectionToolboxWidget::GetWidgetState()
{
	return WIDGET_TYPE_SELECTION_TOOL;
}

