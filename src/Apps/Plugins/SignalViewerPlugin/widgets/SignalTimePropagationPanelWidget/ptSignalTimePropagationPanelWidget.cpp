// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptSignalTimePropagationPanelWidget.h"

ptSignalTimePropagationPanelWidget::ptSignalTimePropagationPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    ptSignalTimePropagationPanelWidgetUI(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	m_processor = Core::SignalTimePropagationProcessor::New();
	UpdateWidget();
}

Core::BaseProcessor::Pointer ptSignalTimePropagationPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptSignalTimePropagationPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"\n\nUsage: set markers per scan and push the button.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptSignalTimePropagationPanelWidget::Enable");

	return bReturn;
}

void ptSignalTimePropagationPanelWidget::OnButtonSignalTimePropagation(wxCommandEvent &event)
{
	UpdateProcessor( );
}

void ptSignalTimePropagationPanelWidget::OnMarkersPerScan( wxCommandEvent &event )
{
	UpdateData();
}

void ptSignalTimePropagationPanelWidget::UpdateData()
{
	long markers = 1;
	m_txtMarkersPerScan->GetValue( ).ToLong( &markers );
	m_processor->SetMarkersPerScan( markers );
}

void ptSignalTimePropagationPanelWidget::UpdateWidget()
{
	long markers = m_processor->GetMarkersPerScan( );
	m_txtMarkersPerScan->SetValue( wxString::Format( "%d", markers ) );
}
