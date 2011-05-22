/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreITKDTITensorImpl_H
#define _coreITKDTITensorImpl_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataEntityImplFactory.h"
#include "itkDTITensor.h"
#include "itkDTITensor.txx"

namespace Core{

/**

Data entity for storing tensors 

\author Chiara Riccobene
\date 07 June 2010
\ingroup gmDataHandling
*/

class GMDATAHANDLING_EXPORT ITKDTITensorImpl : public DataEntityImpl
{
public:
	typedef itk::DTITensor<float> ComponentType;
	typedef itk::Image<ComponentType,3>::Pointer DataType;

public:

	coreDeclareSmartPointerClassMacro( ITKDTITensorImpl, DataEntityImpl );

	coreDefineSingleDataFactory( ITKDTITensorImpl, DataType, TensorTypeId )

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
	ITKDTITensorImpl( );

	//!
	virtual ~ITKDTITensorImpl();

	//! Not implemented
	ITKDTITensorImpl(const Self&);

	//! Not implemented
   	void operator=(const Self&);

private:
	//!
	DataType m_Data;

};


}

#endif //_coreITKDTITensorImpl_H
