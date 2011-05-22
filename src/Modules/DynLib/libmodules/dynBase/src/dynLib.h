/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynLib_H
#define _dynLib_H

#include "DynLibWin32Header.h"
#include <string>
#include "dynLibImpl.h"

/**
Facilitates searching a symbol in a dynamic library using the function name
and parameters and returns the SymbolPointer
\author Xavi Planes
\date 09 July 2010
\ingroup DynLib
*/
class DYNLIB_EXPORT dynLib
{
public:
	typedef void (*SymbolPointer)();

	//!
	dynLib( );

	//!
	~dynLib( );

	//!
	std::string GetLibraryPath() const;
	void SetLibraryPath(std::string val);

	//!
	SymbolPointer FindSymbol( const std::string &name );

	//! Return missing dependent DLLs when trying to load a DLL in string format
	bool CheckMissingDependencies( 
		const char* filename,
		const char* path,
		std::string &dllArrayNames );


private:
	//!
	std::string m_libraryPath;
	//!
	dynLibImpl *m_Impl;
};


#endif // _dynLib_H

