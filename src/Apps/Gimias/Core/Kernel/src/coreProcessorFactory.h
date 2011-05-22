/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreProcessorFactory_H
#define _coreProcessorFactory_H

#include "coreObject.h"
#include "coreBaseProcessor.h"

namespace Core
{

/**
\brief Base Factory for BaseWindow

\ingroup gmKernel
\author Xavi Planes
\date 10 May 2010
*/
class GMKERNEL_EXPORT ProcessorFactory : public Core::SmartPointerObject
{
public:
	coreClassNameMacro(ProcessorFactory);
	coreDeclareSmartPointerTypesMacro(
		ProcessorFactory, 
		Core::SmartPointerObject);

	//!
	virtual std::string GetProcessorClassname() const = 0;

	//!
	virtual Core::BaseProcessor::Pointer Create( ) = 0;

protected:
};


} // namespace Core

/**
\brief Create a ProcessorFactory derived class
\ingroup gmKernel
\author Xavi Planes
\date 10 May 2010
*/
#define coreDefineProcessorFactory(className) \
	class Factory : public Core::ProcessorFactory \
	{ \
	public:  \
		coreDeclareSmartPointerTypesMacro(Factory,ProcessorFactory) \
		coreFactorylessNewMacro(Factory) \
		coreClassNameMacro(Factory)\
		static ProcessorFactory::Pointer NewBase(void) \
		{ \
			return New( ).GetPointer( );\
		} \
		virtual Core::BaseProcessor::Pointer Create( ) \
		{ \
			return className::New( ).GetPointer( ); \
		} \
		std::string GetProcessorClassname() const \
		{ \
			return #className; \
		} \
	}; \



#endif // coreProcessorFactory_H

