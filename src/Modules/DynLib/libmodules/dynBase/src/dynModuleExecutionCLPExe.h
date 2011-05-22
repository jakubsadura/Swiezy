/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynModuleExecutionCLPExe_H
#define _dynModuleExecutionCLPExe_H

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
class DYNLIB_EXPORT dynModuleExecutionCLPExe : public dynModuleExecutionCLPBase
{
public:
	typedef dynModuleExecutionCLPExe Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	defineModuleFactory( dynModuleExecutionCLPExe );

protected:
	//!
	dynModuleExecutionCLPExe( );

	//!
	virtual ~dynModuleExecutionCLPExe( );

	//!
	virtual void BuildCommandLineLocation( );

	//!
	virtual void RunFilter( );

	//!
	virtual void WaitForData( );

	//! Remove the embedded XML from the stdout stream
	virtual void ProcessOutputInformation( );

private:

	//!
	itksysProcess *m_Process;
};



#endif // _dynModuleExecutionCLPExe_H

