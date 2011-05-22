/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "TavernaPluginModuleConverter.h"

#include "blTextUtils.h"
#include "coreKernel.h"
#include "coreSettings.h"
#include "itksys/SystemTools.hxx"
#include "coreDirectory.h"

TavernaPlugin::ModuleConverter::ModuleConverter( )
{
}

TavernaPlugin::ModuleConverter::~ModuleConverter()
{
}


void TavernaPlugin::ModuleConverter::SetWorkflow( wflWorkflow::Pointer val )
{
	m_Workflow = val;
}

void TavernaPlugin::ModuleConverter::Update( )
{
	if ( m_Workflow.IsNull() )
	{
		return;
	}

	// Convert wflWorkflow to ModuleDescription and call base class 
	// function SetModule( ) that will configure the in / out
	m_Module.SetTitle( m_Workflow->GetProperty("name") );
	m_Module.SetType( "TavernaModule" );

	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();
	std::string tavernaPath;
	if ( settings->GetPluginProperty( "Taverna Plugin", "TavernaPath", tavernaPath ) )
	{
		std::string path = itksys::SystemTools::GetProgramPath( tavernaPath.c_str() );
#ifdef _WIN32
		m_Module.SetLocation( path + Core::IO::SlashChar + "executeworkflow.bat" );
#else
		m_Module.SetLocation( path + Core::IO::SlashChar + "executeworkflow.sh" );
#endif
	}
	else
	{
		throw Core::Exceptions::Exception( 
			"WorkingArea::Enable", 
			"Please configure Taverna Workbench in the Preferences dialog" );
	}

	std::string wofklowFilename = "/" + m_Workflow->GetProperty("filename");
	blTextUtils::StrSub( wofklowFilename, ":\\", ":/" );
	blTextUtils::StrSub( wofklowFilename, ":/", "://" );
	blTextUtils::StrSub( wofklowFilename, "\\", "/" );
	m_Module.SetTarget( wofklowFilename );
	
	ModuleParameterGroup group;
	group.SetLabel( "IO" );

	std::vector<wflInputPort::Pointer> inputPorts = m_Workflow->GetInputPorts();
	for ( int i = 0 ; i < inputPorts.size() ; i++ )
	{
		ModuleParameter parameter;
		char buffer[128];
		sprintf( buffer, "%d", i );
		parameter.SetIndex( buffer );
		parameter.SetChannel( "input" );
		parameter.SetLongFlag( "inputvalue" );
		parameter.SetName( inputPorts[ i ]->GetProperty("name") );
		parameter.SetLabel( inputPorts[ i ]->GetProperty("name") );
		std::string datatype = "image";
		if ( inputPorts[ i ]->GetProperty("datatype") == "image" )
		{
			datatype = "image";
		}
		else if ( inputPorts[ i ]->GetProperty("datatype") == "surface mesh" )
		{
			datatype = "geometry";
		}
		parameter.SetTag( datatype );
		group.AddParameter( parameter );
	}

	std::vector<wflOutputPort::Pointer> outputPorts = m_Workflow->GetOutputPorts();
	for ( int i = 0 ; i < outputPorts.size() ; i++ )
	{
		ModuleParameter parameter;
		char buffer[128];
		sprintf( buffer, "%d", i );
		parameter.SetIndex( buffer );
		parameter.SetChannel( "output" );
		parameter.SetName( outputPorts[ i ]->GetProperty("name") );
		parameter.SetLabel( outputPorts[ i ]->GetProperty("name") );
		parameter.SetTag( "image" );
		group.AddParameter( parameter );
	}

	// Set output directory
	ModuleParameter parameter;
	parameter.SetTag( "directory" );
	parameter.SetName( "OutputDirectory" );
	parameter.SetDefault( "OutputDirectory" );
	parameter.SetChannel( "output" );
	parameter.SetLongFlag( "outputdir" );
	group.AddParameter( parameter );

	m_Module.AddParameterGroup( group );
}

ModuleDescription TavernaPlugin::ModuleConverter::GetModule() const
{
	return m_Module;
}
