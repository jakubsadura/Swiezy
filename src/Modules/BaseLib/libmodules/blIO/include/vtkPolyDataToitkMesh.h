/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __vtkPolyDataToitkMesh_h__
#define __vtkPolyDataToitkMesh_h__

#include "BaseLibVTKWin32Header.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "itkDefaultDynamicMeshTraits.h"
#include "itkMesh.h"
#include "itkTriangleCell.h"


/** 
  \class vtkPolyDataToitkMesh
  \brief 
    \warning
  \sa 
  */

class BASELIBVTK_EXPORT vtkPolyDataToitkMesh
{

 public:

  vtkPolyDataToitkMesh( void );
  virtual ~vtkPolyDataToitkMesh( void );

  typedef itk::DefaultDynamicMeshTraits<double, 3, 3,double,double> TriangleMeshTraits;
  typedef itk::Mesh<double,3, TriangleMeshTraits> TriangleMeshType;

  /**
  The SetInput method provides pointer to the vtkPolyData
  */
  void SetInput( vtkPolyData * polydata);
  TriangleMeshType * GetOutput();
  void ConvertvtkToitk();

  TriangleMeshType::Pointer  m_itkMesh;

  vtkPolyData           * m_PolyData;

  
};

#endif
