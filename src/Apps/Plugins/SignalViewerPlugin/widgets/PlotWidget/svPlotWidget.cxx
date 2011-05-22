/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svPlotWidget.h"
#include "coreDataEntityListBrowser.h"
#include "coreReportExceptionMacros.h"
#include "svSignalPlotAnnotationsWidget.h"
#include "coreSignalESEDPropagationProcessor.h"
#include "svSignalNotebookPage.h"

BEGIN_EVENT_TABLE(svPlotWidget, svPlotWidgetUI)
END_EVENT_TABLE();


svPlotWidget::svPlotWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
svPlotWidgetUI(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	SetName( "Signal viewer" );

	// Remove dafault tab
	m_plotNotebook->DeleteAllPages( );

	m_ParamHolder = svPlotParametersHolderType::New( );
	m_ParamHolder->AddObserver(
		this,
		&svPlotWidget::OnModifiedParams );
}


/**
*/
svPlotWidget::~svPlotWidget()
{
	m_RenderingTreeObserver.disconnect( );
}

void svPlotWidget::SetTimeStepHolder(
	Core::IntHolderType::Pointer timeStepHolder)
{
	this->m_TimeStepHolder = timeStepHolder;
}

/**
*/
void svPlotWidget::Add( 
	Core::DataEntity::Pointer dataEntity )
{
	size_t pageID;
	if ( GetsvSignalNotebookPageID( dataEntity, pageID ) )
	{
		return;
	}

	// Update visible property
	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree( )->GetNode( dataEntity );
	Core::CastAnyProcessingData( anyData, node );
	if ( node.IsNull() )
	{
		return;
	}

	// If node is already in the map -> Don't add it again
	if ( m_MapObserversNode.find( node ) == m_MapObserversNode.end( ) )
	{
		itk::SimpleMemberCommand<svPlotWidget>::Pointer command;
		command = itk::SimpleMemberCommand<svPlotWidget>::New();
		command->SetCallbackFunction(
			this,
			&svPlotWidget::OnModifiedRenderingTree );
		m_MapObserversNode[ node ] = node->GetPropertyList()->AddObserver(
			itk::ModifiedEvent(), 
			command);
	}

	wxString  tabTitle = dataEntity->GetMetadata( )->GetName();

	svSignalNotebookPage* page = new svSignalNotebookPage( m_plotNotebook );
	page->SetTimeStepHolder( m_TimeStepHolder );
	page->SetDataEntityListBrowser( GetListBrowser() );
	// This call will automatically update the windows
	page->SetInputDataEntity( dataEntity );
	m_plotNotebook->AddPage( page, tabTitle, true );


}

void svPlotWidget::Remove( Core::DataEntity::Pointer dataEntity )
{
	size_t pageID;
	if ( GetsvSignalNotebookPageID( dataEntity, pageID ) )
	{
		m_plotNotebook->DeletePage( pageID );
	}

}

bool svPlotWidget::Enable( bool enable /*= true */ )
{
	bool previousEnabled = IsEnabled();
	bool ret = svPlotWidgetUI::Enable( enable );

	if ( enable )
	{
		RefreshSelectedInput( );
	}

	return ret;
}

void svPlotWidget::OnModifiedInputDataEntity()
{
	if ( !IsEnabled( ) )
	{
		OnModifiedRenderingTree( );
	}
	else
	{
		RefreshSelectedInput();

		// Update ED and ES Time steps
		Core::SignalESEDPropagationProcessor::Pointer processor;
		processor = Core::SignalESEDPropagationProcessor::New( );
		processor->SetInputDataEntity( 0, m_InputDataHolder->GetSubject( ) );
		processor->Update( );
	}
}

void svPlotWidget::RefreshSelectedInput()
{
	try
	{
		OnModifiedRenderingTree();

		Core::DataEntity::Pointer dataEntity;
		dataEntity = m_InputDataHolder->GetSubject();

		// If is registered -> only select it
		size_t pageID;
		if ( GetsvSignalNotebookPageID( dataEntity, pageID ) )
		{
			m_plotNotebook->SetSelection( pageID );
		}

	}
	coreCatchExceptionsReportAndNoThrowMacro( 
		"RefreshSelectedInput" );
}


