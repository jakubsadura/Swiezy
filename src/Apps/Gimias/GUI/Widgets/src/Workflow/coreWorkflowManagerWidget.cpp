/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkflowManagerWidget.h"
#include "coreWorkflowManager.h"
#include "coreWorkflowEditorWidget.h"
#include "coreWxMitkGraphicalInterface.h"

#include "GenericWorkflow.xpm"

#define wxID_WorkflowEditorWidget wxID( "wxID_WorkflowEditorWidget" )

BEGIN_EVENT_TABLE(Core::Widgets::WorkflowManagerWidget, coreWorkflowManagerWidgetUI)
	EVT_TREE_BEGIN_LABEL_EDIT( wxID_TREE_WORKFLOWS, Core::Widgets::WorkflowManagerWidget::OnBeginLabelEdit)
	EVT_TREE_END_LABEL_EDIT( wxID_TREE_WORKFLOWS, Core::Widgets::WorkflowManagerWidget::OnEndLabelEdit)
END_EVENT_TABLE()

namespace Core{
namespace Widgets{

class WorkflowData : public wxTreeItemData 
{
public:
	WorkflowData( std::string workflowName )
	{
		m_WorkflowName = workflowName;
	}
	std::string GetWorkflowName() const { return m_WorkflowName; }
	void SetWorkflowName(std::string val) { m_WorkflowName = val; }

private:
	//!
	std::string m_WorkflowName;
};

}
}

Core::Widgets::WorkflowManagerWidget::WorkflowManagerWidget(wxWindow* parent, int id):
    coreWorkflowManagerWidgetUI(parent, id, "Workflow Manager")
{
	m_bmpBitmap->SetBitmap( genericworkflow_xpm );

	UpdateWidget( );

	m_txtWorkflowsFolder->SetValue(
		Core::Runtime::Kernel::GetWorkflowManager()->GetWorkflowsFolder( ) );
}

void Core::Widgets::WorkflowManagerWidget::UpdateWidget()
{
	m_treeWorkflows->DeleteAllItems();
	m_treeWorkflows->AddRoot(wxT( "Workflows" ), -1, -1);

	Core::Workflow::Pointer activeWorkflow;
	activeWorkflow = Core::Runtime::Kernel::GetWorkflowManager()->GetActiveWorkflow( );

	Core::WorkflowManager::WorkflowListType list;
	list = Core::Runtime::Kernel::GetWorkflowManager()->GetWorkflowList( );

	Core::WorkflowManager::WorkflowListType::iterator it;
	for ( it = list.begin() ; it != list.end( ) ; it++ )
	{
		wxTreeItemId item;
		item = m_treeWorkflows->AppendItem( 
			m_treeWorkflows->GetRootItem(), 
			(*it)->GetFullName( ),
			-1, -1, new WorkflowData( (*it)->GetName( ) ) );

		if ( (*it) == activeWorkflow )
		{
			m_treeWorkflows->SetItemBold( item, true );
		}

	}

	m_treeWorkflows->ExpandAll();
}

void Core::Widgets::WorkflowManagerWidget::OnBtnNew(wxCommandEvent &event)
{
	try
	{

		Core::Workflow::Pointer workflow = Workflow::New( "New Workflow" );
		Core::Runtime::Kernel::GetWorkflowManager()->AddWorkflow( workflow );
		UpdateWidget( );

	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowManagerWidget::OnBtnNew )

}


void Core::Widgets::WorkflowManagerWidget::OnBtnEdit(wxCommandEvent &event)
{
	try
	{

		std::string name = GetSelectedWorkflow( );

		Core::Workflow::Pointer workflow;
		workflow = Core::Runtime::Kernel::GetWorkflowManager()->GetWorkflow( name );

		if ( workflow.IsNull() )
		{
			return;
		}

		m_WorkflowEditorWidget = new WorkflowEditorWidget( this , wxID_WorkflowEditorWidget );
		m_WorkflowEditorWidget->SetWorkflow( workflow );
		m_WorkflowEditorWidget->Show();

		this->Hide();

	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowManagerWidget::OnBtnEdit )
}

