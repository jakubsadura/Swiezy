/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifdef WIN32 
#pragma warning(push)
#pragma warning(disable: 4675)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#endif

#include "coreVisualProperties.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include <wxWidgetStackControl.h>
#include "coreInputControl.h"
#include "Palette.xpm"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"

using namespace Core::Widgets;

// Declare events to process
BEGIN_EVENT_TABLE(VisualProperties, wxScrolledWindow)
END_EVENT_TABLE()


bool VisualProperties::resetTargetAfterResize=false; 
//!
VisualProperties::VisualProperties(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxScrolledWindow(parent, id, pos, size, style, name)
{
	SetBitmap( palette_xpm );

	// Create the widgets
	m_StackControl = new wxWidgetStackControl(this, wxID_ANY);
	m_AcquireInput = new InputControl(this, wxID_ANY, true);
	m_EmptyLabel = new wxStaticText(
		m_StackControl, 
		wxID_ANY, 
		wxT("\n\nThere are not available rendering properties \nfor the current input rendering node"), 
		wxDefaultPosition, 
		wxDefaultSize );

	// Add the widgets to the stackControl
	m_StackControl->Add(m_EmptyLabel);

	// Init holders
	m_InputDataEntityHolder = DataEntityHolder::New();
	m_InputDataEntityHolder->SetName( _U( GetLabel() ) + ": Input" );
	m_InputDataEntityHolder->AddObserver(this, &VisualProperties::OnInputDataEntityHolderChanged);

	m_AcquireInput->SetAcquiredInputDataHolder(m_InputDataEntityHolder, Core::UnknownTypeId);

	m_InputRenderingHolder = Core::DataEntityHolder::New();
	m_InputRenderingHolder->SetName( _U( GetLabel() ) + ": Input rendering node" );
	m_InputRenderingHolder->AddObserver(this, &VisualProperties::OnInputRenderingHolderChanged);

	m_hasRenderWindowAttached = false;
	m_StackControl->Raise(m_EmptyLabel);
	SetProperties( );
	DoLayout( );

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	baseWindowFactory->GetFactoriesHolder()->AddObserver(
		this,
		&VisualProperties::UpdateRegisteredWindows );
	UpdateRegisteredWindows( );

}

void VisualProperties::SetProperties()
{
	SetScrollRate(10, 10);
}

void VisualProperties::DoLayout()
{
	// layout them
	wxBoxSizer* vlayout = new wxBoxSizer(wxVERTICAL);
	vlayout->Add(m_AcquireInput, 0, wxEXPAND | wxALL, 4);
	vlayout->Add(m_StackControl, 1, wxEXPAND | wxALL, 4);

	SetSizer(vlayout);
	vlayout->Fit(this);

}
//!
VisualProperties::~VisualProperties(void)
{
}

//!
void VisualProperties::SetListBrowser(
	Core::Widgets::DataEntityListBrowser* listBrowser )
{
	BaseWindow::SetListBrowser( listBrowser );
	m_AcquireInput->SetDataEntityListBrowser( listBrowser ); 
}

void Core::Widgets::VisualProperties::OnInputRenderingHolderChanged( void )
{
	Core::DataEntity::Pointer dataEntity = m_InputRenderingHolder->GetSubject( );
	if(dataEntity.IsNull())
	{
		m_StackControl->Raise(m_EmptyLabel);
		return;
	}

	wxWindow* foundWindow = NULL;
	if ( dataEntity.IsNotNull() )
	{
		for ( int i = 0 ; i < m_StackControl->GetNumberOfWidgets( ) ; i++ )
		{
			wxWindow* win = m_StackControl->GetWidgetByIndex( i );
			VisualPropertiesBase* widget = dynamic_cast<VisualPropertiesBase*> ( win );
			if ( widget && widget->IsValidData( dataEntity ) )
			{
				foundWindow = win;
			}
		}
	}

	if ( foundWindow )
	{
		m_StackControl->Raise( foundWindow );
	}
	else
		m_StackControl->Raise(m_EmptyLabel);

	FitInside( );
	SetSize( GetSize( ) );
}

void Core::Widgets::VisualProperties::SetRenderingTree( RenderingTree::Pointer tree )
{
	if ( GetRenderingTree() )
	{
		GetRenderingTree()->RemoveObserverOnModified( 
			this,
			&VisualProperties::OnModifiedRenderingTree );
	}

	BaseWindow::SetRenderingTree( tree );


	if ( GetRenderingTree() )
	{
		GetRenderingTree()->AddObserverOnModified( 
			this,
			&VisualProperties::OnModifiedRenderingTree );
	}

	Core::DataEntity::Pointer dataEntity = m_InputDataEntityHolder->GetSubject();

	// Update observers
	m_InputRenderingHolder->SetSubject( dataEntity );
}

//!
void VisualProperties::OnInputDataEntityHolderChanged(void)
{
	Core::DataEntity::Pointer dataEntity = m_InputDataEntityHolder->GetSubject();

	// Update observers
	m_InputRenderingHolder->SetSubject( dataEntity );

}

void Core::Widgets::VisualProperties::SetMultiRenderWindow( 
	Core::Widgets::RenderWindowBase *multiRenderWindow )
{
	BaseWindow::SetMultiRenderWindow( multiRenderWindow );

	for ( int i = 0 ; i < m_StackControl->GetNumberOfWidgets( ) ; i++ )
	{
		wxWindow* win = m_StackControl->GetWidgetByIndex( i );
		VisualPropertiesBase* widget = dynamic_cast<VisualPropertiesBase*> ( win );
		if ( widget )
		{
			widget->SetMultiRenderWindow( multiRenderWindow );
		}
	}

	m_InputRenderingHolder->NotifyObservers( );
}

bool Core::Widgets::VisualProperties::Show( bool show /* = true */ )
{
	return wxPanel::Show( show );
}

Core::BaseProcessor::Pointer Core::Widgets::VisualProperties::GetProcessor()
{
	return NULL;
}

void Core::Widgets::VisualProperties::OnModifiedRenderingTree()
{
	m_InputRenderingHolder->NotifyObservers( );
}

void Core::Widgets::VisualProperties::UpdateRegisteredWindows()
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( WIDGET_TYPE_VISUAL_PROPS );
	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		std::list<std::string>::iterator itFound;
		itFound = std::find( m_RegisteredFactoryNameList.begin(), m_RegisteredFactoryNameList.end(), *it );
		if ( itFound == m_RegisteredFactoryNameList.end() )
		{
			BaseWindow *baseWin = baseWindowFactory->CreateWindow( *it, m_StackControl );
			wxWindow* win = dynamic_cast<wxWindow*> ( baseWin );
			VisualPropertiesBase* widget = dynamic_cast<VisualPropertiesBase*> ( win );
			if ( widget == NULL )
			{
				throw Core::Exceptions::Exception( 
					"VisualProperties::UpdateRegisteredWindows",
					"A Visual props window should derive from VisualPropertiesBase" );
			}
			m_StackControl->Add( win );
			m_RegisteredFactoryNameList.push_back( *it );
			widget->SetInputHolder(m_InputRenderingHolder);
		}

	}

	SetMultiRenderWindow( GetMultiRenderWindow() );
}

#ifdef WIN32 
#pragma warning(pop)
#endif
