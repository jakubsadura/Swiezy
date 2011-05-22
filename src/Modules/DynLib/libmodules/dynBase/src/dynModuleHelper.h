/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynModuleHelper_H
#define _dynModuleHelper_H

#include "DynLibWin32Header.h"
#include "ModuleDescription.h"

/**
Helper functions to search parameters

\author Xavi Planes
\date 05 Jan 2011
\ingroup DynLib
*/
class DYNLIB_EXPORT dynModuleHelper
{
public:

	//! channel == "input" and tag is not file or directory
	static long GetNumberOfInputs( ModuleDescription* module );

	//! channel == "output" and tag is not file or directory
	static long GetNumberOfOutputs( ModuleDescription* module );

	//! 
	static ModuleParameter *GetInput( ModuleDescription* module, int count );

	//! 
	static ModuleParameter *GetOutput( ModuleDescription* module, int count );

	//! Find a parameter using channel value and count
	static ModuleParameter *FindParameter( 
		ModuleDescription* module,
		std::string channel = "",
		int count = 0,
		std::string name = "",
		const std::string &tag ="" );

	//!
	static void* StringToPointer( const std::string name );

	//!
	static std::string PointerToString( void* ptr);

protected:

	//!
	dynModuleHelper( );

	//!
	virtual ~dynModuleHelper( );

private:
};



#endif // _dynModuleHelper_H

