# Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
# This software is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
#  Used to build all BaseLib projects

import csnBuild
import csnCilab
import csnUtility
from csnGIMIASDef import *

baseLibAll = csnCilab.CilabModuleProject("BaseLibAll", "library")
baseLibAll.AddProjects( [baseLib, baseLibSignal, baseLibNumericData, baseLibITK, baseLibVTK, baseLibDICOM, baseLibMITK, baseLibWxWidgets] )

