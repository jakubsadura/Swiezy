/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSignalTimePropagationProcessor_H
#define _coreSignalTimePropagationProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"

namespace Core{

/**
Propagates time information from signal to connected data entity (3d +t).

\ingroup SignalViewerPlugin
\author Chiara Riccobene
\date 9 dec 2009
*/

class PLUGIN_EXPORT SignalTimePropagationProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(SignalTimePropagationProcessor, Core::BaseProcessor);
	//!
	void Update();
	//!
	bool CheckInputs();
	//!
	int GetMarkersPerScan() const;
	void SetMarkersPerScan(int val);
	
private:
	//!
	SignalTimePropagationProcessor( );	

	//!
	int m_MarkersPerScan;
};

} // Core

#endif //_coreSignalTimePropagationProcessor_H
