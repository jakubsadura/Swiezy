/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreNumericImpl_H
#define _coreNumericImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "blTagMap.h"


namespace Core{

/**

Data entity for storing numeric results (i.e. a blTagMap) or a vector of numeric results

\author Martin Bianculli
\date Sept 2009
\ingroup gmDataHandling
\sa blTagMap coreNumericDataEntityBuilder
*/

class GMDATAHANDLING_EXPORT NumericImpl : public DataEntityImpl
{
public:
	typedef blTagMap::Pointer DataType;

public:
	coreDeclareSmartPointerClassMacro( NumericImpl, DataEntityImpl );

	coreDefineSingleDataFactory( NumericImpl, DataType, NumericDataTypeId )


	//@{ 
	/// \name Interface
public:
	boost::any GetDataPtr() const;

private:
	virtual bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	virtual bool GetData( blTagMap::Pointer tagMap );
	void SetAnyData( boost::any val );
	virtual void ResetData( );
	//@}

protected:
	//!
	NumericImpl( );

	//!
	virtual ~NumericImpl();

	//! Not implemented
	NumericImpl(const Self&);

	//! Not implemented
   	void operator=(const Self&);

private:

	//!
	DataType m_Data;
};


}

#endif //_coreNumericImpl_H
