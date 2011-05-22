/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "coreRenderWindowContainer.h"
#include "corePluginTab.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreMultiRenderWindow.h"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(RenderWindowContainer, wxPanel)
  EVT_MOUSE_EVENTS(RenderWindowContainer::OnWxMouseEvent)
END_EVENT_TABLE()


//!
RenderWindowContainer::RenderWindowContainer(
	wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name)
: wxPanel(parent, id, pos, size, style, name)
{
	SetName( "RenderWindowContainer" );

	m_ActiveWindowHolder = RenderWindowHolderType::New( );
	m_ActiveWindowHolder->SetSubject( NULL );
	m_ActiveWindow = NULL;

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	// Init AUI manager
	m_wxAuiManager.SetManagedWindow( this );
	m_wxAuiManager.SetFlags( wxAUI_MGR_ALLOW_FLOATING |
		wxAUI_MGR_TRANSPARENT_HINT |
		wxAUI_MGR_HINT_FADE |
		wxAUI_MGR_NO_VENETIAN_BLINDS_FADE |
		wxAUI_MGR_ALLOW_ACTIVE_PANE );

	m_MultiRenderWindowLinker = RenderWindowLinker::New( );

	m_StateHolder = Core::DataHolder<State>::New( );
}

//!
RenderWindowContainer::~RenderWindowContainer(void)
{
	m_ActiveWindowHolder = NULL;
	m_wxAuiManager.UnInit();
}

Core::Widgets::RenderWindowHolderType::Pointer 
Core::Widgets::RenderWindowContainer::GetActiveWindowHolder() const
{
	return m_ActiveWindowHolder;
}

RenderWindowBase* 
Core::Widgets::RenderWindowContainer::GetActiveWindow()
{
	return GetActiveWindowHolder( )->GetSubject();
}

bool Core::Widgets::RenderWindowContainer::Enable( bool enable /* = true */ )
{
	bool retVal = wxPanel::Enable( enable );


	if ( enable )
	{
		InitializeUsingProperties();
	}

	std::list<RenderWindowBase*>::iterator it;
	for ( it = m_MultiRenderWindowList.begin() ; 
		 it != m_MultiRenderWindowList.end() ; 
		 it++ )
	{
		(*it)->Enable( enable );
	}

	// Notify observers of current RenderWindowBase that this is the active one
	if ( enable )
	{
		SetActiveWindow( m_ActiveWindow );
	}
	else
	{
		if ( m_ActiveWindow )
		{
			m_ActiveWindow->SetEvtHandlerEnabled( false );
		}

		m_ActiveWindowHolder->SetSubject( NULL );
		if ( GetRenderingTreeManager().IsNotNull() )
		{
			GetRenderingTreeManager()->SetActiveTree( NULL );
		}
	}

	return retVal;
}

void Core::Widgets::RenderWindowContainer::Add( 
	RenderWindowBase* window, int layer )
{
	// Add to the list
	m_MultiRenderWindowList.push_back( window );

	wxWindow* win = dynamic_cast<wxWindow*>( window );
	if ( win == NULL )
	{
		return;
	}

	// Add to aui manager
	if ( layer == -1 )
	{
		m_wxAuiManager.AddPane( win, wxAuiPaneInfo().Layer( GetMaxLayer() + 1 ).Name( win->GetName() ) );
	}
	else
	{
		m_wxAuiManager.AddPane( win, wxAuiPaneInfo().Layer( layer ).Name( win->GetName() ) );
	}
	m_wxAuiManager.Update( );

	// Register rendering tree
	GetRenderingTreeManager()->Add( window->GetPrivateRenderingTree( ) );

	// Add the window to the linker
	m_MultiRenderWindowLinker->AddWindow( window );

	UpdateWindowsPanes( );
}

void Core::Widgets::RenderWindowContainer::Destroy( 
	RenderWindowBase* window )
{
	wxWindow* win = dynamic_cast<wxWindow*>( window );
	if ( win == NULL )
	{
		return;
	}

	SetActiveWindow((Core::Widgets::RenderWindowBase*) NULL );

	// Remove from the list
	m_MultiRenderWindowList.remove( window );

	// Detach from aui manager
	m_wxAuiManager.DetachPane( win );
	m_wxAuiManager.Update( );

	// Remove rendering tree
	GetRenderingTreeManager()->Remove( window->GetPrivateRenderingTree() );

	// Remove window from the linker
	m_MultiRenderWindowLinker->RemoveWindow( window );

	// Destroy window
	win->Destroy();

	if ( m_MultiRenderWindowList.size() )
	{
		SetActiveWindow( *( m_MultiRenderWindowList.rbegin() ) );
	}

	UpdateWindowsPanes( );
}

