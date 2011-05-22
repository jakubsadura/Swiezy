/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef coreFrontEndPlugin_H
#define coreFrontEndPlugin_H

#include "gmFrontEndPluginWin32Header.h"
#include "coreObject.h"
#include "coreBaseFrontEndPlugin.h"
#include "corePluginMacros.h"

namespace Core{
namespace FrontEndPlugin {

/**
\brief This class is the base class for Core plug-ins.
A plug-in is a computer program that interacts with a main 
application to provide a certain,
usually very specific, function.

The main application provides services which the plug-ins can use, 
including a way for plug-ins to register
themselves with the main application and a protocol by which data is 
exchanged with plug-ins. Plug-ins are
dependent on these services provided by the main application and do 
not usually work by themselves.
Conversely, the main application is independent of the plug-ins, making 
it possible for plug-ins to be
added and updated dynamically without changes to the main application.

Plug-ins are implemented as shared objects or libraries that are not 
explicitly linked to the libraries
of the main application, but are created and attached to it during runtime.

The FrontEndPlugin base class can provide a GUI widget that can be 
inserted in the Core graphical interface dynamically.
In that sense, a FrontEndPlugin derived class can provide a command
panel, a working area and/or button toolbar.
If the plug-in does not provide a working area, then the outputs of the 
plug-in can be displayed using the View plug-in
(this is another plug-in that is loaded in the application for almost 
all user profiles).

The FrontEndPlugin base class provides access to the rendering engine 
of the Core, and manages it by itself, though every
plugin is responsible of rendering its own scene.


<h2>Implementation issues</h2>

The main application can decide to hide the plug-in if it does not 
belong to the user's profile.
A FrontEndPlugins derived class must provide some functions that 
returns an initialized plug-in and allows
to configure it. These functions are used by the Core to instantiate 
the plugin.

You must use the <i>coreBeginDefinePluginMacro()</i>, a set of 
<i>coreDefinePluginAddProfileMacro(className, caption)</i>,
and close it by using coreEndDefinePluginMacro() that will construct 
the required definitions for you. 
Place the macros at your plugin cxx file.

\ingroup gmFrontEndPlugin
\author Juan A. Moya
\date 10 Jan 2008
*/
class GMFRONTENDPLUGIN_EXPORT FrontEndPlugin 
	: public Core::FrontEndPlugin::BaseFrontEndPlugin
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		FrontEndPlugin, 
		Core::FrontEndPlugin::BaseFrontEndPlugin);
	

protected:
	//!
    FrontEndPlugin();

	//!
	virtual ~FrontEndPlugin(void);

private:

	coreDeclareNoCopyConstructors(FrontEndPlugin);

private:

};

} // FrontEndPlugin  )
} // Core

#endif

