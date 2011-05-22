/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//#include "corePythonScriptingInterface.h"
#include "coreKernel.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntityBuildersRegistration.h"
#include "coreXMLSubsystem.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreDataEntityIORegistration.h"

#include "itkDataEntityIOFactory.h"

#include "coreLogger.h"
#include "coreSettings.h"
#include "coreDataContainer.h"
#include "coreProcessorFactories.h"
#include "coreWorkflowManager.h"

using namespace Core::Runtime;

class Kernel::Impl
{
public:
	//! Application Settings
	ApplicationSettingsPointer m_ApplicationSettings;
	//! Log manager 
	RuntimeLoggerPointer m_LogManager;
	//! Application runtime
	ApplicationRuntimePointer m_ApplicationRuntime;
	//! Graphical Interface layer
	RuntimeGraphicalInterfacePointer m_GraphicalInterface;
	//! Data members of the model
	DataContainerPointer m_DataContainer;
	//! Scripting Interface layer 
	void* m_ScriptingInterface;
	//! Main application
	wxApp* m_Application;
	//!
	ProcessorFactoriesPointer m_ProcessorFactories;
	WorkflowManagerPointer m_WorkflowManager;
};

bool Kernel::m_isInitialized = false;
Kernel::Impl Kernel::m_Impl;



Core::Runtime::Kernel::~Kernel()
{
}

/** Initializer for the class Domain */
void Kernel::Initialize(
	wxApp* application, 
	Core::Runtime::AppRunMode mode,
	std::string applicationName ) 
{
	try
	{
		if(isInitialized())
			Terminate();
	}
	coreCatchExceptionsCastToNewTypeAndThrowMacro(Core::Exceptions::OnAppInitException, Kernel::Initialize)

	try
	{
		m_Impl.m_Application = application;
		
		// First thing to do: Initialize the XML subsystem
		Core::IO::XML::XMLSubsystem::Initialize();

		// Initialize the Settings Manager object. third thing to do
		if(application != NULL && application->argv != NULL && application->argv[0] != NULL) 
			applicationName = wxString::FromUTF8( application->argv[0] );
		InitializeSettings( applicationName, mode );

		InitializeData();

		// After settings
		m_Impl.m_ProcessorFactories = ProcessorFactories::New();
		m_Impl.m_WorkflowManager = WorkflowManager::New();
		m_Impl.m_WorkflowManager->Initialize( );

		InitializeApplication( application, mode );

		InitializeGraphicalInterface( mode );

	}
	coreCatchExceptionsAddTraceAndThrowMacro(Kernel::Initialize);

	m_isInitialized = true;
}

/** Destructor for the class Domain */
void Kernel::Terminate(void)
{
	try
	{
		// Unregister data entity builders
		Core::DataEntityBuildersRegistration::UnRegisterDataEntityBuilders();

		// Unregister all readers/writers
		Core::IO::DataEntityReader::UnRegisterAllFormats();
		Core::IO::DataEntityWriter::UnRegisterAllFormats();

		GetProcessorFactories()->CleanRegisteredFactories( );

		// Load specific itk reader for DataEntity
		itk::DataEntityIOFactory::UnRegisterOneFactory( );

		// First delete the data container.
		// When a plug in allocates a blSignal object and builds a
		// data entity, the memory of this processing data is in the
		// memory space of the plug in, and when the plug ins are unloaded
		// the memory will be invalid
		m_Impl.m_DataContainer = NULL;
		m_Impl.m_ScriptingInterface = NULL;
		
		// Unload all the plug ins
		m_Impl.m_GraphicalInterface = NULL;
		m_Impl.m_ApplicationRuntime = NULL;
		m_Impl.m_LogManager = NULL;

		// Destroy WorkflowManager befor applications settings
		m_Impl.m_WorkflowManager = NULL;

		m_Impl.m_ApplicationSettings = NULL;
		
		// Terminate the XML subsystem
		Core::IO::XML::XMLSubsystem::Terminate();
		m_isInitialized = false;
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Kernel::Terminate)
}


/**
Returns true if a previous call to Kernel::Initialize worked perfectly. 
Therefore, true would mean that the kernel was initialized properly 
*/
bool Kernel::isInitialized(void)
{
	return m_isInitialized;
}


/**
 */
Core::Runtime::Kernel::ApplicationSettingsPointer Core::Runtime::Kernel::GetApplicationSettings()
{
	return m_Impl.m_ApplicationSettings;
}

/** 
 */
