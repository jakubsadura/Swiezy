# Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
# This software is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
#  Used to build all MeshLib applications

import csnBuild
import csnCilab
import csnUtility
from csnToolkitOpen import *

meshLibApps = csnCilab.CilabModuleProject("MeshLibApps", "library")
meshLibApps.AddSources( [csnUtility.GetDummyCppFilename()] )
meshLibApps.AddProjects( [meshLib] )
meshLibApps.AddApplications([
    "meAppTools", 
    "meAppPolyline",
    "meAppSegValidationTools"
    ], _holderName="MeshLibApplications")
