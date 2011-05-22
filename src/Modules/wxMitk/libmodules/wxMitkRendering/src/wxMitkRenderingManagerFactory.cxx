/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wxMitkRenderingManagerFactory.h"
#include "wxMitkRenderingManager.h"

using namespace mitk;

//!
wxMitkRenderingManagerFactory::wxMitkRenderingManagerFactory()
{
  mitk::RenderingManager::SetFactory( this );

}

//!
wxMitkRenderingManagerFactory::~wxMitkRenderingManagerFactory()
{
}

//!
mitk::RenderingManager::Pointer wxMitkRenderingManagerFactory::CreateRenderingManager() const
{
  wxMitkRenderingManager::Pointer specificSmartPtr = wxMitkRenderingManager::New();
  mitk::RenderingManager::Pointer smartPtr = specificSmartPtr.GetPointer();
  return smartPtr;
}

