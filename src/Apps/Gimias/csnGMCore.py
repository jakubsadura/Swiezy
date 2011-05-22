# Used to configure gmCore
import csnBuild
import csnCilab
from csnGIMIASDef import *
import os.path

# Use gmCore and MITK dependency for compatibility
gmCore = csnBuild.Project("gmCore", "library")
gmCore.AddProjects([gmCoreLight,mitkPlugin] )
