/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreMitkImageImpl_H
#define _coreMitkImageImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "mitkImage.h"

namespace Core{

/**
Set of DataEntityImpl
\author Xavi Planes
\date 06 Sept 2010
\ingroup gmDataHandling
*/
class PLUGIN_EXPORT MitkImageImpl : public Core::DataEntityImpl
{
public:
	coreDeclareSmartPointerClassMacro( MitkImageImpl, DataEntityImpl )

	coreDefineMultipleDataFactory( MitkImageImpl, mitk::Image::Pointer, ImageTypeId )


public:

	//@{ 
	/// \name Interface

	virtual size_t GetSize() const;
	virtual void SetSize(size_t size, DataEntityImpl::Pointer data) const;
	virtual DataEntityImpl::Pointer GetAt( size_t num );
	virtual bool SetAt( 
		size_t pos, 
		DataEntityImpl::Pointer data,
		ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool SwitchImplementation( const std::type_info &type );

	virtual boost::any GetDataPtr() const;
	virtual void ResetData( );
	virtual void* GetVoidPtr( ) const;
	virtual void SetVoidPtr( void* ptr);

private:
	virtual void SetAnyData( boost::any val );
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );
	//@}


protected:
	//!
	MitkImageImpl( );

	//!
	virtual ~MitkImageImpl( );

protected:

	//!
	mitk::Image::Pointer m_Data;
	//! There's a bug in MITK
	mitk::Point3D m_Origin;
};

} // end namespace Core

#endif // _coreMitkImageImpl_H

