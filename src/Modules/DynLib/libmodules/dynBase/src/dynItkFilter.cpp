/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynItkFilter.h"
#include "dynLib.h"
#include "dynCalls.h"

#include <stdexcept>
#include <sstream>

dynItkFilter::dynItkFilter(  )
{
}

dynItkFilter::~dynItkFilter()
{
}

void dynItkFilter::NewInstance()
{
	m_FilterIFace = CALL_FN( m_FuncNameConstructor, exNewSmartPointer );
}

void dynItkFilter::Delete()
{
}

dynFilterIFace* dynItkFilter::GetInstance()
{
	return reinterpret_cast<dynFilterIFace*> ( m_FilterIFace.GetPointer() );
}

dynFilterFunction dynItkFilter::GetFunctionSetInput()
{
	dynFilterFunction func;
	func.SetName( "SetNthInput" );
	func.AddParameter( "unsigned int" );
	func.OpenParenthesis();
	return func;
}

dynFilterFunction dynItkFilter::GetFunctionGetOutput()
{
	dynFilterFunction func;
	func.SetName( "GetOutput" );
	func.AddParameter( "unsigned int" );
	func.OpenParenthesis();
	return func;
}
