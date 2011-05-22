/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DcmAPIPCH_H
#define DcmAPIPCH_H

// stdio
#include <fstream>
#include <string>
#include <vector>
// Generic macros
#include <CILabBoostMacros.h>
#include <CILabExceptionMacros.h>
// Baselib
#include <blImageUtils.h>
#include <blLightObject.h>
#include <blSignalCollective.h>
#include <blTextUtils.h>
#include <blITKImageUtils.h>
#include <blITKImagesDefines.h>
#include <blVTKHelperTools.h>
// gdcm
#include <gdcmDocEntry.h>
#include <gdcmUtil.h>
#include <gdcmValEntry.h>
// itk
#include <itkExtractImageFilter.h>
#include <itkGDCMImageIO.h>
#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
#include <itkOrientImageFilter.h>
// vtk
#include <vtkImageData.h>
#include <vtkImageFlip.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>


#endif //DcmAPIPCH_H
