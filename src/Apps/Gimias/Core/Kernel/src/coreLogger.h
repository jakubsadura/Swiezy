/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreLogger_H
#define coreLogger_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include <stdio.h>
#include <string>

namespace Core
{
// Fwd declaration
namespace Exceptions { class Exception; }

namespace Runtime
{
class Kernel;

/** 
\brief The logger tracks down all the errors, warnings and messages 
history made in Core and stores it to a file
for later inspection.

\note You should access and get the instance for this class through 
the runtime high-end object: Kernel

\sa Core::Runtime::Kernel
\ingroup gmKernel
\author Juan Antonio Moya
\date 31 May 2006
*/
class GMKERNEL_EXPORT Logger : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(Logger, Core::SmartPointerObject);

	std::string GetFileName() const;
	void SetFileName(const std::string& filename);
	void LogMessage(const std::string& message);
	void LogException(Core::Exceptions::Exception& e) throw();
	void Clear(void);
	std::string ReadLogFileGetFullText(void);
	void AppendNewLogEntry(void);

protected:
	Logger(void);
	virtual ~Logger(void);

private:
	std::string m_FileName;
	FILE* _fp;

	coreDeclareNoCopyConstructors(Logger);
};

}
}

#endif
