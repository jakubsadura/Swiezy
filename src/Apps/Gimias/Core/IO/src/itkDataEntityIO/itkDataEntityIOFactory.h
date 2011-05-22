/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __itkDataEntityIOFactory_h
#define __itkDataEntityIOFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

#include "itkDataEntityIO.h"

#include "gmItkDataEntityIOWin32Header.h"

namespace itk
{
/** \class DataEntityIOFactory
 * \brief Create instances of MRMLIDImageIO objects using an object factory.
 */
class GMITKDATAENTITYIO_EXPORT DataEntityIOFactory : public ObjectFactoryBase
{
public:  
  /** Standard class typedefs. */
  typedef DataEntityIOFactory   Self;
  typedef ObjectFactoryBase  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;
  
  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DataEntityIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    m_Factory = DataEntityIOFactory::New();
    ObjectFactoryBase::RegisterFactory(m_Factory);
  }

  //!
  static void UnRegisterOneFactory(void)
  {
	  ObjectFactoryBase::UnRegisterFactory(m_Factory);
  }

protected:
  DataEntityIOFactory();
  ~DataEntityIOFactory();

private:
  DataEntityIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  static DataEntityIOFactory::Pointer m_Factory;
};
  
  
} /// end namespace itk

#endif
