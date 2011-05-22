import csnBuild
from guiBridgeLib import csnGuiBridgeLib
from csnGIMIASDef import *

guiBridgeLibWxWidgets = csnBuild.Project("GuiBridgeLibWxWidgets", "library")
guiBridgeLibWxWidgets.AddIncludeFolders(["src"])
guiBridgeLibWxWidgets.AddSources(["src/gblBridgeWxWidgets.cpp", "src/gblBridgeWxWidgets.h"])
guiBridgeLibWxWidgets.AddSources(["src/gblWxTextCtrlValueProxy.cpp"])
guiBridgeLibWxWidgets.AddSources(["src/gblWxTextCtrlValueProxy.h"])
guiBridgeLibWxWidgets.AddProjects([csnGuiBridgeLib.guiBridgeLib, wxWidgets] )
