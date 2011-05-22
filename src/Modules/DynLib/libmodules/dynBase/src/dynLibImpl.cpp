/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynLibImpl.h"
#include <stdexcept>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include "blTextUtils.h"

MapEntry dynLibImpl::m_DataEntry;
int dynLibImpl::m_ndata = 0;
std::string dynLibImpl::m_csdata = "";

dynLibImpl::dynLibImpl( )
{
	m_enableImportData = false;
	m_enableOnlyExportFunctions = true;
}

dynLibImpl::~dynLibImpl()
{
}

std::string dynLibImpl::GetLibraryPath() const
{
	return m_libraryPath;
}

void dynLibImpl::SetLibraryPath( std::string val )
{
	m_libraryPath = val;

	m_Treeobj = GetTreeNode( GetLibraryPath(), "" );

	TNode* pNode = m_Treeobj.GetNode( );
	DLL_ENTRY *EntrySingle = GetDllEntry( pNode->GetNode( 0 )->GetNodeData() );

	if ( !EntrySingle->m_bIsStable )
	{
		std::ostringstream strError;
		strError <<"Failed opening dynamic library: " << m_libraryPath;
		throw std::runtime_error( strError.str() );
	}

}

dynLibImpl::SymbolPointer dynLibImpl::FindSymbol(
	const std::string &name )
{
	std::string symbolName;
	FunctionPointer symbolPtr = 0;
	FindExportedSymbol( 
		PreProcessSymbolName( name ),
		symbolName,
		symbolPtr );

	return *reinterpret_cast<SymbolPointer*>(&symbolPtr);
}

void dynLibImpl::CheckMissingDlls(
	std::vector<std::string> &dllArrayName,
	TNode* pNode )
{
	if ( pNode == NULL )
	{
		pNode = m_Treeobj.GetNode( );
	}
	bool ret = true;
	int nIDX = 0;
	while( pNode->GetNode( nIDX ).IsNotNull() )
	{
		SingleMapEntryArray Entryexport;
		int nStable = 2;

		DLL_ENTRY *EntrySingle = GetDllEntry( pNode->GetNode( nIDX )->GetNodeData() );
		if( EntrySingle )
		{
			std::string fullPath = EntrySingle->m_csFullPath;
			if( fullPath.empty())
			{
				nStable = 3;
				dllArrayName.push_back( pNode->GetNode( nIDX )->GetNodeData() );
				ret = false;
			}
			else if( !EntrySingle->m_bIsStable )
			{
				nStable = 2;
				dllArrayName.push_back( pNode->GetNode( nIDX )->GetNodeData() );
				ret = false;
			}
			else
			{
				nStable = 1;
			}
		}

		CheckMissingDlls( dllArrayName, pNode->GetNode( nIDX ) );
		nIDX++;
	}
}

void dynLibImpl::FindExportedSymbol( 
	const std::string &name,
	std::string &symbolName, 
	FunctionPointer &symbolPtr,
	TNode* pNode /*= NULL */ )
{
	if ( pNode == NULL )
	{
		pNode = m_Treeobj.GetNode( );
	}
	if ( pNode == NULL )
	{
		return;
	}
	int nIDX = 0;
	while( pNode->GetNode( nIDX ).IsNotNull() )
	{
		SingleMapEntryArray Entryexport;

		DLL_ENTRY *EntrySingle = GetDllEntry( pNode->GetNode( nIDX )->GetNodeData() );
		if( EntrySingle )
		{

			SingleMapEntryArray::iterator itSym;
			itSym = EntrySingle->ArrayExport.begin( );
			while ( symbolPtr == 0 && itSym != EntrySingle->ArrayExport.end( ) )
			{
				if ( itSym->csFunction.find( name ) != std::string::npos )
				{
					symbolName = itSym->csFunction;
					symbolPtr = itSym->dwData;
					std::cout << symbolName << ": " << EntrySingle->m_csFullPath << std::endl;
				}

				itSym++;
			}
		}

		FindExportedSymbol( name, symbolName, symbolPtr, pNode->GetNode( nIDX ) );
		nIDX++;
	}
}

