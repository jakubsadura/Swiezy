/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynLibImplLinux.h"

#ifndef _WIN32

#include <stdexcept>
#include <sstream>
#include <dlfcn.h>
#include <stdio.h>
#include <cxxabi.h> 

#include "config.h"
#include "gelf.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


dynLibImplLinux::dynLibImplLinux( )
{

}

dynLibImplLinux::~dynLibImplLinux()
{
}

bool dynLibImplLinux::GetExportData( 
	const char* dynamicLibraryName, 
	SingleMapEntryArray& ExportEntriesArr, 
	std::string& csPath_o )
{
	Elf* elf;
	struct link_map* lm;
	if ( !OpenDynamicLibrary( dynamicLibraryName, elf, lm, csPath_o ) )
	{
		return false;
	}

	if ( GetDllEntry( csPath_o  ) == NULL )
	{
	     	Elf_Scn* section = 0;
        	ElfW(Shdr) *shdr;
		FindSection( elf, SHT_SYMTAB, section, shdr );

		std::list<std::string> symbols;
		ExtractSymbols( elf, section, shdr, STT_FUNC, symbols );

		std::list<std::string>::iterator it;
		for ( it = symbols.begin( ) ; it != symbols.end( ) ; it++) 
		{
			SINGLE_FUNCT_ENTRY newElement;
			newElement.csFunction = Demangle( it->c_str( ) );
			// Use dlsym because st_value sometimes is 0
			//newElement.dwData = (void*) (lm->l_addr + esym->st_value);
			newElement.dwData = dlsym( lm, it->c_str( ) );
			newElement.csSymbol = *it;
			ExportEntriesArr.push_back( newElement );
			
		}	

	}

	CloseDynamicLibrary( elf );

	return true;
}

bool dynLibImplLinux::GetImportData( 
	const char* dynamicLibraryName, 
	SingleMapEntryArray& ImportEntriesArr, 
	bool &bIsStable )
{
	return false;
}

void dynLibImplLinux::GetImportAndExportData( 
	const char* dllName )
{
	if( std::string(dllName).empty())
	{
		return;
	}

	Elf* mainElf;
	struct link_map* mainLm;
	std::string csPath_o;
	OpenDynamicLibrary( dllName, mainElf, mainLm, csPath_o );
	if ( mainLm == NULL )
	{
		return;
	}

	Elf_Scn* section = 0;
	ElfW(Shdr) *shdr;
	FindSection( mainElf, SHT_DYNAMIC, section, shdr );

	std::list<std::string> dependencies;
	ExtractDynSymbols( mainElf, section, shdr, STT_NOTYPE, dependencies );

	std::list<std::string>::iterator it;
	for(it = dependencies.begin( ); it != dependencies.end( ) ; it++) {

		AddString( it->c_str() );

		if ( GetDllEntry( it->c_str()  ) == NULL )
		{
			DLL_ENTRY stImpExport;

			std::string csPath_o;
			if ( GetExportData( it->c_str(), stImpExport.ArrayExport, csPath_o ) )
			{
				stImpExport.m_bIsStable = true;
				stImpExport.m_csFullPath = csPath_o;

				// Add data to static internal map
				MakeUpper( *it );
				m_DataEntry[ *it ] = stImpExport;

			}

		}

		DeleteString( it->c_str() );
	}

	dlclose( mainLm );
}

bool dynLibImplLinux::OpenDynamicLibrary( 
	const std::string &dllName, 
	Elf* &elf, 
	struct link_map* &lm,
	std::string& csPathDLLDepend )
{
  int fd;


  if (elf_version(EV_CURRENT) == EV_NONE) {
    /* library out of date */
    std::cerr << "ELF library is out of date, Quittingn";
    return false;
  }

  void* handle;
  std::string filename = dllName;
  if ( dllName.find( "lib" ) == std::string::npos && 
       dllName.find( ".so" ) == std::string::npos )
  {
    filename = "lib" + dllName + ".so";
  }
  handle = dlopen(filename.c_str( ), RTLD_NOW);
  if (handle == 0) {
    std::cerr << " Unable to open " << filename << std::endl;
    return false;
  }
  lm = (struct link_map *)(handle);
  csPathDLLDepend = lm->l_name;

  //filename = "/home/cistib/cistib-code/gimias/development/trunk/bin/thirdparty/Debug/lib/libvtkImaging.so.5.4";
  fd = open( csPathDLLDepend.c_str( ), O_RDONLY );
  if (fd < 0) {
    std::cerr << " Unable to open " << filename << std::endl;
    return false;
  }
  elf = elf_begin(fd, ELF_C_READ, NULL);

  GElf_Ehdr elfhdr;
  if (gelf_getehdr(elf, &elfhdr) ==0){
    std::cerr << "Cannot read elf header. This usually means that the file " << filename << " is not a valid elf file" << std::endl;
    return false;
  }

  dlclose( handle );

  return true;
}

