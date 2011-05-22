/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynLibImplWin32.h"

#ifdef _WIN32

#include <stdexcept>
#include <sstream>
#include <tchar.h>
#include <algorithm>
#include "Dbghelp.h"
#include "blTextUtils.h"

std::string demangle(const char* symbol) 
{
	std::string result;
	char buffer[ 256 ];
	int ret = UnDecorateSymbolName(symbol, buffer, 256, UNDNAME_COMPLETE);  
	if ( ret != 0 )
	{
		result = buffer;
	}
	return result;
}



dynLibImplWin32::dynLibImplWin32( )
{
}

dynLibImplWin32::~dynLibImplWin32()
{
}

void dynLibImplWin32::GetImportAndExportData( const char* dynamicLibraryName )
{
	if( std::string(dynamicLibraryName).empty())
	{
		return;
	}
	
	HMODULE hMod;
	IMAGE_IMPORT_DESCRIPTOR* IID = GetImportDataPointer( dynamicLibraryName, hMod );
	if( 0 == IID )
	{
		return;
	}

	while(IID->FirstThunk)
	{
		std::string csData = (char*)((BYTE*)hMod+IID->Name); 
		IMAGE_THUNK_DATA* ITD =(IMAGE_THUNK_DATA*)((BYTE*)hMod+ IID->OriginalFirstThunk);
		if( FALSE != IsBadReadPtr( ITD, sizeof( IMAGE_THUNK_DATA )))
		{
			return;
		}

		AddString( csData.c_str() );

		if ( GetDllEntry( csData ) == NULL )
		{
			DLL_ENTRY stImpExport;
			stImpExport.m_bIsStable = true;

			if( csData != "oleaut32.dll" && csData != "WS2_32.dll" && csData != "WS2_32.dll" &&
				csData != "KERNEL32.dll" && csData != "MSVCP71D.dll" && csData != "MSVCR71D.dll" )
			{
				GetExportData( csData.c_str(), stImpExport.ArrayExport, stImpExport.m_csFullPath );
				GetImportData( csData.c_str(), stImpExport.ArrayImport, stImpExport.m_bIsStable );
			}

		
			// Add data to static internal map
			MakeUpper( csData );
			m_DataEntry[ csData ] = stImpExport;
		}

		IID++;
		DeleteString( csData.c_str() );
	}
	m_MapExport.clear();
}

