/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityBuildersRegistration_H
#define coreDataEntityBuildersRegistration_H

#include "gmDataHandlingWin32Header.h"
#include "coreObject.h"

namespace Core{

/** 
DataEntityBuildersRegistration enables registering available 
DataEntityImpl to DataEntityImplFactory.

\ingroup gmDataHandling
\author: Jakub Lyko
\date: 5.06.2008
*/

class GMDATAHANDLING_EXPORT DataEntityBuildersRegistration : public Core::Object
{
public:
	//! Register all builders
	static void RegisterDataEntityBuilders();

	//! Register all builders
	static void UnRegisterDataEntityBuilders();

protected:

	//! 
	DataEntityBuildersRegistration();
	//! 
	~DataEntityBuildersRegistration();

};

} // namespace Core{

#endif