TreeR dynLibImpl::GetTreeNode( 
	const std::string &inputFileName, 
	const std::string &inputPath )
{
	std::string formatFilename = inputFileName;
	blTextUtils::StrSub( formatFilename, "/", "\\" );

	std::string formatPath = inputPath;
	if ( !inputPath.empty() )
	{
		blTextUtils::StrSub( formatPath, "/", "\\" );
	}

	m_csdata = "";
	m_ndata = 0;

	std::string firstDLL = RemovePath( formatFilename );
	m_csCurrentDirectory = formatPath;
	if ( !formatPath.empty() )
		m_csCurrentDirectory += std::string( "\\" );

	DLL_ENTRY stImpExport;
	bool status = GetExportData( formatFilename.c_str(), stImpExport.ArrayExport, stImpExport.m_csFullPath );
	if ( !status )
	{
		throw std::runtime_error( "Cannot open dynamic library" );
	}

	firstDLL = RemovePath( stImpExport.m_csFullPath );
	AddString( firstDLL );

	if ( GetDllEntry( stImpExport.m_csFullPath  ) == NULL )
	{
		stImpExport.m_bIsStable = 1;
		MakeUpper( firstDLL );
		m_DataEntry[ firstDLL ] = stImpExport;

	}

	GetImportAndExportData( formatFilename.c_str() );

	return m_Treeobj;
}

bool dynLibImpl::AddString( std::string csData )
{
	std::cout << csData << std::endl;

	// Remove the path from csData
	std::string filename = RemovePath( csData );

	m_ndata++;
	if( 1 >= m_ndata)
	{
		m_csdata = "";
		m_csdata += "\\";
		m_csdata += filename;
		std::string csUpper = m_csdata;
		MakeUpper( csUpper );
		m_Treeobj.InsertNode( csUpper );
		return true;
	}
	else
	{
		if( std::string::npos == m_csdata.find( filename, 0 ))
		{
			m_csdata += "\\";
			m_csdata += filename;
			std::string csUpper = m_csdata;
			MakeUpper( csUpper );
			m_Treeobj.InsertNode( csUpper );
			return true;
		}
		else
		{
			m_csdata += "\\";
			m_csdata += filename;
			std::string csUpper = m_csdata;
			MakeUpper( csUpper );
			m_Treeobj.InsertNode( csUpper );
			return false;
		}
	}
}

bool dynLibImpl::DeleteString( std::string csData )
{
	std::string csStr;
	csStr += "\\";
	csStr += csData;
	size_t nData = m_csdata.rfind( '\\');
	if ( nData == std::string::npos )
	{
		nData = m_csdata.rfind( '/');
	}
	size_t nFinalLength = nData + csData.size();
	size_t Totak = m_csdata.size();
	if( nFinalLength ==  Totak - 1)
	{
		m_csdata = m_csdata.substr( 0, nData );
		m_ndata--;
	}
	else
	{
		return false;
	}
	return true;
}

void dynLibImpl::MakeUpper( std::string &input )
{
	std::transform( input.begin(), input.end(), input.begin(), toupper );
}

DLL_ENTRY *dynLibImpl::GetDllEntry( const std::string &input )
{
	std::string name = RemovePath( input );
	MakeUpper( name );
	MapEntry::iterator it = m_DataEntry.find( name );
	if( it != m_DataEntry.end() )
	{
		return &(it->second);
	}

	return NULL;
}



std::string dynLibImpl::RemovePath( const std::string& filePath )
{
	size_t nSize = filePath.rfind( '\\');
	if ( nSize == std::string::npos )
		nSize = filePath.rfind( '/');
	return filePath.substr( nSize + 1, ( filePath.size() - nSize ));
}

void dynLibImpl::AddDynamicEntry( const std::string &csData, DLL_ENTRY &stImpExport )
{
	m_DataEntry[ csData ] = stImpExport;
}

std::string dynLibImpl::PreProcessSymbolName( const std::string name )
{
	return name;
}

