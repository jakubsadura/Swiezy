/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreKernel_H
#define coreKernel_H

#include "coreObject.h"
#include "coreEnvironment.h"

namespace Core
{
class DataContainer;
class ProcessorFactories;
class WorkflowManager;

namespace Runtime
{

class Logger;
class Settings;
class wxMitkGraphicalInterface;

/** 
\brief The Kernel class provides access to all static functionalities the 
application is capable of. It essentially works as an broker for the 
programmer who wants to extend any functionality or to get access to the 
high level managers initialized by the kernel.

Is responsible of initializing, keeping alive and terminating the 
whole environment for the platform.
\ingroup gmKernel
\author Juan Antonio Moya
\date 31 May 2007
*/
class GMKERNEL_EXPORT Kernel : Core::Object
{
public:
	coreClassNameMacro(Kernel)

	//! Define virtual destructor to avoid warning
	virtual ~Kernel( );

	/**
	\brief 
	You can initialize without wxApp
	It uses only applicationName that should be the place where is gimias.exe
	"H:\Toolkit\TrunkBuild\bin\Debug\gimias.exe"
	*/
	static void Initialize(
		wxApp* application, 
		Core::Runtime::AppRunMode mode,
		std::string applicationName = "" );

	//!
	static void Terminate(void);

	typedef coreDeclareSwigWrappableSmartPointerTypeMacro(Core::Runtime::Environment) 
		ApplicationRuntimePointer;
	typedef coreDeclareSwigWrappableSmartPointerTypeMacro(Core::Runtime::Logger) 
		RuntimeLoggerPointer;
	typedef coreDeclareSwigWrappableSmartPointerTypeMacro(Core::Runtime::Settings) 
		ApplicationSettingsPointer;
	typedef coreDeclareSwigWrappableSmartPointerTypeMacro(Core::Runtime::wxMitkGraphicalInterface) 
		RuntimeGraphicalInterfacePointer;
	typedef coreDeclareSwigWrappableSmartPointerTypeMacro(Core::DataContainer) 
		DataContainerPointer;
	typedef coreDeclareSwigWrappableSmartPointerTypeMacro(Core::ProcessorFactories) 
		ProcessorFactoriesPointer;
	typedef coreDeclareSwigWrappableSmartPointerTypeMacro(Core::WorkflowManager) 
		WorkflowManagerPointer;

	static ApplicationRuntimePointer GetApplicationRuntime();
	static RuntimeLoggerPointer GetLogManager();
	static ApplicationSettingsPointer GetApplicationSettings();
	static RuntimeGraphicalInterfacePointer GetGraphicalInterface();
	static DataContainerPointer GetDataContainer();
	static ProcessorFactoriesPointer GetProcessorFactories();
	static WorkflowManagerPointer GetWorkflowManager();
	
	//! Function to override the default instance of the application settings
	static void SetApplicationSettings(ApplicationSettingsPointer settings);

	// Allow to retrieve Scripting interface only when not wrapping swig
	#ifndef SWIG_WRAPPING  
		 static void* GetScriptingInterface(void);;
	#endif

	static bool isInitialized(void);

	/**
	\brief Return a pointer to the wxApp derived class. This function is 
	used by the levelsetEvolution class in order to process the wx pending 
	events.
	*/
	static wxApp* GetApplication();

private:
	/**
	\brief Initialize m_DataContainer, 
	DataEntityIORegistration and DataEntityBuildersRegistration
	*/
	static void InitializeData( );

	//! Initialize m_ApplicationRuntime and m_ScriptingInterface
	static void InitializeApplication( 
		wxApp* application,
		Core::Runtime::AppRunMode mode );

	//! Initialize m_GraphicalInterface
	static void InitializeGraphicalInterface( 
		Core::Runtime::AppRunMode mode );

	//! m_ApplicationSettings, wxApp, m_LogManager,
	static void InitializeSettings( 
		std::string appName,
		Core::Runtime::AppRunMode mode );

private:			

	class Impl;

	static Impl m_Impl;

	//!
	static bool m_isInitialized;

};

} // namespace Runtime
} // namespace Core

#endif
