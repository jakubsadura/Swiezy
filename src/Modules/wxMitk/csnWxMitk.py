# Used to configure wxMitk
import csnBuild
import csnCilab
import csnUtility
from csnGIMIASDef import *

wxMitkRendering = csnCilab.CilabModuleProject("wxMitkRendering", "dll")
wxMitkRendering.AddLibraryModules(["wxMitkRendering"])
wxMitkRendering.AddProjects([
  vtk,
  tpExtLibMITK,
  tpExtLibITK,
  baseLibITK,
  boost,
  tpExtLibWxWidgets,
  cilabMacros,
  mitk
	])
wxMitkRendering.SetPrecompiledHeader("wxMitkPCH.h")

wxMitkWidgets = csnCilab.CilabModuleProject("wxMitkWidgets", "dll")
wxMitkWidgets.AddLibraryModules(["wxMitkWidgets"])
wxMitkWidgets.AddProjects([wxMitkRendering, tpExtLibMITK, baseLibVTK, baseLibSignal])
wxMitkWidgets.AddIncludeFolders(["./libmodules/wxMitkWidgets/resource"])
wxMitkWidgets.SetPrecompiledHeader("wxMitkPCH.h")

wxMitk = csnCilab.CilabModuleProject("wxMitk", "library")
wxMitk.AddProjects([wxMitkWidgets, tpExtLibWxWidgets])

# running error with this one (YM)
#wxMitk.AddTests(["tests/wxMitkTestString/*.*"], cxxTest, _enableWxWidgets = 1 )
#wxMitk.AddTests(["tests/wxMitkIDTest/*.*"], cxxTest )
#wxMitk.testProject.SetPrecompiledHeader("wxMitkPCH.h")
