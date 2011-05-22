/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynModuleExecutionTaverna_H
#define _dynModuleExecutionTaverna_H

#include "DynLibWin32Header.h"
#include "dynModuleExecutionCLPExe.h"
#include "ModuleDescription.h"
#include <list>
#include <map>
#include <itksys/Process.h>

namespace TavernaPlugin{

/**
Executes a ModuleDescription for Taverna Workbench 2.2

\ingroup TavernaPlugin
\author Xavi Planes
\date 05 Jan 2011
*/
class ModuleExecutionTaverna : public dynModuleExecutionCLPExe
{
public:
	typedef ModuleExecutionTaverna Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	defineModuleFactory( ModuleExecutionTaverna );

	//! 
	void BuildCommandLineLocation( );

	//! Set a parameter value to m_Filter
	void SetParameterValue( ModuleParameter *param );

protected:
	//!
	ModuleExecutionTaverna( );

	//!
	virtual ~ModuleExecutionTaverna( );

private:

	//!
	itksysProcess *m_Process;
};

} // namespace TavernaPlugin{

#endif // _dynModuleExecutionTaverna_H

