# Used to configure CILabMacros
import csnBuild
import csnCilab
from csnGIMIASDef import *

cilabMacros = csnCilab.CilabModuleProject("CilabMacros", "library")
cilabMacros.AddLibraryModules(["CILabMacros", "Loki"])

