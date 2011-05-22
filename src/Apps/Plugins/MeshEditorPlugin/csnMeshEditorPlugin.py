# Used to configure meshEditorPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

meshEditorPlugin = csnCilab.GimiasPluginProject("MeshEditorPlugin")

projects = [
    gmCore,
    meshLib
]
meshEditorPlugin.AddProjects(projects)

meshEditorPlugin.AddSources(["*.cxx", "*.h"])
meshEditorPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
meshEditorPlugin.AddIncludeFolders(["processors"])

widgetModules = [
  "ToolsProcessorWidget",
  "CroppingWidget",
  "RadiographVisualization",
  "SurfaceSelectorWidget"
  ]
meshEditorPlugin.AddWidgetModules(widgetModules, _useQt = 0)

meshEditorPlugin.SetPrecompiledHeader("MeshEditorPluginPCH.h")

meshEditorPlugin.AddFilesToInstall(meshEditorPlugin.Glob("resource"), "resource")
