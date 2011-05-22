/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynFilter.h"
#include "dynLib.h"
#include "dynFilterIFace.h"
#include "dynCalls.h"

#include <stdexcept>
#include <sstream>


dynFilter::dynFilter(  )
{
	m_DynamicLib = NULL;
}

dynFilter::~dynFilter()
{
	if ( m_DynamicLib )
	{
		delete m_DynamicLib;
	}
}

void dynFilter::Init( 
					 const std::string &filterName, 
					 dynFilterFunction funcNameConstructor, 
					 dynFilterFunction funcNameDestructor, 
					 const char* filterDynLib )
{
	m_Name = filterName;
	m_FuncNameConstructor = funcNameConstructor;
	m_FuncNameDestructor = funcNameDestructor;
	m_DynamicLib = new dynLib( );
	m_DynamicLib->SetLibraryPath(  filterDynLib );

	NewInstance();
}

dynLib::SymbolPointer dynFilter::FindSymbol( dynFilterFunction func )
{
	dynLib::SymbolPointer symbol = NULL;

	int count = 0;
	while ( symbol == NULL && !func.GetClass( count ).empty() )
	{
		std::string funcName = func.GetClass( count ) + "::" + func.GetName() + func.GetParameters();
		//std::cout << funcName << std::endl;
		symbol = m_DynamicLib->FindSymbol( funcName );
		count++;
	}

	if ( symbol == NULL )
	{
		std::ostringstream strError;
		strError <<"Failed finding symbol: " << func.GetName();
		throw std::runtime_error( strError.str() );
	}

	return symbol;
}

void dynFilter::SetFloat( dynFilterFunction func, float val )
{
	CALL_MEMBER_FN1(GetInstance( ),func,exSetFloat, val );
}

void dynFilter::SetDouble( dynFilterFunction func, double val )
{
	CALL_MEMBER_FN1(GetInstance( ),func,exSetDouble, val );
}

void dynFilter::SetBool( dynFilterFunction func, bool val )
{
	CALL_MEMBER_FN1(GetInstance( ),func,exSetBool, val );
}

void dynFilter::SetInt( dynFilterFunction func, int val )
{
	CALL_MEMBER_FN1(GetInstance( ),func,exSetInt, val );
}

void dynFilter::SetString( dynFilterFunction func, const char* val )
{
	CALL_MEMBER_FN1(GetInstance( ),func,exSetString, val );
}

void dynFilter::Update( dynFilterFunction func )
{
	CALL_MEMBER_FN(GetInstance( ),func,exEmpty);
}

void dynFilter::SetInput( dynFilterFunction func, int num, void* input )
{
	if ( func.GetParameters().find( "unsigned int" ) != std::string::npos )
	{
		CALL_MEMBER_FN2(GetInstance( ),func,exSetNthInput, num, input );
	}
	else if ( func.GetParameters().find( "int" ) != std::string::npos )
	{
		CALL_MEMBER_FN2(GetInstance( ),func,exSetPtrNum, num, input );
	}
}

void* dynFilter::GetOutput( dynFilterFunction func, int num )
{
	return CALL_MEMBER_FN1(GetInstance( ),func,exGetPtr, num );
}

void dynFilter::SetUInt( dynFilterFunction func, int val )
{
	CALL_MEMBER_FN1(GetInstance( ),func,exSetUInt, val );
}

void dynFilter::SetPoint( dynFilterFunction func, double val1, double val2, double val3)
{
	CALL_MEMBER_FN3(GetInstance( ),func,exSet3Double, val1, val2, val3 );
}