void Core::Widgets::RenderWindowContainer::SetActiveWindow( 
	RenderWindowBase* window )
{
	std::list<RenderWindowBase*>::iterator it;
	for ( it = m_MultiRenderWindowList.begin() ; 
		it != m_MultiRenderWindowList.end() ; 
		it++ )
	{
		(*it)->SetEvtHandlerEnabled( false );
		m_wxAuiManager.GetPane( (*it)->GetName() ).state &= ~wxAuiPaneInfo::optionActive;
	}

	m_ActiveWindow = window;

	if ( IsEnabled() )
	{
		if ( m_ActiveWindow )
		{
			// Eanble menu event handler
			m_ActiveWindow->SetEvtHandlerEnabled( true );
		}

		m_ActiveWindowHolder->SetSubject( window );

		if ( window )
		{
			GetRenderingTreeManager()->SetActiveTree( window->GetPrivateRenderingTree( ) );
			m_wxAuiManager.GetPane( window->GetName() ).state |= wxAuiPaneInfo::optionActive;
		}

	}

	m_wxAuiManager.Update();
}

void Core::Widgets::RenderWindowContainer::SetActiveWindow( 
	int pos )
{
	if ( pos < 0 ) return;
	if ( size_t(pos) > m_MultiRenderWindowList.size() ) return;

	std::list<RenderWindowBase*>::iterator it = m_MultiRenderWindowList.begin();
	std::advance(it,pos);

	SetActiveWindow( *(it) );
}

void Core::Widgets::RenderWindowContainer::OnWxMouseEvent( 
	wxMouseEvent& event )
{
	if ( event.GetButton() == wxMOUSE_BTN_LEFT )
	{
		std::list<RenderWindowBase*>::iterator it;
		for ( it = m_MultiRenderWindowList.begin() ; 
			it != m_MultiRenderWindowList.end() ; 
			it++ )
		{
			if ( m_wxAuiManager.GetPane( (*it)->GetName() ).state & wxAuiPaneInfo::optionActive )
			{
				SetActiveWindow( (*it) );
			}
		}
	}
	
	event.Skip();
}

void Core::Widgets::RenderWindowContainer::SetRenderingTreeManager( 
	RenderingTreeManager::Pointer val )
{
	// Deprecated. Now it uses GetPluginTab()->GetRenderingTreeManager()
	//m_RenderingTreeManager = val;
}

void Core::Widgets::RenderWindowContainer::UpdateWindowsPanes()
{
	int windowCount = 0;
	std::list<RenderWindowBase*>::iterator it;
	for ( it = m_MultiRenderWindowList.begin() ; 
		it != m_MultiRenderWindowList.end() ; 
		it++ )
	{
		RenderWindowBase* window = (*it);

		// Create name
		wxAuiPaneInfo &paneInfo = m_wxAuiManager.GetPane( window->GetName() );

		std::ostringstream stream;
		stream << "View" << windowCount;
		wxWindow* win = dynamic_cast<wxWindow*> ( window );
		win->SetName( stream.str() );

		// Set name
		// Is not possible to set right because the size will not change 
		// on size event and the windows will not be proportional
		paneInfo.Name( window->GetName() ).Caption( window->GetName() ).CloseButton( false )
			.Show( true ).Center( );

		// Set caption visible
		if ( m_MultiRenderWindowList.size() > 1 )
		{
			paneInfo.CaptionVisible( true );
		}
		else
		{
			paneInfo.CaptionVisible( false );
		}

		windowCount++;
	}

	m_wxAuiManager.Update();

}

void Core::Widgets::RenderWindowContainer::SetActiveWindowHolder( 
	RenderWindowHolderType::Pointer val )
{
	m_ActiveWindowHolder = val;
}

Core::DataHolder<RenderWindowContainer::State>::Pointer 
Core::Widgets::RenderWindowContainer::GetStateHolder() const
{
	return m_StateHolder;
}

