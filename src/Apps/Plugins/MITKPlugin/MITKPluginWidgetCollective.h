/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _MITKPluginWidgetCollective_H
#define _MITKPluginWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "MITKPluginProcessorCollective.h"

#define wxID_BBoxWidget wxID( "wxID_BBoxWidget" )

namespace MITKPlugin{

class SubtractPanelWidget;
class ShapeScalePanelWidget;
class ResamplePanelWidget;

/**

\ingroup MITKPlugin
\author Xavi Planes
\date Nov 2010
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

	//!
	void LoadStateMachines();

private:
};

} // namespace MITKPlugin

#endif //_MITKPluginWidgetCollective_H
