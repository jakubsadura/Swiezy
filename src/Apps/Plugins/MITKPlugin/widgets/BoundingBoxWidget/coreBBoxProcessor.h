/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBBoxProcessor_H
#define _coreBBoxProcessor_H

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

class PLUGIN_EXPORT BBoxProcessor : public Core::BaseProcessor
{
public:

	typedef Core::DataHolder< Core::BoundingBox::Pointer > 
		CroppingBoundingBoxHolder;

public:
	coreDeclareSmartPointerClassMacro(BBoxProcessor, Core::BaseFilter);
	//!
	void ResetBox();
	//!
	CroppingBoundingBoxHolder::Pointer GetProcessorDataHolder() const;
	//!
	void ExportBBox( );
private:
	/**
	*/
	BBoxProcessor( );

private:

	//! The cropping parameters
	CroppingBoundingBoxHolder::Pointer m_ProcessorDataHolder;
};

} // Core

#endif //_coreBBoxProcessor_H
