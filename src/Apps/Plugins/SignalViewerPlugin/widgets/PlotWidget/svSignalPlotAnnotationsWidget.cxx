/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svSignalPlotAnnotationsWidget.h"
#include "coreDataEntityListBrowser.h"
#include "coreReportExceptionMacros.h"
#include "svSignalEventData.h"

#include "blSignalEvent.h"
#include "blMitkUnicode.h"

svSignalPlotAnnotationsWidget::svSignalPlotAnnotationsWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
svSignalPlotAnnotationsWidgetUI(parent, id, pos, size, style)
{
	SetName( "SignalPlotAnnotations" );

	CreateComboBoxList();

	m_OutputDataHolder = Core::DataEntityHolder::New( );
	m_OutputDataHolder->SetName( "svSignalPlotAnnotationsWidget: Output" );
}


/**
*/
svSignalPlotAnnotationsWidget::~svSignalPlotAnnotationsWidget()
{
	m_listAnnotation.clear();
}

void svSignalPlotAnnotationsWidget::OnSelectedAnnotation( 
	wxCommandEvent &event )
{
	UpdateWidget();
}

void svSignalPlotAnnotationsWidget::OnComboSelected( wxCommandEvent &event )
{
	UpdateAnnotationsList( );
	UpdateData();
}

void svSignalPlotAnnotationsWidget::UpdateWidget()
{
	if (m_listAnnotation.size() == 0)
	{
		m_listBoxAnnotations->Clear();
		m_cmbType->SetValue("");
		m_txtDuration->SetValue("");
		return;
	}
	
	int selectedAnnotation = m_listBoxAnnotations->GetSelection();
	m_listBoxAnnotations->Clear();
	for ( int i = 0 ; i < m_listAnnotation.size() ; i++ )
	{
		m_listBoxAnnotations->Append( m_listAnnotation[ i ].m_annotation->GetName() );
	}

	if ( selectedAnnotation != -1 && m_listBoxAnnotations->GetCount() != 0 )
	{
		// Check limits of selection
		selectedAnnotation = std::min( unsigned( selectedAnnotation ), m_listBoxAnnotations->GetCount() - 1 );
		m_listBoxAnnotations->SetSelection( selectedAnnotation );
		selectedAnnotation = m_listBoxAnnotations->GetSelection();

		std::vector<blSignalEvent> events = blSignalEventTable::GetAllEvents();

		int annotype = m_listAnnotation.at( selectedAnnotation ).m_annotation->GetType();

		// Find annotype in the combo list
		svSignalEventData *item;
		for ( int i = 0 ; i < m_cmbType->GetCount() ; i++ )
		{
			item = dynamic_cast<svSignalEventData*> ( m_cmbType->GetClientObject( i ) );
			if ( item->GetSignalEvent().m_Type == annotype )
			{
				m_cmbType->SetSelection( i );
			}
		}

		float duration = m_listAnnotation.at( selectedAnnotation ).m_annotation->GetDuration();
		m_txtDuration->SetValue(wxString::Format("%.1f",duration));
	}
}

void svSignalPlotAnnotationsWidget::UpdateData()
{
	try
	{
		blSignalCollective::Pointer signals;
		Core::DataEntityHelper::GetProcessingData(
			m_InputDataHolder, signals);

		int selectedAnnotation = m_listBoxAnnotations->GetSelection();
		if ( selectedAnnotation != -1 )
		{
			int selectedAnnotationID = m_listAnnotation.at(selectedAnnotation).m_annotationId;
			int signalID = m_listAnnotation.at(selectedAnnotation).m_signalId;

			svSignalEventData *item = NULL;
			if ( m_cmbType->GetSelection() != -1 )
			{
				item = dynamic_cast<svSignalEventData*> ( 
					m_cmbType->GetClientObject( m_cmbType->GetSelection() ) );
			}

			if ( item != NULL )
			{
				blSignalAnnotation::Pointer annotation;
				annotation = signals->GetSignal( signalID )->GetAnnotation( selectedAnnotationID );

				unsigned long time = annotation->GetMTime();
				annotation->SetType( item->GetSignalEvent().m_Type );
				annotation->SetDuration( std::atof( m_txtDuration->GetValue() ) );
				if ( time != annotation->GetMTime() )
				{
					// Refresh the observers
					m_OutputDataHolder->GetSubject()->Modified( );
					m_OutputDataHolder->NotifyObservers( );
				}
			}
		}

	}
	coreCatchExceptionsLogAndNoThrowMacro(svSignalPlotAnnotationsWidget::UpdateData)

}

