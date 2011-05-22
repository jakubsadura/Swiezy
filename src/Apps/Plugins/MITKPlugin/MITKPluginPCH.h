/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _MITKPluginPCH_H
#define _MITKPluginPCH_H

#include <wx/wxprec.h>
#include "wx/string.h"
#include "wx/tglbtn.h"
#include "wx/toolbar.h"
#include "wx/wx.h"
#include "wx/wxprec.h"
#include "wx/xrc/xmlres.h"
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/tglbtn.h>
#include <wx/wx.h>

#include "CILabBoostMacros.h"
#include "CILabItkMacros.h"
#include "CILabNamespaceMacros.h"
#include "blLookupTables.h"
#include "blMITKUtils.h"
#include "blMitkInteractorHelper.h"
#include "blMitkLineBoundPointInteractor.h"
#include "blMitkPointSelectInteractor.h"
#include "blMitkPointSetInteractor.h"
#include "blMitkScalarBar.h"
#include "blMitkSignal.h"
#include "blMitkSurface.h"
#include "blNumericUtils.h"
#include "blSignal.h"
#include "blSignalCollective.h"
#include "blVTKHelperTools.h"

#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/signal.hpp"

#include "coreAssert.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"
#include "coreBaseWindow.h"
#include "coreCommonDataTypes.h"
#include "coreDataContainer.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreDataEntityHolder.h"
#include "coreDataEntityImplFactory.h"
#include "coreDataEntityInfoHelper.h"
#include "coreDataEntityListBrowser.h"
#include "coreDataEntityUtilities.h"
#include "coreDataEntityUtilities.txx"
#include "coreDataHolder.h"
#include "coreDataTreeHelper.h"
#include "coreEnvironment.h"
#include "coreException.h"
#include "coreFrontEndPlugin.h"
#include "coreImageDataEntityMacros.h"
#include "coreInputControl.h"
#include "coreKernel.h"
#include "coreLogger.h"
#include "coreMainMenu.h"
#include "coreMultiRenderWindow.h"
#include "coreObject.h"
#include "corePluginMacros.h"
#include "corePluginTab.h"
#include "coreProcessorFactories.h"
#include "coreProcessorInputWidget.h"
#include "coreRenderingTree.h"
#include "coreReportExceptionMacros.h"
#include "coreSelectionToolWidget.h"
#include "coreSettings.h"
#include "coreSmartPointerMacros.h"
#include "coreWidgetCollective.h"
#include "coreWindowConfig.h"
#include "coreWxMitkCoreMainWindow.h"
#include "coreWxMitkGraphicalInterface.h"

#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"
#include "gmWidgetsWin32Header.h"
#include "itkCommand.h"
#include "itkComputeFAFilter.h"
#include "itkDTITensor.h"
#include "itkImage.h"
#include "itkImageBase.h"
#include "itkImageRegion.h"
#include "itkImageToVTKImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"

#include "mitkCuboid.h"
#include "mitkDataStorage.h"
#include "mitkDataTree.h"
#include "mitkDataTreeHelper.h"
#include "mitkDataTreeNode.h"
#include "mitkGlobalInteraction.h"
#include "mitkImage.h"
#include "mitkImageAccessByItk.h"
#include "mitkImageTimeSelector.h"
#include "mitkInteractionConst.h"
#include "mitkLevelWindowManager.h"
#include "mitkLevelWindowProperty.h"
#include "mitkLine.h"
#include "mitkLookupTable.h"
#include "mitkLookupTableProperty.h"
#include "mitkMaterialProperty.h"
#include "mitkNodePredicateProperty.h"
#include "mitkPlanarPolygon.h"
#include "mitkPointOperation.h"
#include "mitkPointSet.h"
#include "mitkProperties.h"
#include "mitkRenderingManager.h"
#include "mitkSmartPointerProperty.h"
#include "mitkStandaloneDataStorage.h"
#include "mitkStringProperty.h"
#include "mitkStringVectorProperty.h"
#include "mitkSurface.h"
#include "mitkTransferFunctionProperty.h"
#include "mitkTransform.h"
#include "mitkTransformObjectFactory.h"
#include "mitkUnstructuredGridVtkMapper3D.h"
#include "mitkVector.h"
#include "mitkVtkLayerController.h"
#include "mitkVtkRepresentationProperty.h"
#include "mitkVtkResliceInterpolationProperty.h"
#include "mitkVtkScalarModeProperty.h"

#include "vtkCamera.h"
#include "vtkCellData.h"
#include "vtkCommand.h"
#include "vtkCornerAnnotation.h"
#include "vtkCubeSource.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkImageData.h"
#include "vtkLinearTransform.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStringArray.h"
#include "vtkTextProperty.h"

#include "wxEventHandlerHelper.h"
#include "wxID.h"
#include "wxMitkApp.h"
#include "wxMitkDisplayPlaneSubtreeHelper.h"
#include "wxMitkLevelWindowWidget.h"
#include "wxMitkMultiRenderWindow.h"
#include "wxMitkOrthoSliceFactory.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkSelectableGLWidget.h"
#include "wxMitkVTKWindow.h"
#include "wxUnicode.h"

#include <CILabAssertMacros.h>
#include <CILabItkMacros.h>
#include <CILabNamespaceMacros.h>

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <coreObject.h>

#include <coreReportExceptionMacros.h>

#include <itkCastImageFilter.h>
#include <itkImage.h>
#include <itkPoint.h>

#include <mitkAction.h>
#include <mitkBaseProperty.h>
#include <mitkBaseRenderer.h>
#include <mitkCommon.h>
#include <mitkContour.h>
#include <mitkCuboid.h>
#include <mitkDataTree.h>
#include <mitkDataTreeNode.h>
#include <mitkDisplayPositionEvent.h>
#include <mitkITKImageImport.h>
#include <mitkImage.h>
#include <mitkImageToItk.h>
#include <mitkInteractionConst.h>
#include <mitkPointSet.h>
#include <mitkPositionTracker.h>
#include <mitkProperties.h>
#include <mitkRenderingManager.h>
#include <mitkStateEvent.h>
#include <mitkStateMachine.h>
#include <mitkSurface.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkUndoController.h>
#include <mitkUnstructuredGrid.h>
#include <mitkVector.h>

#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#include <vtkDataSetSurfaceFilter.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

#include <wxMitkMaterialEditorWidget.h>
#include <wxMitkTransferFunctionWidget.h>
#include <wxMitkTransformVisualPropWidget.h>

#include <wxUnicode.h>


#endif //_MITKPluginPCH_H
