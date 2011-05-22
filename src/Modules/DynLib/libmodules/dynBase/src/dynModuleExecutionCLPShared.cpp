/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynModuleExecution.h"
#include "dynModuleExecutionCLPShared.h"
#include <sstream>
#include <stdio.h>
#include <time.h>

#include <itksys/RegularExpression.hxx>
#include "itksys/DynamicLoader.hxx"
#include "itkExceptionObject.h"

typedef int (*ModuleEntryPoint)(int argc, char* argv[]);


dynModuleExecutionCLPShared::dynModuleExecutionCLPShared( )
{
}

dynModuleExecutionCLPShared::~dynModuleExecutionCLPShared()
{
}

void dynModuleExecutionCLPShared::BuildCommandLineLocation()
{
	m_CommandLineAsString.clear();

	if (GetModule( )->GetLocation() != std::string("") && 
		GetModule( )->GetLocation() != GetModule( )->GetTarget())
	{
		m_CommandLineAsString.push_back(GetModule( )->GetLocation());
	}

	//m_CommandLineAsString.push_back( "--processinformationaddress" );

	//char tname[256];
	//sprintf(tname, "%p", GetModule()->GetProcessInformation());
	//m_CommandLineAsString.push_back( tname );


}

void dynModuleExecutionCLPShared::RunFilter()
{
	int returnValue = 0;

#if defined(__APPLE__) && (MAC_OS_X_VERSION_MAX_ALLOWED >= 1030)
	// Mac OS X defaults to RTLD_GLOBAL and there is no way to
	// override in itksys. So make the direct call to dlopen().
	itksys::DynamicLoader::LibraryHandle lib
		= dlopen(GetModule()->GetLocation().c_str(), RTLD_LAZY | RTLD_LOCAL);
#else
	itksys::DynamicLoader::LibraryHandle lib
		= itksys::DynamicLoader::OpenLibrary(GetModule()->GetLocation().c_str());
#endif
	if ( !lib )
	{
		return;
	}

	try
	{

		ModuleEntryPoint entryPoint = NULL;
		entryPoint = (ModuleEntryPoint)itksys::DynamicLoader::GetSymbolAddress(lib, "ModuleEntryPoint");

		// run the module
		if ( entryPoint != NULL ) {
			returnValue = (*entryPoint)(m_CommandLineAsString.size(), m_Command);
		}

	}
	catch (itk::ExceptionObject& exc)
	{
		std::stringstream information;
		information << GetModule()->GetTitle()
			<< " terminated with an exception: " << exc;
		std::cerr << information.str().c_str();
		throw std::runtime_error( information.str().c_str() );
	}
	catch (std::exception& exc)
	{
		std::stringstream information;
		information << GetModule()->GetTitle()
			<< " terminated with an exception: " << exc.what();
		std::cerr << information.str().c_str();
		throw std::runtime_error( information.str().c_str() );
	}
	catch (...)
	{
		std::stringstream information;
		information << GetModule()->GetTitle()
			<< " terminated with an unknown exception." << std::endl;
		std::cerr << information.str().c_str();
		throw std::runtime_error( information.str().c_str() );
	}
	// Check the return status of the module
	if (returnValue)
	{
		std::stringstream information;
		information << GetModule()->GetTitle()
			<< " returned " << returnValue << " which probably indicates an error." << std::endl;
		std::cerr << information.str().c_str();
		throw std::runtime_error( information.str().c_str() );
	}

	// Is not possible to close the library because it creates a DataEntityIO instance
	//	itksys::DynamicLoader::CloseLibrary( lib );
}

