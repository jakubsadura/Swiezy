/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "TavernaPluginModuleExecutionTaverna.h"
#include "itksys/SystemTools.hxx"

TavernaPlugin::ModuleExecutionTaverna::ModuleExecutionTaverna( )
{
}

TavernaPlugin::ModuleExecutionTaverna::~ModuleExecutionTaverna()
{
}

void TavernaPlugin::ModuleExecutionTaverna::BuildCommandLineLocation()
{
	m_CommandLineAsString.clear();

	// Taverna executable
	m_CommandLineAsString.push_back( m_Module->GetLocation() );

	// Workflow
	m_CommandLineAsString.push_back( m_Module->GetTarget( ) );
}

void TavernaPlugin::ModuleExecutionTaverna::SetParameterValue( ModuleParameter *param )
{

	if ( param->GetChannel() == "input" )
	{
		m_CommandLineAsString.push_back( "-" + param->GetLongFlag() );
		m_CommandLineAsString.push_back( param->GetName() );
		m_CommandLineAsString.push_back( param->GetDefault() );
	}
	if ( param->GetChannel() == "output" && param->GetTag() == "directory" )
	{
		// Output directory
		std::string outputDir =  GetWorkingDirectory() + "/" + param->GetDefault();
		itksys::SystemTools::RemoveADirectory( outputDir.c_str() );
		m_CommandLineAsString.push_back( "-" + param->GetLongFlag() );
		m_CommandLineAsString.push_back( outputDir );
	}
}
