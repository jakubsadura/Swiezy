/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreITKTransformRenDataBuilder_H
#define coreITKTransformRenDataBuilder_H

#include "corePluginMacros.h"
#include "coreRenDataBuilder.h"

namespace Core
{
/** 

\ingroup gmDataHandling
\author: Xavi Planes
\date: 30 July 2010
*/

class PLUGIN_EXPORT ITKTransformRenDataBuilder 
	: public Core::RenDataBuilder
{
public:
	coreDeclareSmartPointerClassMacro( 
		ITKTransformRenDataBuilder, 
		RenDataBuilder)

	void Update( );

protected:
	ITKTransformRenDataBuilder();
	~ITKTransformRenDataBuilder();

private:
	coreDeclareNoCopyConstructors(ITKTransformRenDataBuilder);
};

}

#endif
