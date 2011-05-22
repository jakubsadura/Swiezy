# Used to configure coreIO
import csnBuild
from csnGIMIASDef import *
import os.path
import csnProject
#used to access sys,platform. Delete when remove the fura window. Martin 27/03/2009
import sys

gmWidgets = csnBuild.Project("gmWidgets", "dll")
gmWidgets.AddProjects([
  gmProcessors,
  gmFrontEndPlugin,
  guiBridgeLibWxWidgets,
  dynWxAGUILib
])

# GRID
#if (sys.platform == "win32"):
# gmWidgets.AddDefinitions(["-DUSE_GRID_API"] ) 
# gmWidgets.AddProjects([gridAPI])

gmWidgets.SetPrecompiledHeader("gmWidgetsPCH.h")

widgetFolders = gmWidgets.Glob("src/*")
for widgetFolder in widgetFolders:
    if os.path.isdir(widgetFolder):
        gmWidgets.AddSources(["%s/*.cpp" % widgetFolder], _checkExists = 0)
        gmWidgets.AddSources(["%s/*.cxx" % widgetFolder], _checkExists = 0)
        gmWidgets.AddSources(["%s/*.txx" % widgetFolder], _checkExists = 0)
        gmWidgets.AddSources(["%s/*.h" % widgetFolder], _checkExists = 0)
        gmWidgets.AddIncludeFolders([widgetFolder])
        resourceFiles = gmWidgets.Glob("%s/resource" % widgetFolder)
        gmWidgets.AddFilesToInstall(resourceFiles, "resource")

