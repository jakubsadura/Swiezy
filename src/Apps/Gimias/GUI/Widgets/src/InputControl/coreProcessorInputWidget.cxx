/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreProcessorInputWidget.h"
#include "coreDataEntityInfoHelper.h"
#include "coreDataEntityListBrowser.h"
#include "coreDataEntityHelper.h"
#include "coreSelectionComboBox.h"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(ProcessorInputWidget, BaseInputControl)
  EVT_CHECKBOX			(wxID_ANY, ProcessorInputWidget::OnCheckBox)
END_EVENT_TABLE()

//!
ProcessorInputWidget::ProcessorInputWidget(
	wxWindow* parent, wxWindowID id, bool bAutomaticSelection, bool bSetDefaultDataEntity,
	const wxPoint& pos, const wxSize& size, 
	long style, const wxString& name)
: BaseInputControl(parent, id, pos, size, style, name, bAutomaticSelection, bSetDefaultDataEntity)
{
	m_InputPort = Core::BaseFilterInputPort::New();

	// Reset the acquired input holder
	SetAcquiredInputDataHolder( m_InputPort->GetDataEntityHolder().GetPointer() );

	m_CheckableControl = NULL;
}

//!
ProcessorInputWidget::~ProcessorInputWidget(void)
{
}


bool Core::Widgets::ProcessorInputWidget::SetAcquiredInputData( 
	Core::DataEntity::Pointer &subject )
{
	// We are going to change the acquired input
	if ( !CheckSubjectIsOk( subject ) )
	{
		return false;
	}

	bool success = m_InputPort->SetDataEntity( subject );

	return success;
}

void Core::Widgets::ProcessorInputWidget::SetInputPort( 
	Core::BaseFilterInputPort::Pointer inputPort )
{
	m_InputPort = inputPort;
	SetAcquiredInputDataHolder( m_InputPort->GetDataEntityHolder().GetPointer() );
	SetHeaderText( m_InputPort->GetName() );
	FillListBox( );
	UpdateOptional( );
	SetName( m_InputPort->GetName() );
}


bool Core::Widgets::ProcessorInputWidget::CheckSubjectIsOk(
	Core::DataEntity::Pointer &subject )
{
	if ( !IsEnabled() )
	{
		return false;
	}

	// If subject is NULL -> We need to return false to avoid crashes
	if ( subject.IsNull( ) )
	{
		return false;
	}

	// Check if is a children of the father
	bool bTypeIsOk = BaseInputControl::CheckSubjectIsOk( subject );
	if ( !bTypeIsOk )
	{
		return false;
	}

	bTypeIsOk = m_InputPort->CheckDataEntityRestrictions( subject );

	return bTypeIsOk;
}

void Core::Widgets::ProcessorInputWidget::UpdateOptional()
{
	if ( m_InputPort.IsNull() )
	{
		return;
	}

	if ( ( m_InputPort->GetOptional() && m_CheckableControl != NULL )
		|| ( !m_InputPort->GetOptional() && m_CheckableControl == NULL ) )
	{
		return;
	}

	if ( m_InputPort->GetOptional() )
	{
		m_CheckableControl = new wxCheckableControl(this, wxID_ANY);
		GetSizer( )->Detach( m_wxListBox );
		// Set proportion to 0
		m_CheckableControl->SetCheckBox( 
			m_CheckableControl->GetCheckBox( ), 
			wxSizerFlags( ).Align( wxALIGN_CENTER_VERTICAL ) );
		m_CheckableControl->AddWindow( m_wxListBox, wxSizerFlags().Expand().Proportion(1) );
		GetSizer( )->Add( m_CheckableControl, wxSizerFlags( ).Expand().Proportion(1) );
		m_InputPort->SetActive( m_CheckableControl->GetValue() );
	}
}

void Core::Widgets::ProcessorInputWidget::OnCheckBox( wxCommandEvent& event )
{
	m_InputPort->SetActive( m_CheckableControl->GetValue() );
}

wxCheckableControl* Core::Widgets::ProcessorInputWidget::GetCheckableControl() const
{
	return m_CheckableControl;
}
