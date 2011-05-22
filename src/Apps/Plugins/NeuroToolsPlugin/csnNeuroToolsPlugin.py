# Used to configure neuroToolsPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

neuroToolsPlugin = csnCilab.GimiasPluginProject("NeuroToolsPlugin")

projects = [
    gmCore
]
neuroToolsPlugin.AddProjects(projects)

neuroToolsPlugin.AddSources(["*.cxx", "*.h"])
#neuroToolsPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
neuroToolsPlugin.AddIncludeFolders(["processors"])

widgetModules = [
  "ImageContrastWidget"
  ]
neuroToolsPlugin.AddWidgetModules(widgetModules, _useQt = 0)

neuroToolsPlugin.SetPrecompiledHeader("NeuroToolsPluginPCH.h")

neuroToolsPlugin.AddFilesToInstall(neuroToolsPlugin.Glob("resource"), "resource")