Core::Runtime::Kernel::RuntimeLoggerPointer Core::Runtime::Kernel::GetLogManager()
{
	return m_Impl.m_LogManager;
}

/**
 */
Core::Runtime::Kernel::ApplicationRuntimePointer Core::Runtime::Kernel::GetApplicationRuntime()
{
	return m_Impl.m_ApplicationRuntime;
}

/**
 */
Core::Runtime::Kernel::RuntimeGraphicalInterfacePointer Core::Runtime::Kernel::GetGraphicalInterface()
{
	return m_Impl.m_GraphicalInterface;
}

/**
 */
Core::Runtime::Kernel::DataContainerPointer Core::Runtime::Kernel::GetDataContainer()
{
	return m_Impl.m_DataContainer;
}

Core::Runtime::Kernel::ProcessorFactoriesPointer Core::Runtime::Kernel::GetProcessorFactories()
{
	return m_Impl.m_ProcessorFactories;
}

/**
 */
void Core::Runtime::Kernel::InitializeData()
{

	// Initialize the data structures of the model
	m_Impl.m_DataContainer = Core::DataContainer::New();
	if(m_Impl.m_DataContainer.IsNull())
	{
		Core::Exceptions::DataContainerNotSetException e("Kernel::Initialize");
		throw e;
	}

	// register required data entity builders
	Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();


	// register required data entity builders
	Core::IO::DataEntityIORegistration::RegisterDefaultFormats();

	// Load specific itk reader for DataEntity
	itk::DataEntityIOFactory::RegisterOneFactory( );
}

void Core::Runtime::Kernel::InitializeSettings( 
						std::string appName,
						Core::Runtime::AppRunMode mode )
{
	m_Impl.m_ApplicationSettings = Core::Runtime::Settings::New(appName);
	m_Impl.m_ApplicationSettings->TestCreateConfigFile();

	if(m_Impl.m_ApplicationSettings.IsNull())
	{
		Core::Exceptions::SettingsNotSetException e("Kernel::Initialize");
		throw e;
	}
	m_Impl.m_ApplicationSettings->LoadSettings( );

	// Initialize the Logger object for interfacing with system console and all other reporting tools
	m_Impl.m_LogManager = Logger::New();
	if(m_Impl.m_LogManager.IsNull())
	{
		Core::Exceptions::LoggerNotSetException e("Kernel::Initialize");
		throw e;
	}
	m_Impl.m_LogManager->SetFileName(m_Impl.m_ApplicationSettings->GetLogFileFullPath());
}

void Core::Runtime::Kernel::InitializeApplication( 
	wxApp* application,
	Core::Runtime::AppRunMode mode )
{

	if ( application == NULL )
	{
		return;
	}

	// Initialize the m_Impl.m_ApplicationRuntime environment
	m_Impl.m_ApplicationRuntime = Environment::New();
	if(m_Impl.m_ApplicationRuntime.IsNull())
	{
		Core::Exceptions::RuntimeEnvironmentNotSetException e("Kernel::Initialize");
		throw e;
	}

	m_Impl.m_ApplicationRuntime->InitApplication(application->argc, application->argv, mode);

	// Initialize the Scripting Interface
	//m_Impl.m_ScriptingInterface = new Core::Runtime::PythonScriptingInterface();
}

void Core::Runtime::Kernel::InitializeGraphicalInterface( 
	Core::Runtime::AppRunMode mode )
{
	switch(mode)
	{
	case Core::Runtime::Graphical:
		{
			// Create the Graphical Interface
			m_Impl.m_GraphicalInterface = wxMitkGraphicalInterface::New();

			if(m_Impl.m_GraphicalInterface.IsNull())
			{
				Core::Exceptions::GraphicalInterfaceNotSetException e("Kernel::Initialize");
				throw e;
			}

			// Start the Graphical Interface
			m_Impl.m_GraphicalInterface->Start( );
		}

	default:
		break;
	}
}

/**
Return the application itself
*/
wxApp* Core::Runtime::Kernel::GetApplication()
{
	return m_Impl.m_Application;
}

void Core::Runtime::Kernel::SetApplicationSettings( ApplicationSettingsPointer settings )
{
	Core::Runtime::Kernel::m_Impl.m_ApplicationSettings = settings;
}

void* Core::Runtime::Kernel::GetScriptingInterface( void )
{
	return m_Impl.m_ScriptingInterface;
}

Core::Runtime::Kernel::WorkflowManagerPointer Core::Runtime::Kernel::GetWorkflowManager()
{
	return m_Impl.m_WorkflowManager;
}
