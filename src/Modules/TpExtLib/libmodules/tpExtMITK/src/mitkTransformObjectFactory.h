/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _mitkTransformObjectFactory_H
#define _mitkTransformObjectFactory_H

#include "TpExtLibMITKWin32Header.h"
#include "mitkCoreObjectFactory.h"

namespace mitk {

class TPEXTLIBMITK_EXPORT TransformObjectFactory : public CoreObjectFactoryBase
{
  public:
    mitkClassMacro(TransformObjectFactory,CoreObjectFactoryBase);
    itkNewMacro(TransformObjectFactory);
    virtual Mapper::Pointer CreateMapper(mitk::DataTreeNode* node, MapperSlotId slotId);
    virtual void SetDefaultProperties(mitk::DataTreeNode* node);
    virtual const char* GetFileExtensions();
    virtual const char* GetSaveFileExtensions();
    virtual itk::Object::Pointer CreateCoreObject( const std::string& className );
    void RegisterIOFactories();
  protected:
    TransformObjectFactory(bool registerSelf = true); 
  private:
    std::string m_ExternalFileExtensions;
    std::string m_InternalFileExtensions;
    std::string m_SaveFileExtensions;
};

}
// global declaration for simple call by
// applications
void TPEXTLIBMITK_EXPORT RegisterTransformObjectFactory();

#endif // _mitkTransformObjectFactory_H

