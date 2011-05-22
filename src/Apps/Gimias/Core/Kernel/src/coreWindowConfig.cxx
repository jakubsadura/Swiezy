/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWindowConfig.h"

using namespace Core;

Core::WindowConfig::WindowConfig()
{
	m_State = 0;
	SetFlag( WIDGET_STATE_CLOSE_BUTTON, true );
	SetFlag( WIDGET_STATE_CAPTION_VISIBLE, true );
	SetFlag( WIDGET_STATE_PLUGIN_TAB, true );
	m_Type = 0;
	m_Caption = "";
	m_TabPage = "";
	m_PluginName = "";
	m_Dock = WIDGET_DOCK_NONE;
	m_Layer = 0;
	m_Position = 0;
	m_Id = -1;
}

Core::WindowConfig& Core::WindowConfig::Bitmap( const char* const* data )
{
	m_Bitmap_xpm = data;
	return *this;
}

Core::WindowConfig& Core::WindowConfig::Bitmap( const std::string &filename )
{
	m_BitmapFilename = filename;return *this;
}
//!
const char* const* Core::WindowConfig::GetBitmap() const { 

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

WindowConfig& Core::WindowConfig::SetFlag( unsigned int flag, bool option_state )
{
	if (option_state)
		m_State |= flag;
	else
		m_State &= ~flag;
	return *this;
}

bool Core::WindowConfig::HasFlag( unsigned int flag ) const
{
	return (m_State & flag) ? true:false;
}

WindowConfig& Core::WindowConfig::SetType( unsigned int flag, bool option_state )
{
	if (option_state)
		m_Type |= flag;
	else
		m_Type &= ~flag;
	return *this;
}

bool Core::WindowConfig::HasType( unsigned int flag ) const
{
	return (m_Type & flag) ? true:false;
}


Core::WindowConfig& Core::WindowConfig::CommandPanel()
{
	return SetType( WIDGET_TYPE_COMMAND_PANEL, true )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, true );
}

Core::WindowConfig& Core::WindowConfig::VerticalLayout()
{
	m_Layer=3;m_Dock=WIDGET_DOCK_RIGHT;m_Position = 2;return *this;
}

WindowConfig& Core::WindowConfig::Free()
{
	return SetFlag( WIDGET_STATE_PLUGIN_TAB, false );
}

WindowConfig& Core::WindowConfig::PluginName( const std::string &name )
{
	m_PluginName = name;return *this;
}

WindowConfig& Core::WindowConfig::TabPage( const std::string &name )
{
	m_TabPage = name;return *this;
}

Core::WindowConfig& Core::WindowConfig::Toolbar()
{
	return SetType( WIDGET_TYPE_TOOLBAR, true )
		.SetFlag( WIDGET_STATE_CAPTION_VISIBLE, false )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, true )
		.Category( "Toolbars" );
}

WindowConfig& Core::WindowConfig::WorkingArea()
{
	m_Layer = 0;m_Dock = WIDGET_DOCK_CENTER; 
	return SetType( WIDGET_TYPE_WORKING_AREA, true )
		.SetFlag( WIDGET_STATE_CLOSE_BUTTON, false )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, true )
		.SetFlag( WIDGET_STATE_SHOW, false )
		.Category( "Working Areas" );
}

WindowConfig& Core::WindowConfig::ProcessingTool()
{
	return SetType( WIDGET_TYPE_PROCESSING_TOOL, true )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, false );
}

WindowConfig& Core::WindowConfig::SelectionTool()
{
	return SetType( WIDGET_TYPE_SELECTION_TOOL, true )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, false );
}

WindowConfig& Core::WindowConfig::ReaderHeaderWindow()
{
	return SetType( WIDGET_TYPE_READER_HEADER_WINDOW, true )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, false );
}

WindowConfig& Core::WindowConfig::RenderWindow()
{
	return SetType( WIDGET_TYPE_RENDER_WINDOW, true )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, false );
}

WindowConfig& Core::WindowConfig::DataInformation()
{
	return SetType( WIDGET_TYPE_DATA_INFORMATION, true )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, false );
}

WindowConfig& Core::WindowConfig::VisualProps()
{
	return SetType( WIDGET_TYPE_VISUAL_PROPS, true )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, false );
}

WindowConfig& Core::WindowConfig::RenderWindowConfig()
{
	return SetType( WIDGET_TYPE_RENDER_WINDOW_CONFIG, true )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, false );
}

WindowConfig& Core::WindowConfig::Preferences()
{
	return SetType( WIDGET_TYPE_PREFERENCES, true )
		.SetFlag( WIDGET_STATE_PLUGIN_TAB, false );
}

WindowConfig& Core::WindowConfig::Top()
{
	m_Dock = WIDGET_DOCK_TOP;return *this;
}

WindowConfig& Core::WindowConfig::Bottom()
{
	m_Dock = WIDGET_DOCK_BOTTOM;return *this;
}

