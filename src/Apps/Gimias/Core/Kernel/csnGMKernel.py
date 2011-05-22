# Used to configure coreIO
import csnBuild
from csnGIMIASDef import *
import os.path
import csnProject

gmKernel = csnBuild.Project("gmKernel", "dll")
gmKernel.AddProjects([ 
  cilabMacros, 
  itk, 
  baseLibVTK,
  boost,
  gmCommonObjects,
  gmDataHandling,
  gmIO,
  gmFrontEndPlugin,
  dynLib,
  wxWidgets
])

gmKernel.SetPrecompiledHeader("gmKernelPCH.h")

baseFolders = [
  "src"
]

for baseFolder in baseFolders:
        gmKernel.AddSources(["%s/*.cxx" % baseFolder], _checkExists = 0)
        gmKernel.AddSources(["%s/*.txx" % baseFolder], _checkExists = 0)
        gmKernel.AddSources(["%s/*.h" % baseFolder], _checkExists = 0)
        gmKernel.AddIncludeFolders([baseFolder])

gmKernel.AddFilesToInstall(gmKernel.Glob("resource"), "resource")
