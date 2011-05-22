/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef corePluginMacros__H
#define corePluginMacros__H

/** Definitions for handling shared libraries in Windows */
#if (defined(_WIN32) || defined(WIN32)) && !defined(SWIG_WRAPPING)
#	undef PLUGIN_EXPORT
#	define PLUGIN_EXPORT __declspec(dllexport)
#else
/* unix needs nothing */
#	undef  PLUGIN_EXPORT
#	define PLUGIN_EXPORT
#endif

/** 
\brief Definition for exporting plugin objects out of a shared library. 
The PLUGIN_OBJECT macro makes the export definition crosscompiler, because 
extern "C" removes C++ name mangling. 
For all the macros for defining and exporting plugins, you should read the 
Core::FrontEndPlugin::FrontEndPlugin documentation.
\sa Core::FrontEndPlugin::FrontEndPlugin

\ingroup gmFrontEndPlugin
\author Juan Antonio Moya
\date 17 Nov 2007
*/
#ifdef PLUGIN_OBJECT
#undef PLUGIN_OBJECT
#endif
#define PLUGIN_OBJECT extern "C" PLUGIN_EXPORT

/**
\brief Macro for exporting a class named 'pluginClassName' and make it 
importable for the Core FrontEndPluginManager
\ingroup gmFrontEndPlugin

It also exports the profile descriptor, so the Core can know if the plugin 
in this shared library
matches the profile with the users, thus is going to be loaded.
*/
// - 1 of 4 macros ---------------------
/**
\def coreBeginDefinePluginMacro starts a definition context that makes the 
class exportable plugin object
\ingroup gmFrontEndPlugin
\param classname name of the plugin class being defined, without namespace 
specifiers (forced to Core::FrontEndPlugin namespace)
*/
#define coreBeginDefinePluginMacro(classname)								\
	PLUGIN_OBJECT classname::Pointer newFrontEndPlugin(void)				\
	{																		\
		classname::Pointer plugin = classname::New();						\
		return plugin;														\
	};																		\
	PLUGIN_OBJECT Core::Profile::Pointer getProfileDescriptor(void){ \
		Core::Profile::Pointer profile = Core::Profile::New();

// - 2 of 4 macros ---------------------
/** \def coreDefinePluginAddProfileMacro is used inside a plugin 
definition context (after coreBeginDefinePluginMacro call).
It registers a profile name to the plugin class. Read more about profiles
in Core::Profile class.
\ingroup gmFrontEndPlugin
\sa Core::Profile
*/
#define coreDefinePluginAddProfileMacro(profileName) profile->AddToProfile(profileName);

// - 3 of 4 macros ---------------------
/** \def coreDefinePluginAddDefaultProfileMacro is used inside a plugin 
definition context (after coreBeginDefinePluginMacro call).
It registers a default profile name to the plugin class ("Common 
Functionalities"), which groups procedures of common interest.
Read more about profiles in Core::Profile class.
\ingroup gmFrontEndPlugin
\sa Core::Profile
*/
#define coreDefinePluginAddDefaultProfileMacro() \
	coreDefinePluginAddProfileMacro("Common Functionalities");

// - 4 of 4 macros ---------------------
/** \def coreEndDefinePluginMacro ends and closes a plugin definition 
context (opened by coreBeginDefinePluginMacro).
\ingroup gmFrontEndPlugin
\sa Core::Profile
*/
#define coreEndDefinePluginMacro() return profile; }


#endif // corePluginMacros__H

