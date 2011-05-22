/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynLibImpl_H
#define _dynLibImpl_H

#include <string>
#include <stdarg.h>

#include "dynFilterFunction.h"
#include "dynLibFuncEntry.h"
#include "dynLibEntry.h"
#include "dynTreeR.h"

/**
Base class for al DynLib implementations
\author Xavi Planes
\date 09 July 2010
\ingroup DynLib
*/
class dynLibImpl
{
public:	
	typedef void (*SymbolPointer)();

	//!
	dynLibImpl( );

	//!
	~dynLibImpl( );

	//!
	std::string GetLibraryPath() const;
	void SetLibraryPath(std::string val);

	/**
	Find a symbol in all loaded libraries using name like
	"vtkImageThreshold::UpperThreshold" and parameters like
	"int, int"
	*/
	virtual SymbolPointer FindSymbol( 
		const std::string &name );

	//! Scan a dynamic library exported symbols and dependencies
	virtual TreeR GetTreeNode( 
		const std::string &lpctszModName_i, 
		const std::string &path );

	//! Return a list of missing DLLs
	void CheckMissingDlls( 
		std::vector<std::string> &dllArrayName,
		TNode* pNode = NULL );

	//! Find an exported symbol and return the pointer to the function
	void FindExportedSymbol(
		const std::string &name,
		std::string &symbolName, 
		FunctionPointer &symbolPtr,
		TNode* pNode = NULL );

	//! Add string to m_Treeobj
	bool AddString( std::string csData );
	//! Delete string from m_Treeobj
	bool DeleteString( std::string csData );
	//!
	void AddDynamicEntry( const std::string &csData, DLL_ENTRY &stImpExport );
	//!
	std::string RemovePath( const std::string& str );
	//!
	void MakeUpper( std::string &input );
	//! Get DLL_ENTRY from m_DataEntry
	DLL_ENTRY *GetDllEntry( const std::string &input );
protected:

	/**
	Add specific function declaration syntax, like "void" for functions
	without parameters in Windows
	*/
	virtual std::string PreProcessSymbolName( const std::string name );

	/**
	Get export data of a single dynamic library
	Fill ExportEntriesArr and return path of dynamic library in csPath_o
	*/
	virtual bool GetExportData( 
		const char* dynamicLibraryName, 
		SingleMapEntryArray& ExportEntriesArr,
		std::string& csPath_o ) = 0;

	/**
	Get import data of a single dynamic library
	Fill ImportEntriesArr 
	*/
	virtual bool GetImportData( 
		const char* dynamicLibraryName, 
		SingleMapEntryArray& ImportEntriesArr,
		bool &bIsStable ) = 0;

	/**
	Get Export/Import data from all dependent dynamic libraries
	*/
	virtual void GetImportAndExportData( 
		const char* dynamicLibraryName ) = 0;


protected:
	//!
	std::string m_libraryPath;

	//! Information of all loaded dynamic libraries
	static MapEntry m_DataEntry;
	//! Dependency tree
	TreeR m_Treeobj;
	//! Current tree depth
	static int m_ndata;
	//! Current dynamic library tree position using the format "vtkImaging\vtkFiltering"
	static std::string m_csdata;
	//! Current directory
	std::string m_csCurrentDirectory;

	//! Parameter to enable import data
	bool m_enableImportData;
	//! Parameter to enable only export functions
	bool m_enableOnlyExportFunctions;
};


#endif // _dynLibImpl_H