void Core::Widgets::RenderWindowContainer::LinkWindows( bool enable )
{
	if ( enable == m_StateHolder->GetSubject().m_LinkedWindows )
	{
		return;
	}

	m_MultiRenderWindowLinker->SetEnableLink3DViews( enable );
	m_MultiRenderWindowLinker->SetEnableLink2DViews( enable );
	m_MultiRenderWindowLinker->SetEnableLinkTime( enable );
	m_MultiRenderWindowLinker->Update();

	m_StateHolder->GetSubject().m_LinkedWindows = enable;
	m_StateHolder->NotifyObservers();
}

Core::BaseProcessor::Pointer Core::Widgets::RenderWindowContainer::GetProcessor()
{
	return NULL;
}

int Core::Widgets::RenderWindowContainer::GetNumberOfWindows()
{
	return m_MultiRenderWindowList.size();
}


void Core::Widgets::RenderWindowContainer::SetProperties( blTagMap::Pointer tagMap )
{
	m_Properties = tagMap;
}

wxAuiPaneInfo & Core::Widgets::RenderWindowContainer::GetAuiPaneInfo( 
	RenderWindowBase* win )
{
	return m_wxAuiManager.GetPane( win->GetName() );
}

void Core::Widgets::RenderWindowContainer::UpdateAui()
{
	m_wxAuiManager.Update();
}

void Core::Widgets::RenderWindowContainer::GetProperties( blTagMap::Pointer tagMap )
{
	tagMap->AddTag( "Size", (unsigned int) (m_MultiRenderWindowList.size()) );
	tagMap->AddTag( "Icon", std::string( GetName().c_str() ) + ".png" );
	tagMap->AddTag( "Name", std::string( GetName().c_str() ) );
	
	int count = 0;
	std::list<RenderWindowBase*>::iterator it;
	for ( it = m_MultiRenderWindowList.begin() ; 
		it != m_MultiRenderWindowList.end() ; 
		it++ )
	{
		std::ostringstream stream;
		stream << "View" << count++;
		
		blTagMap::Pointer tagMapView = blTagMap::New();
		(*it)->GetProperties( tagMapView );

		int layer = GetAuiPaneInfo( *it ).dock_layer;
		tagMapView->AddTag( "Layer", layer );
		int row = GetAuiPaneInfo( *it ).dock_row;
		tagMapView->AddTag( "Row", row );
		tagMapView->AddTag( "Factory", (*it)->GetFactoryName() );

		tagMap->AddTag( stream.str() , tagMapView );
	}
}

void Core::Widgets::RenderWindowContainer::InitializeUsingProperties()
{
	try
	{

		// Initialize the container using m_Properties
		if ( GetNumberOfWindows() != 0 || m_Properties.IsNull() )
		{
			return;
		}

		blTag::Pointer tag = m_Properties->FindTagByName( "Size" );
		if ( tag.IsNull() )
		{
			return;
		}

		unsigned int size = tag->GetValueCasted<unsigned int>( );

		RenderWindowBase *multiRenderWindow = NULL;
		for ( size_t i = 0 ; i < size ; i++ )
		{
			std::ostringstream stream;
			stream << "View" << i;

			blTagMap::Pointer tagMapView;
			tagMapView = m_Properties->GetTagValue<blTagMap::Pointer>( stream.str() );
			if ( tagMapView.IsNull() )
			{
				continue;
			}

			std::string factoryName = GetPropertyWindowFactory( i );
			if ( factoryName.empty( ) )
			{
				continue;
			}

			BaseWindowFactories::Pointer baseWindowFactories;
			baseWindowFactories = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
			BaseWindow* baseWindow = baseWindowFactories->CreateWindow( factoryName, this );
			multiRenderWindow = dynamic_cast<RenderWindowBase*> ( baseWindow );

			// Init base window 
			GetPluginTab()->InitBaseWindow( baseWindow );

			// Set properties from tag map
			multiRenderWindow->SetProperties( tagMapView );

			// Set name
			wxWindow* win = dynamic_cast<wxWindow*> ( multiRenderWindow );
			win->SetName( stream.str() );

			// Init all the window
			Core::DataEntityList::Pointer dataEntityList;
			dataEntityList = Core::Runtime::Kernel::GetDataContainer( )->GetDataEntityList( );
			Core::DataEntityHolder::Pointer selectedDataEntityHolder;
			selectedDataEntityHolder = dataEntityList->GetSelectedDataEntityHolder();
			multiRenderWindow->Init( selectedDataEntityHolder );

			// Add to the container
			Add( multiRenderWindow );

			tag = tagMapView->FindTagByName( "Layer" );
			if ( tag.IsNotNull() )
			{
				 GetAuiPaneInfo( multiRenderWindow ).dock_layer = tag->GetValueCasted<int>();
			}
			tag = tagMapView->FindTagByName( "Row" );
			if ( tag.IsNotNull() )
			{
				GetAuiPaneInfo( multiRenderWindow ).dock_row = tag->GetValueCasted<int>();
			}
		}

		SetActiveWindow( multiRenderWindow );
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		RenderWindowContainer::InitializeUsingProperties )
}

