# Used to configure coreIO
import csnBuild
from csnGIMIASDef import *
import os.path
import csnProject

gmCommonObjects = csnBuild.Project("gmCommonObjects", "dll")
gmCommonObjects.AddProjects([ 
  itk,
  boost
])

gmCommonObjects.SetPrecompiledHeader("gmCommonObjectsPCH.h")

baseFolders = [
  "src"
]

for baseFolder in baseFolders:
        gmCommonObjects.AddSources(["%s/*.cxx" % baseFolder], _checkExists = 0)
        gmCommonObjects.AddSources(["%s/*.txx" % baseFolder], _checkExists = 0)
        gmCommonObjects.AddSources(["%s/*.h" % baseFolder], _checkExists = 0)
        gmCommonObjects.AddIncludeFolders([baseFolder])

