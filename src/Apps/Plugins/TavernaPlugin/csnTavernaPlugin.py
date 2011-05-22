# Used to configure TavernaPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

tavernaPlugin = csnCilab.GimiasPluginProject("TavernaPlugin")

projects = [
    gmCoreLight,
    wflLib
]
tavernaPlugin.AddProjects(projects)

tavernaPlugin.AddSources(["*.cxx", "*.h"])
tavernaPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
tavernaPlugin.AddIncludeFolders(["processors"])

widgetModules = [
  'TavernaPluginWorkflowPanelWidget',
  'TavernaPluginWorkingArea',
  'TavernaPluginConfigurationPanelWidget',
  'TavernaPluginToolbar'
  ]
tavernaPlugin.AddWidgetModules(widgetModules, _useQt = 0)

tavernaPlugin.SetPrecompiledHeader("TavernaPluginPCH.h")

#tavernaPlugin.AddTests(["tests/*.*"], cxxTest)

tavernaPlugin.AddFilesToInstall(tavernaPlugin.Glob("resource"), "resource")
