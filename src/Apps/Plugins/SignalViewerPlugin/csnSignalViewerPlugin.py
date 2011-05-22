# Used to configure signalViewerPlugin
import csnBuild
import csnCilab
import csnUtility

from csnGIMIASDef import *

signalViewerPlugin = csnCilab.GimiasPluginProject("SignalViewerPlugin")

projects = [
    gmCore,
    wxMathPlot
]
signalViewerPlugin.AddProjects(projects)

signalViewerPlugin.AddSources(["*.cxx", "*.h"])
signalViewerPlugin.AddSources(["processors/*.cxx", "processors/*.h"])
signalViewerPlugin.AddIncludeFolders(["processors"])

widgetModules = [
  "PlotWidget",
  "SignalTimePropagationPanelWidget",
  "SignalReaderWidget"
  ]
signalViewerPlugin.AddWidgetModules(widgetModules, _useQt = 0)

signalViewerPlugin.SetPrecompiledHeader("SignalViewerPluginPCH.h")

