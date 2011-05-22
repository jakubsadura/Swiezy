/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreExecuteCommandProcessor_H
#define _coreExecuteCommandProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"
#include "CILabExceptionMacros.h"

namespace Core{

/**
Execute a Commandline with specific options.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 18 nov 2009
*/

class PLUGIN_EXPORT ExecuteCommandProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(ExecuteCommandProcessor, Core::BaseProcessor);

	//!
	cilabDeclareExceptionMacro( Exception, std::exception );

	//!
	void Update();

	//!
	void UpdateData();

	//!
	void UpdateWidget();

	std::string GetCommandLine(std::string name) const;

	const char* GetExecutableOptions() const;
	void SetExecutableOptions(const char* name );

	const char* GetExecutableName() const;
	void SetExecutableName(const char* name );

	const char* GetInputName() const;
	void SetInputName(const char* name );

private:
	/**
	*/
	ExecuteCommandProcessor( );

	//!
	std::string m_Executable;

	//!
	std::string m_ExecutableOptions;

	//!
	std::string m_fileName;
};

} // Core

#endif //_coreExecuteCommandProcessor_H
