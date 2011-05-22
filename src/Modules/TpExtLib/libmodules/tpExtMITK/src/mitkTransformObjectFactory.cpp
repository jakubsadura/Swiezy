/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "mitkTransformObjectFactory.h"
#include "mitkTransformMapper2D.h"

#include "mitkProperties.h"
#include "mitkBaseRenderer.h"
#include "mitkDataTreeNode.h"

mitk::TransformObjectFactory::TransformObjectFactory(bool registerSelf) 
:CoreObjectFactoryBase()
{
  static bool alreadyDone = false;
  if (!alreadyDone)
  {
    mitk::CoreObjectFactory::GetInstance()->RegisterExtraFactory(this);
    
    alreadyDone = true;
  }

  if (registerSelf) 
  {
    this->RegisterOverride("mitkCoreObjectFactoryBase",
      "mitkTransformObjectFactory",
      "mitk Mapper Creation",
      1,
      itk::CreateObjectFunction<mitk::TransformObjectFactory>::New());
  }
}

#define CREATE_CPP( TYPE, NAME ) else if ( className == NAME ) {pointer = new TYPE(); pointer->Register();}
#define CREATE_ITK( TYPE, NAME ) else if ( className == NAME ) pointer = TYPE::New();

itk::Object::Pointer mitk::TransformObjectFactory::CreateCoreObject( const std::string&  /*className*/ )
{
  itk::Object::Pointer pointer;
  return pointer;
}


mitk::Mapper::Pointer mitk::TransformObjectFactory::CreateMapper(mitk::DataTreeNode* node, MapperSlotId id) 
{
  mitk::Mapper::Pointer newMapper=NULL;

  if ( id == mitk::BaseRenderer::Standard2D )
  {
    std::string classname("Transform");
    if(node->GetData() && classname.compare(node->GetData()->GetNameOfClass())==0)
    {
      newMapper = mitk::TransformMapper2D::New();
      newMapper->SetDataTreeNode(node);
    }
  }
  else if ( id == mitk::BaseRenderer::Standard3D )
  {
    // do nothing
  }

  return newMapper;
}

void mitk::TransformObjectFactory::SetDefaultProperties(mitk::DataTreeNode* node)
{
  std::string classname = "Transform";
  if(node->GetData() && classname.compare(node->GetData()->GetNameOfClass())==0)
  {
    mitk::TransformMapper2D::SetDefaultProperties(node);
  }

}

const char* mitk::TransformObjectFactory::GetFileExtensions() 
{
  return m_ExternalFileExtensions.c_str();
};

const char* mitk::TransformObjectFactory::GetSaveFileExtensions() 
{ 
  return m_SaveFileExtensions.c_str();
};

void mitk::TransformObjectFactory::RegisterIOFactories() 
{
}

void RegisterTransformObjectFactory() 
{
  bool oneObjectFactoryRegistered = false;
  if ( ! oneObjectFactoryRegistered ) {
    LOG_INFO << "Registering TransformObjectFactory..." << std::endl;
    itk::ObjectFactoryBase::RegisterFactory(mitk::TransformObjectFactory::New());
    oneObjectFactoryRegistered = true;
  }
}
