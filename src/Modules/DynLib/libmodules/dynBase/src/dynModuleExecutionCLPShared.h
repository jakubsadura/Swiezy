/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynModuleExecutionCLPShared_H
#define _dynModuleExecutionCLPShared_H

#include "DynLibWin32Header.h"
#include "dynModuleExecutionCLPBase.h"
#include "ModuleDescription.h"
#include <list>
#include <map>
#include <itksys/Process.h>

/**
Dynamic ModuleDescription execution

Executes a ModuleDescription located in a dynamic library

\author Xavi Planes
\date 14 July 2010
\ingroup DynLib
*/
class DYNLIB_EXPORT dynModuleExecutionCLPShared : public dynModuleExecutionCLPBase
{
public:
	typedef dynModuleExecutionCLPShared Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	defineModuleFactory( dynModuleExecutionCLPShared );

protected:
	//!
	dynModuleExecutionCLPShared( );

	//!
	virtual ~dynModuleExecutionCLPShared( );

	//!
	void BuildCommandLineLocation( );

	//!
	void RunFilter( );

private:

};



#endif // _dynModuleExecutionCLPShared_H

