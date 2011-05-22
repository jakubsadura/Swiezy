/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _ntWidgetCollective_H
#define _ntWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "ntProcessorCollective.h"

/**

\ingroup NeuroToolsPlugin
\author Xavi Planes
\date 12 April 2010
*/

class ntWidgetCollective : public Core::WidgetCollective
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		ntWidgetCollective, 
		Core::WidgetCollective );

	//!
	void Init();

private:
	ntWidgetCollective( );

private:

};

#endif //_ntWidgetCollective_H
