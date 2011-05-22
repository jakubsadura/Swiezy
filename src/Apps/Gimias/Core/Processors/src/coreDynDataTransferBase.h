/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDynDataTransferBase_H
#define _coreDynDataTransferBase_H

// CoreLib
#include "gmProcessorsWin32Header.h"
#include "coreDynProcessor.h"

namespace Core{

/**
\brief Use ModuleDescription to call a filter of a Command Line Plugin
as an executable.

The inputs/outputs passed to the plugin through disk.

\ingroup gmKernel
\author Xavi Planes
\date 14 07 2010
*/
class GMPROCESSORS_EXPORT DynDataTransferBase : public Core::SmartPointerObject
{
	typedef std::map<std::string, DynDataTransferBase::Pointer> DataTransferMapType;

public:
	//!
	coreDeclareSmartPointerTypesMacro(DynDataTransferBase,Core::SmartPointerObject)
	coreClassNameMacro(DynDataTransferBase)

	//! Register a DynDataTransferBase
	static void RegisterDataTransfer(const std::string &name, DynDataTransferBase::Pointer);

	//! UnRegister a DynDataTransferBase
	static void UnRegisterDataTransfer(const std::string &name);

	//!
	static DynDataTransferBase::Pointer NewDataTransfer( const std::string &name );

	//!
	DynProcessor::Pointer GetDynProcessor() const;
	void SetDynProcessor(DynProcessor::Pointer val);

	//!
	void UpdateAllModuleParameters();

	//!
	virtual void UpdateAllOutputs();

	//!
	std::string GetTemporaryDirectory() const;
	void SetTemporaryDirectory(std::string val);

	//!
	virtual void CleanTemporaryFiles( );

protected:
	//!
	DynDataTransferBase(void);

	//!
	virtual ~DynDataTransferBase(void);

	//! Find input data entity with reference tag name
	Core::DataEntity::Pointer FindInputReferenceData( ModuleParameter* param );


	/** Update module parameter value 
	Depending on the type of Module, the string will be a:
	- Raw pointer to the object
	- Temporary filename on disk
	\param [in] param Parameter to modify
	\param [in] num Number of input/output relative to this processor
	*/
	virtual void UpdateModuleParameter( ModuleParameter* param, int num ) = 0;

	//!
	virtual void UpdateProcessorOutput( ModuleParameter* param, int num ) = 0;


private:
	//!
	DynDataTransferBase( const Self& );

	//!
	void operator=(const Self&);

protected:

	//!
	DynProcessor::Pointer m_DynProcessor;
	//! 
	std::string m_TemporaryDirectory;
	//!
	static DataTransferMapType m_DataTransferMap;
};

} // namespace Core{


#endif // _coreDynDataTransferBase_H


