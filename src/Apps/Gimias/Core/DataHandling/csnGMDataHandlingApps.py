
import csnBuild
import csnCilab
import csnUtility
from csnGIMIASDef import *

csnGMDataHandlingApps = csnCilab.CilabModuleProject("csnGMDataHandlingApps", "library")
csnGMDataHandlingApps.AddSources( [csnUtility.GetDummyCppFilename()] )
csnGMDataHandlingApps.AddProjects( [gmDataHandling] )
