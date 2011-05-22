/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomPluginTypes_H
#define DicomPluginTypes_H

/** 
This file contains all helpfull types used in the dicom plugin

\ingroup DicomPlugin
\author Jakub Lyko
\date 07 April 2008
*/

#include "CILabNamespaceMacros.h"
#include "coreDataEntity.h"
#include "coreDataHolder.h"
#include "coreVTKImageDataHolder.h"

#include "vtkImageData.h"
#include "itkImage.h"

#include "blSignalCollective.h"

namespace DicomPlugin{

typedef Core::DataEntity::Pointer DataEntityPtr;
typedef Core::DataHolder< DataEntityPtr > DataEntityHolder;
typedef std::vector< Core::vtkImageDataPtr > VtkImageVector;
typedef itk::Image <unsigned char, 2> uch2DItkImage;
typedef itk::Image <char, 2> ch2DItkImage;
typedef itk::Image <unsigned short, 2> ush2DItkImage;
typedef itk::Image <short, 2> sh2DItkImage;
typedef itk::Image <unsigned int, 2> uint2DItkImage;
typedef itk::Image <int, 2> int2DItkImage;
typedef itk::Image <float, 2> float2DItkImage;
typedef itk::Image <double, 2> double2DItkImage;
typedef itk::Image <unsigned char, 3> uch3DItkImage;
typedef itk::Image <char, 3> ch3DItkImage;
typedef itk::Image <unsigned short, 3> ush3DItkImage;
typedef itk::Image <short, 3> sh3DItkImage;
typedef itk::Image <unsigned int, 3> uint3DItkImage;
typedef itk::Image <int, 3> int3DItkImage;
typedef itk::Image <float, 3> float3DItkImage;
typedef itk::Image <double, 3> double3DItkImage;

struct SeriesCollective
{
	//!
	std::vector < float > m_timestamps;
	//!
	VtkImageVector m_images;
	//!
	blSignalCollective::Pointer m_signalCollective;

};

typedef boost::shared_ptr<SeriesCollective> SeriesCollectivePtr;

} // DicomPlugin

#endif // DicomPluginTypes_H
