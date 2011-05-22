# Used to configure cardioToolsPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

cardioToolsPlugin = csnCilab.GimiasPluginProject("CardioToolsPlugin")

projects = [
    gmCore,
    signalViewerPlugin
]
cardioToolsPlugin.AddProjects(projects)

cardioToolsPlugin.AddSources(["*.cxx", "*.h"])
#cardioToolsPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
cardioToolsPlugin.AddIncludeFolders(["processors"])

widgetModules = [
  "BullEyePlotWidget"
  ]
cardioToolsPlugin.AddWidgetModules(widgetModules, _useQt = 0)

cardioToolsPlugin.SetPrecompiledHeader("CardioToolsPluginPCH.h")

cardioToolsPlugin.AddFilesToInstall(cardioToolsPlugin.Glob("resource"), "resource")
