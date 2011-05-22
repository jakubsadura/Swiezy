/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSignalImpl_H
#define _coreSignalImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "blSignalCollective.h"

namespace Core{

/**

Data entity for storing signals

\author Xavi Planes
\date 24 April 2009
\ingroup gmDataHandling
*/

class GMDATAHANDLING_EXPORT SignalImpl : public DataEntityImpl
{
public:
	typedef blSignalCollective::Pointer DataType;

public:
	coreDeclareSmartPointerClassMacro( SignalImpl, DataEntityImpl );

	coreDefineSingleDataFactory( SignalImpl, DataType, SignalTypeId )


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
	SignalImpl( );

	//!
	virtual ~SignalImpl();

	//! Not implemented
	SignalImpl(const Self&);

	//! Not implemented
   	void operator=(const Self&);

private:

	//!
	DataType m_Data;
};


}

#endif //_coreSignalImpl_H
