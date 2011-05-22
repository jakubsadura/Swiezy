/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreITKTransformImpl_H
#define _coreITKTransformImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "itkTransformBase.h"

namespace Core
{
/** 

std::list<itk::TransformBase::Pointer>

\ingroup gmDataHandling
\author: Martin Bianculli
\date: 30 Oct 2008
*/

class GMDATAHANDLING_EXPORT ITKTransformImpl : public DataEntityImpl
{
public:

	typedef itk::TransformBase				TransformType;
	typedef TransformType::Pointer			TransformPointer;

	coreDeclareSmartPointerClassMacro( ITKTransformImpl , DataEntityImpl)

	coreDefineSingleDataFactory( ITKTransformImpl, TransformPointer, TransformId )

	//@{ 
	/// \name Interface
public:
	boost::any GetDataPtr() const;
	bool SetData( blTagMap::Pointer tagMap, ImportMemoryManagementType mem = gmCopyMemory );
	bool GetData( blTagMap::Pointer tagMap );

private:
	virtual void SetAnyData( boost::any val );
	virtual void ResetData( );
	//@}


protected:

	//!
	ITKTransformImpl ();

	//!
	~ITKTransformImpl ();

private:
	//!
	TransformPointer m_Data;

};

}

#endif
