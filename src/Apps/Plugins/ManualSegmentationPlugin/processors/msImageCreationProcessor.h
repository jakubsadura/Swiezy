/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef _msImageCreationProcessor_H
#define _msImageCreationProcessor_H

#include "corePluginMacros.h"
#include "coreBaseProcessor.h"

namespace Core
{
/**
\brief Create a mask image from a mesh 

\sa Core::MeshCreation
\ingroup ManualSegmentationPlugin
\author Xavi Planes
\date 07 May 2010
*/
class PLUGIN_EXPORT msImageCreationProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(msImageCreationProcessor, Core::BaseFilter);

	//!
	void Update( );

protected:
	msImageCreationProcessor(void);
	virtual ~msImageCreationProcessor(void);

private:
	coreDeclareNoCopyConstructors(msImageCreationProcessor);
};

} // namespace Core

#endif

