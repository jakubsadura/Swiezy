/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWorkflowFactory_H
#define coreWorkflowFactory_H

#include "coreObject.h"
#include "coreWorkflow.h"

class wxAuiNotebook;

namespace Core
{

namespace Widgets
{

/**
\brief Create a wxAuiNotebook from a Workflow

\ingroup gmWidgets
\sa FrontEndPlugin
\author Xavi Planes
\date 10 April 2010
*/
class GMWIDGETS_EXPORT WorkflowGUIBuilder : public Core::Object
{
public:

	//!
	static wxAuiNotebook* Build( Core::Workflow::Pointer workflow );


protected:

};

} // namespace Widgets
} // namespace Core

#endif // coreWorkflowFactory_H

