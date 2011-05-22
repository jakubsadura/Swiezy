/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _medWidgetCollective_H
#define _medWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "medProcessorCollective.h"

/**

\ingroup MeshEditorPlugin
\author Xavi Planes
\date 12 April 2010
*/

class medWidgetCollective : public Core::WidgetCollective
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		medWidgetCollective, 
		Core::WidgetCollective );

	//!
	void Init();

private:
	medWidgetCollective( );

private:

};

#endif //_medWidgetCollective_H
