/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svWidgetCollective_H
#define _svWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

/**

\ingroup SceneViewPlugin
\author Xavi Planes
\date 12 April 2010
*/

class svWidgetCollective : public Core::WidgetCollective
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		svWidgetCollective, 
		Core::WidgetCollective );

	//!
	void Init();

private:
	svWidgetCollective( );

private:

};

#endif //_svWidgetCollective_H
