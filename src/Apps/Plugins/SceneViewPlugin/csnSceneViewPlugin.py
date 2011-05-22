# Used to configure SceneViewPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

sceneViewPlugin = csnCilab.GimiasPluginProject("SceneViewPlugin")
sceneViewPlugin.AddProjects([gmCore])
sceneViewPlugin.AddSources(["*.cxx", "*.h"])
sceneViewPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
sceneViewPlugin.AddIncludeFolders(["processors"])

widgetModules = [
  "ExecuteCommandWidget"
  ]
sceneViewPlugin.AddWidgetModules(widgetModules, _useQt = 0)

sceneViewPlugin.SetPrecompiledHeader("SceneViewPluginPCH.h")
