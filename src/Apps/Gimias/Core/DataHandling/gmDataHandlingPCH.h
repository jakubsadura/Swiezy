/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CILabAssertMacros.h"
#include "CILabExceptionMacros.h"
#include "CILabNamespaceMacros.h"
#include "blShapeUtils.h"
//#include "blSignal.h"
#include "blVTKHelperTools.h"
#include "blVTKUtils.h"
#include "coreAssert.h"
#include "coreCreateExceptionMacros.h"
#include "coreDataHolder.h"
#include "coreException.h"
#include "coreObject.h"
#include "itkImage.h"
#include "itkImageIOBase.h"
#include "itkSmartPointer.h"
#include "itkVTKImageToImageFilter.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkGeometryFilter.h"
#include "vtkImageData.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkUnstructuredGrid.h"
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals.hpp>
#include <itkCastImageFilter.h>
#include <itkImage.h>
#include <itkTransformFileReader.h>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