WindowConfig& Core::WindowConfig::Left()
{
	m_Dock = WIDGET_DOCK_LEFT;return *this;
}

WindowConfig& Core::WindowConfig::Right()
{
	m_Dock = WIDGET_DOCK_RIGHT;return *this;
}

WindowConfig& Core::WindowConfig::Center()
{
	m_Dock = WIDGET_DOCK_CENTER;return *this;
}

WindowConfig& Core::WindowConfig::Layer( int layer )
{
	m_Layer = layer;return *this;
}

WindowConfig& Core::WindowConfig::Position( int val )
{
	m_Position = val;return *this;
}

WindowConfig& Core::WindowConfig::ProcessorObservers()
{
	return SetFlag( WIDGET_STATE_CREATE_PROCESSOR_OBSERVERS, true );
}

WindowConfig& Core::WindowConfig::Show()
{
	return SetFlag( WIDGET_STATE_SHOW, true );
}

WindowConfig& Core::WindowConfig::CloseButton()
{
	return SetFlag( WIDGET_STATE_CLOSE_BUTTON, true );
}

WindowConfig& Core::WindowConfig::Float()
{
	return SetFlag( WIDGET_STATE_FLOATING, true );
}

WindowConfig& Core::WindowConfig::HideCaption()
{
	return SetFlag( WIDGET_STATE_CAPTION_VISIBLE, false );
}

WindowConfig& Core::WindowConfig::Caption( const std::string &caption )
{
	m_Caption = caption;return *this;
}

WindowConfig& Core::WindowConfig::Category( const std::string &val )
{
	m_Category = val;return *this;
}

WindowConfig& Core::WindowConfig::Id( int id )
{
	m_Id = id;return *this;
}

bool Core::WindowConfig::GetCreateProcessorObservers() const
{
	return HasFlag( WIDGET_STATE_CREATE_PROCESSOR_OBSERVERS );
}

bool Core::WindowConfig::GetPluginTab() const
{
	return HasFlag( WIDGET_STATE_PLUGIN_TAB );
}

bool Core::WindowConfig::GetCommandPanel() const
{
	return HasType( WIDGET_TYPE_COMMAND_PANEL );
}

bool Core::WindowConfig::GetToolbar() const
{
	return HasType( WIDGET_TYPE_TOOLBAR );
}

bool Core::WindowConfig::GetWorkingArea() const
{
	return HasType( WIDGET_TYPE_WORKING_AREA );
}

bool Core::WindowConfig::GetProcessingTool() const
{
	return HasType( WIDGET_TYPE_PROCESSING_TOOL );
}

bool Core::WindowConfig::GetSelectionTool() const
{
	return HasType( WIDGET_TYPE_SELECTION_TOOL );
}

bool Core::WindowConfig::GetReaderHeaderWindow() const
{
	return HasType( WIDGET_TYPE_READER_HEADER_WINDOW );
}

bool Core::WindowConfig::GetRenderWindow() const
{
	return HasType( WIDGET_TYPE_RENDER_WINDOW );
}

bool Core::WindowConfig::GetDataInformation() const
{
	return HasType( WIDGET_TYPE_DATA_INFORMATION );
}

bool Core::WindowConfig::GetVisualProps() const
{
	return HasType( WIDGET_TYPE_VISUAL_PROPS );
}

bool Core::WindowConfig::GetRenderWindowConfig() const
{
	return HasType( WIDGET_TYPE_RENDER_WINDOW_CONFIG );
}

bool Core::WindowConfig::GetPreferences() const
{
	return HasType( WIDGET_TYPE_PREFERENCES );
}

std::string Core::WindowConfig::GetCaption() const
{
	return m_Caption;
}

std::string Core::WindowConfig::GetCategory() const
{
	return m_Category;
}

WIDGET_DOCK Core::WindowConfig::GetDock() const
{
	return m_Dock;
}

int Core::WindowConfig::GetLayer() const
{
	return m_Layer;
}

int Core::WindowConfig::GetPosition() const
{
	return m_Position;
}

bool Core::WindowConfig::GetShow() const
{
	return HasFlag( WIDGET_STATE_SHOW );
}

bool Core::WindowConfig::GetCloseButton() const
{
	return HasFlag( WIDGET_STATE_CLOSE_BUTTON );
}

bool Core::WindowConfig::GetFLoat() const
{
	return HasFlag( WIDGET_STATE_FLOATING );
}

bool Core::WindowConfig::GetCaptionVisible() const
{
	return HasFlag( WIDGET_STATE_CAPTION_VISIBLE );
}

std::string Core::WindowConfig::GetBitmapFilename() const
{
	return m_BitmapFilename;
}

int Core::WindowConfig::GetId() const
{
	return m_Id;
}

std::string Core::WindowConfig::GetPluginName()
{
	return m_PluginName;
}

std::string Core::WindowConfig::GetTabPage()
{
	return m_TabPage;
}