void Core::Widgets::WorkflowManagerWidget::OnBtnDelete(wxCommandEvent &event)
{
	try
	{
		std::string name = GetSelectedWorkflow( );
		Core::Runtime::Kernel::GetWorkflowManager()->RemoveWorkflow( name );

		UpdateWidget();
	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowManagerWidget::OnBtnDelete )

}

void Core::Widgets::WorkflowManagerWidget::OnBtnActivate( wxCommandEvent &event )
{
	try
	{

		std::string name = GetSelectedWorkflow( );
		Core::Runtime::Kernel::GetWorkflowManager()->SetActiveWorkflow( name );

		Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( )->RestartApplication( );

	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowManagerWidget::OnBtnActivate )

}

bool Core::Widgets::WorkflowManagerWidget::Show( bool show /*= true */ )
{
	if ( show )
	{
		UpdateWidget();
	}
	return coreWorkflowManagerWidgetUI::Show( show );
}

void Core::Widgets::WorkflowManagerWidget::OnItemActivated( wxTreeEvent& event )
{
	try
	{
	    wxCommandEvent commandEvent = wxCommandEvent( );
		OnBtnEdit( commandEvent );
		event.Skip();
	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowManagerWidget::OnItemActivated )
}

void Core::Widgets::WorkflowManagerWidget::OnBtnRename( wxCommandEvent &event )
{
	try
	{
		wxTreeItemId item = m_treeWorkflows->GetSelection();
		m_treeWorkflows->EditLabel( item );
	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowManagerWidget::OnBtnRename )
}

void Core::Widgets::WorkflowManagerWidget::OnBeginLabelEdit( wxTreeEvent &event )
{
	m_OldWorkflowName = GetSelectedWorkflow( );
	// Set workflow name only
	wxTreeItemId item = m_treeWorkflows->GetSelection();
	m_treeWorkflows->SetItemText( item, m_OldWorkflowName );
}

void Core::Widgets::WorkflowManagerWidget::OnEndLabelEdit( wxTreeEvent &event )
{
	try
	{
		Core::Runtime::Kernel::GetWorkflowManager()->RenameWorkflow( 
			m_OldWorkflowName, 
			event.GetLabel().c_str( ) );
	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowManagerWidget::OnEndLabelEdit )

	UpdateWidget();

	event.Veto();
}

std::string Core::Widgets::WorkflowManagerWidget::GetSelectedWorkflow()
{
	wxTreeItemId item = m_treeWorkflows->GetSelection();
	WorkflowData* data = dynamic_cast<WorkflowData*> ( m_treeWorkflows->GetItemData( item ) );
	if ( data == NULL )
	{
		return "";
	}

	std::string name = data->GetWorkflowName( ).c_str();
	return name;
}

void Core::Widgets::WorkflowManagerWidget::OnBtnImport( wxCommandEvent &event )
{
	try
	{
		wxFileDialog* openFileDialog = 
			new wxFileDialog(this, wxT("Import a workflow file"), wxT(""), wxT(""), wxT("*.xml"), 
			wxFD_OPEN | wxFD_FILE_MUST_EXIST );
		
		if(openFileDialog->ShowModal() == wxID_OK)
		{
			Core::Workflow::Pointer workflow;
			workflow = Core::Runtime::Kernel::GetWorkflowManager()->OpenWorkflow( 
				openFileDialog->GetPath().c_str() );
			if ( workflow.IsNotNull() )
			{
				Core::Runtime::Kernel::GetWorkflowManager()->AddWorkflow( workflow );
			}

			UpdateWidget();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkflowManagerWidget::OnEndLabelEdit )
}

void Core::Widgets::WorkflowManagerWidget::OnBtnSaveLayout( wxCommandEvent &event )
{
	Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( )->UpdateActiveWorkflowLayout( );
}
