/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynLibEntry_H
#define _dynLibEntry_H

struct DLL_ENTRY
{
	DLL_ENTRY()
	{
		m_bIsStable = 0;
		m_csFullPath = "";
	}
	DLL_ENTRY(const DLL_ENTRY& obj )
	{
		this->ArrayExport = obj.ArrayExport;
		this->ArrayImport = obj.ArrayImport;
		m_csFullPath = obj.m_csFullPath;
		m_bIsStable = obj.m_bIsStable;
	}
	DLL_ENTRY& operator =( const DLL_ENTRY& obj )
	{
		this->ArrayExport = obj.ArrayExport;
		this->ArrayImport = obj.ArrayImport;
		m_csFullPath = obj.m_csFullPath;
		m_bIsStable = obj.m_bIsStable;
		return *this;
	}
	std::string m_csFullPath;
	bool m_bIsStable;
	SingleMapEntryArray ArrayExport;
	SingleMapEntryArray ArrayImport;
};

typedef std::map<std::string, DLL_ENTRY > MapEntry;
typedef std::map<std::string, int > SearchMapExport;


#endif // _dynLibEntry_H

