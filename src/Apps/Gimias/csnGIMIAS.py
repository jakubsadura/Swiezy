# Used to configure lcore
import csnBuild
import csnCilab
from csnGIMIASDef import *
import os.path
import csnGIMIASCommon

# Used to configure gimias

# GIMIAS
gimias = csnGIMIASCommon.CreateGIMIAS( )
gimias.AddProjects([
	gmCore
])

CollectCommandLinePlugins( )

gimias.AddProjects([
    tavernaPlugin,
	cardioToolsPlugin,
	dicomPlugin,
	imageToolsPlugin,
	neuroToolsPlugin,
	meshEditorPlugin,
	sandboxPlugin,
	sceneViewPlugin,
	signalViewerPlugin,
	manualSegmentationPlugin,
	cmguiPlugin,
    tavernaPlugin,
    commandLinePlugins
], _dependency = False )
