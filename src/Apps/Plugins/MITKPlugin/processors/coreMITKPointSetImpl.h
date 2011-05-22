/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreMITKPointSetImpl_H
#define _coreMITKPointSetImpl_H

#include "corePluginMacros.h"
#include "coreDataEntityImplFactory.h"
#include "mitkPointSet.h"

namespace Core
{
/** 
\brief MITK PointSet data

\ingroup gmDataHandling
\author: Xavi Planes
\date: 09 Sept 2010
*/
class PLUGIN_EXPORT MitkPointSetImpl : public DataEntityImpl
{
public:
	coreDeclareSmartPointerClassMacro( MitkPointSetImpl, DataEntityImpl )

	coreDefineSingleDataFactory( MitkPointSetImpl, mitk::PointSet::Pointer, PointSetTypeId )

	//@{ 
	/// \name Interface
public:
	boost::any GetDataPtr() const;

private:
	void SetAnyData( boost::any val );
	virtual void ResetData( );
	//@}

protected:
	//!
	MitkPointSetImpl();

	//!
	virtual ~MitkPointSetImpl();

private:
	//!
	mitk::PointSet::Pointer	m_Data;
};

}

#endif // _coreMITKPointSetImpl_H
