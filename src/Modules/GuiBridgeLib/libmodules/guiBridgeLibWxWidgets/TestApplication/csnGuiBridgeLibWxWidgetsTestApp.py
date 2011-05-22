import csnBuild
from csnGIMIASDef import *

guiBridgeLibWxWidgetsTestApp = csnBuild.Project("GuiBridgeLibWxWidgetsTest", "executable")
guiBridgeLibWxWidgetsTestApp.AddIncludeFolders(["src"])
guiBridgeLibWxWidgetsTestApp.AddSources(["src/*.cpp", "src/*.h"])
guiBridgeLibWxWidgetsTestApp.AddProjects([guiBridgeLibWxWidgets])
