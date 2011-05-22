/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreChangeOrientationProcessor_H
#define _coreChangeOrientationProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseProcessor.h"

namespace Core{

/**
Cut the surface using using meRingCutAPI.

\ingroup ImageToolsPlugin
\author Chiara Riccobene
\date 6 nov 2009
*/

class PLUGIN_EXPORT ChangeOrientationProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(ChangeOrientationProcessor, Core::BaseProcessor);

	//!
	void Update();

	//!
	bool GetInvertSliceOrder() const;
	void SetInvertSliceOrder(bool val);

	//!
	bool GetFlipUpDown() const;
	void SetFlipUpDown(bool val);

	//!
	bool GetFlipLeftRight() const;
	void SetFlipLeftRight(bool val);

private:
	//!
	ChangeOrientationProcessor( );

private:
	//!
	bool m_InvertSliceOrder;

	//!
	bool m_FlipUpDown;

	//!
	bool m_FlipLeftRight;

};

} // Core

#endif //_coreChangeOrientationProcessor_H
