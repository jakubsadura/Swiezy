/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CardioToolsPluginPCH_H
#define _CardioToolsPluginPCH_H

#include "wx/wx.h"
#include "wx/wxprec.h"
#include <wx/wx.h>
#include <wx/wxprec.h>
#include <wx/image.h>
#include <wx/panel.h>

#include "blLookupTables.h"
#include "blMitkUnicode.h"
#include "blSignal.h"
#include "blSignalCollective.h"
#include "blVtkBullsEyeActor.h"
#include "coreCommonDataTypes.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHolder.h"
#include "coreDataEntityListBrowser.h"
#include "coreFrontEndPlugin.h"
#include "coreInputControl.h"
#include "coreKernel.h"
#include "coreMultiRenderWindow.h"
#include "coreObject.h"
#include "corePluginMacros.h"
#include "corePluginTabFactory.h"
#include "coreProfile.h"
#include "coreReportExceptionMacros.h"
#include "coreSimpleProcessingWidget.h"
#include "coreSmartPointerMacros.h"
#include "coreWidgetCollective.h"
#include "coreWxMitkGraphicalInterface.h"
#include "mitkVtkLayerController.h"
#include "vtkColorTransferFunction.h"
#include "vtkDataObject.h"
#include "vtkFieldData.h"
#include "vtkFloatArray.h"
#include "vtkLookupTable.h"
#include "vtkMath.h"
#include "vtkPointData.h"
#include "vtkProperty2D.h"
#include "vtkScalarBarActor.h"
#include "vtkScalarsToColors.h"
#include "vtkSmartPointer.h"
#include "vtkStringArray.h"
#include "vtkTextProperty.h"

#include "wxID.h"
#include "wxMitkSelectableGLWidget.h"
#include <map>
#include <string>

#endif //_CardioToolsPluginPCH_H
