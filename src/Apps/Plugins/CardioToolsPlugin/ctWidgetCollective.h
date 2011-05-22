/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _ctWidgetCollective_H
#define _ctWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "ctProcessorCollective.h"

/**

\ingroup CardioToolsPlugin
\author Xavi Planes
\date 12 April 2010
*/

class ctWidgetCollective : public Core::WidgetCollective
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		ctWidgetCollective, 
		Core::WidgetCollective );

	//!
	void Init();

private:
	ctWidgetCollective( );

private:

};

#endif //_ctWidgetCollective_H
