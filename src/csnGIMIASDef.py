import csnBuild
import csnCilab
from csnToolkitOpen import *

######################################################################################
# Third Party
    
def mitk():
    return csnCilab.LoadThirdPartyModule('MITK_SVN2', 'csnMITK').mitk

def wxWidgets():
    return csnCilab.LoadThirdPartyModule('WXWIDGETS-2.8.10', 'csnWxWidgets').wxWidgets
    
def wxMathPlot():
    return csnCilab.LoadThirdPartyModule('WXMATHPLOT-0.1.0', 'csnWxMathPlot').wxMathPlot
    
def libElf():
    return csnCilab.LoadThirdPartyModule('LIBELF', 'csnLibElf').libElf
    
def expat():
	import EXPAT.csnExpat
	return EXPAT.csnExpat.exPat

def cmgui():
    return csnCilab.LoadThirdPartyModule('CMGUI', 'csnCMGUI').cmGUI
    
######################################################################################
# Modules

def tpExtLibITK():
    import Modules.TpExtLib.csnTpExtLib
    return Modules.TpExtLib.csnTpExtLib.tpExtLibITK

def tpExtLibMITK():
    import Modules.TpExtLib.csnTpExtLib
    return Modules.TpExtLib.csnTpExtLib.tpExtLibMITK
    
def tpExtLibWxWidgets():
    import Modules.TpExtLib.csnTpExtLib
    return Modules.TpExtLib.csnTpExtLib.tpExtLibWxWidgets

def guiBridgeLib():
    import Modules.GuiBridgeLib.csnGuiBridgeLib
    return Modules.GuiBridgeLib.csnGuiBridgeLib.guiBridgeLib

def guiBridgeLibQt():
    import Modules.GuiBridgeLib.csnGuiBridgeLib
    return Modules.GuiBridgeLib.csnGuiBridgeLib.guiBridgeLibQt

def guiBridgeLibWxWidgets():
    import Modules.GuiBridgeLib.csnGuiBridgeLib
    return Modules.GuiBridgeLib.csnGuiBridgeLib.guiBridgeLibWxWidgets

def wxMitk():
    import Modules.wxMitk.csnWxMitk
    return Modules.wxMitk.csnWxMitk.wxMitk

def dynLib():
    import Modules.DynLib.csnDynLib
    return Modules.DynLib.csnDynLib.dynLib

def dynWxAGUILib():
    import Modules.DynLib.csnDynLib
    return Modules.DynLib.csnDynLib.dynWxAGUILib

def wflLib():
    import Modules.WflLib.csnWflLib
    return Modules.WflLib.csnWflLib.wflLib


######################################################################################
# GIMIAS
   
def CreateGimiasApp():
    import Apps.Gimias.csnGIMIAS
    return Apps.Gimias.csnGIMIAS.gimias
   
def gmCommonObjects():
    import Apps.Gimias.Core.CommonObjects.csnGMCommonObjects
    return Apps.Gimias.Core.CommonObjects.csnGMCommonObjects.gmCommonObjects

def gmDataHandling():
    import Apps.Gimias.Core.DataHandling.csnGMDataHandling
    return Apps.Gimias.Core.DataHandling.csnGMDataHandling.gmDataHandling

def gmKernel():
    import Apps.Gimias.Core.Kernel.csnGMKernel
    return Apps.Gimias.Core.Kernel.csnGMKernel.gmKernel

def gmIO():
    import Apps.Gimias.Core.IO.csnGMIO
    return Apps.Gimias.Core.IO.csnGMIO.gmIO

def gmFrontEndPlugin():
    import Apps.Gimias.Core.FrontEndPlugin.csnGMFrontEndPlugin
    return Apps.Gimias.Core.FrontEndPlugin.csnGMFrontEndPlugin.gmFrontEndPlugin

def gmProcessors():
    import Apps.Gimias.Core.Processors.csnGMProcessors
    return Apps.Gimias.Core.Processors.csnGMProcessors.gmProcessors

def gmWidgets():
    import Apps.Gimias.GUI.Widgets.csnGMWidgets
    return Apps.Gimias.GUI.Widgets.csnGMWidgets.gmWidgets

def gmCoreLight():
    import Apps.Gimias.csnGMCoreLight
    return Apps.Gimias.csnGMCoreLight.gmCoreLight

def gmCore():
    import Apps.Gimias.csnGMCore
    return Apps.Gimias.csnGMCore.gmCore

    
######################################################################################
# Plugins
    
def sandboxPlugin():
    import Apps.Plugins.SandboxPlugin.csnSandboxPlugin
    return Apps.Plugins.SandboxPlugin.csnSandboxPlugin.sandboxPlugin

def signalViewerPlugin():
    import Apps.Plugins.SignalViewerPlugin.csnSignalViewerPlugin
    return Apps.Plugins.SignalViewerPlugin.csnSignalViewerPlugin.signalViewerPlugin

def meshEditorPlugin():
    import Apps.Plugins.MeshEditorPlugin.csnMeshEditorPlugin
    return Apps.Plugins.MeshEditorPlugin.csnMeshEditorPlugin.meshEditorPlugin

def cardioToolsPlugin():
    import Apps.Plugins.CardioToolsPlugin.csnCardioToolsPlugin
    return Apps.Plugins.CardioToolsPlugin.csnCardioToolsPlugin.cardioToolsPlugin

def neuroToolsPlugin():
    import Apps.Plugins.NeuroToolsPlugin.csnNeuroToolsPlugin
    return Apps.Plugins.NeuroToolsPlugin.csnNeuroToolsPlugin.neuroToolsPlugin

def imageToolsPlugin():
    import Apps.Plugins.ImageToolsPlugin.csnImageToolsPlugin
    return Apps.Plugins.ImageToolsPlugin.csnImageToolsPlugin.imageToolsPlugin
    
def manualSegmentationPlugin():
    import Apps.Plugins.ManualSegmentationPlugin.csnManualSegmentationPlugin
    return Apps.Plugins.ManualSegmentationPlugin.csnManualSegmentationPlugin.manualSegmentationPlugin
    
def dicomPlugin():
    import Apps.Plugins.DicomPlugin.csnDicomPlugin
    return Apps.Plugins.DicomPlugin.csnDicomPlugin.dicomPlugin

def sceneViewPlugin():
    import Apps.Plugins.SceneViewPlugin.csnSceneViewPlugin
    return Apps.Plugins.SceneViewPlugin.csnSceneViewPlugin.sceneViewPlugin

def cmguiPlugin():
    import Apps.Plugins.CmguiPlugin.csnCmguiPlugin
    return Apps.Plugins.CmguiPlugin.csnCmguiPlugin.cmguiPlugin

def mitkPlugin():
    import Apps.Plugins.MITKPlugin.csnMITKPlugin
    return Apps.Plugins.MITKPlugin.csnMITKPlugin.mitkPlugin

def tavernaPlugin():
    import Apps.Plugins.TavernaPlugin.csnTavernaPlugin
    return Apps.Plugins.TavernaPlugin.csnTavernaPlugin.tavernaPlugin

def commandLinePlugins():
    import Apps.Gimias.csnGIMIASCommon
    return Apps.Gimias.csnGIMIASCommon.commandLinePlugins
    
def CollectCommandLinePlugins():
	import Modules.TpExtLib.csnTpExtLibApps
	import SLICERAPPS.csnSlicerApps
