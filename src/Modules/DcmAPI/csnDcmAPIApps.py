# Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
# This software is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
#  Used to build all DcmAPI applications

import csnBuild
import csnCilab
import csnUtility
from csnToolkitOpen import *

dcmAPIApps = csnCilab.CilabModuleProject("DcmAPIApps", "library")
dcmAPIApps.AddSources( [csnUtility.GetDummyCppFilename()] )
dcmAPIApps.AddProjects( [dcmAPI] )
dcmAPIApps.AddApplications([
    "viewer"
    ], _holderName="dcmapps")
dcmAPIApps.AddDefinitions(["/bigobj"], _WIN32 = 1) 
