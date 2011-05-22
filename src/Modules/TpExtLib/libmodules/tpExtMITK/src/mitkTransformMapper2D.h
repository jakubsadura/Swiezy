/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-05-13 18:06:46 +0200 (Mi, 13 Mai 2009) $
Version:   $Revision: 17258 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef _mitkTransformMapper2D_H
#define _mitkTransformMapper2D_H

#include "TpExtLibMITKWin32Header.h"
#include "mitkCommon.h"
#include "mitkGLMapper2D.h"
#include "mitkDataTree.h"


namespace mitk {

class BaseRenderer;
class Contour;


/** 
 * \brief OpenGL-based mapper to render display sub-class instances of
 * mitk::PlanarFigure
 * 
 * 
 * \ingroup Mapper
 */
class TPEXTLIBMITK_EXPORT TransformMapper2D : public GLMapper2D
{
public:
    
  mitkClassMacro(TransformMapper2D, Mapper2D);

  itkNewMacro(Self);

  /**
  * reimplemented from Baseclass
  */
  virtual void Paint(BaseRenderer * renderer);
  static void SetDefaultProperties(mitk::DataTreeNode* node, mitk::BaseRenderer* renderer = NULL, bool overwrite = false);
protected:
  TransformMapper2D();

  virtual ~TransformMapper2D();

};

} // namespace mitk



#endif // _mitkTransformMapper2D_H
