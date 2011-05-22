// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreITKDTITensorImpl.h"



Core::ITKDTITensorImpl::ITKDTITensorImpl( )
{
	ResetData();
}

Core::ITKDTITensorImpl::~ITKDTITensorImpl()
{

}

boost::any Core::ITKDTITensorImpl::GetDataPtr() const
{
	return m_Data;
}

void Core::ITKDTITensorImpl::ResetData()
{
	m_Data = itk::Image<ComponentType,3>::New( );
}

void Core::ITKDTITensorImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataType> ( val );
}
