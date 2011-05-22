/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynLibImplLinux_H
#define _dynLibImplLinux_H

#include <string>
#include "dynLibImpl.h"

#ifndef _WIN32

#include "private.h"
#include "libelf.h"
#include "elf.h"
#include <link.h>

/**
Linux implementation DynLib

The decoded function symbols take this form:
- vtkImageThreshold::SetInValue(double)
- vtkImageThreshold::ThresholdByLower(double)
- vtkImageThreshold::New()
- vtkImageThreshold::CollectRevisions(std::ostream&)

See also:
* http://www.sco.com/developers/gabi/2000-07-17/ch4.symtab.html
* http://docsrv.sco.com/SDK_cprog/_Dynamic_Linker.html
* http://fixunix.com/linux/533643-list-symbols-provided-shared-library-runtime.html

Code:
* http://linuxgazette.net/issue85/misc/sandeep/Ptrace.c.txt
* http://nullkey.ath.cx/elfhacks/html/elfhacks_8c-source.html
* http://mykospark.net/2009/09/runtime-backtrace-in-c-with-name-demangling/
* http://www.phrack.com/issues.html?issue=59&id=8&mode=txt
* http://stackoverflow.com/questions/1118705/call-a-function-named-in-a-string-variable-in-c

\author Xavi Planes
\date 09 July 2010
\ingroup DynLib
*/
class dynLibImplLinux : public dynLibImpl
{
public:
	//!
	dynLibImplLinux( );

	//!
	~dynLibImplLinux( );
protected:

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

	//!
	bool OpenDynamicLibrary( 
		const std::string &dllName,
		Elf* &elf,
		struct link_map* &lm,
		std::string& csPathDLLDepend );

	//!
	void CloseDynamicLibrary( Elf* elf );

private:
	//!
	std::string Demangle(const char* symbol);

	bool FindSection( 
		Elf *elf, int type, Elf_Scn* &scn, ElfW(Shdr)* &shdr );

	void ExtractSymbols(
		Elf *elf, 
		Elf_Scn *scn, 
		ElfW(Shdr) *shdr, 
		int type,
		std::list<std::string> &symbols );

	void ExtractDynSymbols(
	      	Elf *elf, 
	      	Elf_Scn *scn, 
	      	ElfW(Shdr) *shdr, 
	      	int type,
	      	std::list<std::string> &dependencies );

};

#endif // _WIN32

#endif // _dynLibImplLinux_H

