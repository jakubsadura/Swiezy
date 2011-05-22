/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWidgetCollective_H
#define coreWidgetCollective_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreBaseWindow.h"

namespace Core {

/**
Accessibility functions

\ingroup gmWidget
\author Xavi Planes
\date 16 july 2009
*/

class GMWIDGETS_EXPORT WidgetCollective : public itk::LightObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		WidgetCollective, 
		Core::SmartPointerObject );

protected:
	WidgetCollective( );

	//!
	Core::Widgets::RenderWindowBase *GetActiveMultiRenderWindow( );

	//!
	Core::Widgets::PluginTab* GetPluginTab() const;


protected:
};

} // Core

#endif //coreWidgetCollective_H
