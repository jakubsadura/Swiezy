# Used to configure coreIO
import csnBuild
import csnCilab
from csnGIMIASDef import *
import os.path
import csnProject

gmProcessors = csnBuild.Project("gmProcessors", "dll")

gmProcessors.AddProjects([ 
	gmDataHandling,
	gmKernel
])
	
gmProcessors.SetPrecompiledHeader("gmProcessorsPCH.h")

baseFolders = [
  "src"
]

for baseFolder in baseFolders:
        gmProcessors.AddSources(["%s/*.cxx" % baseFolder], _checkExists = 0)
        gmProcessors.AddSources(["%s/*.txx" % baseFolder], _checkExists = 0)
        gmProcessors.AddSources(["%s/*.h" % baseFolder], _checkExists = 0)
        gmProcessors.AddIncludeFolders([baseFolder])

