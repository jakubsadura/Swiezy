/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreWindowConfig_H
#define _coreWindowConfig_H

#include <string>
#include "gmKernelWin32Header.h"

enum WIDGET_DOCK
{
	WIDGET_DOCK_NONE = 0,
	WIDGET_DOCK_TOP = 1,
	WIDGET_DOCK_RIGHT = 2,
	WIDGET_DOCK_BOTTOM = 3,
	WIDGET_DOCK_LEFT = 4,
	WIDGET_DOCK_CENTER = 5
};

enum WIDGET_STATE
{
	WIDGET_STATE_FLOATING			= 1 << 0,
	/** Create observers to processor outputs that will render the data 
	automatically
	*/
	WIDGET_STATE_CREATE_PROCESSOR_OBSERVERS = 1 << 2,
	WIDGET_STATE_SHOW				= 1 << 4,
	WIDGET_STATE_CLOSE_BUTTON		= 1 << 5,
	WIDGET_STATE_CAPTION_VISIBLE	= 1 << 6,
	/** 
	By default all windows are attached to the plugin tab.
	Free window is not attached to the plugin tab
	*/
	WIDGET_STATE_PLUGIN_TAB			= 1 << 7,
	WIDGET_STATE_MAX				= INT_MAX
};

enum WIDGET_TYPE
{
	WIDGET_TYPE_COMMAND_PANEL		= 1 << 0,
	//! Toolbar
	WIDGET_TYPE_TOOLBAR				= 1 << 1,
	//! Working area
	WIDGET_TYPE_WORKING_AREA		= 1 << 2,
	//! Processing tool
	WIDGET_TYPE_PROCESSING_TOOL		= 1 << 3,
	//! Selection tool
	WIDGET_TYPE_SELECTION_TOOL		= 1 << 4,
	//! Window to show file header information
	WIDGET_TYPE_READER_HEADER_WINDOW= 1 << 5,
	//! Render window widget
	WIDGET_TYPE_RENDER_WINDOW		= 1 << 6,
	//! Data information widget
	WIDGET_TYPE_DATA_INFORMATION	= 1 << 7,
	//! Visual properties widget
	WIDGET_TYPE_VISUAL_PROPS		= 1 << 8,
	//! Render window config
	WIDGET_TYPE_RENDER_WINDOW_CONFIG= 1 << 9,
	//! Preferences window
	WIDGET_TYPE_PREFERENCES			= 1 << 10,
	WIDGET_TYPE_MAX					= INT_MAX
};

namespace Core
{

/** 
\brief Widget configuration

\ingroup gmKernel
\author Xavi Planes
\date 13 April 2010
*/
class GMKERNEL_EXPORT WindowConfig{
public:
	//!
	WindowConfig( );

	//! Set m_State flag
	WindowConfig& SetFlag(unsigned int flag, bool option_state);

	//! Get m_State flag
	bool HasFlag(unsigned int flag) const;

	//! Set m_Type flag
	WindowConfig& SetType(unsigned int flag, bool option_state);

	//! Get m_Type flag
	bool HasType(unsigned int flag) const;


	//@{ 
	/// \name Properties

	WindowConfig& PluginName( const std::string &name );
	WindowConfig& TabPage( const std::string &name );
	WindowConfig& Caption( const std::string &caption );
	WindowConfig& Category(const std::string &val);
	WindowConfig& Bitmap( const char* const* data );
	WindowConfig& Bitmap( const std::string &filename );
	WindowConfig& Id( int id );
	std::string GetCaption() const;
	std::string GetCategory() const;
	WIDGET_DOCK GetDock() const;
	int GetLayer() const;
	int GetPosition() const;
	const char* const* GetBitmap() const;
	std::string GetBitmapFilename() const;
	int GetId( ) const;
	std::string GetPluginName( );
	std::string GetTabPage( );
	//@}

	//@{ 
	/// \name Window states
	//! Configure vertical layout window combining different properties
	WindowConfig& VerticalLayout( );
	//! Set WIDGET_STATE_PLUGIN_TAB to false
	WindowConfig& Free( );
	WindowConfig& Top( );
	WindowConfig& Bottom( );
	WindowConfig& Left( );
	WindowConfig& Right( );
	WindowConfig& Center( );
	WindowConfig& Layer( int layer );
	WindowConfig& Position(int val);
	WindowConfig& ProcessorObservers();
	WindowConfig& Show();
	WindowConfig& CloseButton();
	WindowConfig& Float( );
	WindowConfig& HideCaption( );
	bool GetCreateProcessorObservers() const;
	bool GetPluginTab() const;
	bool GetShow() const;
	bool GetCloseButton() const;
	bool GetFLoat( ) const;
	bool GetCaptionVisible( ) const;
	//@}

	//@{ 
	/// \name Window types
	//! Added to plugin tab
	WindowConfig& Toolbar( );
	//! Added to plugin tab
	WindowConfig& CommandPanel( );
	//! Added to plugin tab
	WindowConfig& WorkingArea();
	WindowConfig& ProcessingTool( );
	WindowConfig& SelectionTool( );
	WindowConfig& ReaderHeaderWindow( );
	WindowConfig& RenderWindow( );
	WindowConfig& DataInformation( );
	WindowConfig& VisualProps( );
	WindowConfig& RenderWindowConfig( );
	WindowConfig& Preferences( );

	bool GetCommandPanel() const;
	bool GetToolbar() const;
	bool GetWorkingArea() const;
	bool GetProcessingTool( ) const;
	bool GetSelectionTool( ) const;
	bool GetReaderHeaderWindow( ) const;
	bool GetRenderWindow( ) const;
	bool GetDataInformation( ) const;
	bool GetVisualProps( ) const;
	bool GetRenderWindowConfig( ) const;
	bool GetPreferences( ) const;
	//@}

protected:
	//!
	std::string m_Caption;
	//!
	std::string m_Category;
	//!
	std::string m_TabPage;
	//!
	std::string m_PluginName;
	//!
	WIDGET_DOCK m_Dock;
	//!
	int m_Layer;
	//!
	int m_Position;
	//! A combination of WIDGET_STATE values
	unsigned int m_State;
	//! A combination of WIDGET_TYPE values
	unsigned int m_Type;
	//! Bitmap in xpm format
	boost::any m_Bitmap_xpm;
	//! Window ID
	int m_Id;
	//! Bitmap filename
	std::string m_BitmapFilename;
};


}

#endif // _coreWindowConfig_H
