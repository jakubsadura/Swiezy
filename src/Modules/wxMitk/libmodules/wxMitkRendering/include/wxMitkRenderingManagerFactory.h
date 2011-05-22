/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkRenderingManagerFactory_H
#define wxMitkRenderingManagerFactory_H

#include "wxMitkRenderingWin32Header.h"
#include <mitkRenderingManagerFactory.h>

namespace mitk
{

/**
\brief wxWidgets specific implementation of mitk::RenderingManagerFactory.

This class create wxMitkRenderingManager instances via #CreateRenderingManager.

A static instance of wxMitkRenderingManagerFactory is created in
wxRenderWindow, forcing the usage of wxMitkRenderingManager for the 
wxWidgets platform.

\ingroup wxMitkRendering
\author Juan Antonio Moya
\date 20 Sep 2007
 */
class WXMITKRENDERING_EXPORT wxMitkRenderingManagerFactory 
	: public RenderingManagerFactory
{

public:
  wxMitkRenderingManagerFactory(void);
  virtual ~wxMitkRenderingManagerFactory(void);
  virtual mitk::RenderingManager::Pointer CreateRenderingManager(void) const;

};

} // namespace mitk

#endif // wxMitkRenderingManagerFactory_H
