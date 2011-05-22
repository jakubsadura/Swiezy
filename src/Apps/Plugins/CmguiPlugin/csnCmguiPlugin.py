# Used to configure CmguiPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

cmguiPlugin = csnCilab.GimiasPluginProject("CmguiPlugin")

projects = [
    gmCoreLight,
    guiBridgeLib,
    baseLibVTK,
    guiBridgeLibWxWidgets,
    cmgui
]
cmguiPlugin.AddProjects(projects)

cmguiPlugin.AddSources(["*.cxx", "*.h"])
cmguiPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
cmguiPlugin.AddIncludeFolders(["processors"])
cmguiPlugin.AddLibraries(["glu32"], _WIN32 = 1)

widgetModules = [
  "CmguiPluginRenderWidget",
  "CmguiPluginCommandlineWidget",
  "CmguiPluginInfoWidget",
  "CmguiPluginCommandWidget",
  "CmguiPluginToolbar",
  "CmguiPluginVisualProps"
  ]
cmguiPlugin.AddWidgetModules(widgetModules, _useQt = 0)

cmguiPlugin.SetPrecompiledHeader("CmguiPluginPCH.h")

resourceFiles = cmguiPlugin.Glob("resource/WorkingAreas" )
cmguiPlugin.AddFilesToInstall(resourceFiles, "resource/WorkingAreas")

#install toolbar files
installFolder = "%s/debug/Toolbar" % cmguiPlugin.installSubFolder
cmguiPlugin.installManager.AddFilesToInstall( cmguiPlugin.Glob( "resource/Toolbar" ), installFolder, _debugOnly = 1, _WIN32 = 1 )
installFolder = "%s/release/Toolbar" % cmguiPlugin.installSubFolder
cmguiPlugin.installManager.AddFilesToInstall( cmguiPlugin.Glob( "resource/Toolbar" ), installFolder, _releaseOnly = 1, _WIN32 = 1 )
installFolder = "%s/Toolbar" % cmguiPlugin.installSubFolder
cmguiPlugin.installManager.AddFilesToInstall( cmguiPlugin.Glob( "resource/Toolbar" ), installFolder, _NOT_WIN32 = 1 )

