# Used to configure DicomAPI
import csnBuild
import csnCilab
from csnToolkitOpen import *

dcmAPIData = csnCilab.CilabModuleProject("dcmAPIData", "dll")
dcmAPIData.AddLibraryModules(["dcmData"])
dcmAPIData.AddProjects([dcmtk, boost, cilabMacros, baseLib])
dcmAPIData.AddDefinitions(["-DHAVE_CONFIG_H"],_private =1, _NOT_WIN32 =1)
dcmAPIData.SetPrecompiledHeader("DcmDataPCH.h")

dcmAPIIO = csnCilab.CilabModuleProject("dcmAPIIO", "dll")
dcmAPIIO.AddLibraryModules(["dcmIO"])
dcmAPIIO.AddProjects([dcmAPIData, itk, zlib, baseLibVTK, baseLibSignal, baseLibITK])
dcmAPIIO.AddDefinitions(["-DHAVE_CONFIG_H"],_private =1, _NOT_WIN32 =1)
dcmAPIIO.SetPrecompiledHeader("DcmIOPCH.h")

# wrapper
dcmAPI = csnCilab.CilabModuleProject("DcmAPI", "library")
dcmAPI.AddProjects([dcmAPIData, dcmAPIIO])
dcmAPI.AddTests(["tests/dcmAPI/*.*"], cxxTest)

