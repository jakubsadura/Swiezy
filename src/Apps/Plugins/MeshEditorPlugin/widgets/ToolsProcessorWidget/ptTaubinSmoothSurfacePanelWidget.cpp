// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptTaubinSmoothSurfacePanelWidget.h"

ptTaubinSmoothSurfacePanelWidget::ptTaubinSmoothSurfacePanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptTaubinSmoothSurfacePanelWidget::ptTaubinSmoothSurfacePanelWidget
    btnTaubinSmoothSurface = new wxButton(this, wxID_btnTaubinSmoothSurface, wxT("Taubin Smoothing"));
    m_processor = TaubinSmoothProcessor::New();
	m_processor->SetName( "TaubinSmoothProcessor" );
	m_processor->GetOutputPort( 0 )->SetDataEntityName( "Taubin Smoothed" );

	do_layout();
}


BEGIN_EVENT_TABLE(ptTaubinSmoothSurfacePanelWidget, wxPanel)
    // begin wxGlade: ptTaubinSmoothSurfacePanelWidget::event_table
    EVT_BUTTON(wxID_btnTaubinSmoothSurface, ptTaubinSmoothSurfacePanelWidget::OnButtonTaubinSmoothSurface)
    // end wxGlade
END_EVENT_TABLE();


void ptTaubinSmoothSurfacePanelWidget::OnButtonTaubinSmoothSurface(wxCommandEvent &event)
{
	UpdateProcessor( );
}


void ptTaubinSmoothSurfacePanelWidget::do_layout()
{
    // begin wxGlade: ptTaubinSmoothSurfacePanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    GlobalSizer->Add(btnTaubinSmoothSurface, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptTaubinSmoothSurfacePanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptTaubinSmoothSurfacePanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Taubin Smooth Surface: surface smoothing using the" \
			"\nTaubin algorithm." \
			"\n\nUsage: no options, just run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptTaubinSmoothSurfacePanelWidget::Enable");

	return bReturn;
}
