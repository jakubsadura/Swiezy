import csnBuild
from csnGIMIASDef import *

guiBridgeLib = csnBuild.Project("GuiBridgeLib", "library")
guiBridgeLib.AddIncludeFolders(["src"])
guiBridgeLib.AddSources(["src/gblBridge.cpp", "src/gblBridge.h"])
guiBridgeLib.AddSources(["src/gblValueConverter.cpp", "src/gblValueConverter.h"])
guiBridgeLib.AddSources(["src/gblMockController.cpp", "src/gblMockController.h"])
guiBridgeLib.AddSources(["src/gblException.h"])
guiBridgeLib.AddProject(boost)
