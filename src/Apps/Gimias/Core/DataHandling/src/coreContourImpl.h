/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreContourImpl_H
#define _coreContourImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"

namespace Core{

/**

Data entity for storing contours

\author Xavi Planes
\date 24 April 2009
\ingroup gmDataHandling
*/

class GMDATAHANDLING_EXPORT ContourImpl : public DataEntityImpl
{
public:
	typedef std::vector<float> DataType;

public:
	coreDeclareSmartPointerClassMacro( ContourImpl, DataEntityImpl );

	coreDefineSingleDataFactory( ContourImpl, DataType, ContourTypeId )

	//@{ 
	/// \name Interface
public:
	boost::any GetDataPtr() const;

private:
	virtual void SetAnyData( boost::any val );
	virtual void ResetData( );
	//@}


protected:
	//!
	ContourImpl( );

	//!
	virtual ~ContourImpl();

	//! Not implemented
	ContourImpl(const Self&);

	//! Not implemented
   	void operator=(const Self&);

private:
	//!
	DataType m_Data;

};


}

#endif //_coreContourImpl_H
