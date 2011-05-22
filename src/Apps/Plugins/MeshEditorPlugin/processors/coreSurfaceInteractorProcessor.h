/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSurfaceInteractorProcessor_H
#define _coreSurfaceInteractorProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"
#include "CILabExceptionMacros.h"

#include "coreVTKProcessor.h"
#include "meVTKSmoothFilter.h"
#include "meVTKLocalRefinerFilter.h"
#include "meVTKEdgeSwappingFilter.h"
#include "meVTKLoopSubdivisionRefinerFilter.h"


namespace Core{

/**
Processor to apply filters on selection of surfaces.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 14 Apr 2010
*/

class PLUGIN_EXPORT SurfaceInteractorProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(SurfaceInteractorProcessor, Core::BaseProcessor);

	//!
	cilabDeclareExceptionMacro( Exception, std::exception );

	typedef VTKProcessor<meVTKLocalRefinerFilter> LocalRefinerProcessor;
	typedef Core::VTKProcessor<meVTKEdgeSwappingFilter> EdgeSwappingProcessor;
	typedef VTKProcessor<meVTKLoopSubdivisionRefinerFilter> LoopSubdivisionProcessor;
	typedef VTKProcessor<meVTKSmoothFilter> TaubinSmoothProcessor;

	enum ProcessorType{
		RemoveCells,
		LocalRefiner,
		LoopSubdivision,
		EdgeSwap,
		TaubinSmooth
	};

	//!
	void Update();

	//! 
	void SetProcessorType( ProcessorType type);

private:
	/**
	*/
	SurfaceInteractorProcessor( );

	ProcessorType m_processorType;

};

} // Core

#endif //_coreSurfaceInteractorProcessor_H
