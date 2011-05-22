# Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
# This software is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
#  Used to build all wxMitk applications

import csnBuild
import csnCilab
import csnUtility
from csnGIMIASDef import *

wxMitkApps = csnCilab.CilabModuleProject("wxMitkApps", "library")
wxMitkApps.AddSources( [csnUtility.GetDummyCppFilename()] )
#wxMitkApps.AddApplications(["wxMitkAppRenderingTree","wxMitkScrolledWindow","wxMitkSurfaceVisor", "wxMitkVisor"])
wxMitkApps.AddProjects([wxMitk])
wxMitkApps.AddApplications(["wxMitkAppRenderingTree"], _pch = "wxMitkPCH.h")
#wxMitkApps.AddDemos([
#  "TestWxMitkColorSelectorControl", 
#  "TestWxMitkDataTreeListView",
#  "TestWxMitkMaterialEditorWidget",
#  "TestWxMitkToolBoxControl"
#])


