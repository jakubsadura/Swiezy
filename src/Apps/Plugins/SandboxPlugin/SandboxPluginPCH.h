/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _SandboxPluginPCH_H
#define _SandboxPluginPCH_H

// CoreLib
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreFrontEndPlugin.h"
#include "coreReportExceptionMacros.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"

// GUIBridgeLib
#include "gblBridge.h"
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"

// VTK
#include "vtkPolyData.h"

// STD
#include <limits>
#include <string>

// wxWidgets
#include <wx/image.h>
#include <wx/wx.h>
#include <wx/wxprec.h>

// BaseLib
#include "blVTKHelperTools.h"

// CoreLib
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

// GUIBridgeLib
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"

// ITK
#include "itkImage.h"
#include "itkResampleImageFilter.h"
#include "itkScaleTransform.h"
#include "itkSubtractImageFilter.h"

// VTK
#include "vtkPolyData.h"

// BaseLib
#include "wxID.h"

// STD
#include <iostream>
#include <limits>
#include <string>

// WxWidgets
#include <wx/image.h>
#include <wx/spinbutt.h>
#include <wx/wx.h>
#include <wx/wxprec.h>


#endif //_SandboxPluginPCH_H
