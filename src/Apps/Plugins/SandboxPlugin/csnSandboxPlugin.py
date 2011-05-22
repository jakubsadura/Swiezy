# Used to configure SandboxPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

sandboxPlugin = csnCilab.GimiasPluginProject("SandboxPlugin")

projects = [
    gmCore, 
    guiBridgeLib, 
    baseLibVTK,
    guiBridgeLibWxWidgets
]
sandboxPlugin.AddProjects(projects)

sandboxPlugin.AddSources(["*.cxx", "*.h"])
sandboxPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
sandboxPlugin.AddIncludeFolders(["processors"])

widgetModules = [
  "SandboxPluginShapeScalePanelWidget",
  "SandboxPluginSubtractPanelWidget",
  "SandboxPluginResamplePanelWidget"
  ]
sandboxPlugin.AddWidgetModules(widgetModules, _useQt = 0)

sandboxPlugin.SetPrecompiledHeader("SandboxPluginPCH.h")
