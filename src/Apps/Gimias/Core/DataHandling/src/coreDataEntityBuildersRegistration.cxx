/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityBuildersRegistration.h"

#include "coreDataEntityImplFactory.h"
#include "coreMultipleDataEntityImpl.h"
#include "coreVTKPolyDataImpl.h"
#include "coreVTKImageDataImpl.h"
#include "coreITKImageImpl.h"
#include "coreVTKUnstructuredGridImpl.h"
#include "coreITKTransformImpl.h"
#include "coreSignalImpl.h"
#include "coreContourImpl.h"
#include "coreSliceImageImpl.h"
#include "coreNumericImpl.h"
#include "coreITKDTITensorImpl.h"

using namespace Core;

DataEntityBuildersRegistration::DataEntityBuildersRegistration()
{
}

DataEntityBuildersRegistration::~DataEntityBuildersRegistration()
{
}


void DataEntityBuildersRegistration::RegisterDataEntityBuilders()
{
	DataEntityImplFactory::RegisterFactory( VtkPolyDataImpl::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( VtkImageDataImpl::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( MultipleDataEntityImpl::Factory::NewBase() );

	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<char,3> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<unsigned char,3> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<int,3> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<unsigned int,3> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<short,3> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<unsigned short,3> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<float,3> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<double,3> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<long,3> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<unsigned long,3> >::Factory::NewBase() );

	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<char,2> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<unsigned char,2> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<int,2> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<unsigned int,2> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<short,2> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<unsigned short,2> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<float,2> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<double,2> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<long,2> >::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKImageImpl<itk::Image<unsigned long,2> >::Factory::NewBase() );

	DataEntityImplFactory::RegisterFactory( VtkUnstructuredGridImpl::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKTransformImpl::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( SignalImpl::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ContourImpl::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( SliceImageImpl::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( NumericImpl::Factory::NewBase() );
	DataEntityImplFactory::RegisterFactory( ITKDTITensorImpl::Factory::NewBase() );
}

void Core::DataEntityBuildersRegistration::UnRegisterDataEntityBuilders()
{
	DataEntityImplFactory::UnRegisterAll();
}
