# Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
# This software is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
#  Used to build all GuiBridgeLib projects

import csnBuild
import csnCilab
import csnUtility
from csnGIMIASDef import *

guiBridgeLibAll = csnCilab.CilabModuleProject("GuiBridgeLibAll", "library")
guiBridgeLibAll.AddProjects( [guiBridgeLib, guiBridgeLibWxWidgets] )

