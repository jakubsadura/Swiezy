/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginWidgetCollective_H
#define _TavernaPluginWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "TavernaPluginProcessorCollective.h"

namespace TavernaPlugin{

/** 

\ingroup TavernaPlugin
\author Xavi Planes
\date 23 Dec 2010
*/
class WidgetCollective : public Core::WidgetCollective
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		WidgetCollective, 
		Core::WidgetCollective );

private:
    //! The constructor instantiates all the widgets and registers them.
	WidgetCollective( );

	//!
	~WidgetCollective( );

};

} // namespace TavernaPlugin

#endif //_TavernaPluginWidgetCollective_H
