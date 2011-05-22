# Used to configure ManualSegmentationPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

manualSegmentationPlugin = csnCilab.GimiasPluginProject("ManualSegmentationPlugin")

projects = [
    gmCore
]
manualSegmentationPlugin.AddProjects(projects)

manualSegmentationPlugin.AddSources(["*.cxx", "*.h"])
manualSegmentationPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
manualSegmentationPlugin.AddIncludeFolders(["processors"])

widgetModules = [
  "ManualSegmentationWidget",
  "MultiLevelROIPanelWidget"
  ]
manualSegmentationPlugin.AddWidgetModules(widgetModules, _useQt = 0)

manualSegmentationPlugin.SetPrecompiledHeader("ManualSegmentationPluginPCH.h")

manualSegmentationPlugin.AddFilesToInstall(manualSegmentationPlugin.Glob("resource"), "resource")
