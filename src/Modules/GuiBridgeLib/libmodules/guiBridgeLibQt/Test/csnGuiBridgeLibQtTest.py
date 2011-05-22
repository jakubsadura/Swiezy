import csnBuild
import csnCilab
from csnGIMIASDef import *

gblQtTestGUI = csnBuild.Project("gblQtTestGUI", "library")
gblQtTestGUI.AddIncludeFolders(["src"])
gblQtTestGUI.AddSources(["src/gblQtTestGUI.cpp", "src/gblQtTestGUI.h"])
gblQtTestGUI.AddProjects([guiBridgeLibQt, qt])

guiBridgeLibQtTest = csnBuild.Project("GuiBridgeLibQtTest", "executable")
guiBridgeLibQtTest.AddIncludeFolders(["src"])
guiBridgeLibQtTest.AddSources(["src/BridgeLibQtTest.cpp", "src/BridgeLibQtTest.h"])
guiBridgeLibQtTest.AddProjects([gblQtTestGUI, cxxTest, qt])
