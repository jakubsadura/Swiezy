/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreRingCutProcessor_H
#define _coreRingCutProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"
#include "meRingCut.h"

namespace Core{

/**
Cut the surface using using meRingCutAPI.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 6 nov 2009
*/

class PLUGIN_EXPORT RingCutProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(RingCutProcessor, Core::BaseProcessor);
	//!
	void Update();
	//!
	void ComputePlaneUsingRingCut();
	//!
	bool CheckInputs();
	
private:
	/**
	*/
	RingCutProcessor( );
	
	meRingCut::Pointer	m_meRingCut;
	
};

} // Core

#endif //_coreRingCutProcessor_H
