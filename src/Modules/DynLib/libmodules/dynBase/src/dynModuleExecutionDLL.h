/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynModuleExecutionDLL_H
#define _dynModuleExecutionDLL_H

#include "DynLibWin32Header.h"
#include "dynModuleExecutionImpl.h"
#include "dynFilter.h"
#include "ModuleDescription.h"
#include <list>
#include <map>

/**
Dynamic ModuleDescription execution

Executes a ModuleDescription located in a dynamic library

\author Xavi Planes
\date 14 July 2010
\ingroup DynLib
*/
class DYNLIB_EXPORT dynModuleExecutionDLL : public dynModuleExecutionImpl
{
public:
	typedef dynModuleExecutionDLL Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	defineModuleFactory( dynModuleExecutionDLL );

	//!
	void Update( );

protected:
	//!
	dynModuleExecutionDLL( );

	//!
	virtual ~dynModuleExecutionDLL( );

	//! Set a parameter value to m_Filter
	void SetParameterValue( ModuleParameter *param );

	//! Get a parameter value from m_Filter
	void GetParameterValue( ModuleParameter *param );

	//! Build the function to call to set a parameters using ModuleParameter
	dynFilterFunction BuildSetFunction( ModuleParameter &param );

	//! Build the function to call to get a parameters using ModuleParameter
	dynFilterFunction BuildGetFunction( ModuleParameter &param );

private:

	//!
	dynFilter::Pointer m_Filter;
};



#endif // _dynModuleExecutionDLL_H

