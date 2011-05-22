# Used to configure EvoLib
import csnBuild
import csnCilab
import csnUtility
from csnGIMIASDef import *
        
guiBridgeLib = csnCilab.CilabModuleProject("GuiBridgeLib", "library")
guiBridgeLib.AddLibraryModules(["guiBridgeLib"])
guiBridgeLib.AddProjects([boost, cilabMacros])
guiBridgeLib.SetPrecompiledHeader("guiBridgeLibPCH.h")

guiBridgeLibWxWidgets = csnCilab.CilabModuleProject("GuiBridgeLibWxWidgets", "library")
guiBridgeLibWxWidgets.AddLibraryModules(["guiBridgeLibWxWidgets"])
guiBridgeLibWxWidgets.AddProjects([guiBridgeLib, wxWidgets, cilabMacros, tpExtLibWxWidgets])
guiBridgeLibWxWidgets.SetPrecompiledHeader("guiBridgeLibPCH.h")
