/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef baseLibVTKPCH_h
#define baseLibVTKPCH_h

#include "BaseLibVTKWin32Header.h"

#include "CILabAssertMacros.h"
#include "CILabExceptionMacros.h"

#include "blNumericUtils.h"
#include "blPDShape.h"

#include "boost/intrusive_ptr.hpp"

#include "itkDefaultDynamicMeshTraits.h"
#include "itkMesh.h"
#include "itkPoint.h"
#include "itkTriangleCell.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "itksys/SystemTools.hxx"

#include "vtkAppendPolyData.h"
#include "vtkCardinalSpline.h"
#include "vtkCellArray.h"
#include "vtkCellLocator.h"
#include "vtkCleanPolyData.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkDelaunay2D.h"
#include "vtkFeatureEdges.h"
#include "vtkFloatArray.h"
#include "vtkGlyph3D.h"
#include "vtkGlyphSource2D.h"
#include "vtkIVWriter.h"
#include "vtkImageData.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageReader.h"
#include "vtkImageWriter.h"
#include "vtkLandmarkTransform.h"
#include "vtkLookupTable.h"
#include "vtkLoopSubdivisionFilter.h"
#include "vtkMassProperties.h"
#include "vtkMath.h"
#include "vtkObjectBase.h"
#include "vtkPointData.h"
#include "vtkPointLocator.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkSTLReader.h"
#include "vtkSTLWriter.h"
#include "vtkStructuredPoints.h"
#include "vtkStructuredPointsReader.h"
#include "vtkStructuredPointsWriter.h"
#include "vtkThreshold.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTriangleFilter.h"
#include "vtkTubeFilter.h"
#include "vtkType.h"
#include "vtkXMLImageDataReader.h"
#include "vtkXMLImageDataWriter.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"

#include <blLightObject.h>
#include <blMacro.h>
#include <blMath.h>
#include <blMatrix.h>
#include <blSmartPointer.h>
#include <blVector.h>

#include <boost/crc.hpp>
#include <boost/cstdint.hpp>

#include <exception>
#include <iostream>

#include <itkCastImageFilter.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIterator.h>

#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>

#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <vnl/vnl_cross_product_matrix.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_vector.h>

#include <vtkAppendPolyData.h>
#include <vtkDataArray.h>
#include <vtkDataObject.h>
#include <vtkDataSet.h>
#include <vtkFloatArray.h>
#include <vtkIdList.h>
#include <vtkImageData.h>
#include <vtkImageReader.h>
#include <vtkLookupTable.h>
#include <vtkPlane.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkProbeFilter.h>
#include <vtkShortArray.h>
#include <vtkType.h>

#endif // baseLibVTKPCH_h

