/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynLibImplWin32_H
#define _dynLibImplWin32_H

#include <string>
#include "dynLibImpl.h"

#ifdef _WIN32


/**
\brief PE format parser all the export and import functions of a Windows DLL.
It also creates the dependency tree

- Source: http://www.codeproject.com/KB/tips/PE_File.aspx?display=Print
- Modified to use it using STL

The decoded function symbols take this form:
- public: virtual double __thiscall vtkImageThreshold::GetInValue(void)
- public: void __thiscall vtkImageThreshold::ThresholdByLower(double)
- public: static class vtkImageThreshold * __cdecl vtkImageThreshold::New(void)
- protected: virtual void __thiscall vtkImageThreshold::CollectRevisions(class std::basic_ostream<char,struct std::char_traits<char> > &)
- public: virtual int __thiscall vtkImageThreshold::IsA(char const *)
public: class vtkImageThreshold * __thiscall vtkImageThreshold::NewInstance(void)const
- public: virtual void __thiscall vtkObjectBase::Delete(void)

\author Xavi Planes
\date 24 March 2010
\ingroup DynLib
*/
class dynLibImplWin32 : public dynLibImpl
{
public:
	//!
	dynLibImplWin32( );

	//!
	~dynLibImplWin32( );

protected:
	//! Redefined
	std::string PreProcessSymbolName( const std::string name );

	//! Redefined
	bool GetExportData( 
		const char* dynamicLibraryName, 
		SingleMapEntryArray& ExportEntriesArr,
		std::string& csPath_o );

	//! Redefined
	bool GetImportData( 
		const char* dynamicLibraryName, 
		SingleMapEntryArray& ImportEntriesArr,
		bool &bIsStable );

	//! Redefined
	void GetImportAndExportData( 
		const char* dynamicLibraryName );

	/**
	Return the handle of the dynamic library
	*/
	bool GetDLLLoadAddress(  
		std::string csDLLName,  
		HMODULE& hModDLL,
		std::string& csPathDLLDepend_o );

	//!
	IMAGE_IMPORT_DESCRIPTOR* GetImportDataPointer( 
		const char* dynamicLibraryName,
		HMODULE& hModDLL );

private:
	//! Current Export map 
	SearchMapExport m_MapExport;

};

#endif // _WIN32

#endif // _dynLibImplWin32_H