void svSignalPlotAnnotationsWidget::SetInputDataHolder( 
	Core::DataEntityHolder::Pointer input )
{
	m_InputDataHolder = input;

	m_InputDataHolder->AddObserver(this,
		&svSignalPlotAnnotationsWidget::OnInputDataChanged);
}

void svSignalPlotAnnotationsWidget::OnInputDataChanged()
{

	try
	{
		// Update the current output and connect to data list
		// to update the observers when annotations are modified
		// Input holder is already connected by the svPlotManagerWidget
		m_OutputDataHolder->SetSubject( m_InputDataHolder->GetSubject( ) );
		Core::DataEntityList::Pointer list;
		list = m_DataEntityListBrowser->GetDataEntityList();
		list->ConnectOutputHolder( m_OutputDataHolder );

		UpdateAnnotationsList();

		UpdateWidget();

	}
	coreCatchExceptionsReportAndNoThrowMacro(svSignalPlotAnnotationsWidget::OnInputDataChanged)
}

void svSignalPlotAnnotationsWidget::CreateComboBoxList()
{
	m_cmbType->Clear();
	std::vector<blSignalEvent> events = blSignalEventTable::GetAllEvents();
	std::vector<blSignalEvent>::iterator iterator = events.begin();
	while (iterator != events.end())
	{
		if ( blSignalEventTable::IsEventSelected( iterator->m_Type ) )
		{
			m_cmbType->Append( 
				_U(iterator->m_Description.c_str()),
				new svSignalEventData( *iterator ) );
		}
		iterator++;
	}
}

void svSignalPlotAnnotationsWidget::OnRemoveButton( wxCommandEvent &event )
{
	try
	{
		blSignalCollective::Pointer signals;
		Core::DataEntityHelper::GetProcessingData(
			m_InputDataHolder, signals);

		if ( m_listAnnotation.size() == 0)
			return;
		
		int selectedAnnotation = m_listBoxAnnotations->GetSelection();
		if ( selectedAnnotation != -1 )
		{
			int selectedAnnotationID = m_listAnnotation.at(selectedAnnotation).m_annotationId;
			int signalID = m_listAnnotation.at(selectedAnnotation).m_signalId;
			signals->GetSignal(signalID)->RemoveAnnotation( selectedAnnotationID );
		}
		
		m_OutputDataHolder->GetSubject()->Modified( );
		m_OutputDataHolder->NotifyObservers( );

		UpdateAnnotationsList();

		UpdateWidget();

	}
	coreCatchExceptionsReportAndNoThrowMacro(svSignalPlotAnnotationsWidget::OnRemoveButton)
}

void svSignalPlotAnnotationsWidget::UpdateAnnotationsList()
{
	try
	{
		m_listAnnotation.clear();

		blSignalCollective::Pointer signals;
		Core::DataEntityHelper::GetProcessingData(
			m_InputDataHolder, signals);

		for (int i=0; i < signals->GetNumberOfSignals(); i++)
		{
			std::vector< blSignalAnnotation::Pointer > annotations;
			annotations = signals->GetSignal(i)->GetAnnotationsVector();
			for (int k=0; k< annotations.size(); k++)
			{
				svSignalsAnnotation ann;
				ann.m_annotation = annotations.at(k);
				ann.m_signalId = i;
				ann.m_annotationId =k;
				m_listAnnotation.push_back(ann);
			}
		}

	}
	coreCatchExceptionsLogAndNoThrowMacro(svSignalPlotAnnotationsWidget::UpdateAnnotationsList)


}

void svSignalPlotAnnotationsWidget::OnDuration( wxCommandEvent &event )
{
	UpdateData();
}

void svSignalPlotAnnotationsWidget::SetDataEntityListBrowser( 
	Core::Widgets::DataEntityListBrowser* val )
{
	m_DataEntityListBrowser = val;
}
