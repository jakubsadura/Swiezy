/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreExecuteCommandProcessor.h"
#include "coreImageDataEntityMacros.h"
#include "coreDataEntityHelper.h"
#include "coreReportExceptionMacros.h"
#include "coreDirectory.h"
#include "coreSettings.h"

#include "blSignalWriter.h"


Core::ExecuteCommandProcessor::ExecuteCommandProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input" );
	GetInputPort( 0 )->SetDataEntityType( Core::UnknownTypeId );
	SetNumberOfOutputs( 1 );
	SetOutputDataName( 0, "Output" );
	SetName( "ExecuteCommandLineProcessor" );

	m_Executable = "";
	m_ExecutableOptions = "";
	m_fileName = "input";
}

void Core::ExecuteCommandProcessor::Update()
{
	try
	{
		SetState(Core::Runtime::APP_STATE_PROCESSING);
		
		std::string dataPath = 
			Core::Runtime::Kernel::GetApplicationSettings()->GetApplicationPath();
		std::string name = "";

		if (GetInputDataEntity(0).IsNotNull() &&
			m_fileName != "")
		{
			
			Core::DataEntityHelper::SaveDataEntity( 
				dataPath + Core::IO::SlashChar + m_fileName, 
				GetInputDataEntity(0));
			name = dataPath + Core::IO::SlashChar + m_fileName;

		}
		
		std::string command = GetCommandLine(name);
		std::cout << "Executing line: " << command.c_str() << std::endl;
		int res = system( command.c_str() );
		std::cout << "Returned value: "<< res << std::endl;

		// Set the output to the output of this processor
		/*
		Core::DataEntity::Pointer dataEntity =
		DataEntityHelper::LoadDataEntity( outputname);
		m_processor->SetOutputDataEntity(dataEntity);
		//UpdateOutput( 0, GetOutputDataEntity(0), "ScaleShape", false, 1, GetInputDataEntity( 0 ) );
		*/
		SetState(Core::Runtime::APP_STATE_IDLE);
	}
	catch( ... )
	{
		SetState(Core::Runtime::APP_STATE_IDLE);
		throw;
	}
}

std::string Core::ExecuteCommandProcessor::GetCommandLine( std::string nameInput ) const
{
	// surround the hole command with double quotes
	std::string commandline = "\"";
	// surround the exe with double quotes
	commandline += '\"' + m_Executable+ '\"';
    if( nameInput.size() != 0 )
    {
	   commandline += " \"" + nameInput + '\"';
    } 
    if( m_ExecutableOptions.size() != 0 )
    {
      commandline += " \"" + m_ExecutableOptions + '\"';
    }
	commandline += '\"';

	return commandline;
}

const char* Core::ExecuteCommandProcessor::GetExecutableOptions() const
{
	return m_ExecutableOptions.c_str();
}

void Core::ExecuteCommandProcessor::SetExecutableOptions( const char* name )
{
	m_ExecutableOptions = name;
}

const char* Core::ExecuteCommandProcessor::GetExecutableName() const
{
	return m_Executable.c_str();
}

void Core::ExecuteCommandProcessor::SetExecutableName( const char* name )
{
	m_Executable = name;
}

const char* Core::ExecuteCommandProcessor::GetInputName() const
{
	return m_fileName.c_str();
}

void Core::ExecuteCommandProcessor::SetInputName( const char* name )
{
	m_fileName = name;
}