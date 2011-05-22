/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMITKPointSetImpl.h"

using namespace Core;

MitkPointSetImpl::MitkPointSetImpl()
{
	ResetData();
}

MitkPointSetImpl::~MitkPointSetImpl()
{
}

boost::any Core::MitkPointSetImpl::GetDataPtr() const
{
	return m_Data;
}

void Core::MitkPointSetImpl::ResetData()
{
	m_Data = mitk::PointSet::New( );
}

void Core::MitkPointSetImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<mitk::PointSet::Pointer> ( val );
}
