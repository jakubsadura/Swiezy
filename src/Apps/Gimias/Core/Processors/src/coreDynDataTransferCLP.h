/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDynDataTransferBaseCLP_H
#define _coreDynDataTransferBaseCLP_H

// CoreLib
#include "gmProcessorsWin32Header.h"
#include "coreDynDataTransferBase.h"
#include "dynModuleExecution.h"
#include <set>

namespace Core{

/**
\brief Use ModuleDescription to call a filter of a Command Line Plugin
as an executable.

The inputs/outputs passed to the plugin through disk.

\ingroup gmKernel
\author Xavi Planes
\date 14 07 2010
*/
class GMPROCESSORS_EXPORT DynDataTransferCLP : public Core::DynDataTransferBase
{
	//!
	typedef std::map<int, std::string> DataEntityToFileNameMap;
public:
	//!
	coreDeclareSmartPointerClassMacro(DynDataTransferCLP, Core::DynDataTransferBase)

	//!
	virtual void UpdateModuleParameter( ModuleParameter* param, int num );

	//!
	virtual void UpdateProcessorOutput( ModuleParameter* param, int num );

protected:
	//!
	DynDataTransferCLP(void);

	//!
	virtual ~DynDataTransferCLP(void);

	//!
	virtual void CleanTemporaryFiles();

	/** Search files that doesn't exist but the multiple time steps files exist
	Return a file list separated by ;
	*/
	std::string SearchMultipleTimeSteps( const std::string &filename );


private:
	//!
	DynDataTransferCLP( const Self& );

	//!
	void operator=(const Self&);

protected:
	//!
	std::set<std::string> m_FilesToDelete;
};

} // namespace Core{


#endif // _coreDynDataTransferBaseCLP_H


