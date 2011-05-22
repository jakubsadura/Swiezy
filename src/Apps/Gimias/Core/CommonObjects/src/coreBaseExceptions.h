/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreBaseExceptions_H
#define coreBaseExceptions_H

#include "coreCreateExceptionMacros.h"

//////////////////////////////////////////////////////
// CODE ALL Core EXCEPTIONS IN THIS H_FILE			//
//////////////////////////////////////////////////////
/** 
\author Juan Antonio Moya
\date 02 Jun 2006
*/

//
//-- RuntimeExceptions: Non-Critical ---------------------------------------------------------------/
//

// -- Settings and profile --
coreCreateCoreRuntimeExceptionClassMacro(CannotRetrieveSettingsException, \
	"the program was unable to retrieve the application settings. ");
coreCreateCoreRuntimeExceptionClassMacro(CannotAddProfileException, \
	"cannot add the requested user type to the current profile. ");
coreCreateCoreRuntimeExceptionClassMacro(UndefinedProfileException, \
	"the user profile is undefined. ");
coreCreateCoreRuntimeExceptionClassMacro(CannotReadProfileException, \
	"cannot get the user profile from the config file. ");
coreCreateCoreRuntimeExceptionClassMacro(UserTypeUnknownException, \
	"the specified user type is unknown. ");

// -- Filesystem --
coreCreateCoreRuntimeExceptionClassMacro(FileNotFoundException, \
	"requested file was not found. ");
coreCreateCoreRuntimeExceptionClassMacro(CannotOpenFileException, \
	"requested file cannot be opened or either is locked. ");
coreCreateCoreRuntimeExceptionClassMacro(CannotReadFileException, \
	"cannot read from requested file. ");
coreCreateCoreRuntimeExceptionClassMacro(CannotWriteFileException, \
	"cannot write to requested file. ");

// -- DataTree and rendering --
coreCreateCoreRuntimeExceptionClassMacro(DataTreeNodeIsNullException, \
	"an unexpected error occourred when accessing the rendering node. ");
coreCreateCoreRuntimeExceptionClassMacro(DataTreeAccessException, \
	"an unexpected error occourred when accessing the current rendering data tree. ");
coreCreateCoreRuntimeExceptionClassMacro(AssingDataToWidgetException, \
	"an unexpected error occoured when setting a DataTreeNode as data\n source for this widget. ");

// -- DataEntityList and processing --
coreCreateCoreRuntimeExceptionClassMacro(CannotInsertDataEntityInfoException, \
	"an unexpected error occourred when inserting the data entity info\n in \
	the list of the data entity list browser. ");
coreCreateCoreRuntimeExceptionClassMacro(CannotRemoveDataEntityInfoException, \
	"an unexpected error occourred when removing the data entity info\n from the \
	list of the data entity list browser.\n The requested data entity could not be found. ");

// -- GUI --
coreCreateCoreRuntimeExceptionClassMacro(ViewLayoutException, \
	"an unexpected error occourred when setting the layout of the graphical interface.\n\
	The program may crash and its use may lead to undesirable results.\n\
	Save your work and restart the program. ");

// -- Scripting --
coreCreateCoreRuntimeExceptionClassMacro(OnLoadScriptingModuleException, \
	"an unexpected error occoured when loading a scripting module.\n\
	That module will not be loaded. ");
coreCreateCoreRuntimeExceptionClassMacro(CannotExecScriptFileException, \
	"cannot execute the requested script file. ");

// -- Plugins --
coreCreateCoreRuntimeExceptionClassMacro(OnLoadPluginException, \
	"an unexpected error occoured when loading a plugin.\nThat plugin will not be loaded. ");
coreCreateCoreRuntimeExceptionClassMacro(OnLoadPluginObjectAlreadyLockedException, \
	"an unexpected error occoured when loading a plugin, \
	its shared library has been locked by another resource.\nThat plugin will not be loaded. ");
coreCreateCoreRuntimeExceptionClassMacro(OnLoadPluginProcedureNotFoundException, \
	"an unexpected error occoured when loading a plugin, expected procedure \
	newFrontEndPlugin()\n was not found.\nThat call has not been defined there, \
	or either the shared library is not a recognized plugin for this application. ");
coreCreateCoreRuntimeExceptionClassMacro(OnLoadPluginBadObjectException, \
	"an unexpected error occoured when loading a plugin, plugin object is \
	badly formed or is of unknown type.\nThat plugin will not be loaded. ");
coreCreateCoreRuntimeExceptionClassMacro(OnLoadPluginGUINotFoundException, \
	"an unexpected error occoured when loading a plugin, it does not have \
	a valid GUI to be attached.\nCheck that it was created properly and \
	that contains an initialized PluginTab.\nThat plugin will not be loaded. ");
coreCreateCoreRuntimeExceptionClassMacro(OnAttachPluginToGUIException, \
	"an unexpected error occoured when attaching the plugin to the GUI. \
	Its GUI is not well formed or either\n the graphical interface of the \
	application is not properly set.\nThat plugin will not be loaded. ");
coreCreateCoreRuntimeExceptionClassMacro(OnDetachPluginFromGUIException, \
	"an unexpected error occoured when detaching the plugin from the GUI. \
	Some resources could not be released.\n\
	That plugin might not be properly unloaded. ");

// -- Resources --
coreCreateCoreRuntimeExceptionClassMacro(CannotLoadImageException, \
	"cannot load the requested image. ");
coreCreateCoreRuntimeExceptionClassMacro(SplashWindowBitmapIsNotSetException, \
	"the bitmap for the splash window was not properly set or it is missing.\n\
	The program may not show the splash window. ");
coreCreateCoreRuntimeExceptionClassMacro(CannotCloseImageException, \
	"the requested image cannot be closed. ");

// -- XML --
coreCreateCoreRuntimeExceptionClassMacro(ParsingXMLException, \
	"an error was found when parsing the XML file. ");
coreCreateCoreRuntimeExceptionClassMacro(ProcessingDOMException, \
	"an error was found when processing the XML document. ");



//
//-- CriticalExceptions: Program will be terminated ------------------------------------------------/
//

// -- Initialization --
coreCreateCoreCriticalExceptionClassMacro(OnAppInitException, \
	"unexpected error found while initializing the application. \n\
	The kernel was trying to reset and the Terminate call threw an exception.\n\
	The kernel is unstable and will kill the application. ");
coreCreateCoreCriticalExceptionClassMacro(GraphicalInterfaceNotSetException, \
	"unexpected error found while starting the graphical interface. ");
coreCreateCoreCriticalExceptionClassMacro(BaseAppNotSetException, \
	"unexpected error found when starting the base application. ");
coreCreateCoreCriticalExceptionClassMacro(RuntimeEnvironmentNotSetException, \
	"unexpected error found when starting the runtime environment. ");
coreCreateCoreCriticalExceptionClassMacro(InitializingXMLSubsystemException, \
	"unable to initialize the runtime XML subsystem. ");
coreCreateCoreCriticalExceptionClassMacro(LoggerNotSetException, \
	"unexpected error found when starting the log manager. ");
coreCreateCoreCriticalExceptionClassMacro(SettingsNotSetException, \
	"unexpected error found when starting the application settings manager. ");
coreCreateCoreCriticalExceptionClassMacro(DataContainerNotSetException, \
	"unexpected error found when creating the data structures of the kernel. ");

// -- Miscellaneous --
coreCreateCoreCriticalExceptionClassMacro(UnhandledException, \
	"unhandled exception caught. ");


#endif
