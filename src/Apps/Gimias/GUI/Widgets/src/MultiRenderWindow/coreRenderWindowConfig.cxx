/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreRenderWindowConfig.h"
#include "corePluginTab.h"
#include "coreRenderWindowContainer.h"
#include "coreWorkingAreaManagerWidget.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"

Core::Widgets::RenderWindowConfig::RenderWindowConfig( 
	wxWindow* parent, int id, const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= 0*/)
	: coreRenderWindowConfigUI( parent, id, pos, size, style )
{
	InitComboBox( );

	m_MultiRenderWindowConfigBase = NULL;

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	baseWindowFactory->GetFactoriesHolder()->AddObserver(
		this,
		&RenderWindowConfig::UpdateRegisteredWindows );
	UpdateRegisteredWindows( );
}

void Core::Widgets::RenderWindowConfig::SetMultiRenderWindow( 
	Core::Widgets::RenderWindowBase* multiRenderWindow )
{
	BaseWindow::SetMultiRenderWindow( multiRenderWindow );

	if ( m_MultiRenderWindowConfigBase )
	{
		m_MultiRenderWindowConfigBase->SetView( multiRenderWindow );
	}

	UpdateWidget();
}

Core::BaseProcessor::Pointer Core::Widgets::RenderWindowConfig::GetProcessor()
{
	return NULL;
}


void Core::Widgets::RenderWindowConfig::OnAddCol( wxCommandEvent &event )
{
	RenderWindowContainer* workingArea = GetWorkingArea( );
	if ( !GetWorkingArea( ) )
	{
		return;
	}

	RenderWindowBase *multiRenderWindow = CreateView( workingArea );

	// Add to the container
	workingArea->Add( multiRenderWindow );
	workingArea->SetActiveWindow( multiRenderWindow );

	UpdateWidget();
}

void Core::Widgets::RenderWindowConfig::OnAddRow( wxCommandEvent &event )
{
	RenderWindowContainer* workingArea = GetWorkingArea( );
	if ( !GetWorkingArea( ) )
	{
		return;
	}

	RenderWindowBase *multiRenderWindow = CreateView( workingArea );

	// Add to the container
	RenderWindowBase* window = workingArea->GetActiveWindow( );
	int layer = workingArea->GetMaxLayer();
	if ( window )
	{
		layer = workingArea->GetAuiPaneInfo( window ).dock_layer;
	}
	workingArea->Add( multiRenderWindow, layer );
	workingArea->SetActiveWindow( multiRenderWindow );

	UpdateWidget();
}

void Core::Widgets::RenderWindowConfig::OnRemove( wxCommandEvent &event )
{
	RenderWindowContainer* workingArea = GetWorkingArea( );
	if ( !GetWorkingArea( ) )
	{
		return;
	}

	// Add to the container
	RenderWindowBase* multiRenderWindow = workingArea->GetActiveWindow( );
	workingArea->Destroy( multiRenderWindow );

	UpdateWidget();
}

Core::Widgets::RenderWindowContainer* Core::Widgets::RenderWindowConfig::GetWorkingArea()
{
	if ( !GetPluginTab() )
	{
		return NULL;
	}

	BaseWindow* baseWindow = GetPluginTab()->GetCurrentWorkingArea();
	if ( baseWindow == NULL )
	{
		return NULL;
	}

	RenderWindowContainer* workingArea = dynamic_cast<RenderWindowContainer*> ( baseWindow );
	if ( workingArea == NULL )
	{
		return NULL;
	}

	return workingArea;
}

void Core::Widgets::RenderWindowConfig::UpdateWidget()
{
	RenderWindowContainer* workingArea = GetWorkingArea( );
	RenderWindowBase* window = NULL;
	if ( workingArea )
	{
		window = workingArea->GetActiveWindow( );
	}

	m_btnAddColView->Enable( workingArea );
	m_btnAddRowView->Enable( workingArea );
	m_btnRemoveView->Enable( workingArea && window );

}

void Core::Widgets::RenderWindowConfig::InitComboBox()
{

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	std::list<std::string> factoryList;
	factoryList = baseWindowFactory->GetWindowsList( WIDGET_TYPE_RENDER_WINDOW );
	if ( factoryList.empty() || factoryList.size() == m_ViewList.size( ) )
	{
		return;
	}

	m_ViewList.clear( );
	m_CmbNewView->Clear( );
	std::list<std::string>::iterator it;
	for ( it = factoryList.begin(); it != factoryList.end() ; it++ )
	{
		m_ViewList.push_back( *it );
		WindowConfig config;
		baseWindowFactory->GetWindowConfig( *it, config );
		m_CmbNewView->AppendString( config.GetCaption() );
	}

	m_CmbNewView->SetSelection( 0 );
}

Core::Widgets::RenderWindowBase* 
Core::Widgets::RenderWindowConfig::CreateView( RenderWindowContainer* workingArea )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	std::string factory = m_ViewList[ m_CmbNewView->GetSelection() ];

	BaseWindow* baseWindow = baseWindowFactory->CreateWindow( factory, workingArea );
	RenderWindowBase *multiRenderWindow;
	multiRenderWindow = dynamic_cast<RenderWindowBase*> ( baseWindow );

	GetPluginTab()->InitBaseWindow( baseWindow );

	multiRenderWindow->InitDefaultLayout( );

	wxWindow* win = dynamic_cast<wxWindow*> ( multiRenderWindow );
	std::ostringstream stream;
	stream << "View" << workingArea->GetNumberOfWindows();
	win->SetName( stream.str() );

	Core::DataEntityList::Pointer dataEntityList;
	dataEntityList = Core::Runtime::Kernel::GetDataContainer( )->GetDataEntityList( );
	Core::DataEntityHolder::Pointer selectedDataEntityHolder;
	selectedDataEntityHolder = dataEntityList->GetSelectedDataEntityHolder();
	multiRenderWindow->Init( selectedDataEntityHolder );

	return multiRenderWindow;
}

void Core::Widgets::RenderWindowConfig::UpdateRegisteredWindows()
{
	InitComboBox();

	if ( m_MultiRenderWindowConfigBase != NULL )
	{
		return;
	}

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( WIDGET_TYPE_RENDER_WINDOW_CONFIG );
	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		BaseWindow *baseWin = baseWindowFactory->CreateWindow( *it, this );
		wxWindow* win = dynamic_cast<wxWindow*> ( baseWin );

		wxStaticBox *staticbox = new wxStaticBox(this, -1, wxT("Current View configuration"));
		wxStaticBoxSizer* sizer = new wxStaticBoxSizer(staticbox, wxVERTICAL);
		sizer->Add( win, wxSizerFlags( ).Proportion( 1 ).Expand() );
		GetSizer( )->Add( sizer, wxSizerFlags( ).Proportion( 1 ).Expand().Border() );
		m_MultiRenderWindowConfigBase = dynamic_cast<RenderWindowConfigBase*> ( baseWin );
		// Refresh the pointer
		SetMultiRenderWindow( GetMultiRenderWindow() );
	}
}
