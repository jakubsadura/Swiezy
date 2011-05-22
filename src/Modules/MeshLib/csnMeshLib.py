# Used to configure meshLib
import csnCilab
from csnToolkitOpen import *

meshLib = csnCilab.CilabModuleProject("MeshLib", "library")
meshLib.AddLibraryModules(["meMesh", "meTools", "mePolyline", "meMeshFilters"])
meshLib.AddProjects([baseLibVTK, baseLibSignal, vtk, cxxTest, netGen, toolkit, baseLibTetgen])
meshLib.SetPrecompiledHeader("meshLibPCH.h")
# disable tetgen main function defined in tetgen.cxx file
meshLib.AddDefinitions(["-DTETLIBRARY"], _private = 1) 

meshLib.AddTests(["tests/meTestMeshFilters/*.*"], cxxTest)
#meshLib.testProject.SetPrecompiledHeader("meshLibPCH.h")



