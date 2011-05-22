/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginPCH_H
#define _TavernaPluginPCH_H

// GUIBridgeLib
#include "gblBridge.h"
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"


// WxWidgets
#include "wxID.h"
#include <wx/image.h>
#include <wx/wx.h>
#include <wx/wxprec.h>
#include <wx/spinbutt.h>

// BaseLib
#include "blVTKHelperTools.h"

// CoreLib
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreFrontEndPlugin.h"
#include "coreKernel.h"
#include "coreReportExceptionMacros.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreDataTreeHelper.h"
#include "coreFrontEndPlugin.h"
#include "coreMultiRenderWindow.h"
#include "coreObject.h"
#include "corePluginTab.h"
#include "coreRenderingTree.h"
#include "coreReportExceptionMacros.h"
#include "coreSmartPointerMacros.h"
#include "coreUserHelperWidget.h"
#include "coreVTKPolyDataHolder.h"
#include "coreWxMitkCoreMainWindow.h"
#include "coreWxMitkGraphicalInterface.h"

// ITK
#include "itkImage.h"
#include "itkResampleImageFilter.h"
#include "itkScaleTransform.h"
#include "itkSubtractImageFilter.h"

// VTK
#include "vtkPolyData.h"

// STD
#include <iostream>
#include <limits>
#include <string>



#endif //_TavernaPluginPCH_H
