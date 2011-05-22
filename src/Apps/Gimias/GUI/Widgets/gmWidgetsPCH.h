/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wx/wxprec.h>
#include "wx/combo.h"
#include "wx/dir.h"
#include "wx/imaglist.h"
#include "wx/scrolwin.h"
#include "wx/statusbr.h"
#include "wx/toolbar.h"
#include "wx/treebase.h"
#include "wx/wx.h"
#include "wx/wxprec.h"
#include <wx/arrstr.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/bmpbuttn.h>
#include <wx/clipbrd.h>
#include <wx/combo.h>
#include <wx/dataobj.h>
#include <wx/dialog.h>
#include <wx/dnd.h>
#include <wx/docview.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/progdlg.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/tglbtn.h>
#include <wx/tooltip.h>
#include <wx/treectrl.h>
#include <wx/utils.h>
#include <wx/wizard.h>
#include <wx/wupdlock.h>
#include <wx/wx.h>

#include "CILabAssertMacros.h"
#include "CILabNamespaceMacros.h"
#include "coreAssert.h"
#include "coreException.h"
#include "coreBaseExceptions.h"
#include "coreBaseFilter.h"
#include "coreBaseMainWindow.h"
#include "coreBaseProcessor.h"
#include "coreBaseWindow.h"
#include "coreBaseWindowFactories.h"
#include "coreBaseWorkingAreaStorage.h"

#include "coreCommonDataTypes.h"
#include "coreCommonMacros.h"
#include "coreDataContainer.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHolder.h"
#include "coreDataEntityInfoHelper.h"
#include "coreDataEntityList.h"
#include "coreDataEntityList.txx"
#include "coreDataEntityMetadata.h"
#include "coreDataEntityReader.h"
#include "coreDataEntityWriter.h"
#include "coreDataHolder.h"
#include "coreDirectory.h"
#include "coreDynProcessor.h"

#include "coreException.h"
#include "coreFrontEndPluginManager.h"
#include "coreKernel.h"
#include "coreLogger.h"
#include "coreLoggerHelper.h"
#include "coreObject.h"
#include "coreProcessorOutputObserver.h"
#include "coreProcessorOutputsObserverBuilder.h"
#include "coreProfile.h"
#include "coreRenderingTree.h"
#include "coreRenderingTreeManager.h"
#include "coreReportExceptionMacros.h"
#include "coreSessionReader.h"
#include "coreSessionWriter.h"
#include "coreSettings.h"
#include "coreSplashScreen.h"
#include "coreStyleManager.h"
#include "coreWindowConfig.h"
#include "coreWorkflow.h"
#include "coreWorkflowManager.h"
#include "coreWxMitkGraphicalInterface.h"

#include "gmWidgetsWin32Header.h"

#include "itkCommand.h"
#include "itksys/SystemTools.hxx"
#include "thumbnailctrl.h"
#include "vtkImageCast.h"
#include "wxID.h"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/signals.hpp>
#include <boost/thread/thread.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include <wxToolBoxControl.h>
#include <wxToolBoxItem.h>
#include <wxWidgetStackControl.h>
#include "wxUnicode.h"
#include <wxUnicode.h>
#include "wxCheckableControl.h"
#include "blwxTreeCtrl.h"
