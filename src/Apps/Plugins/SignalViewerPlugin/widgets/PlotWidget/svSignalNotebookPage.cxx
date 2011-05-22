// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "svSignalNotebookPage.h"
#include "coreDataEntityListBrowser.h"
#include "blSignalCollective.h"

#define wxID_ZOOM_IN_Y_ALL wxID( "wxID_ZOOM_IN_Y_ALL" )
#define wxID_ZOOM_OUT_Y_ALL wxID( "wxID_ZOOM_OUT_Y_ALL" )
#define wxID_FIT_ALL wxID( "wxID_FIT_ALL" )


BEGIN_EVENT_TABLE(svSignalNotebookPage, wxWindow)
  EVT_MENU( wxID_ZOOM_IN_Y_ALL, svSignalNotebookPage::OnZoomInYAll)
  EVT_MENU( wxID_ZOOM_OUT_Y_ALL, svSignalNotebookPage::OnZoomOutYAll)
  EVT_MENU( wxID_FIT_ALL, svSignalNotebookPage::OnFitAll)
END_EVENT_TABLE();

svSignalNotebookPage::svSignalNotebookPage( 
	wxWindow *parent, 
	wxWindowID winid /*= wxID_ANY*/ )
	: wxScrolledWindow( parent, winid )
{

	m_Mapper = svSignalPlotMapper::New();
	m_Mapper->SetParentWindow( this );
	m_Mapper->GetInputPort( 0 )->GetDataEntityHolder()->AddObserverOnModified<svSignalNotebookPage>(
		this,
		&svSignalNotebookPage::OnModifiedInputDataEntity );

	m_DataEntityListBrowser = NULL;
}

svSignalNotebookPage::~svSignalNotebookPage()
{
	// We don't need to destroy anything because all the child windows 
	// of this wxWindow are destroyed automatically
	if ( m_timeStepHolderConnection.connected() )
	{
		m_timeStepHolderConnection.disconnect();
	}

	m_Mapper->GetInputPort( 0 )->GetDataEntityHolder()->RemoveObserverOnModified<svSignalNotebookPage>(
		this,
		&svSignalNotebookPage::OnModifiedInputDataEntity );
}

void svSignalNotebookPage::UpdateMapper( )
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = m_Mapper->GetInputDataEntity( 0 );

	// If input is NULL -> Removed from Data List
	if ( dataEntity.IsNull() )
	{
		return;
	}

	// Check if number of signals has changed
	// and new plot windows needs to be created
	blSignalCollective::Pointer signalCollective;
	Core::DataEntityHelper::GetProcessingData(
		m_Mapper->GetInputDataEntityHolder( 0 ), 
		signalCollective);

	std::vector<mpWindow*> plotWindows;
	plotWindows = m_Mapper->GetsvSignalPlotWindowList( );

	if ( signalCollective->GetNumberOfSignals() != plotWindows.size() )
	{
		// If number of windows != number of signals -> Clear all old windows
		// and create new ones
		if ( GetSizer() )
		{
			GetSizer()->Clear( true );
		}

		m_Mapper->Update();

		plotWindows = m_Mapper->GetsvSignalPlotWindowList( );

		wxBoxSizer* sizerVert = new wxBoxSizer(wxVERTICAL);
		std::vector<mpWindow*>::iterator it;
		for (it = plotWindows.begin(); it != plotWindows.end(); it++)
		{
			// Append zoom in Y all to all windows
			(*it)->GetPopupMenu()->Append( wxID_SEPARATOR );
			(*it)->GetPopupMenu()->Append( wxID_ZOOM_IN_Y_ALL, _("Zoom in Y all") );
			(*it)->GetPopupMenu()->Append( wxID_ZOOM_OUT_Y_ALL, _("Zoom out Y all") );
			(*it)->GetPopupMenu()->Append( wxID_FIT_ALL, _("Fit all") );

			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add((*it), 1, wxEXPAND | wxALL, 2);
			(*it)->Show();
			sizerVert->Add(sizer,1, wxEXPAND, 0);
			wxSize size = sizerVert->GetMinSize( );
			size = size;
		}

		SetSizer(sizerVert);
		SetScrollRate(10,10);
	}
	else
	{
		// Update data of signals
		m_Mapper->UpdateSignals();
	}

}

void svSignalNotebookPage::OnModifiedInputDataEntity()
{
	UpdateMapper( );
}

void svSignalNotebookPage::SetTimeStepHolder( 
	Core::IntHolderType::Pointer timeStepHolder )
{
	m_timeStepHolder = timeStepHolder;
	m_Mapper->SetTimeStepHolder( m_timeStepHolder );
}

void svSignalNotebookPage::SetInputDataEntity( 
	Core::DataEntity::Pointer dataEntity )
{
	m_Mapper->SetInputDataEntity( 0, dataEntity );
	m_Mapper->SetOutputDataEntity( 0, dataEntity );

	// Connect input/output to data entity list
	Core::DataEntityList::Pointer list;
	list = m_DataEntityListBrowser->GetDataEntityList();
	list->ConnectInputHolder( m_Mapper->GetInputDataEntityHolder( 0 ) );
	list->ConnectOutputHolder( m_Mapper->GetOutputDataEntityHolder( 0 ) );

}

bool svSignalNotebookPage::GetDataEntityId( unsigned int &dataEntityID )
{
	if ( m_Mapper->GetInputDataEntity( 0 ).IsNotNull() )
	{
		dataEntityID = m_Mapper->GetInputDataEntity( 0 )->GetId();
		return true;
	}

	return false;
}

void svSignalNotebookPage::SetDataEntityListBrowser( Core::Widgets::DataEntityListBrowser* val )
{
	m_DataEntityListBrowser = val;
}


void svSignalNotebookPage::OnZoomInYAll( wxCommandEvent& event )
{
	m_Mapper->ZoomInYAll( );
}

void svSignalNotebookPage::OnZoomOutYAll( wxCommandEvent& event )
{
	m_Mapper->ZoomOutYAll( );
}

void svSignalNotebookPage::OnFitAll( wxCommandEvent& event )
{
	m_Mapper->FitAll( );
}

svSignalPlotMapper::Pointer svSignalNotebookPage::GetMapper() const
{
	return m_Mapper;
}

