
import csnBuild
import csnCilab
import csnUtility
from csnGIMIASDef import *

dynLibApps = csnCilab.CilabModuleProject("DynLibApps", "library")
dynLibApps.AddSources( [csnUtility.GetDummyCppFilename()] )
dynLibApps.AddProjects( [dynLib, vtk] )
dynLibApps.AddApplications([
    "dynAppExecute"
    ], _holderName="DynLibApplications", _pch = "dynLibPCH.h")

dynLibWxApps = csnCilab.CilabModuleProject("DynLibWxApps", "library")
dynLibWxApps.AddSources( [csnUtility.GetDummyCppFilename()] )
dynLibWxApps.AddProjects( [dynWxAGUILib,toolkit] )
dynLibWxApps.AddApplications([
    "dynAppWxAGUI"
    ], _holderName="DynLibWxApplications", _pch = "dynWxAGUILibPCH.h")


#dynCLPSample = csnCilab.CommandLinePlugin( "dynCLPSample" )
#dynCLPSample.AddSources( [u'Applications/dynCLPSample/dynCLPSample.cxx' ] )
#dynCLPSample.AddProjects( [generateClp, slicer] )
#dynLibWxApps.AddProjects( [dynCLPSample] )


