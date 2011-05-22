/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynModuleExecution_H
#define _dynModuleExecution_H

#include "DynLibWin32Header.h"
#include "ModuleDescription.h"
#include "dynModuleExecutionImpl.h"
#include "blLightObject.h"
#include <list>
#include <map>

/**
Dynamic ModuleDescription execution

Executes a ModuleDescription located in:
- Dynamic library
- Command line plugin

Also provides some helper functions to manage in/out of ModuleDescription that
should be moved to ModuleDescription

\author Xavi Planes
\date 14 July 2010
\ingroup DynLib
*/
class DYNLIB_EXPORT dynModuleExecution : public blLightObject
{

	typedef std::map<std::string, dynModuleExecutionImpl::Factory::Pointer> FactoryMap;

public:
	typedef dynModuleExecution Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);

	//!
	static void RegisterImpl( const std::string &name, dynModuleExecutionImpl::Factory::Pointer );

	//!
	static void UnRegisterImpl( const std::string &name );

	//!
	ModuleDescription *GetModule() const;
	void SetModule(ModuleDescription *val);

	//!
	void Update( );

	//!
	std::string GetModuleType() const;

	//!
	static std::string GetModuleType( ModuleDescription *val );

	//!
	std::string GetWorkingDirectory() const;
	void SetWorkingDirectory(std::string val);

	//!
	std::string GetForceExecutionMode() const;
	void SetForceExecutionMode(const std::string &val);

	//!
	void SetSaveScript( bool val );
	bool GetSaveScript( );

	//!
	void Clean( );

protected:

	//!
	dynModuleExecution( );

	//!
	virtual ~dynModuleExecution( );

private:
	//!
	ModuleDescription *m_Module;

	//!
	dynModuleExecutionImpl::Pointer m_Impl;

	//!
	std::string m_ModuleType;

	//!
	std::string m_WorkingDirectory;

	//!
	bool m_SaveScript;

	//! Forces execution of this kind of mode if possible
	std::string m_ForceExecutionMode;

	//!
	static FactoryMap m_Factories;
};



#endif // _dynModuleExecution_H

