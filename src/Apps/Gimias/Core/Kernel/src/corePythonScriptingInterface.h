/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef corePythonScriptingInterface_H
#define corePythonScriptingInterface_H

#include "coreObject.h"
#include <string>

namespace Core
{
namespace Runtime
{

/** 
\brief The Scripting interface adds members to call to any interaction with the
presentation layer and offers routines to bypass to the application layer.
The Python interface offers a scripting language that follows the 
Python syntax and can call operations from the Core domain.
Core+Python scripts are also called by developers as GPS

\note: when including PythonScriptingInterface.h, put it on top of the 
include files because it also includes Python.h and that must be always 
the first include in order to compile it properly

\ingroup gmKernel
\author Juan Antonio Moya
\date 01 Apr 2006
*/
class GMKERNEL_EXPORT PythonScriptingInterface
{
public:
	// NOTE: Cannot be a SmartPointer class because it will force forward 
	// declaration issues in Kernel.h
	// and it will fail compiling because Python wants always to be the 
	// first include
	PythonScriptingInterface(void);
	~PythonScriptingInterface(void);
	
	void ParseFile(const std::string& filename);
	//QThread* ParseFileThreaded(const char* filename); // Deprecated
	void ParseString(const char* script);
	//QThread* ParseStringThreaded(const char* script); // Deprecated
    void LogMessage(const std::string& message);

	void Reset(void);

protected:
	void InitializeModules(bool log);
	bool CheckScriptHeader(const std::string& str);
	
};


} // namespace Core
} // namespace Runtime

#endif
