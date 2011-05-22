/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreMultipleDataEntityImpl_H
#define _coreMultipleDataEntityImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"

namespace Core{

/**
Set of DataEntityImpl
\author Xavi Planes
\date 06 Sept 2010
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT MultipleDataEntityImpl : public DataEntityImpl
{
public:
	coreDeclareSmartPointerClassMacro( MultipleDataEntityImpl, DataEntityImpl )

	coreDefineMultipleDataFactory( MultipleDataEntityImpl, DataEntityImplSetType, UnknownTypeId )

public:

	//@{ 
	/// \name Interface
	virtual size_t GetSize() const;
	virtual DataEntityImpl::Pointer GetAt( size_t num );
	virtual bool SetAt( 
		size_t pos, 
		DataEntityImpl::Pointer data,
		ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool SwitchImplementation( const std::type_info &type );

	virtual boost::any GetDataPtr() const;
	virtual void ResetData( );

private:
	virtual void SetAnyData( boost::any val );
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );
	//@}


protected:
	//!
	MultipleDataEntityImpl( );

	//!
	virtual ~MultipleDataEntityImpl( );

protected:

	//! Set of MultipleDataEntityImpl
	DataEntityImplSetType m_Data;
	//! Factory for each time step
	DataEntityImplFactory::Pointer m_Factory;
};

} // end namespace Core

#endif // _coreMultipleDataEntityImpl_H

