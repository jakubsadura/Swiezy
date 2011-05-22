# Used to configure TpExtLib
import csnBuild
import csnCilab
from csnGIMIASDef import *

tpExtLibMITK = csnCilab.CilabModuleProject("TpExtLibMITK", "dll")
tpExtLibMITK.AddLibraryModules(["tpExtMITK"])
tpExtLibMITK.AddProjects([mitk, baseLibVTK, baseLibSignal])
tpExtLibMITK.SetPrecompiledHeader("tpExtLibMITKPCH.h")

tpExtLibWxWidgets = csnCilab.CilabModuleProject("TpExtLibWxWidgets", "dll")
tpExtLibWxWidgets.AddLibraryModules(["tpExtWxWidgets"])
tpExtLibWxWidgets.AddProjects([baseLib, wxWidgets, boost])
tpExtLibWxWidgets.SetPrecompiledHeader("tpExtLibWxWidgetsPCH.h")
tpExtLibWxWidgets.AddLibraries(["psapi"], _WIN32 = 1)

tpExtLibITK = csnCilab.CilabModuleProject("TpExtLibITK", "dll")
tpExtLibITK.AddLibraryModules(["tpExtITK"])
tpExtLibITK.AddProjects([itk])
tpExtLibITK.SetPrecompiledHeader("tpExtLibITKPCH.h")

