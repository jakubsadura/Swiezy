/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CoreShapeScaleProcessor_H
#define _CoreShapeScaleProcessor_H


#include "corePluginMacros.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"

namespace Core{

/**
Processor for scaling a shape

\ingroup MeshEditorPlugin
\author Chiara Riccobene (from Maarten Nieber)
\date 01 feb 2010
*/
class  PLUGIN_EXPORT ShapeScaleProcessor : public Core::BaseProcessor
{
public:
	typedef Core::DataHolder< float > ParametersHolder;

public:
	//!
	coreDeclareSmartPointerClassMacro(ShapeScaleProcessor,  Core::BaseProcessor);

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

} // namespace Core{

#endif //_CoreShapeScaleProcessor_H
