/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginPCH_H
#define _CmguiPluginPCH_H

#define WX_USER_INTERFACE

extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_element.h"
#include "api/cmiss_field.h"
#include "api/cmiss_field_composite.h"
#include "api/cmiss_field_image.h"
#include "api/cmiss_region.h"
#include "api/cmiss_scene_viewer.h"
#include "api/cmiss_texture.h"

#include "command/cmiss.h"
#include "graphics/scene.h"
#include "graphics/scene_viewer.h"
#include "user_interface/message.h"
}

#include "blTextUtils.h"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/foreach.hpp"
#include "coreBaseDataEntityReader.h"
#include "coreDataEntityImplFactory.h"
#include "coreDataEntityReader.h"
#include "coreFrontEndPlugin.h"
#include "coreObject.h"
#include "corePluginMacros.h"
#include "corePluginTab.h"
#include "coreProcessingWidget.h"
#include "coreRenderingTree.h"
#include "coreReportExceptionMacros.h"
#include "coreSmartPointerMacros.h"
#include "coreStringHelper.h"
#include "coreWidgetCollective.h"
#include "coreWxMitkGraphicalInterface.h"

#include "itksys/SystemTools.hxx"
#include "region/cmiss_region.h"
#include "tinyxml.h"
#include "wx/glcanvas.h"
#include "wx/treectrl.h"
#include "wxID.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <itksys/SystemTools.hxx>
#include <limits>
#include <list>
#include <wx/wx.h>
#include <wx/wxprec.h>

#endif //_CmguiPluginPCH_H
