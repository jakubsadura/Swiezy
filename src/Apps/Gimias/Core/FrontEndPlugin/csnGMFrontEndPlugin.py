# Used to configure coreIO
import csnBuild
from csnGIMIASDef import *
import os.path
import csnProject

gmFrontEndPlugin = csnBuild.Project("gmFrontEndPlugin", "dll")
gmFrontEndPlugin.AddProjects([
  gmCommonObjects,
  baseLib,
  baseLibNumericData
  ])

gmFrontEndPlugin.SetPrecompiledHeader("gmFrontEndPluginPCH.h")

baseFolders = [
  "src"
]

for baseFolder in baseFolders:
        gmFrontEndPlugin.AddSources(["%s/*.cxx" % baseFolder], _checkExists = 0)
        gmFrontEndPlugin.AddSources(["%s/*.txx" % baseFolder], _checkExists = 0)
        gmFrontEndPlugin.AddSources(["%s/*.h" % baseFolder], _checkExists = 0)
        gmFrontEndPlugin.AddIncludeFolders([baseFolder])
