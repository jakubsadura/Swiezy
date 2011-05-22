/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreExtractScalarProcessor_H
#define _coreExtractScalarProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseProcessor.h"

namespace Core{

/**
Cut the surface using using meRingCutAPI.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 6 nov 2009
*/

class PLUGIN_EXPORT ExtractScalarProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(ExtractScalarProcessor, Core::BaseProcessor);
	//!
	void Update();

	//!
	std::string GetScalarName() const;
	void SetScalarName(std::string val);

private:
	//!
	ExtractScalarProcessor( );

private:
	//!
	std::string m_ScalarName;
	
};

} // Core

#endif //_coreExtractScalarProcessor_H
