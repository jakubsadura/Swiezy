# Used to configure BaseLib
import csnBuild
import csnCilab
from csnToolkitOpen import *

def CreateCilabLibraryModule(_name, _libmodules, _type = "library"):
	lib = csnCilab.CilabModuleProject(_name, _type)
	lib.AddLibraryModules(_libmodules)
	lib.AddDefinitions(["-fpermissive", "-DHAVE_CONFIG_H"], _private = 1, _NOT_WIN32 = 1) 
	return lib

baseLib = CreateCilabLibraryModule("BaseLib",
	["blCommon",
	"blUtilities"])
baseLib.AddProjects([cilabMacros, boost, log4cplus])
baseLib.SetPrecompiledHeader("baseLibPCH.h")
baseLib.AddFilesToInstall(baseLib.Glob("libmodules/blCommon/resource/logging.conf"), "resource")
baseLib.AddLibraries(["psapi"], _WIN32 = 1)
baseLib.AddTests(["tests/blCommon/*.*"], cxxTest)

baseLibSignal = CreateCilabLibraryModule("BaseLibSignal", ["blSignal"], _type = "dll")
baseLibSignal.AddProjects([itk,baseLib])
baseLibSignal.SetPrecompiledHeader("baseLibSignalPCH.h")
baseLibSignal.AddFilesToInstall(baseLibSignal.Glob("libmodules/blSignal/resource/GDFEventCodes.txt"), "resource")
baseLibSignal.AddFilesToInstall(baseLibSignal.Glob("libmodules/blSignal/resource/CISTIBEventCodes.txt"), "resource")

baseLibNumericData = CreateCilabLibraryModule("BaseLibNumericData", ["blNumericData"])
baseLibNumericData.AddProjects([baseLib,tinyXml])
#baseLibNumericData.SetPrecompiledHeader("baseLibNumericDataPCH.h")

baseLibITK = CreateCilabLibraryModule("BaseLibITK",
	["blPDShape",
	"blImage",
	"blImageProperties",
	"blImageUtilities",
	"blMath",
	"blPDAlignment",
	"blUtilitiesITK"])
baseLibITK.AddProjects([itk, baseLib])
baseLibITK.AddIncludeFolders([itk().netlibIncludeFolder])
#baseLibITK.AddTests(["tests/blUblasTest/*.*"], cxxTest)
#baseLibITK.AddTests(["tests/blLinearAlgebraOperationsTest/*.*"], cxxTest)
#baseLibITK.AddTests(["libmodules/blMath/tests/*.*"], cxxTest)
baseLibITK.SetPrecompiledHeader("baseLibITKPCH.h")

baseLibVTK = CreateCilabLibraryModule("BaseLibVTK", 
	["blUtilitiesVTK",
	"blIO",
	"blCardioModel",
	"blSimplexDeformableModels"],
	_type = "dll")
baseLibVTK.AddProjects([baseLib, baseLibITK, vtk, cgns, toolkit])
#baseLibVTK.AddTests(["tests/blVTKSmartPointerTest/*.*"], cxxTest)
baseLibVTK.AddTests(["tests/blShapeUtilsTest/*.*"], cxxTest)
#baseLibVTK.AddTests(["tests/blVTKToStringTest/*.*"], cxxTest)
baseLibVTK.SetPrecompiledHeader("baseLibVTKPCH.h")
baseLibVTK.AddTests(["tests/blUtilitiesVTK/*.*"], cxxTest)
baseLibVTK.AddDefinitions(["/bigobj"], _private = 1, _WIN32 = 1) 

baseLibTetgen = CreateCilabLibraryModule("BaseLibTetgen",
	["blMeshIO",
	"blTetgen"])
baseLibTetgen.AddProjects([baseLib])
