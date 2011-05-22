# Used to configure lcore
from csnGIMIASDef import *

import csnBuild
import csnCilab
import csnProject
import csnUtility

import os.path
import inspect

import csnVisualStudio2003
import csnVisualStudio2005
import csnVisualStudio2008
if csnBuild.version < 2.22:
    import csnVisualStudio2008x64

# Used to configure gimias
def CreateGIMIAS():
    
    gimias = csnCilab.CilabModuleProject("Gimias", "executable")
    gimias.AddSources(gimias.Glob("GUI/MainApp/src/*.h"), _checkExists = 1)
    gimias.AddSources(gimias.Glob("GUI/MainApp/src/*.cxx"), _checkExists = 1)
    gimias.SetPrecompiledHeader( "GUI/MainApp/gmMainAppPCH.h" )
    gimias.AddIncludeFolders([ gimias.GetBuildFolder() ])
    
    return gimias

commandLinePlugins = csnProject.Project(
	"CommandLinePlugins",
	"container",
	csnUtility.NormalizePath(os.path.dirname(inspect.stack()[1][1])),
	_categories = ["GIMIASCommandLinePlugins"] )

    


