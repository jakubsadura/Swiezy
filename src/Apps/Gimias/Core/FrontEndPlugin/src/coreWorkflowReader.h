/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _WorkflowReader_H
#define _WorkflowReader_H

#include "coreBaseWorkflowIO.h"

namespace Core{

/**
Reader for a Workflow

\author Xavi Planes
\date Nov 2010
\ingroup gmFrontEndPlugin
*/
class GMFRONTENDPLUGIN_EXPORT WorkflowReader : public BaseWorkflowIO 
{
public:
	coreDeclareSmartPointerClassMacro( WorkflowReader, BaseWorkflowIO );
	
protected:
	WorkflowReader( );

	~WorkflowReader( );

	//!
	virtual void InternalUpdate( );

protected:
};

} // Core

#endif //_WorkflowReader_H
