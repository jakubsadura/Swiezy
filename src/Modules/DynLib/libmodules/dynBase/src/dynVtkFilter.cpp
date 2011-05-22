/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynVtkFilter.h"
#include "dynLib.h"
#include "dynVtkFilterIFace.h"
#include "dynCalls.h"

#include <stdexcept>
#include <sstream>


dynVtkFilter::dynVtkFilter(  )
{
}

dynVtkFilter::~dynVtkFilter()
{
	if ( GetInstance( ) )
	{
		Delete();
	}
}

void dynVtkFilter::Delete()
{
	CALL_MEMBER_FN(GetInstance( ),m_FuncNameDestructor,exEmpty);
}

void dynVtkFilter::NewInstance()
{
	m_FilterIFace = CALL_FN( m_FuncNameConstructor, exNew );
}

dynFilterIFace* dynVtkFilter::GetInstance()
{
	return m_FilterIFace;
}

dynFilterFunction dynVtkFilter::GetFunctionSetInput()
{
	dynFilterFunction func;
	func.SetName( "SetInput" );
	func.AddParameter( "int" );
	func.OpenParenthesis();
	return func;
}

dynFilterFunction dynVtkFilter::GetFunctionGetOutput()
{
	dynFilterFunction func;
	func.SetName( "GetOutput" );
	func.AddParameter( "int" );
	func.OpenParenthesis();
	return func;
}
