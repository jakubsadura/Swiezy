/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DICOMPLUGINPCH_H
#define DICOMPLUGINPCH_H

#include "CILabNamespaceMacros.h"

#include "coreException.h"
#include "coreFrontEndPlugin.h"
#include "coreReportExceptionMacros.h"

#include "wxMitkSelectableGLWidget.h"

#include "dcmDataSet.h"

#include "itkImageSeriesReader.h"
#include "itkImageToVTKImageFilter.h"

#include "mitkITKImageImport.h"

#include "vtkImageFlip.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/any.hpp>

#include <itkExtractImageFilter.h>
#include <itkImageToVTKImageFilter.h>

#include <iostream>
#include <string>

#include <vtkImageClip.h>
#include <vtkImageFlip.h>
#include <vtkImageReslice.h>
#include "vtkImageData.h"
#include "vtkImageFlip.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkImageData.h"


#include "wxID.h"
#include <wx/datectrl.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/notebook.h>
#include <wx/process.h>
#include <wx/treectrl.h>
#include <wx/wx.h>
#include <wx/wxprec.h>
#include "wx/busyinfo.h"
#include "wx/dynarray.h"
#include "wx/imaglist.h"

#include "mitkDataStorage.h"
#include "mitkDataTree.h"
#include "mitkITKImageImport.h"
#include "mitkImage.h"
#include "mitkLevelWindowManager.h"
#include "mitkSliceNavigationController.h"

#include "itkAffineTransform.h"
#include "itkChangeInformationImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageSeriesReader.h"
#include "itkOrientImageFilter.h"
#include "itkResampleImageFilter.h"

#include "dcmDataSet.h"
#include "dcmDataSetReader.h"
#include "dcmFile.h"
#include "dcmIOUtils.h"
#include "dcmImageUtilities.h"
#include "dcmMultiSliceReader.h"
#include "dcmPACSQueryFileReader.h"
#include "dcmSearch.h"
#include "dcmTypes.h"

#include "pacsAPIDcmtkInclude.h"

#endif //DICOMPLUGINPCH_H
