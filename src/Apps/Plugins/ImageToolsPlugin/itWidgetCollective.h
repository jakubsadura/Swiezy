/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _itWidgetCollective_H
#define _itWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "itProcessorCollective.h"

#define wxID_ImageInformationWidget wxID( "wxID_ImageInformationWidget" )

/**

\ingroup ImageToolsPlugin
\author Xavi Planes
\date 12 April 2010
*/

class itWidgetCollective : public Core::WidgetCollective
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		itWidgetCollective, 
		Core::WidgetCollective );

	//!
	void Init();

private:
	itWidgetCollective( );

private:

};

#endif //_itWidgetCollective_H
