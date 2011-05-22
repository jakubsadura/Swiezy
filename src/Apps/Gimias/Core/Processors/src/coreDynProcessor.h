/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDynProcessor_H
#define _coreDynProcessor_H

// CoreLib
#include "gmProcessorsWin32Header.h"
#include "coreBaseProcessor.h"
#include "dynModuleExecution.h"

namespace Core{

/**
\brief Use ModuleDescription to call a processing filter.

The main responsibility of this class is to pass DataEntities to the filter. 
Depending on the subclass, the data can be passed as:
- Raw pointer to data type (like vtkPolyData*)
- File written into Disk
- DataEntity pointer. There's a specific DataEntityIO class responsible of 
extracting the itk::Image from DataEntity

\ingroup gmKernel
\author Xavi Planes
\date 14 07 2010
*/
class GMPROCESSORS_EXPORT DynProcessor : public Core::BaseProcessor
{
public:
	//!
	coreDeclareSmartPointerClassMacro(DynProcessor,Core::BaseProcessor)

	//!
	void SetModule( ModuleDescription* module );

	//!
	ModuleDescription *GetModule( );

	//!
	virtual void Update( );

	//!
	dynModuleExecution::Pointer GetModuleExecution() const;

	//!
	std::string GetTemporaryDirectory() const;
	void SetTemporaryDirectory(std::string val);

	//!
	void SetWorkingDirectory(std::string val);

protected:
	//!
	DynProcessor(void);

	//!
	virtual ~DynProcessor(void);

private:
	//!
	DynProcessor( const Self& );

	//!
	void operator=(const Self&);



protected:
	//!
	dynModuleExecution::Pointer m_ModuleExecution;
	//! 
	std::string m_TemporaryDirectory;
};

} // namespace Core{


#endif // _coreDynProcessor_H


