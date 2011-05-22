/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _WorkflowWriter_H
#define _WorkflowWriter_H

#include "coreBaseWorkflowIO.h"

namespace Core{

/**
Writer for Workflow

\author Xavi Planes
\date Nov 2010
\ingroup gmFrontEndPlugin
*/
class GMFRONTENDPLUGIN_EXPORT WorkflowWriter : public BaseWorkflowIO 
{
public:
	coreDeclareSmartPointerClassMacro( WorkflowWriter, BaseWorkflowIO );


protected:
	WorkflowWriter( );

	~WorkflowWriter( );

	//!
	virtual void InternalUpdate( );

protected:
};

} // Core

#endif //_WorkflowWriter_H
