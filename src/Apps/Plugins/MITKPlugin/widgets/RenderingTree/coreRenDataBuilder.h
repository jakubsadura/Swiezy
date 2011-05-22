/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreRenDataBuilder_H
#define coreRenDataBuilder_H

#include "corePluginMacros.h"
#include "coreBaseFilter.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

namespace Core
{

/** 
\brief This is a base class for the RenDataBuilder classes.
The RenDataBuilder is responsible of translating the processing 
representation of a DataEntity to its rendering representation.

\ingroup gmDataHandling
\author Juan Antonio Moya
\date 28 Mar 2008
*/
class PLUGIN_EXPORT RenDataBuilder : public Core::BaseFilter
{
public:

	coreDeclareSmartPointerTypesMacro( RenDataBuilder, Core::BaseFilter )
	coreClassNameMacro( RenDataBuilder )

	/**
	Provide a body for this function that builds the Rendering 
	representation of the \a processing data.
	If the builds fails, it should return a NULL pointer.
	*/
	virtual void Update( ) = 0;

	//! Check the input
	bool CheckInput( );

protected:
	//!
	RenDataBuilder(void);;
	
	//!
	virtual ~RenDataBuilder(void);;

protected:

	coreDeclareNoCopyConstructors(RenDataBuilder);

	//!
	boost::any m_RenderingData;
};

}

#endif
