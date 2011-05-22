/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreTetraGenerationProcessor_H
#define _coreTetraGenerationProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"

#include "meVTKTetraGenerationFilter.h"
#include "meNGTetraFilter.h"

namespace Core{

/**
Close the holes of the surface using meVTKTetraGenerationFilter.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 4 nov 2009
*/

class PLUGIN_EXPORT TetraGenerationProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(TetraGenerationProcessor, Core::BaseProcessor);
	//!
	void Update();
	//!
	void SetNetgenParams(meMeshParamsOptimizePtr param);
	//!
	meMeshParamsOptimizePtr GetNetgenParams( );

	//!
	void SetParams(meMeshTetraGenerationParamsPtr param);
	//!
	meMeshTetraGenerationParamsPtr GetParams( );

	//! set type of tetra used
	void SetFlag(bool val);
	bool GetFlag();

	bool CheckInput();

private:
	/**
	*/
	TetraGenerationProcessor( );

private:
	//!
	vtkSmartPointer<meVTKTetraGenerationFilter> m_FilterTet;
	//!
	meNGTetraFilter::Pointer m_Filter;
	//!
	bool m_type;
};

} // Core

#endif //_coreTetraGenerationProcessor_H
