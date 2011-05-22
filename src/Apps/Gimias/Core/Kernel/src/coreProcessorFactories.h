/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreProcessorFactories_H
#define _coreProcessorFactories_H

#include "coreObject.h"
#include "coreBaseWindow.h"
#include "coreWindowConfig.h"
#include "coreProcessorFactory.h"

namespace Core
{

/**
\brief Repository of all BaseWindow registered to the framework

\ingroup gmKernel
\sa FrontEndPlugin
\author Xavi Planes
\date 10 April 2010
*/
class GMKERNEL_EXPORT ProcessorFactories : public Core::SmartPointerObject
{
public:
	//! Map using classname as key
	typedef std::map<std::string, ProcessorFactory::Pointer> FactoriesMapType;
	//!
	typedef Core::DataHolder<FactoriesMapType> FactoriesMapHolderType;

public:

	coreDeclareSmartPointerClassMacro(
		ProcessorFactories, 
		Core::SmartPointerObject);

	//!
	void CleanRegisteredFactories(  );

	//! Create a processor
	Core::BaseProcessor::Pointer Create( const std::string &classname );

	//! Create a processor
	template<class T>
	bool CreateProcessor( typename T::Pointer &processor )
	{
		Core::BaseProcessor::Pointer baseProcessor = Create( typeid( T ).name( ) );
		if ( baseProcessor.IsNull() )
		{
			return false;
		}
		processor = dynamic_cast<T*> ( baseProcessor.GetPointer() );
		return processor.IsNotNull();
	}

	//!
	FactoriesMapHolderType::Pointer GetFactoriesHolder();

	//! Register a factory that will be used to create an instance
	void RegisterFactory( ProcessorFactory::Pointer factory );

protected:
	//!
	ProcessorFactories( );

	//!
	FactoriesMapType &GetFactoriesMap();

protected:

	//!
	FactoriesMapHolderType::Pointer m_CreatorsMapHolder;
};

} // namespace Core

#endif // _coreProcessorFactories_H

