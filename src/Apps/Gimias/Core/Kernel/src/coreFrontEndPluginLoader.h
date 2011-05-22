/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef coreFrontEndPluginLoader_H
#define coreFrontEndPluginLoader_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include <itksys/DynamicLoader.hxx>
#include "coreProfile.h"
#include "coreBaseFrontEndPlugin.h"

namespace Core
{
namespace Runtime
{

/**
\brief Loads a plugin dinamically.


\ingroup gmKernel
\author Xavi Planes
\date 10 April 2010
*/
class GMKERNEL_EXPORT FrontEndPluginLoader : public Core::SmartPointerObject
{
public:
	typedef Core::FrontEndPlugin::BaseFrontEndPlugin::Pointer 
		(*FrontEndPluginConstructorFunc)(void);
	typedef Core::Profile::Pointer (*GetProfileFromPluginFunc)(void);

	coreDeclareSmartPointerClassMacro(
		FrontEndPluginLoader, 
		Core::SmartPointerObject);
	
	//!
	void SetPath( const std::string path );

	//! 
	void LoadXML( );

	//! Load plugin library into memory
	void LoadLibrary( );

	//!
	void CloseLibrary( );

	//! Resolve the procedure "newFrontEndPlugin"
	void CreateBaseFrontEndPlugin( );

	//!
	Core::FrontEndPlugin::BaseFrontEndPlugin::Pointer GetBaseFrontEndPlugin();

	//!
	void ResetBaseFrontEndPlugin();

	//!
	itksys::DynamicLoader::LibraryHandle GetLibraryHandle();

	//! Resolve the procedure "getProfileDescriptor"
	Core::Profile::Pointer GetProfileDescriptor( );

	//!
	std::string GetLibraryFilename() const;

	//!
	std::list<std::string> GetDependencies() const;

	//!
	std::string GetName() const;

	//!
	std::string GetLibraryPath() const;

protected:
	FrontEndPluginLoader(void);
	virtual ~FrontEndPluginLoader(void);

	coreDeclareNoCopyConstructors(FrontEndPluginLoader);

	//!
	void LoadLibraryHandle( const std::string &filePath );

	//!
	void ReadXML( );

	//!
	void WriteXML( );

	//!
	void FindLibraryPath( );
private:

	//!
	std::string m_Path;

	//! "plugins/SceneViewPlugin/lib/Debug/"
	std::string m_LibraryPath;

	//!
	std::string m_LibraryFilename;

	//!
	itksys::DynamicLoader::LibraryHandle m_LibraryHandle;

	//!
	Core::Profile::Pointer m_Profile;

	//!
	Core::FrontEndPlugin::BaseFrontEndPlugin::Pointer m_BaseFrontEndPlugin;

	//!
	std::list<std::string> m_Dependencies;

	//!
	std::string m_Name;

	//! Create a default unique name 
	static int m_PluginNameIndex;
};

} // namespace Runtime
} // namespace Core

#endif

