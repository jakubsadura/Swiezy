# Used to configure WflLibApps
import csnCilab
import csnUtility
from csnToolkitOpen import *
from csnGIMIASDef import * 


wflLibApps = csnCilab.CilabModuleProject("WflLibApps", "library")
wflLibApps.AddSources( [csnUtility.GetDummyCppFilename()] )
wflLibApps.AddProjects( [wflLib] )
wflLibApps.AddApplications([ "wflAppReadT2" ], _holderName="WflLibApplications")
