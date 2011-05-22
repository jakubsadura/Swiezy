/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreProcessorOutputsObserverBuilder_H
#define _coreProcessorOutputsObserverBuilder_H

#include "gmKernelWin32Header.h"
#include "coreBaseProcessor.h"
#include "coreProcessorOutputObserver.h"
#include "coreSmartPointerMacros.h"
#include "coreRenderingTree.h"
#include "coreObject.h"


namespace Core{
namespace Widgets{

/**
\brief Add OutputsObserver to the panel widget depending on the
Processor output port properties

\ingroup gmKernel
\author Chiara Riccobene
\date 10 11 2009
*/
class GMKERNEL_EXPORT ProcessorOutputsObserverBuilder : 
	public Core::SmartPointerObject
{
public:

	//!
	coreDeclareSmartPointerClassMacro(ProcessorOutputsObserverBuilder,
									Core::SmartPointerObject);

	/**
	\param [in] val Input processor
	\param [in] tree Rendering Tree
	\output [out] vector of ProcessorOutputObserver for every output
	*/
	void Init( 
		Core::BaseFilter::Pointer val,
		Core::RenderingTree::Pointer tree);
	
	Core::ProcessorOutputObserver::Pointer
		GetOutputObserver(int num);

	//!
	void SetRenderingTree( Core::RenderingTree::Pointer tree );

	//!
	std::vector<Core::ProcessorOutputObserver::Pointer> GetList() const;

private:
	//!
	ProcessorOutputsObserverBuilder( );

	//!
	~ProcessorOutputsObserverBuilder();

private:

	//!
	std::vector<Core::ProcessorOutputObserver::Pointer> m_list;
};

} // Widgets
} // Core

#endif //_coreProcessorOutputsObserverBuilder_H
