
# Used to configure modLib
import csnCilab
import csnUtility
from csnGIMIASDef import *

dynLib = csnCilab.CilabModuleProject("DynLib", "dll")
dynLib.AddLibraryModules(["dynBase"])
dynLib.AddProjects( [expat, itk, slicer, baseLib, libElf] )
dynLib.SetPrecompiledHeader("dynLibPCH.h")
# Dbghelp is not included for Release version by default
dynLib.AddLibraries( ["Dbghelp.lib"], _WIN32 = 1 )

dynWxAGUILib = csnCilab.CilabModuleProject("DynWxAGUILib", "dll")
dynWxAGUILib.AddLibraryModules(["dynWxAGUI"])
dynWxAGUILib.AddProjects( [dynLib, guiBridgeLibWxWidgets, wxWidgets] )
dynWxAGUILib.SetPrecompiledHeader("dynWxAGUILibPCH.h")

