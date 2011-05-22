# Used to configure DicomPlugin
import csnCilab
from csnGIMIASDef import *

dicomPlugin = csnCilab.GimiasPluginProject("DicomPlugin")

projects = [
    baseLibITK,
    tpExtLibMITK, 
    gmCore,  
    boost, 
    cilabMacros,
    dcmAPI,
    pacsAPI	
]
widgetModules = ["DicomWorkingAreaWidget", "DicomConnectToPACSWidget", "DICOMTree"]

dicomPlugin.AddProjects(projects)
dicomPlugin.AddSources(["*.h", "*.cpp","*.cxx"])
dicomPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
dicomPlugin.AddWidgetModules(widgetModules)
dicomPlugin.AddIncludeFolders(["processors"])
dicomPlugin.SetPrecompiledHeader("DicomPluginPCH.h")
dicomPlugin.AddDefinitions(["/bigobj"],  _WIN32 = 1) 

dicomPlugin.AddFilesToInstall(dicomPlugin.Glob("widgets/resource/*.ico"), "resource")


