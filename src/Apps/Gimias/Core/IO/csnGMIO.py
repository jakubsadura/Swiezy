# Used to configure coreIO
import csnBuild
from csnGIMIASDef import *
import os.path
import csnProject

gmItkDataEntityIO = csnBuild.Project("gmItkDataEntityIO", "dll")
gmItkDataEntityIO.AddProjects([ itk, gmDataHandling ]) 
gmItkDataEntityIO.AddSources(["src/itkDataEntityIO/*.cxx" ], _checkExists = 0)
gmItkDataEntityIO.AddSources(["src/itkDataEntityIO/*.txx" ], _checkExists = 0)
gmItkDataEntityIO.AddSources(["src/itkDataEntityIO/*.h" ], _checkExists = 0)
gmItkDataEntityIO.SetPrecompiledHeader("gmitkDataEntityIOPCH.h")
gmItkDataEntityIO.AddIncludeFolders(["src/itkDataEntityIO"])

gmIO = csnBuild.Project("gmIO", "dll")
gmIO.AddProjects([ 
  cilabMacros, 
  xerces, 
  itk, 
  baseLibVTK,
  boost,
  dcmAPI,
  gmCommonObjects,
  gmDataHandling,
  cgns,
  hdf5,
  gmItkDataEntityIO
])

gmIO.SetPrecompiledHeader("gmIOPCH.h")

gmIO.AddSources(["src/*.cxx" ], _checkExists = 0)
gmIO.AddSources(["src/*.txx" ], _checkExists = 0)
gmIO.AddSources(["src/*.h" ], _checkExists = 0)
gmIO.AddIncludeFolders(["src"])

IOFolders = gmIO.Glob("src/DataFormatObjects/*")
for IOFolder in IOFolders:
    if os.path.isdir(IOFolder):
        gmIO.AddSources(["%s/*.cxx" % IOFolder], _checkExists = 0)
        gmIO.AddSources(["%s/*.txx" % IOFolder], _checkExists = 0)
        gmIO.AddSources(["%s/*.h" % IOFolder], _checkExists = 0)
        gmIO.AddIncludeFolders([IOFolder])
        

gmIO.AddDefinitions(["/bigobj"], _private = 1, _WIN32 = 1) 
