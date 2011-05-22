/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkingAreaManagerWidget.h"
#include "coreWorkingAreaManager.h"
#include "corePluginTab.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreWorkingAreaFactory.h"

Core::Widgets::WorkingAreaManagerWidget::WorkingAreaManagerWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    coreWorkingAreaManagerWidgetUI(parent, id, pos, size, wxTAB_TRAVERSAL)
{
}

void Core::Widgets::WorkingAreaManagerWidget::OnNew( wxCommandEvent &event )
{
	std::string name = AskName( "New Working Area" );
	if ( name.empty() )
	{
		return;
	}

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	BaseWorkingAreaStorage::Pointer workingAreaStorage;
	workingAreaStorage = Core::Runtime::Kernel::GetGraphicalInterface()->GetWorkingAreaStorage( );

	std::string factoryName = workingAreaStorage->GetDefaultWorkingAreaFactoryName( );
	BaseWindow *baseWindow = baseWindowFactory->CreateWindow( factoryName, GetPluginTab() );
	wxWindow* window = dynamic_cast<wxWindow*> (baseWindow);
	window->SetName( name );
	window->SetId( wxNewId() );

	Core::WindowConfig config;
	baseWindowFactory->GetWindowConfig( factoryName, config );
	GetPluginTab()->AddWindow( baseWindow, config );
	GetPluginTab()->ShowWindow( name );
}

void Core::Widgets::WorkingAreaManagerWidget::OnSave( wxCommandEvent &event )
{
	WorkingAreaManager* manager = GetPluginTab()->GetWorkingAreaManager();

	BaseWorkingAreaStorage::Pointer workingAreaStorage;
	workingAreaStorage = Core::Runtime::Kernel::GetGraphicalInterface()->GetWorkingAreaStorage( );
	workingAreaStorage->Save( dynamic_cast<BaseWindow*> ( GetActiveWorkingArea() ) );
}

void Core::Widgets::WorkingAreaManagerWidget::OnDelete( wxCommandEvent &event )
{
	BaseWorkingAreaStorage::Pointer workingAreaStorage;
	workingAreaStorage = Core::Runtime::Kernel::GetGraphicalInterface()->GetWorkingAreaStorage( );
	workingAreaStorage->Delete( dynamic_cast<BaseWindow*> ( GetActiveWorkingArea() ) );

	GetPluginTab()->RemoveWindow( GetActiveWorkingArea() );

	WorkingAreaManager* manager = GetPluginTab()->GetWorkingAreaManager();

	std::map<int, wxWindow*> workingAreas = manager->GetWorkingAreaMap();
	if ( workingAreas.size() == 0 )
	{
		return;
	}
	GetPluginTab()->ShowWindow( workingAreas.begin()->first );
}

void Core::Widgets::WorkingAreaManagerWidget::UpdateWidget()
{
	WorkingAreaManager* manager = GetPluginTab()->GetWorkingAreaManager();

	// Refresh list
	m_List->Clear();
	std::map<int, wxWindow*> workingAreas = manager->GetWorkingAreaMap();
	std::map<int, wxWindow*>::iterator it;
	for ( it = workingAreas.begin() ; it != workingAreas.end() ; it++ )
	{
		m_List->Append( it->second->GetName( ) );
	}
	if ( GetActiveWorkingArea() )
	{
		m_List->SetStringSelection( GetActiveWorkingArea()->GetName() );
	}

	m_btnDelete->Enable( manager->GetActiveWorkingArea() );
	m_btnSave->Enable( manager->GetActiveWorkingArea() );
}

Core::Widgets::RenderWindowContainer* Core::Widgets::WorkingAreaManagerWidget::GetActiveWorkingArea()
{
	WorkingAreaManager* manager = GetPluginTab()->GetWorkingAreaManager();
	RenderWindowContainer* workingArea = dynamic_cast<RenderWindowContainer*> ( manager->GetActiveWorkingArea() );
	if ( workingArea == NULL )
	{
		return NULL;
	}

	return workingArea;
}

Core::BaseProcessor::Pointer Core::Widgets::WorkingAreaManagerWidget::GetProcessor()
{
	return NULL;
}

void Core::Widgets::WorkingAreaManagerWidget::SetPluginTab( Core::Widgets::PluginTab* val )
{
	BaseWindow::SetPluginTab( val );

	GetPluginTab()->GetWindowsMapHolder()->AddObserver( 
		this,
		&WorkingAreaManagerWidget::UpdateWidget );

	if ( GetPluginTab()->GetWorkingAreaManager( ) )
	{
		GetPluginTab()->GetWorkingAreaManager( )->GetActiveWorkingAreaHolder( )->AddObserver( 
			this,
			&WorkingAreaManagerWidget::UpdateWidget );
	}
}

void Core::Widgets::WorkingAreaManagerWidget::OnSelectedWorkingArea( wxCommandEvent &event )
{
	GetPluginTab()->ShowWindow( m_List->GetStringSelection() );
}

bool Core::Widgets::WorkingAreaManagerWidget::Enable( bool enable /*= true */ )
{
	if ( GetPluginTab() )
	{
		// If manager is not present -> Don't enable this window
		WorkingAreaManager* manager = GetPluginTab()->GetWorkingAreaManager();
		if ( manager == NULL )
		{
			enable = false;
		}
	}

	return coreWorkingAreaManagerWidgetUI::Enable( enable );
}

void Core::Widgets::WorkingAreaManagerWidget::OnRename( wxCommandEvent &event )
{
	if ( !GetActiveWorkingArea() )
	{
		return;
	}

	std::string name = AskName( GetActiveWorkingArea()->GetName().c_str() );
	if ( name.empty() )
	{
		return;
	}

	BaseWorkingAreaStorage::Pointer workingAreaStorage;
	workingAreaStorage = Core::Runtime::Kernel::GetGraphicalInterface()->GetWorkingAreaStorage( );
	workingAreaStorage->Delete( dynamic_cast<BaseWindow*> ( GetActiveWorkingArea() ) );

	GetActiveWorkingArea()->SetName( name );

	workingAreaStorage->Save( dynamic_cast<BaseWindow*> ( GetActiveWorkingArea() ) );

	UpdateWidget();

	GetPluginTab()->GetWindowsMapHolder()->NotifyObservers();
}

std::string Core::Widgets::WorkingAreaManagerWidget::AskName( std::string name )
{
	// Ask name of new working area
	wxString result = wxGetTextFromUser( "Name: ", "Working Area Name", name );
	if ( result.empty() )
	{
		return "";
	}

	if ( GetPluginTab()->GetWindow( result ) != NULL )
	{
		wxMessageBox( "This name already exists!", "Error" );
		return "";
	}
	
	return result.c_str();
}
