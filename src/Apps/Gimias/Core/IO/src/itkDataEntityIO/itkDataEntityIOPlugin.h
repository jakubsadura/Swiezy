/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _itkDataEntityIOPlugin_H
#define _itkDataEntityIOPlugin_H

#include "itkObjectFactoryBase.h"
#include "gmItkDataEntityIOWin32Header.h"

/**
 * Routine that is called when the shared library is loaded by
 * itk::ObjectFactoryBase::LoadDynamicFactories().
 *
 * itkLoad() is C (not C++) function.
 */
extern "C" {
    GMITKDATAENTITYIO_EXPORT itk::ObjectFactoryBase* itkLoad();
} 

#endif  // _itkDataEntityIOPlugin_H
