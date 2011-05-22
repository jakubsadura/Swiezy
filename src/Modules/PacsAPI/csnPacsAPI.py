# Used to configure pacsAPI
import csnCilab
from csnToolkitOpen import *

pacsAPI = csnCilab.CilabModuleProject("PacsAPI", "dll")
pacsAPI.AddLibraryModules(["pacsAPI"])
pacsAPI.AddProjects([dcmtk, dcmAPI])
pacsAPI.AddTests(["tests/pacsAPITests/*.*"], cxxTest)
pacsAPI.SetPrecompiledHeader("pacsAPIPCH.h")