/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSliceImageImpl_H
#define _coreSliceImageImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "blSliceImage.h"

namespace Core{

/**

Data entity for storing images defined by slices and not volumes

\author Chiara Riccobene
\date 11 Aug 2009
\ingroup gmDataHandling
*/

class GMDATAHANDLING_EXPORT SliceImageImpl : public DataEntityImpl
{
public:
	typedef blSliceImage::Pointer DataType;

	coreDeclareSmartPointerClassMacro( SliceImageImpl, DataEntityImpl );

	coreDefineSingleDataFactory( SliceImageImpl, DataType, ImageTypeId )

	//@{ 
	/// \name Interface
public:
	boost::any GetDataPtr() const;

private:
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );
	virtual void SetAnyData( boost::any val );
	virtual void ResetData( );
	//@}

protected:
	//!
	SliceImageImpl( );

	//!
	virtual ~SliceImageImpl();

	//! Not implemented
	SliceImageImpl(const Self&);

	//! Not implemented
   	void operator=(const Self&);

private:
	//!
	DataType m_Data;
};


}

#endif //_coreSliceImageImpl_H
