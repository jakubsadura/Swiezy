/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreReportExceptionMacros.h"
#include "coreKernel.h"
#include "coreLogger.h"
#include "coreEnvironment.h"
#include "coreWxMitkGraphicalInterface.h"

void Core::LogExceptionWithLoggerAndGraphicalInterface(Core::Exceptions::Exception& e)
{
	Core::Runtime::Logger::Pointer logger = Core::Runtime::Kernel::GetLogManager();
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = 
		Core::Runtime::Kernel::GetGraphicalInterface();
	if(logger.IsNotNull()) 
		logger->LogException(e);
	if(graphicalIface.IsNotNull()) 
		graphicalIface->LogException(e);
}

void Core::LogException(Core::Exceptions::Exception& e)
{
	Core::Runtime::Logger::Pointer logger = Core::Runtime::Kernel::GetLogManager();
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = 
		Core::Runtime::Kernel::GetGraphicalInterface();
	if(logger.IsNotNull()) 
		logger->LogException(e);
	if(graphicalIface.IsNotNull()) 
		graphicalIface->LogMessage(e.what());
}

