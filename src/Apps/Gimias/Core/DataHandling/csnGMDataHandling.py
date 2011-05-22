# Used to configure lcore
import csnBuild
from csnGIMIASDef import *
import os.path
import csnProject

#used to access sys,platform. Delete when remove the fura window. Martin 27/03/2009
import sys

gmDataHandling = csnBuild.Project("gmDataHandling", "dll")
gmDataHandling.AddProjects([ 
  cilabMacros, 
  itk, 
  vtk,
  baseLibVTK,
  baseLibSignal,
  baseLibNumericData,
  boost,
  dcmAPI,
  gmCommonObjects
])

gmDataHandling.SetPrecompiledHeader("gmDataHandlingPCH.h")

baseFolders = [
  "src"
]

for baseFolder in baseFolders:
        gmDataHandling.AddSources(["%s/*.cxx" % baseFolder], _checkExists = 0)
        gmDataHandling.AddSources(["%s/*.txx" % baseFolder], _checkExists = 0)
        gmDataHandling.AddSources(["%s/*.h" % baseFolder], _checkExists = 0)
        gmDataHandling.AddIncludeFolders([baseFolder])

gmDataHandling.AddDefinitions(["/bigobj"], _private = 1, _WIN32 = 1) 