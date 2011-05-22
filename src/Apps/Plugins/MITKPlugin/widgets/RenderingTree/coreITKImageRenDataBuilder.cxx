/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreITKImageRenDataBuilder.h"
#include "coreITKImageRenDataBuilder.txx"
#include "coreAssert.h"
#include <mitkITKImageImport.h>
#include <typeinfo>

using namespace Core;

//!
ITKImageRenDataBuilder::ITKImageRenDataBuilder(void)
{
}

//!
ITKImageRenDataBuilder::~ITKImageRenDataBuilder(void)
{
}

//!
void ITKImageRenDataBuilder::Update( )
{
	// Get input
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );

	if ( !CheckInput( ) )
	{
		return;
	}

	mitk::BaseData::Pointer data;
	data = BuildSingleTimeStep( dataEntity->GetTimeStep( 0 ) );
	dataEntity->SetRenderingData( data );
}

mitk::BaseData::Pointer Core::ITKImageRenDataBuilder::BuildSingleTimeStep(
	const Core::DataEntityImpl::Pointer timeStep)
{
	if(timeStep->GetDataPtr().empty())
		return mitk::BaseData::Pointer();

	const std::type_info* typeID = &timeStep->GetDataPtr().type();
	mitk::Image::Pointer result;

	//////////////////////////////////////////////////////////////////////////////
	// Now match the processingData type with one of the many possible image types
	//////////////////////////////////////////////////////////////////////////////
	if(*typeID == typeid(itk::Image<char, 2>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<char, 2> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<char, 3>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<char, 3> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<char, 4>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<char, 4> >(timeStep->GetDataPtr());

	else if(*typeID == typeid(itk::Image<unsigned char, 2>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<unsigned char, 2> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<unsigned char, 3>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<unsigned char, 3> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<unsigned char, 4>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<unsigned char, 4> >(timeStep->GetDataPtr());

	else if(*typeID == typeid(itk::Image<int, 2>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<int, 2> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<int, 3>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<int, 3> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<int, 4>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<int, 4> >(timeStep->GetDataPtr());

	else if(*typeID == typeid(itk::Image<unsigned int, 2>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<unsigned int, 2> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<unsigned int, 3>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<unsigned int, 3> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<unsigned int, 4>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<unsigned int, 4> >(timeStep->GetDataPtr());

	else if(*typeID == typeid(itk::Image<short, 2>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<short, 2> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<short, 3>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<short, 3> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<short, 4>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<short, 4> >(timeStep->GetDataPtr());

	else if(*typeID == typeid(itk::Image<unsigned short, 2>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<unsigned short, 2> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<unsigned short, 3>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<unsigned short, 3> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<unsigned short, 4>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<unsigned short, 4> >(timeStep->GetDataPtr());

	else if(*typeID == typeid(itk::Image<float, 2>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<float, 2> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<float, 3>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<float, 3> >(timeStep->GetDataPtr());
	else if(*typeID == typeid(itk::Image<float, 4>::Pointer))
		result = this->GetMitkImageFromItkImage<itk::Image<float, 4> >(timeStep->GetDataPtr());

	////////////////////////////////////////////////////////////////////////

	return mitk::BaseData::Pointer(result);
}
