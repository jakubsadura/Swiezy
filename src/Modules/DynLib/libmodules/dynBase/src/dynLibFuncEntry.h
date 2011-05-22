/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynLibFuncEntry_H
#define _dynLibFuncEntry_H

#include <map>

#ifdef _WIN32
  typedef FARPROC FunctionPointer;
#else
  typedef void *FunctionPointer;
#endif


struct SINGLE_FUNCT_ENTRY
{
	std::string csOrdinal;
	std::string csHint;
	std::string csFunction;
	std::string csSymbol;
	std::string csEntryPoint;
	std::string csIndex;
	FunctionPointer dwData;
	bool m_bIsInvalidValid;
};

typedef std::vector<SINGLE_FUNCT_ENTRY> SingleMapEntryArray;


#endif // _dynLibFuncEntry_H

