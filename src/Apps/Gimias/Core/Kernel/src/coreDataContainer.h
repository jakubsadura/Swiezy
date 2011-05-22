/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataContainer_H
#define coreDataContainer_H

#include "coreObject.h"
#include "coreDataHolder.h"
#include "coreDataEntity.h"
#include "coreDataEntityList.h"

namespace Core{
/** 
\brief This class contains data shared by all plugins: DataEntityList.

\sa Core::Runtime::Kernel
\note You should access and get the instance for this class through the 
runtime Kernel
\ingroup gmKernel
\author Juan Antonio Moya
\date 23 Jan 2008
*/
class GMKERNEL_EXPORT DataContainer : public Core::SmartPointerObject
{
public:
	
	coreDeclareSmartPointerClassMacro(DataContainer, Core::SmartPointerObject);
	
	//!
	DataEntityList::Pointer GetDataEntityList(void) const;

protected:
	DataContainer(void);
	virtual ~DataContainer(void);

private:
	//!
	DataEntityList::Pointer m_DataEntityList;
	
	coreDeclareNoCopyConstructors(DataContainer);
};

} //end namesapce Core

#endif