bool dynLibImplWin32::GetImportData( 
	const char* dynamicLibraryName, 
	SingleMapEntryArray& csImportArr,
	bool &bIsStable )
{
	if ( !m_enableImportData )
	{
		return false;
	}

	HMODULE hMod;
	IMAGE_IMPORT_DESCRIPTOR* IID = GetImportDataPointer( dynamicLibraryName, hMod );
	if( 0 == IID )
	{
		return false;
	}

	IMAGE_THUNK_DATA* ITD =(IMAGE_THUNK_DATA*)((BYTE*)hMod+ IID->OriginalFirstThunk);

	int nIndex = 1;
	while(ITD->u1.Function)
	{
		char* szFunctionName2;
		szFunctionName2 =  (char*)((BYTE*)hMod + (DWORD)ITD->u1.AddressOfData+2);

		std::string csFunctioname = szFunctionName2;
		IMAGE_IMPORT_BY_NAME* pImportByName = (IMAGE_IMPORT_BY_NAME*)((BYTE*)hMod + ITD->u1.AddressOfData);
		if( csFunctioname.empty())
		{
			if(0 == IsBadReadPtr(pImportByName,sizeof(IMAGE_IMPORT_BY_NAME)))
			{
				ITD++;
			}
			else
			{
				SINGLE_FUNCT_ENTRY newElement;
				pImportByName = (IMAGE_IMPORT_BY_NAME*)(ITD);
				std::string csFormat;
				std::string csOrdinal;
				csOrdinal = blTextUtils::format( "%d(0X%x)",pImportByName->Hint, 
					pImportByName->Hint );
				MakeUpper( csOrdinal );
				blTextUtils::StrSub( csOrdinal, "X", "x");
				DWORD dwref = 2147490110 - (DWORD)ITD->u1.AddressOfData;
				newElement.csOrdinal = blTextUtils::format( "%s", csOrdinal.c_str() );
				newElement.csEntryPoint = blTextUtils::format( "0x%x", dwref );
				MakeUpper( newElement.csEntryPoint );
				blTextUtils::StrSub( newElement.csEntryPoint, "X", "x");
				newElement.csHint = "N/A";
				newElement.csFunction = "N/A";
				newElement.csIndex = blTextUtils::format("%d", nIndex);
				csImportArr.push_back( newElement );
				newElement.m_bIsInvalidValid = false;
				pImportByName++;
			}
		}
		else
		{
			SINGLE_FUNCT_ENTRY newElement;
			newElement.csFunction = csFunctioname;
			std::string csFormat;
			std::string csOrdinal;
			csOrdinal = blTextUtils::format( "%d(0X%x)",pImportByName->Hint, 
				pImportByName->Hint );
			MakeUpper( csOrdinal );
			blTextUtils::StrSub( csOrdinal, "X", "x");
			newElement.csHint = "N/A";
			newElement.csOrdinal = "N/A";

			newElement.csHint = blTextUtils::format( "%s", csOrdinal.c_str() );
			newElement.csEntryPoint = blTextUtils::format( "NB" );
			SearchMapExport::iterator it = m_MapExport.find( csFunctioname );
			if( it == m_MapExport.end( ) )
			{
				bIsStable = false;
				newElement.m_bIsInvalidValid = true;
			}
			else
			{
				bIsStable = true;
				newElement.m_bIsInvalidValid = false;
			}
			newElement.csIndex = blTextUtils::format("%d", nIndex);
			csImportArr.push_back( newElement );
		}
		ITD++;
		nIndex++;
	}

	return true;
}


