/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynModuleExecution.h"
#include "dynModuleExecutionCLPBase.h"

#include "blTextUtils.h"

#include <sstream>
#include <stdio.h>
#include <time.h>
#include <fstream>

#include <itksys/RegularExpression.hxx>
#include "itksys/DynamicLoader.hxx"
#include "itkExceptionObject.h"


dynModuleExecutionCLPBase::dynModuleExecutionCLPBase( )
{
	m_Command = NULL;
}

dynModuleExecutionCLPBase::~dynModuleExecutionCLPBase()
{
}

void dynModuleExecutionCLPBase::Update()
{
	BuildCommandLineLocation();

	AddCommandLineReturnParameters( );

	SetAllParameters();

	PrintCommandLine( );

	if ( m_SaveScript )
	{
		SaveCommandLine( );
	}
	else
	{
		RunFilter();

		WaitForData();

		ProcessOutputInformation( );
	}

	if ( m_Command )
	{
		delete [] m_Command;
		m_Command = NULL;
	}
}

void dynModuleExecutionCLPBase::SetParameterValue( ModuleParameter *param )
{
	std::string prefix;
	std::string flag;
	bool hasFlag = false;

	if ( param->GetLongFlag() != "")
	{
		prefix = "--";
		flag = param->GetLongFlag();
		hasFlag = true;
	}
	else if (param->GetFlag() != "")
	{
		prefix = "-";
		flag = param->GetFlag();
		hasFlag = true;
	}


	if (hasFlag)
	{
		if (   param->GetTag() != "boolean"
			&& param->GetTag() != "file" 
			&& param->GetTag() != "directory"
			&& param->GetTag() != "string"
			&& param->GetTag() != "integer-vector"
			&& param->GetTag() != "float-vector"
			&& param->GetTag() != "double-vector"
			&& param->GetTag() != "string-vector"
			&& param->GetTag() != "image"
			&& param->GetTag() != "point"
			&& param->GetTag() != "region"
			&& param->GetTag() != "transform"
			&& param->GetTag() != "geometry"
			&& param->GetTag() != "table"
			&& param->GetTag() != "measurement")
		{
			// simple parameter, write flag and value
			m_CommandLineAsString.push_back(prefix + flag);
			m_CommandLineAsString.push_back(param->GetDefault());
		}
		else if (param->GetTag() == "boolean" && param->GetDefault() == "true")
		{
			m_CommandLineAsString.push_back(prefix + flag);
		}
		else if (param->GetTag() == "file" 
			|| param->GetTag() == "directory"
			|| param->GetTag() == "string"
			|| param->GetTag() == "integer-vector"
			|| param->GetTag() == "float-vector"
			|| param->GetTag() == "double-vector"
			|| param->GetTag() == "string-vector")
		{
			// Only write out the flag if value is not empty
			if ( param->GetDefault() != "")
			{
				m_CommandLineAsString.push_back(prefix + flag);
				m_CommandLineAsString.push_back( param->GetDefault() );
			}
		}
		// data passed as parameter
		else if ( param->GetTag() == "image" 
			|| param->GetTag() == "geometry"
			|| param->GetTag() == "transform" 
			|| param->GetTag() == "table" 
			|| param->GetTag() == "measurement" )
		{
			if ( param->GetDefault() != "")
			{
				m_CommandLineAsString.push_back(prefix + flag);
				m_CommandLineAsString.push_back(param->GetDefault());
			}
		}
		else if ( param->GetTag() == "region" )
		{
			m_CommandLineAsString.push_back(prefix + flag);
			m_CommandLineAsString.push_back( param->GetDefault() );
		}
		else if ( param->GetTag() == "point" )
		{
			std::list<std::string> points;
			blTextUtils::ParseLine( param->GetDefault(), ';', points );

			std::list<std::string>::iterator it;
			for ( it = points.begin() ; it != points.end() ; it++ )
			{
				m_CommandLineAsString.push_back(prefix + flag);
				m_CommandLineAsString.push_back( *it );
			}
		}

	}

	// If index is not empty -> It's a command line argument arg0, arg1, ... without flag prefix
	if ( param->GetIndex() != "")
	{
		m_CommandLineAsString.push_back( param->GetDefault() );
	}

}

void dynModuleExecutionCLPBase::GetParameterValue( ModuleParameter *param )
{

}

void dynModuleExecutionCLPBase::AddCommandLineReturnParameters()
{

	// Add a command line flag for a file of return types
	if ( GetModule( )->HasReturnParameters())
	{
		m_CommandLineAsString.push_back( "--returnparameterfile" );

		std::ostringstream pidString;
#ifdef _WIN32
		pidString << GetCurrentProcessId();
#else
		pidString << getpid();
#endif

		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		srand(time(0));

		std::ostringstream code;
		for (int ii = 0; ii < 10; ii++)
		{
			code << alphanum[rand() % (sizeof(alphanum)-1)];
		}

		std::string returnFile = this->m_TemporaryDirectory + "/" + pidString.str()
			+ "_" + code.str() + ".params";

		m_CommandLineAsString.push_back( returnFile );
	}

}

void dynModuleExecutionCLPBase::PrintCommandLine()
{
	// print the command line
	//
	std::cout << GetModule()->GetTitle()
		<< " command line: " << std::endl << std::endl;
	for (std::vector<std::string>::size_type i=0; i < m_CommandLineAsString.size(); ++i)
	{
		std::cout << m_CommandLineAsString[i] << " ";
	}
	std::cout << std::endl;


	// copy the command line arguments into an array of pointers to
	// chars
	m_Command = new char*[m_CommandLineAsString.size()+1];
	for (std::vector<std::string>::size_type i=0; i < m_CommandLineAsString.size(); ++i)
	{
		m_Command[i] = const_cast<char*>(m_CommandLineAsString[i].c_str());
	}
	m_Command[m_CommandLineAsString.size()] = 0;
}

void dynModuleExecutionCLPBase::WaitForData()
{

}

void dynModuleExecutionCLPBase::ProcessOutputInformation()
{

}

void dynModuleExecutionCLPBase::SaveCommandLine()
{
	std::string filename = m_WorkingDirectory + "/script.cmd";
	std::ofstream file( filename.c_str() );
	for (std::vector<std::string>::size_type i=0; i < m_CommandLineAsString.size(); ++i)
	{
		file << m_CommandLineAsString[i] << " ";
	}
	file << std::endl;
	file.close( );
}

