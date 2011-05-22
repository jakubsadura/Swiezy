/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CILabBoostMacros.h"
#include "CILabExceptionMacros.h"
#include "CILabNamespaceMacros.h"

#include "ModuleDescription.h"
#include "ModuleFactory.h"

#include "blShapeUtils.h"
#include "blTagMap.h"
#include "blTextUtils.h"

#include "boost/bind.hpp"
#include "boost/filesystem.hpp"

#include "coreAssert.h"
#include "coreBaseFilter.h"
#include "coreBaseFrontEndPlugin.h"
#include "coreCommonDataTypes.h"
#include "coreConfigVars.h"
#include "coreCreateExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityBuildersRegistration.h"
#include "coreDataEntityHolder.h"
#include "coreDataEntityIORegistration.h"
#include "coreDataEntityInfoHelper.h"
#include "coreDataEntityReader.h"
#include "coreDataEntityWriter.h"
#include "coreDataHolder.h"
#include "coreDirectory.h"
#include "coreFile.h"
#include "coreObject.h"
#include "coreProfile.h"
#include "coreProgressTicket.h"
#include "coreSession.h"
#include "coreSmartPointerMacros.h"
#include "coreWorkflow.h"
#include "coreWorkflowSerialize.h"
#include "coreXMLSubsystem.h"
#include "coreException.h"
#include "coreSettingsIO.h"
#include "gmKernelWin32Header.h"

#include "dynLib.h"
#include "tinyxml.h"

#include <boost/algorithm/string.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/signals.hpp>

#include <itksys/DynamicLoader.hxx>

#include <ctime>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <time.h>
#include <typeinfo>

#include <wx/colour.h>
#include <wx/font.h>
#include <wx/frame.h>
#include <wx/splash.h>
#include <wx/sysopt.h>
#include <wx/wx.h>
#include <wx/wxprec.h>
#include "wx/busyinfo.h"
#include "wx/wx.h"
#include "wx/wxprec.h"

