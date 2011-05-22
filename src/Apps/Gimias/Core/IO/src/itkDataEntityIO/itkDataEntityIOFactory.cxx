/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "itkDataEntityIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkDataEntityIO.h"
#include "itkVersion.h"

  
namespace itk
{
DataEntityIOFactory::Pointer DataEntityIOFactory::m_Factory;

DataEntityIOFactory::DataEntityIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
                         "itkDataEntityIO",
                         "ImageIO to communicate directly with a DataEntity.",
                         1,
                         CreateObjectFunction<DataEntityIO>::New());
}
  
DataEntityIOFactory::~DataEntityIOFactory()
{
}

const char* 
DataEntityIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* 
DataEntityIOFactory::GetDescription() const
{
  return "ImageIOFactory that imports/exports data to a DataEntity.";
}

} // end namespace itk

