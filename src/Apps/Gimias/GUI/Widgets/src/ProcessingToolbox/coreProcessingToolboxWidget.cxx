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
#include "coreProcessingToolboxWidget.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataContainer.h"
#include "coreKernel.h"
#include "coreProcessorWidgetsBuilder.h"
#include "corePluginTab.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"

#include "ProcessingTools.xpm"

using namespace Core::Widgets;

#define wxID_PROCESSING_TOOLBOX_CAPTION wxID( "wxID_PROCESSING_TOOLBOX_CAPTION" )

BEGIN_EVENT_TABLE(ProcessingToolboxWidget, wxScrolledWindow)
END_EVENT_TABLE();


//!
ProcessingToolboxWidget::ProcessingToolboxWidget(wxWindow* parent, 
											wxWindowID id, 
											const wxPoint& pos, 
											const wxSize& size, 
											long style, 
											const wxString& name)
: BaseToolboxWidget(parent, id, pos, size, style, name)
{
	SetBitmap( processingtools_xpm );

	m_processorWidget = NULL;
}

//!
ProcessingToolboxWidget::~ProcessingToolboxWidget(void)
{
}

Core::Widgets::ProcessingWidget* Core::Widgets::ProcessingToolboxWidget::GetProcessorWidget()
{
	return m_processorWidget;
}

wxWindow* Core::Widgets::ProcessingToolboxWidget::GetToolWindow()
{
	if ( m_processorWidget == NULL )
	{
		return NULL;
	}

	wxWindow* window = dynamic_cast<wxWindow*> (m_processorWidget);
	return window;
}

void Core::Widgets::ProcessingToolboxWidget::SetToolWindow( wxWindow* win )
{
	ProcessingWidget* processingWidget;
	processingWidget = dynamic_cast<ProcessingWidget*> ( win );
	m_processorWidget = processingWidget;
}

std::string Core::Widgets::ProcessingToolboxWidget::GetNameMenuItem()
{
	return  "Tools";
}

WIDGET_TYPE Core::Widgets::ProcessingToolboxWidget::GetWidgetState()
{
	return WIDGET_TYPE_PROCESSING_TOOL;
}

