# Used to configure imageToolsPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

imageToolsPlugin = csnCilab.GimiasPluginProject("ImageToolsPlugin")

projects = [
    gmCore
]
imageToolsPlugin.AddProjects(projects)

imageToolsPlugin.AddSources(["*.cxx", "*.h"])
imageToolsPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
imageToolsPlugin.AddIncludeFolders(["processors"])

widgetModules = [
  "CroppingWidget",
  "ProcessingToolsWidget"
  ]
imageToolsPlugin.AddWidgetModules(widgetModules, _useQt = 0)

imageToolsPlugin.SetPrecompiledHeader("ImageToolsPluginPCH.h")

imageToolsPlugin.AddFilesToInstall(imageToolsPlugin.Glob("resource"), "resource")
