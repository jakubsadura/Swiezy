# Used to configure mitkPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

mitkPlugin = csnCilab.GimiasPluginProject("MITKPlugin")

projects = [
	mitk,
	tpExtLibMITK,
	wxMitk,
    gmCoreLight, 
    guiBridgeLibWxWidgets
]
mitkPlugin.AddProjects(projects)

mitkPlugin.AddSources(["*.cxx", "*.h"])
mitkPlugin.AddSources(["processors/*.cxx", "processors/*.h"], _checkExists = 0)
mitkPlugin.AddIncludeFolders(["processors"])

widgetModules = [
	"BoundingBoxWidget",
	"DataInformation",
	"ImageInfoWidget",
	"Interactors",
	"LandmarkSelectorWidget",
	"MeasurementWidget",
	"PointsTableWidget",
	"RenderingTree",
	"RenderWindow",
	"Toolbars",
	"VisualProps"
]
mitkPlugin.AddWidgetModules(widgetModules, _useQt = 0)

mitkPlugin.SetPrecompiledHeader("MITKPluginPCH.h")

mitkPlugin.AddFilesToInstall(mitkPlugin.Glob("widgets/Interactors/GimiasStateMachines.xml"), "resource")
mitkPlugin.AddFilesToInstall(mitkPlugin.Glob("resource"), "resource")
