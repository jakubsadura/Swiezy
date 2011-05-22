/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDynDataTransferCLPCLPShared_H
#define _coreDynDataTransferCLPCLPShared_H

// CoreLib
#include "gmProcessorsWin32Header.h"
#include "coreDynDataTransferCLP.h"
#include "dynModuleExecution.h"

namespace Core{

/**
\brief Use ModuleDescription to call a filter of a Command Line Plugin
as a shared library.

To pass images to the plugin, it uses as filename "DataEntity:%p" with a 
pointer to the input DataEntity. The itk::DataEntityIO will be used to 
extract the itk::Image needed by the plugin.

To retrieve images from the plugin, it uses m_OutputDataEntities that
is created before calling the filter. After the filter finishes, the 
processing data is set as output of this processor.

For the rest of data types, it uses the parent class DynDataTransferCLP.

\ingroup gmKernel
\author Xavi Planes
\date 14 07 2010
*/
class GMPROCESSORS_EXPORT DynDataTransferCLPShared : public Core::DynDataTransferCLP
{
	//!
	typedef std::map<int, Core::DataEntity::Pointer> OutputNumToDataEntityMap;

public:
	//!
	coreDeclareSmartPointerClassMacro(DynDataTransferCLPShared, Core::DynDataTransferCLP)

	//!
	virtual void UpdateModuleParameter( ModuleParameter* param, int num );

	//!
	virtual void UpdateProcessorOutput( ModuleParameter* param, int num );

protected:
	//!
	DynDataTransferCLPShared(void);

	//!
	virtual ~DynDataTransferCLPShared(void);

	//!
	void CleanTemporaryFiles();

private:
	//!
	DynDataTransferCLPShared( const Self& );

	//!
	void operator=(const Self&);

protected:

	//!
	OutputNumToDataEntityMap m_OutputDataEntities;
};

} // namespace Core{


#endif // _coreDynProcessorCLPShared_H


