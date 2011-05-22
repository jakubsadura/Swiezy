/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseWindow.h"
#include "coreProcessorOutputsObserverBuilder.h"

Core::BaseWindow::BaseWindow()
{
	m_HelperWidget = NULL;
	m_MultiRenderWindow = NULL;
	//m_LandmarkSelectorWidget = NULL;
	m_ListBrowser = NULL;
	m_PluginTab = NULL;
	m_Module = NULL;
	m_MetadataHolder = MetadataHolderType::New( );
	m_MetadataHolder->SetSubject( blTagMap::New( ) );
	m_MetadataHolder->AddObserver( this, &BaseWindow::OnModifiedMetadata );
}

Core::BaseWindow::~BaseWindow()
{

}

void Core::BaseWindow::SetRenderingTree( RenderingTree::Pointer tree )
{
	m_RenderingTree = tree;
	for ( int i = 0 ; i < m_ProcessorOutputObserverVector.size() ; i++ )
	{
		m_ProcessorOutputObserverVector[ i ]->SetRenderingTree( tree );
	}
}

Core::RenderingTree::Pointer Core::BaseWindow::GetRenderingTree() const
{
	return m_RenderingTree;
}

Core::Widgets::UserHelper * Core::BaseWindow::GetHelperWidget() const
{
	return m_HelperWidget;
}

void Core::BaseWindow::SetHelperWidget( Core::Widgets::UserHelper * val )
{
	m_HelperWidget = val;
}

Core::Widgets::RenderWindowBase* Core::BaseWindow::GetMultiRenderWindow() const
{
	return m_MultiRenderWindow;
}

void Core::BaseWindow::SetMultiRenderWindow( Core::Widgets::RenderWindowBase* val )
{
	m_MultiRenderWindow = val;
}

Core::Widgets::DataEntityListBrowser* Core::BaseWindow::GetListBrowser() const
{
	return m_ListBrowser;
}

void Core::BaseWindow::SetListBrowser( Core::Widgets::DataEntityListBrowser* val )
{
	m_ListBrowser = val;
}

void Core::BaseWindow::InitProcessorObservers( bool enableDefaultObservers )
{
	if ( GetProcessor().IsNull() )
	{
		return;
	}

	if ( enableDefaultObservers )
	{
		Core::Widgets::ProcessorOutputsObserverBuilder::Pointer observerBuilder;
		observerBuilder = Core::Widgets::ProcessorOutputsObserverBuilder::New( );
		observerBuilder->Init( GetProcessor().GetPointer(), GetRenderingTree() );
		m_ProcessorOutputObserverVector = observerBuilder->GetList();
	}

	// Connect input observers
	for ( int i = 0 ; i < GetProcessor()->GetNumberOfInputs() ; i++ )
	{
		Core::DataEntityHolder::Pointer holder;
		holder = GetProcessor()->GetInputDataEntityHolder( i );
		Core::DataHolderBase::SignalType *signal;
		signal = holder->GetSignal( DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT );
		signal->connect( boost::bind( &BaseWindow::OnModifiedInput, this, i ) );
		signal = holder->GetSignal( DH_NEW_SUBJECT );
		signal->connect( boost::bind( &BaseWindow::OnNewInput, this, i ) );
	}

	// Connect output observers
	for ( int i = 0 ; i < GetProcessor()->GetNumberOfOutputs() ; i++ )
	{
		Core::DataEntityHolder::Pointer holder;
		holder = GetProcessor()->GetOutputDataEntityHolder( i );
		Core::DataHolderBase::SignalType *signal;
		signal = holder->GetSignal( DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT );
		signal->connect( boost::bind( &BaseWindow::OnModifiedOutput, this, i ) );
		signal = holder->GetSignal( DH_NEW_SUBJECT );
		signal->connect( boost::bind( &BaseWindow::OnNewOutput, this, i ) );
	}
}

Core::Widgets::ProcessorInputWidget* Core::BaseWindow::GetInputWidget( int num )
{
	if ( m_ProcessorInputWidgetMap.find( num ) == m_ProcessorInputWidgetMap.end( ) )
	{
		return NULL;
	}
	return m_ProcessorInputWidgetMap[ num ];
}

void Core::BaseWindow::SetInputWidget( int num, Core::Widgets::ProcessorInputWidget* widget )
{
	if ( m_ProcessorInputWidgetMap.find( num ) != m_ProcessorInputWidgetMap.end( ) )
	{
		return;
	}
	m_ProcessorInputWidgetMap[ num ] = widget;
}

Core::ProcessorOutputObserver::Pointer Core::BaseWindow::GetProcessorOutputObserver( int num )
{
	return m_ProcessorOutputObserverVector.at( num );
}

void Core::BaseWindow::OnModifiedInput( int num )
{

}

void Core::BaseWindow::OnModifiedOutput( int num )
{

}

int Core::BaseWindow::GetTimeStep() const
{
	if ( m_TimeStepHolder.IsNull() )
	{
		return 0;
	}
	return m_TimeStepHolder->GetSubject();
}

void Core::BaseWindow::SetTimeStepHolder( Core::IntHolderType::Pointer val )
{
	m_TimeStepHolder = val;
}

void Core::BaseWindow::SetTimeStep( int time )
{
	m_TimeStepHolder->SetSubject( time );
}

void Core::BaseWindow::OnNewInput( int num )
{

}

void Core::BaseWindow::OnNewOutput( int num )
{

}

Core::Widgets::PluginTab* Core::BaseWindow::GetPluginTab() const
{
	return m_PluginTab;
}

void Core::BaseWindow::SetPluginTab( Core::Widgets::PluginTab* val )
{
	m_PluginTab = val;
}

void Core::BaseWindow::SetBitmap( const char* const* data )
{
	m_Bitmap_xpm = data;
}

const char* const* Core::BaseWindow::GetBitmap() const { 

	if ( m_Bitmap_xpm.empty() )
	{
		return NULL;
	}

	try
	{
		return boost::any_cast< const char* const*>( m_Bitmap_xpm ); 
	}
	catch(boost::bad_any_cast&)
	{
	}
	return NULL;
}

void Core::BaseWindow::Init( )
{
	OnInit();
}

void Core::BaseWindow::OnInit()
{

}

std::string Core::BaseWindow::GetFactoryName() const
{
	return m_FactoryName;
}

void Core::BaseWindow::SetFactoryName( std::string val )
{
	m_FactoryName = val;
}

void Core::BaseWindow::SetProperties( blTagMap::Pointer tagMap )
{

}

void Core::BaseWindow::GetProperties( blTagMap::Pointer tagMap )
{

}

std::string Core::BaseWindow::GetBitmapFilename() const
{
	return m_BitmapFilename;
}

void Core::BaseWindow::SetBitmapFilename( std::string val )
{
	m_BitmapFilename = val;
}

void Core::BaseWindow::SetModule( ModuleDescription* module )
{
	m_Module = module;
}

ModuleDescription* Core::BaseWindow::GetModule()
{
	return m_Module;
}

Core::BaseProcessor::Pointer Core::BaseWindow::GetProcessor()
{
	return NULL;
}

Core::DataHolder<blTagMap::Pointer>::Pointer Core::BaseWindow::GetMetadataHolder() const
{
	return m_MetadataHolder;
}

blTagMap::Pointer Core::BaseWindow::GetMetadata() const
{
	return m_MetadataHolder->GetSubject();
}

void Core::BaseWindow::OnModifiedMetadata()
{
	
}
