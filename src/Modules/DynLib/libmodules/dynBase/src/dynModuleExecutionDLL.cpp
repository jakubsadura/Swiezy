/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynModuleExecutionDLL.h"
#include "dynModuleExecution.h"
#include <sstream>
#include <stdio.h>
#include "dynItkFilter.h"
#include "dynVtkFilter.h"
#include "blTextUtils.h"
#include "dynModuleHelper.h"

dynModuleExecutionDLL::dynModuleExecutionDLL( )
{
}

dynModuleExecutionDLL::~dynModuleExecutionDLL()
{
}


void dynModuleExecutionDLL::Update()
{

	if ( GetModule()->GetHierarchy().find( "itk::ProcessObject" ) != std::string::npos )
	{
		m_Filter = dynItkFilter::New( );
	}
	else
	{
		m_Filter = dynVtkFilter::New( );
	}

	m_Filter->Init(
		GetModule()->GetTitle().c_str(),
		dynFilterFunction( GetModule()->GetConstructor(), GetModule()->GetHierarchy() ),
		dynFilterFunction( GetModule()->GetDestructor(), GetModule()->GetHierarchy() ),
		GetModule()->GetDynamicLibrary().c_str() );

	SetAllParameters();

	m_Filter->Update( dynFilterFunction( GetModule()->GetUpdateFunc( ), GetModule()->GetHierarchy() ) );

	GetAllParameters();

}

dynFilterFunction dynModuleExecutionDLL::BuildSetFunction( ModuleParameter &param )
{
	dynFilterFunction func;

	// Function name
	if ( param.GetFunction().empty() )
	{
		if ( param.GetChannel() == "input" )
		{
			func = m_Filter->GetFunctionSetInput();
		}
		else if ( param.GetChannel() == "output" )
		{
			//func = m_Filter->GetFunctionSetOutput();
		}
		else if ( param.GetTag() == "point" )
		{
			func.SetName( "Set" + param.GetName() );
			func.AddParameter( "double" );
			func.AddParameter( "double" );
			func.AddParameter( "double" );
		}
		else
		{
			func.SetName( "Set" + param.GetName() );
			func.AddParameter( param.GetCPPType() );
		}
	}
	else
	{
		func.SetName( param.GetFunction() );
		func.AddParameter( param.GetCPPType() );
	}

	func.AddClass( GetModule()->GetHierarchy() );

	return func;
}

dynFilterFunction dynModuleExecutionDLL::BuildGetFunction( ModuleParameter &param )
{
	dynFilterFunction func;

	// Function name
	if ( param.GetFunction().empty() )
	{
		if ( param.GetChannel() == "input" )
		{
			func.SetName( "GetInput" );
			func.AddParameter( "int" );
		}
		else if ( param.GetChannel() == "output" )
		{
			func = m_Filter->GetFunctionGetOutput();
		}
		else
		{
			func.SetName( "Get" + param.GetName());
		}
	}
	else
	{
		func.SetName( param.GetFunction() );
	}

	func.AddClass( GetModule()->GetHierarchy() );

	return func;
}

void dynModuleExecutionDLL::SetParameterValue( ModuleParameter *param )
{
	dynFilterFunction functionToCall = BuildSetFunction( *param );
	if ( param->GetTag() == "double" )
	{
		double val = atof( param->GetDefault().c_str() );
		m_Filter->SetDouble( functionToCall, val );
	}
	else if ( param->GetTag() == "integer" )
	{
		int val = atoi( param->GetDefault().c_str() );
		m_Filter->SetInt( functionToCall, val );
	}
	else if ( param->GetTag() == "uinteger" )
	{
		unsigned int val = strtoul( param->GetDefault().c_str(), NULL, 0 );
		m_Filter->SetUInt( functionToCall, val );
	}
	else if ( param->GetTag() == "float" )
	{
		float val = float( atof( param->GetDefault().c_str() ) );
		m_Filter->SetFloat( functionToCall, val );
	}
	else if ( param->GetTag() == "string" )
	{
		std::string val = param->GetDefault();
		m_Filter->SetString( functionToCall, val.c_str() );
	}
	else if ( param->GetTag() == "boolean" )
	{
		bool val = true;
		if ( param->GetDefault() == "true" )
		{
			val = true;
		}
		else
		{
			val = false;
		}
		m_Filter->SetBool( functionToCall, val );
	}
	else if ( param->GetChannel() == "input" )
	{
		void *ptr = dynModuleHelper::StringToPointer( param->GetDefault() );
		m_Filter->SetInput( functionToCall, 0, ptr );
	}
	else if ( param->GetTag() == "point" )
	{
		std::list<std::string> points;
		blTextUtils::ParseLine( param->GetDefault(), ';', points );

		std::list<std::string>::iterator it;
		for ( it = points.begin() ; it != points.end() ; it++ )
		{
			std::list<std::string> values;
			blTextUtils::ParseLine( *it, ',', values );

			double point[ 3 ] = {0,0,0};
			int pos = 0;
			std::list<std::string>::iterator itValue = values.begin();
			while ( itValue != values.end() && pos < 3 )
			{
				point[ pos++ ] = atof( itValue->c_str() );
				itValue++;
			}

			m_Filter->SetPoint( functionToCall, point[ 0 ], point[ 1 ], point[ 2 ] );
		}

	}
}

void dynModuleExecutionDLL::GetParameterValue( ModuleParameter *param )
{
	dynFilterFunction functionToCall = BuildGetFunction( *param );
	if ( param->GetChannel() == "output" )
	{
		void *ptr = m_Filter->GetOutput( functionToCall, 0 );
		param->SetDefault( dynModuleHelper::PointerToString( ptr ) );
	}
}