void dynLibImplLinux::CloseDynamicLibrary( Elf* elf )
{
  elf_end(elf);
  close(elf->e_fd);
}

bool dynLibImplLinux::FindSection( 
	Elf *elf, int type, Elf_Scn* &section, ElfW(Shdr)* &shdr )
{
     section = 0;
     while ((section = elf_nextscn(elf, section)) != 0) {

        #if __LIBELF64
		shdr = elf64_getshdr (section);
        #else
		shdr = elf32_getshdr (section);
        #endif// __LIBELF64

        if ( shdr != 0) {

                if (shdr->sh_type == type) {
		  return true;
                }
         }
     }
     /* no symtab */
     return false;
}


void dynLibImplLinux::ExtractSymbols(
      Elf *elf, 
      Elf_Scn *scn, 
      ElfW(Shdr) *shdr, 
      int type,
      std::list<std::string> &symbols )
 {
      Elf_Data *data;
      char *name;
      data = 0;

      if ((data = elf_getdata(scn, data)) == 0 || data->d_size == 0){
          /* error or no data */
          std::cerr << "Section had no data!" << std::endl;
          return;
      }

      /*now extract the symbols*/
      ElfW(Sym) *esym = (ElfW(Sym)*) data->d_buf;
      ElfW(Sym) *lastsym = (ElfW(Sym)*) ((char*) data->d_buf + data->d_size);

      /* now loop through the symbol table and extract it*/
      for (; esym < lastsym; esym++){

        #if __LIBELF64
          if ((ELF64_ST_TYPE(esym->st_info)!= type) ||
		esym->st_value == 0 ) 
                    continue;
        #else
          if ((ELF32_ST_TYPE(esym->st_info)!= type) ||
		esym->st_value == 0 ) 
                    continue;
        #endif // __LIBELF64

           name = elf_strptr(elf,shdr->sh_link , (size_t)esym->st_name);
           if(!name){
                std::cerr << elf_errmsg(elf_errno());
                continue;
           }

           symbols.push_back( name );
       }
}


void dynLibImplLinux::ExtractDynSymbols(
      Elf *elf, 
      Elf_Scn *scn, 
      ElfW(Shdr) *shdr, 
      int type,
      std::list<std::string> &symbols )
 {
      Elf_Data *data;
      data = 0;

      if ((data = elf_getdata(scn, data)) == 0 || data->d_size == 0){
          /* error or no data */
          std::cerr << "Section had no data!" << std::endl;
          return;
      }

      /*now extract the symbols*/
      ElfW(Dyn) *esym = (ElfW(Dyn)*) data->d_buf;
      ElfW(Dyn) *lastsym = (ElfW(Dyn)*) ((char*) data->d_buf + data->d_size);

      ElfW(Word)  offset = 0;
      /* now loop through the symbol table and extract it*/
      for (; esym < lastsym; esym++){

	// This element holds the string table offset of a null-terminated string, 
	//giving the name of a needed library. The offset is an index into the table 
	// recorded in the DT_STRTAB entry.
	if ( esym->d_tag == DT_NEEDED )
	{
		offset = esym->d_un.d_val;
           	char* name = elf_strptr(elf,shdr->sh_link , (size_t)esym->d_un.d_val);
	        if(!name){
                	std::cerr << elf_errmsg(elf_errno());
        		continue;
	        }
		symbols.push_back( name );
	}
       }

}

std::string dynLibImplLinux::Demangle(const char* symbol) {
  int status;
  char* demangled;

  if ( symbol == NULL || strlen( symbol ) == 0 )
  {
    return "";
  }

  //first, try to demangle a c++ name
    if (NULL != (demangled = abi::__cxa_demangle(symbol, NULL, NULL, &status))) {
      std::string result(demangled);
      free(demangled);
      return result;
    }
 
  //if all else fails, just return the symbol
  return "";
}

#endif // _WIN32
