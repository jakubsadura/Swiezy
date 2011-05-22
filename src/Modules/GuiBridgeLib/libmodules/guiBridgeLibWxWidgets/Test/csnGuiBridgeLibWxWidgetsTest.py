import csnBuild
from csnGIMIASDef import *

gblWxWidgetsTestGUI = csnBuild.Project("gblWxWidgetsTestGUI", "library")
gblWxWidgetsTestGUI.AddIncludeFolders(["src"])
gblWxWidgetsTestGUI.AddSources(["src/gblWxWidgetsTestGUI.cpp", "src/gblWxWidgetsTestGUI.h"])
gblWxWidgetsTestGUI.AddProjects([guiBridgeLibWxWidgets])
gblWxWidgetsTestGUI.AddProjects([cxxTest])

guiBridgeLibWxWidgetsTest = csnBuild.Project("GuiBridgeLibWxWidgetsTest", "executable")
guiBridgeLibWxWidgetsTest.AddIncludeFolders(["src"])
guiBridgeLibWxWidgetsTest.AddSources(["src/BridgeLibWxWidgetsTest.cpp", "src/BridgeLibWxWidgetsTest.h"])
guiBridgeLibWxWidgetsTest.AddProjects([gblWxWidgetsTestGUI])
