/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreCroppingProcessor_H
#define _coreCroppingProcessor_H

#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBoundingBox.h"
#include "coreBaseProcessor.h"

namespace Core{

/**
Crops an image or contour using a bounding box.

\ingroup gmInteractors
\author Maarten Nieber
\date 18 nov 2008
*/

class PLUGIN_EXPORT CroppingProcessor : public Core::BaseProcessor
{
public:

	typedef Core::DataHolder< Core::BoundingBox::Pointer > 
		CroppingBoundingBoxHolder;

public:
	coreDeclareSmartPointerClassMacro(CroppingProcessor, Core::BaseFilter);
	//!
	void Update();
	//!
	CroppingBoundingBoxHolder::Pointer GetProcessorDataHolder() const;

private:
	/**
	*/
	CroppingProcessor( );

private:

	//! The cropping parameters
	CroppingBoundingBoxHolder::Pointer m_ProcessorDataHolder;
};

} // Core

#endif //_coreCroppingProcessor_H
