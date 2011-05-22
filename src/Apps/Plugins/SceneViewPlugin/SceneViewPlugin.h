/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef SceneViewPlugin_H
#define SceneViewPlugin_H

#include "coreFrontEndPlugin.h"
#include "coreWidgetCollective.h"
#include "svWidgetCollective.h"

namespace Core
{

namespace Plugins
{

/** 
A class that places all the graphical interface items for the Visualization 
functionalities and to trigger all Visualization use cases. The core main 
window adds it on startup time, 
if the profile chosen is type of 'Common Functionalities' (the default profile)

The Visualization Tab Page contains widgets for displaying images and to 
achieve complex 3D interaction
for visualization of data in a scene.

\ingroup SceneViewPlugin
\author Juan Antonio Moya
\date 07 Jan 2008
*/
class PLUGIN_EXPORT SceneViewPlugin 
	: public Core::FrontEndPlugin::FrontEndPlugin
{
public:
	coreDeclareSmartPointerClassMacro(
		SceneViewPlugin, 
		Core::FrontEndPlugin::FrontEndPlugin);

protected:
	SceneViewPlugin(void);
	virtual ~SceneViewPlugin(void);

private:
	coreDeclareNoCopyConstructors(SceneViewPlugin);

private:
	//!
	svWidgetCollective::Pointer m_WidgetCollective;
};

}
}

#endif // SceneViewPlugin_H
