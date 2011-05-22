/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDynDataTransferBaseDLL_H
#define _coreDynDataTransferBaseDLL_H

// CoreLib
#include "gmProcessorsWin32Header.h"
#include "coreDynDataTransferBase.h"
#include "dynModuleExecution.h"

namespace Core{

/**
\brief Use ModuleDescription to call a filter of a dynamic library.

Use the tag "DataType" of the XML description to check the data type
of the processing data. 

\ingroup gmKernel
\author Xavi Planes
\date 14 07 2010
*/
class GMPROCESSORS_EXPORT DynDataTransferDLL : public Core::DynDataTransferBase
{
public:
	//!
	coreDeclareSmartPointerClassMacro(DynDataTransferDLL, Core::DynDataTransferBase)

	//!
	virtual void UpdateModuleParameter( ModuleParameter* param, int num );

	//!
	virtual void UpdateProcessorOutput( ModuleParameter* param, int num );

protected:
	//!
	DynDataTransferDLL(void);

	//!
	virtual ~DynDataTransferDLL(void);

private:
	//!
	DynDataTransferDLL( const Self& );

	//!
	void operator=(const Self&);

protected:
};

} // namespace Core{


#endif // _coreDynDataTransferBaseDLL_H


