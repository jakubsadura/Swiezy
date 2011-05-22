# Used to configure WflLib
import csnCilab
from csnToolkitOpen import *

wflLib = csnCilab.CilabModuleProject("WflLib", "library")
wflLib.AddLibraryModules(["wflBase", "wflIO"])
wflLib.AddProjects([baseLib,tinyXml,itk])
#wflLib.AddTests(["tests/tlFirstTest/*.*"], cxxTest)
wflLib.SetPrecompiledHeader("WflLibPCH.h")