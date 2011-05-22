# Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
# This software is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
#  Used to build all BaseLib applications

import csnBuild
import csnCilab
import csnUtility
from csnToolkitOpen import *

baseLibApps = csnCilab.CilabModuleProject("BaseLibApps", "library")
baseLibApps.AddSources( [csnUtility.GetDummyCppFilename()] )
baseLibApps.AddProjects( [baseLibVTK, baseLibSignal] )
baseLibApps.AddApplications([
    #"blAppBullsEye",
    #"blAppDICOMReader",
    "blAppGDFReader",
    "blAppIdsVtkConverter"
    ], _holderName="BaseLibApplications")

#baseLibApps.AddApplications(["blwxAppTreeCtrl"], _applicationDependenciesList =[baseLibWxWidgets])
