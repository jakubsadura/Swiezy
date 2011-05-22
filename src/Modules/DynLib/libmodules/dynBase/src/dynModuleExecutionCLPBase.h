/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynModuleExecutionCLPBase_H
#define _dynModuleExecutionCLPBase_H

#include "DynLibWin32Header.h"
#include "dynModuleExecutionImpl.h"
#include "ModuleDescription.h"
#include <list>
#include <map>
#include <itksys/Process.h>

/**
Base class implementation for Dynamic ModuleDescription execution for 
Command Line Plugins

\author Xavi Planes
\date 14 July 2010
\ingroup DynLib
*/
class DYNLIB_EXPORT dynModuleExecutionCLPBase : public dynModuleExecutionImpl
{
public:
	typedef dynModuleExecutionCLPBase Self;
	typedef blSmartPointer<Self> Pointer;

	//!
	void Update( );

protected:
	//!
	dynModuleExecutionCLPBase( );

	//!
	virtual ~dynModuleExecutionCLPBase( );

	//! Set a parameter value to m_Filter
	virtual void SetParameterValue( ModuleParameter *param );

	//! Get a parameter value from m_Filter
	virtual void GetParameterValue( ModuleParameter *param );

	//! Build Command line
	virtual void BuildCommandLineLocation( ) = 0;

	//! Print Command line
	void PrintCommandLine( );

	//!
	void SaveCommandLine( );

	//! Add --returnparameterfile
	void AddCommandLineReturnParameters( );

	//! Run the filter 
	virtual void RunFilter( ) = 0;

	//! Wait for output data
	virtual void WaitForData( );

	//! Remove the embedded XML from the stdout stream
	virtual void ProcessOutputInformation( );

protected:

	//!
	std::vector<std::string> m_CommandLineAsString;
	//!
	std::string m_stdoutbuffer;
	//!
	std::string m_stderrbuffer;
	//!
	char **m_Command;
};



#endif // _dynModuleExecutionCLPBase_H

