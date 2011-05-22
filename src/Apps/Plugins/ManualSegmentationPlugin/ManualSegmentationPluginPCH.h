/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _ManualSegmentationPluginPCH_H
#define _ManualSegmentationPluginPCH_H

#include "blMITKUtils.h"

#include "coreAssert.h"
#include "coreBaseProcessor.h"
#include "coreBaseWindow.h"
#include "coreCommonDataTypes.h"
#include "coreDataContainer.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHolder.h"
#include "coreDataEntityList.h"
#include "coreDataEntityList.txx"
#include "coreDataEntityListBrowser.h"
#include "coreDataHolder.h"
#include "coreDataTreeHelper.h"
#include "coreEnvironment.h"
#include "coreException.h"
#include "coreFrontEndPlugin.h"
#include "coreKernel.h"
#include "coreLogger.h"
#include "coreObject.h"
#include "corePluginMacros.h"
#include "corePluginTab.h"
#include "corePluginTabFactory.h"
#include "coreProcessorFactory.h"
#include "coreProcessorInputWidget.h"
#include "coreProfile.h"
#include "coreRenderingTree.h"
#include "coreReportExceptionMacros.h"
#include "coreSimpleProcessingWidget.h"
#include "coreSmartPointerMacros.h"
#include "coreUserHelperWidget.h"
#include "coreVTKImageDataHolder.h"
#include "coreVTKImageDataRenDataBuilder.h"
#include "coreVTKPolyDataHolder.h"
#include "coreWidgetCollective.h"
#include "coreWxMitkGraphicalInterface.h"

#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"

#include "itkAddImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"
#include "itkMultiplyByConstantImageFilter.h"
#include "itkNaryAddImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkVTKImageToImageFilter.h"
#include "itkVector.h"

#include "mitkContour.h"
#include "mitkContourInteractor.h"
#include "mitkContourTool.h"
#include "mitkDataStorage.h"
#include "mitkDataTree.h"
#include "mitkDataTreeHelper.h"
#include "mitkDataTreeNode.h"
#include "mitkGlobalInteraction.h"
#include "mitkImage.h"
#include "mitkInteractionConst.h"
#include "mitkLevelWindowProperty.h"
#include "mitkRenderingManager.h"
#include "mitkSmartPointerProperty.h"
#include "mitkToolManager.h"
#include "mitkVtkResliceInterpolationProperty.h"

#include "vtkImageCast.h"
#include "vtkImageMathematics.h"
#include "vtkImageStencil.h"
#include "vtkImageThreshold.h"
#include "vtkImageToImageStencil.h"
#include "vtkImageWriter.h"
#include "vtkPolyDataToImageStencil.h"
#include "vtkSmartPointer.h"

#include "wxID.h"

#include "wxMitkColorSelectorControl.h"
#include "wxMitkRenderWindow.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

#include <mitkBaseRenderer.h>
#include <mitkCommon.h>
#include <mitkDataTree.h>
#include <mitkPointSet.h>
#include <mitkRenderingManager.h>

#include <sstream>
#include <vector>

#include <wx/image.h>
#include <wx/spinctrl.h>
#include <wx/tglbtn.h>
#include <wx/wx.h>
#include <wx/wxprec.h>

#endif //_ManualSegmentationPluginPCH_H
