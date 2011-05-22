import csnBuild
import csnCilab
from csnGIMIASDef import *

guiBridgeLibTest = csnBuild.Project("GuiBridgeLibTest", "executable")
guiBridgeLibTest.AddIncludeFolders(["src"])
guiBridgeLibTest.AddSources(["src/BridgeLibTest.cpp", "src/BridgeLibTest.h"])
guiBridgeLibTest.AddProjects([guiBridgeLib, boost, cxxTest])
		
