/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _SandboxPluginShapeScaleProcessor_H
#define _SandboxPluginShapeScaleProcessor_H

#include "coreBaseProcessor.h"
#include "coreProcessorFactory.h"
#include "coreSmartPointerMacros.h"
#include "corePluginMacros.h"
#include "coreProcessorFactory.h"

namespace SandboxPlugin{

/**
Processor for scaling a shape

\ingroup SandboxPlugin
\author Maarten Nieber
\date 22 jun 2008
*/
class PLUGIN_EXPORT ShapeScaleProcessor : public Core::BaseProcessor
{
public:
	typedef Core::DataHolder< float > ParametersHolder;

public:
	//!
	coreProcessor(ShapeScaleProcessor, Core::BaseProcessor);

	//! Call library to perform operation
	void Update( );

	//!
	ParametersHolder::Pointer GetParametersHolder() const;

private:
	//!
	ShapeScaleProcessor( );

	//!
	~ShapeScaleProcessor();

	//! Purposely not implemented
	ShapeScaleProcessor( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//!
	ParametersHolder::Pointer m_ParametersHolder;
};

} // namespace SandboxPlugin{

#endif //_SandboxPluginShapeScaleProcessor_H
