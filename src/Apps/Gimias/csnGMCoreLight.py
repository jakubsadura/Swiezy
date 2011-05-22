# Used to configure gmCoreLight
import csnBuild
from csnGIMIASDef import *
import os.path

gmCoreLight = csnBuild.Project("gmCoreLight", "library")
gmCoreLight.AddProjects([ 
  gmCommonObjects,
  gmDataHandling,
  gmKernel,
  gmWidgets,
  gmProcessors,
  gmFrontEndPlugin
])

#gmCoreLight.SetPrecompiledHeader("gmCoreLightPCH.h")
gmCoreLight.compileManager.generateWin32Header = False

gmCoreLight.AddLibraries(["util"], _NOT_WIN32 = 1)

gmCoreLight.AddFilesToInstall(gmCoreLight.Glob("Copyrights/*.txt"), "Copyrights")
gmCoreLight.AddFilesToInstall(gmCoreLight.Glob("resource"), "resource")
gmCoreLight.AddFilesToInstall(gmCoreLight.Glob("Core/*.xml"), "")
gmCoreLight.AddFilesToInstall(gmCoreLight.Glob("Core/*.xsl"), "")

#gmCoreLight.AddTests(["Tests/*.*"], cxxTest)
#gmCoreLight.testProject.SetPrecompiledHeader("gmCoreLightPCH.h")

