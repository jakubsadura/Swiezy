/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _msWidgetCollective_H
#define _msWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "msProcessorCollective.h"

/**

\ingroup ManualSegmentationPlugin
\author Xavi Planes
\date 12 April 2010
*/

class msWidgetCollective : public Core::WidgetCollective
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		msWidgetCollective, 
		Core::WidgetCollective );

	//!
	void Init();

private:
	msWidgetCollective( );

private:

};

#endif //_msWidgetCollective_H