bool dynLibImplWin32::GetExportData( const char* dynamicLibraryName, 
								   SingleMapEntryArray& ExportEntriesArr,
								   std::string& csPath_o )
{
	m_MapExport.clear();
	HMODULE hMod;
	bool bStatus = GetDLLLoadAddress(  dynamicLibraryName,  hMod, csPath_o );
	if(!bStatus )
	{
		return false;
	}

	if ( GetDllEntry( csPath_o  ) != NULL )
	{
		FreeLibrary(hMod);
		return true;
	}

	IMAGE_DOS_HEADER* IDH = (IMAGE_DOS_HEADER*)hMod;
	IMAGE_OPTIONAL_HEADER* IOH = (IMAGE_OPTIONAL_HEADER*)((BYTE*)hMod + IDH->e_lfanew + 24);
	_IMAGE_EXPORT_DIRECTORY* pExportDescriptor = 
		(_IMAGE_EXPORT_DIRECTORY*)((BYTE*)hMod + IOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	int  nSize = pExportDescriptor->Name;
	BYTE* pName = 0;
	std::string csName;
	LPSTR name = 0;
	DWORD nData = 0;
	bool bIntial = true;
	std::string csType;
	unsigned long ulbase = pExportDescriptor->Base - 1;
	DWORD dwData = pExportDescriptor->AddressOfFunctions;
	SINGLE_FUNCT_ENTRY SingleEntry;
	int nHint = -2;
	while( nData <= pExportDescriptor->NumberOfFunctions )
	{ 
		DWORD dwData =  DWORD( pExportDescriptor->AddressOfNameOrdinals );
		if( 0 == pExportDescriptor->NumberOfNames )
		{
			if(bIntial)
			{
				bIntial = false;
			}
			else
			{
				std::string csOrdinal;
				csOrdinal = blTextUtils::format( "%d(0X%x)",ulbase, ulbase );
				MakeUpper( csOrdinal );
				blTextUtils::StrSub( csOrdinal, "X", "x");
				SingleEntry.csFunction = "N/A";
				SingleEntry.csHint = "N/A";
				SingleEntry.csOrdinal = blTextUtils::format( "%s", csOrdinal.c_str() );
				SingleEntry.csIndex = blTextUtils::format( "%d", nData );
				SingleEntry.dwData = 0;
				ExportEntriesArr.push_back( SingleEntry );
			}
			ulbase++;
		}
		else
		{
			if( pExportDescriptor->NumberOfNames >= nData )
			{
				if( FALSE == IsBadReadPtr((BYTE*)hMod +  pExportDescriptor->Name,1))
				{ 
					pName = (BYTE*)hMod + nSize;
					name = reinterpret_cast<LPSTR>(pName);
				}
			}
			else
			{
				name = "";
			}
			csName = name;
			if( csName.empty())
			{
				SingleEntry.csFunction = "N/A";
				SingleEntry.csEntryPoint = "";
				SingleEntry.dwData = 0;
			}
			else
			{
				SingleEntry.dwData = GetProcAddress( hMod,  csName.c_str() );
				if ( !m_enableOnlyExportFunctions )
				{
					DWORD dwDiff = ( (DWORD)SingleEntry.dwData - (DWORD)hMod );
					SingleEntry.csEntryPoint = blTextUtils::format("0x%x",dwDiff );
					MakeUpper( SingleEntry.csEntryPoint );
					blTextUtils::StrSub( SingleEntry.csEntryPoint, "X", "x");
					nHint++;
				}
				if ( m_enableImportData )
				{
					m_MapExport[ csName ] = 0;
				}
				SingleEntry.csFunction = demangle( csName.c_str() );
				SingleEntry.csSymbol = csName;

			}
			nSize = nSize  + csName.size() + 1;
			if(bIntial)
			{
				bIntial = false;
			}
			else
			{
				if ( !m_enableOnlyExportFunctions )
				{
					if( csName.empty())
					{
						SingleEntry.csHint = "N/A";
					}
					else
					{
						SingleEntry.csHint = blTextUtils::format( "%d(0x0%x)",nHint,nHint);
						MakeUpper( SingleEntry.csHint );
						blTextUtils::StrSub( SingleEntry.csHint, "X", "x");
					}

					//****Code is not completed due to unavailability of Information.
					SingleEntry.csOrdinal = blTextUtils::format( "%d(0x0%x)",ulbase,ulbase);
					MakeUpper( SingleEntry.csOrdinal );
					blTextUtils::StrSub( SingleEntry.csOrdinal, "X", "x");
					SingleEntry.csIndex = blTextUtils::format( "%d", nData );
				}
				ExportEntriesArr.push_back( SingleEntry );

			}
		}
		ulbase++;
		dwData+=4;
		nData++;
	}
	FreeLibrary(hMod);

	return true;
}

bool dynLibImplWin32::GetDLLLoadAddress(  
	std::string csDLLName,  
	HMODULE& hModDLL,
	std::string& csPathDLLDepend_o )
{
	csPathDLLDepend_o = "";

	hModDLL = GetModuleHandle( csDLLName.c_str() );
	if( 0 != hModDLL )
	{
		char buffer[ MAX_PATH ] = {0};
		GetModuleFileName( hModDLL, buffer, MAX_PATH );
		csPathDLLDepend_o = buffer;
		return true;
	}

	if( !m_csCurrentDirectory.empty() )
	{
		std::string csFullPath = csDLLName;
		if( std::string::npos == csDLLName.find( "\\", 0 ) )
		{
			csFullPath = m_csCurrentDirectory;
			csFullPath += csDLLName;
		}
		HANDLE hCurrentFile = CreateFile( csFullPath.c_str(), GENERIC_READ, 
			FILE_SHARE_READ | STANDARD_RIGHTS_REQUIRED,NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_READONLY,
			NULL);
		HFILE hFil;
		if( INVALID_HANDLE_VALUE == hCurrentFile )
		{
			OFSTRUCT ReOpenBuff;
			hFil = OpenFile( csFullPath.c_str(), & ReOpenBuff, OF_EXIST );
		}
		if( INVALID_HANDLE_VALUE != hCurrentFile ||
			HFILE_ERROR != hFil )
		{
			hModDLL =  LoadLibraryEx( csFullPath.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES );
			if( 0 != hModDLL )
			{
				csPathDLLDepend_o = csFullPath;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	if( std::string::npos != csDLLName.find( "\\", 0 ))
	{
		HANDLE hCurrentFile = CreateFile( csDLLName.c_str(), GENERIC_READ, 
			FILE_SHARE_READ,NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if( INVALID_HANDLE_VALUE != hCurrentFile )
		{
			hModDLL = LoadLibraryEx( csDLLName.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES ); 
			if( 0 != hModDLL )
			{
				csPathDLLDepend_o = csDLLName;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	std::string csPath;
	std::string csFileName = "\\";
	csFileName += csDLLName;
	DWORD nNum;
	char buffer[ MAX_PATH ] = {0};
	nNum = GetCurrentDirectory( MAX_PATH, LPSTR( buffer ) );
	buffer[ nNum ] = 0;
	csPath += buffer + csFileName;
	std::cout << csPath << std::endl;
	HANDLE hFile = CreateFile( csPath.c_str(), GENERIC_READ, 
		FILE_SHARE_READ,NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if( INVALID_HANDLE_VALUE == hFile )
	{
		std::string csData = _tgetenv( _T( "Path" ));
		int nIndex = 0;
		int nPrev = 0;
		blTextUtils::StrSub( csData, " ", "" );
		csPath = "";

		while( std::string::npos !=( nIndex =  csData.find( ";", nIndex )))
		{
			csPath = csData.substr( nPrev, ( nIndex - nPrev ));
			csPath += csFileName;
			BOOL m_bLoaded = FALSE;
			HANDLE hFile = CreateFile( csPath.c_str(), GENERIC_READ, 
				FILE_SHARE_READ,NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if( INVALID_HANDLE_VALUE != hFile )
			{
				break;
			}
			nPrev = ++nIndex;
		}
	}
	hModDLL = LoadLibraryEx( csPath.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES );
	if( 0 != hModDLL )
	{
		csPathDLLDepend_o = csPath;
		return true;
	}
	else
	{
		return false;
	}
}

IMAGE_IMPORT_DESCRIPTOR* dynLibImplWin32::GetImportDataPointer( 
	const char* dynamicLibraryName,
	HMODULE &hMod )
{
	std::string csPath;
	bool bStatus = GetDLLLoadAddress( dynamicLibraryName, hMod, csPath );
	if ( !bStatus )
	{
		return NULL;
	}

	IMAGE_DOS_HEADER* IDH = (IMAGE_DOS_HEADER*)hMod;
	if( 0 == IDH)
	{
		return NULL;
	}
	IMAGE_OPTIONAL_HEADER* IOH = 
		(IMAGE_OPTIONAL_HEADER*)((BYTE*)hMod + IDH->e_lfanew + 24);
	if( 0 == IOH)
	{
		return NULL;
	}
	IMAGE_IMPORT_DESCRIPTOR* IID = 
		(IMAGE_IMPORT_DESCRIPTOR*)((BYTE*)hMod + IOH->DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	if( 0 == IID )
	{
		return NULL;
	}

	return IID;
}

std::string dynLibImplWin32::PreProcessSymbolName( const std::string name )
{
	std::string outName = name;
	// Replace "()" to "(void)"
	blTextUtils::StrSub( outName, "()", "(void)" );
	return outName;
}

#endif // _WIN32