bool Core::Widgets::RenderWindowContainer::CheckValidProperties( )
{
	try
	{
		// Initialize the container using m_Properties
		if ( m_Properties.IsNull() )
		{
			return false;
		}

		blTag::Pointer tag = m_Properties->FindTagByName( "Size" );
		if ( tag.IsNull() )
		{
			return false;
		}

		unsigned int size = tag->GetValueCasted<unsigned int>( );

		RenderWindowBase *multiRenderWindow = NULL;
		for ( size_t i = 0 ; i < size ; i++ )
		{
			if ( GetPropertyWindowFactory( i ).empty( ) )
			{
				return false;
			}
		}

		return true;
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		RenderWindowContainer::CheckValidProperties )

	return false;
}

std::string 
Core::Widgets::RenderWindowContainer::GetPropertyWindowFactory( int i )
{
	try
	{

		std::ostringstream stream;
		stream << "View" << i;

		blTagMap::Pointer tagMapView;
		tagMapView = m_Properties->GetTagValue<blTagMap::Pointer>( stream.str() );
		if ( tagMapView.IsNull() )
		{
			return "";
		}

		// Find if factory exists
		blTag::Pointer tag = tagMapView->FindTagByName( "Factory" );
		std::string factoryName;
		if ( tag.IsNotNull() )
		{
			tag->GetValue( factoryName );
		}
		else
		{
            BaseWindowFactories::Pointer baseWindowFactories;
            baseWindowFactories = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
            std::list<std::string> factories;
            factories = baseWindowFactories->GetWindowsList( 
                WIDGET_TYPE_RENDER_WINDOW, wxT( "Multi Render Window MITK" ) );

			// If tag is not found -> use MITK window for backwards compatibility
            if ( !factories.empty( ) )
            {
			    factoryName = *factories.begin( );
            }
		}

		// Check if factory is registered
		BaseWindowFactories::Pointer baseWindowFactories;
		baseWindowFactories = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
		if ( !baseWindowFactories->FindFactory( factoryName ) )
		{
			factoryName = "";
		}


		return factoryName;
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		RenderWindowContainer::GetPropertyWindowFactory )

	return "";
}


Core::RenderingTreeManager::Pointer 
Core::Widgets::RenderWindowContainer::GetRenderingTreeManager()
{
	if ( GetPluginTab() == NULL )
	{
		return NULL;
	}

	return GetPluginTab()->GetRenderingTreeManager();
}

int Core::Widgets::RenderWindowContainer::GetMaxLayer()
{
	int maxLayer = 0;
	std::list<RenderWindowBase*>::iterator it;
	for ( it = m_MultiRenderWindowList.begin() ; 
		it != m_MultiRenderWindowList.end() ; 
		it++ )
	{
		maxLayer = std::max( m_wxAuiManager.GetPane( (*it)->GetName() ).dock_layer, maxLayer );
	}

	return maxLayer;
}


RenderWindowBase* Core::Widgets::RenderWindowContainer::GetWindow( int pos )
{
	std::list<RenderWindowBase*>::iterator it = m_MultiRenderWindowList.begin();	

	for (int i=0; it != m_MultiRenderWindowList.end() && i<pos; i++) it++;
	if ( it == m_MultiRenderWindowList.end() )
	{
		return NULL;
	}

	return *(it);
}
