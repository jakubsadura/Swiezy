/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _ImageToolsPluginPCH_H
#define _ImageToolsPluginPCH_H
#include "wx/tglbtn.h"
#include "wx/xrc/xmlres.h"
#include "wxID.h"
#include <wx/image.h>
#include <wx/wx.h>
#include <wx/wxprec.h>

#include "CILabNamespaceMacros.h"
#include "blMitkLineBoundPointInteractor.h"
#include "boost/filesystem.hpp"
#include "boost/shared_ptr.hpp"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"
#include "coreDataContainer.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHolder.h"
#include "coreDataEntityListBrowser.h"
#include "coreDataTreeHelper.h"
#include "coreDirectory.h"
#include "coreFrontEndPlugin.h"
#include "coreImageDataEntityMacros.h"
#include "coreKernel.h"
#include "coreObject.h"
#include "corePluginMacros.h"
#include "corePluginTabFactory.h"
#include "coreProcessingWidget.h"
#include "coreProfile.h"
#include "coreRenderingTree.h"
#include "coreReportExceptionMacros.h"
#include "coreSimpleProcessingWidget.h"
#include "coreSmartPointerMacros.h"
#include "coreVTKPolyDataHolder.h"
#include "coreWidgetCollective.h"
#include "coreWxMitkGraphicalInterface.h"
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"
#include "gmWidgetsWin32Header.h"
#include "itkCommand.h"
#include "mitkCuboid.h"
#include "mitkGlobalInteraction.h"
#include "mitkPointSet.h"
#include "mitkProperties.h"
#include "mitkRenderingManager.h"
#include "mitkVtkRepresentationProperty.h"
#include "vtkAppendPolyData.h"
#include "vtkBox.h"
#include "vtkClipPolyData.h"
#include "vtkConeSource.h"
#include "vtkCubeSource.h"
#include "vtkImageReslice.h"
#include "vtkLineSource.h"
#include "vtkPlane.h"
#include "vtkPlaneSource.h"
#include "vtkSmartPointer.h"
#include <blMitkUnicode.h>
#include <boost/shared_ptr.hpp>
#include <mitkCuboid.h>
#include <mitkPointSet.h>
#include <mitkProperties.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>

#endif //_ImageToolsPluginPCH_H
