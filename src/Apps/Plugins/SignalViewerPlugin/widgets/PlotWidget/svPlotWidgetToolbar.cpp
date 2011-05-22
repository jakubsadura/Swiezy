/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svPlotWidgetToolbar.h"
#include "svPlotManagerWidget.h"
#include "svPlotWidget.h"
#include "svSignalPlotAnnotationsWidget.h"
#include "svPlotFilterAnnotationsWidget.h"
#include "svConfigurationWidget.h"

#include "Annotations.xpm"
#include "Synchronize.xpm"
#include "Filter.xpm"
#include "Config.xpm"

#define wxID_SYNCHRONIZE wxID( "wxID_SYNCHRONIZE" )

BEGIN_EVENT_TABLE(svPlotWidgetToolbar, wxToolBar)
  EVT_TOOL(wxID_ANNOTATIONS, svPlotWidgetToolbar::OnBtnAnnotations)
  EVT_TOOL(wxID_SYNCHRONIZE, svPlotWidgetToolbar::OnBtnSynchronize)
  EVT_TOOL(wxID_FILTER_ANNOTATIONS, svPlotWidgetToolbar::OnBtnFilterAnnotations)
  EVT_TOOL(wxID_SIGNAL_VIEWER_CONFIGURATION, svPlotWidgetToolbar::OnBtnConfiguration)
END_EVENT_TABLE()


svPlotWidgetToolbar::svPlotWidgetToolbar(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxToolBar(parent, id, pos, size, style, name)
{
	SetName( "PlotWidgetToolbar" );

	wxBitmap bitmap = wxBitmap( annotations_xpm );
	AddTool(wxID_ANNOTATIONS, _T("Annotations"),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		_T("Show Annotations widget"), _T("Open annotations widget"));

	bitmap = wxBitmap( synchronize_xpm );
	AddTool(wxID_SYNCHRONIZE, _T("Synchronize"),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		_T("Synchronize with parent data"), _T("Synchronize with parent data"));

	bitmap = wxBitmap( filter_xpm );
	AddTool(wxID_FILTER_ANNOTATIONS, _T("Filter annotations"),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		_T("Filter annotations"), _T("Filter annotations"));

	bitmap = wxBitmap( config_xpm );
	AddTool(wxID_SIGNAL_VIEWER_CONFIGURATION, _T("Signal Viewer Configuration"),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		_T("Signal Viewer Configuration"), _T("Signal Viewer Configuration"));
	
	Realize();
}


svPlotManagerWidget* svPlotWidgetToolbar::GetsvPlotManagerWidget() const
{
	return m_svPlotManagerWidget;
}

void svPlotWidgetToolbar::SetsvPlotManagerWidget( svPlotManagerWidget* val )
{
	m_svPlotManagerWidget = val;
}

void svPlotWidgetToolbar::OnBtnAnnotations(wxCommandEvent &event)
{
	try
	{
		m_svPlotManagerWidget->Show( 
			wxID_ANNOTATIONS,
			GetToolState( wxID_ANNOTATIONS ) );
	}
	coreCatchExceptionsReportAndNoThrowMacro( svPlotWidgetToolbar::OnBtnAnnotations )
}

void svPlotWidgetToolbar::OnBtnSynchronize( wxCommandEvent &event )
{
	try
	{
		m_svPlotManagerWidget->GetPlotWidget()->SynchronizeAll(
			GetToolState( wxID_SYNCHRONIZE ) );
	}
	coreCatchExceptionsReportAndNoThrowMacro( svPlotWidgetToolbar::OnBtnSynchronize )
}

void svPlotWidgetToolbar::OnBtnFilterAnnotations( wxCommandEvent &event )
{
	try
	{
		m_svPlotManagerWidget->Show( 
			wxID_FILTER_ANNOTATIONS,
			GetToolState( wxID_FILTER_ANNOTATIONS ) );
	}
	coreCatchExceptionsReportAndNoThrowMacro( svPlotWidgetToolbar::OnBtnFilterAnnotations )
}

void svPlotWidgetToolbar::OnBtnConfiguration( wxCommandEvent &event )
{
	try
	{
		m_svPlotManagerWidget->Show( 
			wxID_SIGNAL_VIEWER_CONFIGURATION,
			GetToolState( wxID_SIGNAL_VIEWER_CONFIGURATION ) );
	}
	coreCatchExceptionsReportAndNoThrowMacro( svPlotWidgetToolbar::OnBtnConfiguration )
}
