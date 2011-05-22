/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreRenDataFactory_H
#define _coreRenDataFactory_H

#include "corePluginMacros.h"
#include "coreBaseFilter.h"

namespace Core
{

/** 
\brief This is a factory for the RenDataFactory classes.

RenDataFactory will check the type of DataEntity and will call the 
concrete implementation of RenDataBuilder. Currently the supported 
rendering representation is MITK. When the rendering data is created, it 
is passed as a boost::any pointer calling to the DataEntity::SetRenderingData( ).

RenderingData is created automatically when the DataEntity is added to 
the RenderingTree.

\ingroup gmDataHandling
\author Xavi Planes
\date 03 Nov 2009
*/
class PLUGIN_EXPORT RenDataFactory : public Core::BaseFilter
{
public:

	coreDeclareSmartPointerClassMacro( RenDataFactory, Core::BaseFilter )

	/**
	Provide a body for this function that builds the Rendering 
	representation of the \a processing data.
	If the builds fails, it should return a NULL pointer.
	*/
	void Update( );

protected:
	//!
	RenDataFactory(void);;
	
	//!
	virtual ~RenDataFactory(void);;

protected:

	coreDeclareNoCopyConstructors(RenDataFactory);
};

}

#endif
