# Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
# This software is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
#  Used to build all TpExtLib applications

import csnBuild
import csnCilab
import csnUtility
from csnGIMIASDef import *

tpExtLibApps = csnCilab.CilabModuleProject("TpExtLibApps", "container")
tpExtLibApps.AddLibraries(["glu32.lib"], _WIN32 = 1)
tpExtLibApps.AddApplications([
	"blwxAppTreeCtrl"
	], _applicationDependenciesList =[tpExtLibWxWidgets,itk,vtk])

tpExtLibApps.AddApplications([
	"cmguiLoadImage"
	], _applicationDependenciesList =[wxWidgets,cmgui,vtk],
	_pch = "tpExtLibCmguiPCH.h")

tpExtLibApps.AddApplications([
	"blwxAppEmbeddedWindow"
	], _applicationDependenciesList =[tpExtLibWxWidgets,vtk])

localCommandLinePlugins = []
	
tpExtLibCLPApps = csnCilab.CilabModuleProject("TpExtLibCLPApps", "container")
affineTransform = csnCilab.CommandLinePlugin("AffineTransform")
affineTransform.AddSources( [u'Applications/AffineTransform/AffineTransform.cxx'] )
affineTransform.AddProjects( [itk, generateClp, slicer] )
affineTransform.AddDefinitions(["/bigobj"], _private = 1, _WIN32 = 1) 
localCommandLinePlugins.append( affineTransform )

marchingCubes = csnCilab.CommandLinePlugin("MarchingCubes")
marchingCubes.AddSources( [u'Applications/MarchingCubes/MarchingCubes.cxx'] )
marchingCubes.AddProjects( [itk,vtk, generateClp, slicer] )
localCommandLinePlugins.append( marchingCubes )

binaryThresholdImageFilter = csnCilab.CommandLinePlugin("BinaryThresholdImageFilter")
binaryThresholdImageFilter.AddSources( [u'Applications/BinaryThresholdImageFilter/BinaryThresholdImageFilter.cxx'] )
binaryThresholdImageFilter.AddProjects( [itk, generateClp, slicer] )
localCommandLinePlugins.append( binaryThresholdImageFilter )

tpExtLibCLPApps.AddProjects( localCommandLinePlugins )
commandLinePlugins().AddProjects( localCommandLinePlugins )