bool svPlotWidget::GetsvSignalNotebookPageID( 
	Core::DataEntity::Pointer dataEntity, size_t &selectedPageID )
{
	if ( dataEntity.IsNull() )
	{
		return false;
	}

	for ( int pageID = 0 ; pageID < m_plotNotebook->GetPageCount() ; pageID++ )
	{
		svSignalNotebookPage* page;
		page = dynamic_cast<svSignalNotebookPage*> ( m_plotNotebook->GetPage( pageID ) );

		if ( page )
		{
			Core::DataEntity::Pointer currentDataEntity;
			currentDataEntity = page->GetMapper()->GetInputDataEntity( 0 );
			if ( currentDataEntity.IsNotNull() && 
				currentDataEntity->GetId() == dataEntity->GetId() )
			{
				selectedPageID = pageID;
				return true;
			}
		}
	}

	return false;
}

void svPlotWidget::SynchronizeAll( bool val )
{
	for ( int pageID = 0 ; pageID < m_plotNotebook->GetPageCount() ; pageID++ )
	{
		svSignalNotebookPage* page;
		page = dynamic_cast<svSignalNotebookPage*> ( m_plotNotebook->GetPage( pageID ) );

		if ( page )
		{
			page->GetMapper()->SynchronizeAll( val );
		}
	}
}

void svPlotWidget::SetInputDataHolder( Core::DataEntityHolder::Pointer val )
{
	m_InputDataHolder = val;
	m_InputDataHolder->AddObserver(
		this,
		&svPlotWidget::OnModifiedInputDataEntity );
}

wxNotebook* svPlotWidget::GetPlotNotebook() const
{
	return m_plotNotebook;
}

void svPlotWidget::SetRenderingTree( Core::RenderingTree::Pointer val )
{
	m_RenderingTreeObserver.disconnect( );

	BaseWindow::SetRenderingTree( val );

	if ( GetRenderingTree( ).IsNotNull() )
	{
		m_RenderingTreeObserver = GetRenderingTree( )->AddObserverOnModified( 
			this, 
			&svPlotWidget::OnModifiedRenderingTree );
		OnModifiedRenderingTree( );
	}
}

void svPlotWidget::OnModifiedRenderingTree()
{
	if ( GetRenderingTree( ).IsNull() || GetListBrowser() == NULL )
	{
		return;
	}

	Core::DataEntityList::Pointer dataList;
	dataList = GetListBrowser( )->GetDataEntityList();

	Core::DataEntityList::iterator it;
	for( it = dataList->Begin();  it != dataList->End(); dataList->Next(it) )
	{
		Core::DataEntity::Pointer dataEntity = dataList->Get(it);
		if ( dataEntity.IsNotNull( ) && dataEntity->GetType() == Core::SignalTypeId )
		{
			bool showedDataEntity = GetRenderingTree( )->IsDataEntityShown( dataEntity );
			if ( showedDataEntity )
			{
				Add( dataEntity );
			}
			else
			{
				Remove( dataEntity );
			}
		}
	}

	// Remove pages with NULL input
	for ( int pageID = 0 ; pageID < m_plotNotebook->GetPageCount() ; pageID++ )
	{
		svSignalNotebookPage* page;
		page = dynamic_cast<svSignalNotebookPage*> ( m_plotNotebook->GetPage( pageID ) );

		if ( page )
		{
			Core::DataEntity::Pointer currentDataEntity;
			currentDataEntity = page->GetMapper()->GetInputDataEntity( 0 );
			if ( currentDataEntity.IsNull() )
			{
				m_plotNotebook->DeletePage( pageID );
			}
		}

	}
}

Core::BaseProcessor::Pointer svPlotWidget::GetProcessor()
{
	return NULL;
}

svPlotParametersHolderType::Pointer svPlotWidget::GetParamHolder() const
{
	return m_ParamHolder;
}

void svPlotWidget::OnPageChanged( wxNotebookEvent &event )
{
	svSignalNotebookPage* page;
	page = dynamic_cast<svSignalNotebookPage*> ( m_plotNotebook->GetCurrentPage( ) );

	// Set the current subject
	if ( page )
	{
		m_ParamHolder->SetSubject( page->GetMapper()->GetParamHolder( )->GetSubject( ) );
	}
}

void svPlotWidget::OnModifiedParams()
{
	svSignalNotebookPage* page;
	page = dynamic_cast<svSignalNotebookPage*> ( m_plotNotebook->GetCurrentPage( ) );

	// Notify current mapper to update the changes
	if ( page )
	{
		page->GetMapper()->GetParamHolder( )->NotifyObservers( );
	}
}
