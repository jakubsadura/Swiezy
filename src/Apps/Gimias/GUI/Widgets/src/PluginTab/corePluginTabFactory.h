/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef corePluginTabFactory_H
#define corePluginTabFactory_H

#include "corePluginTab.h"
#include "coreObject.h"


#define wxID_WorkflowNavigation wxID( "wxID_WorkflowNavigation" )
#define wxID_IOToolbar wxID( "wxID_IOToolbar" )
#define wxID_CommandPanel wxID("wxID_CommandPanel")
#define wxID_WorkingAreaConfig wxID("wxID_WorkingAreaConfig")
#define wxID_SelectionToolboxWidget wxID("wxID_SelectionToolboxWidget")


namespace Core
{

namespace Widgets
{

/**
\brief Create PluginTab

\ingroup gmWidgets
\sa FrontEndPlugin
\author Xavi Planes
\date 10 April 2010
*/
class GMWIDGETS_EXPORT PluginTabFactory : public Core::Object
{
public:

	//!
	static PluginTab* Build( const std::string &caption );

	//!
	static PluginTab* CreatePluginTab( const std::string &caption );

	//!
	static void CreateWidgets( Core::Widgets::PluginTab* pluginTab );

	//!
	static void RegisterDefaultWidgets( );

	//! Check if the window needs to be created for this tabPage
	static bool CheckCreateWindow( 
		const std::string &factoryName,
		const std::string &tabName );

	//! Add window to plugin tab
	static void AddWindow(
		const std::string &factoryName,
		Core::Widgets::PluginTab* pluginTab );

	//! Get list of common widgets when creating a new plugin
	static std::list<std::string> GetCommonWidgetsList( const std::string &name );

protected:

};

} // namespace Widgets
} // namespace Core

#endif // corePluginTabFactory_H

