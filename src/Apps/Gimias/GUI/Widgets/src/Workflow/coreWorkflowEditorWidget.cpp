/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkflowEditorWidget.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreWorkflowManager.h"
#include "coreWorkflowTreeItemData.h"

BEGIN_EVENT_TABLE(Core::Widgets::WorkflowEditorWidget, coreWorkflowEditorWidgetUI)
    EVT_TREE_BEGIN_DRAG( wxID_TREE_REGISTERED_WINDOWS, Core::Widgets::WorkflowEditorWidget::OnBeginDragTreeRegisteredWindows)
	EVT_TREE_BEGIN_DRAG( wxID_TREE_WORKFLOW, Core::Widgets::WorkflowEditorWidget::OnBeginDragTreeWorkflow)
END_EVENT_TABLE()


Core::Widgets::WorkflowEditorWidget::WorkflowEditorWidget( wxWindow* parent, int id )
	: coreWorkflowEditorWidgetUI( parent, id, "Workflow Editor" )
{
	UpdateRegisteredWindows( );
}

void Core::Widgets::WorkflowEditorWidget::SetWorkflow( Core::Workflow::Pointer workflow )
{
	m_TreeWorkflow->SetWorkflow( workflow );
}

void Core::Widgets::WorkflowEditorWidget::UpdateRegisteredWindows()
{
	m_TreeRegisteredWindows->UpdateRegisteredWindows();
}

void Core::Widgets::WorkflowEditorWidget::OnBeginDragTreeRegisteredWindows( wxTreeEvent& event )
{
	try
	{

		// need to explicitly allow drag
		wxTextDataObject data( m_TreeRegisteredWindows->GetItemText( event.GetItem() ) );
		wxDropSource dragSource( this );
		dragSource.SetData( data );
		wxDragResult result = dragSource.DoDragDrop( wxDrag_DefaultMove );

		if ( result == wxDragMove )
		{
			wxArrayTreeItemIds selection;
			m_TreeRegisteredWindows->GetSelections( selection );
			std::list<std::string> items;
			for ( size_t i = 0 ; i < selection.size() ; i++ )
			{
				items.push_back( m_TreeRegisteredWindows->GetItemFactoryName( selection[ i ] ) );
			}

			m_TreeWorkflow->AddItems( items );
		}

	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowEditorWidget::OnBeginDragTreeRegisteredWindows )
}

void Core::Widgets::WorkflowEditorWidget::OnBeginDragTreeWorkflow( wxTreeEvent& event )
{
	try
	{

		// need to explicitly allow drag
		if ( event.GetItem() == m_TreeWorkflow->GetRootItem() )
		{
			return;
		}

		wxArrayTreeItemIds selection;
		m_TreeWorkflow->GetSelections( selection );
		m_TreeWorkflow->SetRestrictedSelection( true );


		wxTextDataObject textData( m_TreeWorkflow->GetItemFactoryName( event.GetItem() ) );
		wxDropSource dragSource( this );
		dragSource.SetData( textData );
		wxDragResult result = dragSource.DoDragDrop( wxDrag_DefaultMove );

		if ( result == wxDragNone )
		{
			m_TreeWorkflow->DeleteItems( selection );
		}
		else 
		{
			wxArrayTreeItemIds newSelection;
			m_TreeWorkflow->GetSelections( newSelection );
			m_TreeWorkflow->MoveItems( selection, newSelection );
		}

		m_TreeWorkflow->SetRestrictedSelection( false );

	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowEditorWidget::OnBeginDragTreeWorkflow )

}

void Core::Widgets::WorkflowEditorWidget::OnOk( wxCommandEvent &event )
{
	try
	{

		m_TreeWorkflow->UpdateWorkflow();

		// Save all workflows
		Core::Runtime::Kernel::GetWorkflowManager()->SaveAll( );

		if ( GetParent() != NULL )
		{
			GetParent()->Show( );
		}

		event.Skip();

	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowEditorWidget::OnApply )
}

bool Core::Widgets::WorkflowEditorWidget::Show( bool show /*= true */ )
{
	if ( show )
	{
		UpdateRegisteredWindows();
	}

	return coreWorkflowEditorWidgetUI::Show( show );
}

//!
void Core::Widgets::WorkflowEditorWidget::OnCancel(wxCommandEvent& event)
{
	try
	{

		if ( GetParent() != NULL )
		{
			GetParent()->Show( );
		}

		event.Skip();

	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowEditorWidget::OnCancel )

}
