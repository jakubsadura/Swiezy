/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CILabNamespaceMacros.h"

#include "blTestParams.h"
#include "blTestParamsFolderInfo.h"

#include "boost/shared_ptr.hpp"
#include "boost/function.hpp"

#include "itkGradientMagnitudeImageFilter.h"
#include "itkImage.h"
#include "itkScalarImageToHistogramGenerator.h"

#include "mitkDataTree.h"
#include "mitkDataTreeHelper.h"
#include "mitkEllipsoid.h"
#include "mitkImageAccessByItk.h"
#include "mitkMaterialProperty.h"
#include "mitkProperties.h"
#include "mitkRenderingManager.h"
#include "mitkStateMachineFactory.h"
#include "mitkVtkPropRenderer.h"

#include "vtkColorTransferFunction.h"
#include "vtkObject.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"

#include "wx/app.h"
#include "wx/bitmap.h"
#include "wx/button.h"
#include "wx/checkbox.h"
#include "wx/event.h"
#include "wx/frame.h"
#include "wx/panel.h"
#include "wx/sizer.h"
#include "wx/statbmp.h"
#include "wx/stattext.h"
#include "wx/textctrl.h"
#include "wx/univ/renderer.h"
#include "wx/window.h"
#include "wx/wxprec.h"

#include <iostream>
#include <itkCommand.h>
#include <itkLightObject.h>
#include <itkSmartPointer.h>
#include <list>
#include <map>
#include <math.h>

#include <mitkCameraRotationController.h>
#include <mitkDataStorage.h>
#include <mitkDataTree.h>
#include <mitkDataTreeNode.h>
#include <mitkDataTreeNodeFactory.h>
#include <mitkDisplayInteractor.h>
#include <mitkDisplayPositionEvent.h>
#include <mitkDisplayVectorInteractor.h>
#include <mitkGeometry2DDataMapper2D.h>
#include <mitkGlobalInteraction.h>
#include <mitkHistogramGenerator.h>
#include <mitkLevelWindowManager.h>
#include <mitkLogoRendering.h>
#include <mitkMaterialProperty.h>
#include <mitkProperties.h>
#include <mitkPropertyObserver.h>
#include <mitkRenderingManager.h>
#include <mitkRenderingManagerFactory.h>
#include <mitkSliceNavigationController.h>
#include <mitkSlicesRotator.h>
#include <mitkSlicesSwiveller.h>
#include <mitkStringProperty.h>
#include <mitkSurface.h>
#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkVtkPropRenderer.h>

#include <sstream>
#include <stdexcept>
#include <string>

#include <vtkColorTransferFunction.h>
#include <vtkCommand.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMitkRenderProp.h>
#include <vtkObjectFactory.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <wx/button.h>
#include <wx/clrpicker.h>
#include <wx/colordlg.h>
#include <wx/combobox.h>
#include <wx/control.h>
#include <wx/defs.h>
#include <wx/event.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>
#include <wx/tglbtn.h>
#include <wx/timer.h>
#include <wx/window.h>
#include <wx/wxprec.h>

