/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreContourImpl.h"

Core::ContourImpl::ContourImpl( )
{
	ResetData();
}

Core::ContourImpl::~ContourImpl()
{

}

boost::any Core::ContourImpl::GetDataPtr() const
{
	return m_Data;
}

void Core::ContourImpl::ResetData()
{
	m_Data.clear();
}

void Core::ContourImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataType> ( val );
}
