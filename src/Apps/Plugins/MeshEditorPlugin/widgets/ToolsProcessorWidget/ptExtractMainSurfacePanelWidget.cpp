// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptExtractMainSurfacePanelWidget.h"

ptExtractMainSurfacePanelWidget::ptExtractMainSurfacePanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    btnExtractMainSurface = new wxButton(this, wxID_btnExtractMainSurface, wxT("Extract main surface"));
	m_processor = ExtractMainSurfaceProcessor::New();
	m_processor->SetName( "ExtractSurfaceProcessor" );
	m_processor->GetOutputPort( 0 )->SetDataEntityName( "Extracted Main Surface" );

	do_layout();
}


BEGIN_EVENT_TABLE(ptExtractMainSurfacePanelWidget, wxPanel)
    // begin wxGlade: ptExtractMainSurfacePanelWidget::event_table
    EVT_BUTTON(wxID_btnExtractMainSurface, ptExtractMainSurfacePanelWidget::OnButtonExtractMainSurface)
    // end wxGlade
END_EVENT_TABLE();

void ptExtractMainSurfacePanelWidget::OnButtonExtractMainSurface(wxCommandEvent &event)
{
	UpdateProcessor( );
}

void ptExtractMainSurfacePanelWidget::do_layout()
{
    // begin wxGlade: ptExtractMainSurfacePanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    GlobalSizer->Add(btnExtractMainSurface, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptExtractMainSurfacePanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptExtractMainSurfacePanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Extract Main Surface: extract the main (biggest) surface." \
			"\n\nUsage: no options, just run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptExtractMainSurfacePanelWidget::Enable");

	return bReturn;
}
