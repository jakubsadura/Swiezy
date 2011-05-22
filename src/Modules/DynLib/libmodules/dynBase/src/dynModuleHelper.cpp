/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynModuleHelper.h"
#include "ModuleDescriptionUtilities.h"
#include <sstream>
#include <stdio.h>

dynModuleHelper::dynModuleHelper( )
{
}

dynModuleHelper::~dynModuleHelper()
{
}

ModuleParameter *dynModuleHelper::FindParameter( 
	ModuleDescription* module,
	std::string channel /*= "" */,
	int count /*= 0*/,
	std::string name /*= ""*/,
	const std::string &tag /*=""*/ )
{

	return findModuleParameter( module, channel, count, name, tag );
}

long dynModuleHelper::GetNumberOfInputs( ModuleDescription* module )
{
	int count = 0;
	int inputs = 0;
	ModuleParameter* param;
	while ( param = FindParameter( module, "input", count ) )
	{
		if ( param->GetTag() != "file" && param->GetTag() != "directory" )
		{
			inputs++;
		}

		count++;
	}
	return inputs;
}

long dynModuleHelper::GetNumberOfOutputs( ModuleDescription* module )
{
	int count = 0;
	int outputs = 0;
	ModuleParameter* param;
	while ( param = FindParameter( module, "output", count ) )
	{
		if ( param->GetTag() != "file" && param->GetTag() != "directory" )
		{
			outputs++;
		}

		count++;
	}
	return outputs;
}

void* dynModuleHelper::StringToPointer( const std::string name )
{
	void* ptr = NULL;
	sscanf( name.c_str(), "%p", &ptr );
	return ptr;
}

std::string dynModuleHelper::PointerToString( void* ptr )
{
	char buffer[128];
	sprintf( buffer, "%p", ptr );
	return buffer;
}

ModuleParameter * dynModuleHelper::GetInput( ModuleDescription* module, int inputNum )
{
	int count = 0;
	int inputs = 0;
	ModuleParameter* param;
	while ( param = FindParameter( module, "input", count ) )
	{
		if ( param->GetTag() != "file" && param->GetTag() != "directory" )
		{
			if ( inputs == inputNum )
			{
				return param;
			}

			inputs++;
		}

		count++;
	}

	return 0;
}

ModuleParameter * dynModuleHelper::GetOutput( ModuleDescription* module, int outputNum )
{
	int count = 0;
	int outputs = 0;
	ModuleParameter* param;
	while ( param = FindParameter( module, "output", count ) )
	{
		if ( param->GetTag() != "file" && param->GetTag() != "directory" )
		{
			if ( outputs == outputNum )
			{
				return param;
			}

			outputs++;
		}

		count++;
	}

	return 0;
}

