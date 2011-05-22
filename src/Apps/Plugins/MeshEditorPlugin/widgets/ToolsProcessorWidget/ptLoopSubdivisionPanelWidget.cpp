// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptLoopSubdivisionPanelWidget.h"

ptLoopSubdivisionPanelWidget::ptLoopSubdivisionPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptLoopSubdivisionPanelWidget::ptLoopSubdivisionPanelWidget
    btnLoopSubdivisionSurface = new wxButton(this, wxID_btnLoopSubdivisionSurface, wxT("Loop Subdivision Refine surface"));

   m_processor = LoopSubdivisionProcessor::New();
   m_processor->GetOutputPort( 0 )->SetDataEntityName( "LoopSubdivision Refined" );
   m_processor->SetName( "LoopSubdivisionProcessor" );

   do_layout();
}


BEGIN_EVENT_TABLE(ptLoopSubdivisionPanelWidget, wxPanel)
    // begin wxGlade: ptLoopSubdivisionPanelWidget::event_table
    EVT_BUTTON(wxID_btnLoopSubdivisionSurface, ptLoopSubdivisionPanelWidget::OnButtonLoopSubdivisionSurface)
    // end wxGlade
END_EVENT_TABLE();


void ptLoopSubdivisionPanelWidget::OnButtonLoopSubdivisionSurface(wxCommandEvent &event)
{
	UpdateProcessor( );
}


void ptLoopSubdivisionPanelWidget::do_layout()
{
    // begin wxGlade: ptLoopSubdivisionPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    GlobalSizer->Add(btnLoopSubdivisionSurface, 0, wxALL|wxALIGN_RIGHT, 2);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptLoopSubdivisionPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptLoopSubdivisionPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Loop Subdivision: iterativelly refine the surface." \
			"\n\nUsage: set the density, angle and minimum edges" \
			"\nand then run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptLoopSubdivisionPanelWidget::Enable");

	return bReturn;
}
