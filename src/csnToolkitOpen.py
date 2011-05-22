import csnBuild
import csnCilab

######################################################################################
# Third Party
    
def ann():
    return csnCilab.LoadThirdPartyModule('ANN', 'csnANN').ann

def itk():
    return csnCilab.LoadThirdPartyModule('ITK-3.20', 'csnITK').itk

def vtk():
    return csnCilab.LoadThirdPartyModule('VTK-5.6.1', 'csnVTK').vtk

def xerces():
    return csnCilab.LoadThirdPartyModule('XERCES-3.0.1', 'csnXerces').xerces

def netGen():
    return csnCilab.LoadThirdPartyModule('NETGEN-4.5', 'csnNetGen').netGen

def cxxTest():
    return csnCilab.LoadThirdPartyModule('CXXTEST', 'csnCxxTest').cxxTest

def dcmtk():
    return csnCilab.LoadThirdPartyModule('DCMTK-3.5.4', 'csnDCMTK').dcmtk
    
def boost():
    return csnCilab.LoadThirdPartyModule('BOOST-1.45.0', 'csnBoost').boost

def zlib():
    return csnCilab.LoadThirdPartyModule('ZLIB', 'csnZLIB').zlib 

def cgns():
    return csnCilab.LoadThirdPartyModule( 'CGNS', 'csnCGNS').cgns

def hdf5():
    return csnCilab.LoadThirdPartyModule( 'HDF5', 'csnHDF5').hdf5
	
def tinyXml():
	import TinyXml.csnTinyXml
	return TinyXml.csnTinyXml.tinyXml

def log4cplus():
	return csnCilab.LoadThirdPartyModule('LOG4CPLUS', 'csnLOG4CPLUS').log4cplus

def slicer():
	return csnCilab.LoadThirdPartyModule('SLICER', 'csnSlicer').slicer

def generateClp():
	return csnCilab.LoadThirdPartyModule('SLICER', 'csnSlicer').generateClp

def tclap():
	return csnCilab.LoadThirdPartyModule('SLICER', 'csnSlicer').tclap

    
######################################################################################
# Modules

def baseLib():
    import Modules.BaseLib.csnBaseLib
    return Modules.BaseLib.csnBaseLib.baseLib
    
def baseLibITK():
    import Modules.BaseLib.csnBaseLib
    return Modules.BaseLib.csnBaseLib.baseLibITK
    
def baseLibVTK():
    import Modules.BaseLib.csnBaseLib
    return Modules.BaseLib.csnBaseLib.baseLibVTK

def baseLibSignal():
    import Modules.BaseLib.csnBaseLib
    return Modules.BaseLib.csnBaseLib.baseLibSignal
    
def baseLibNumericData():
    import Modules.BaseLib.csnBaseLib
    return Modules.BaseLib.csnBaseLib.baseLibNumericData
    
def baseLibCxxTest():
    import Modules.BaseLib.csnBaseLib
    return Modules.BaseLib.csnBaseLib.baseLibCxxTest

def baseLibTetgen():
    import Modules.BaseLib.csnBaseLib
    return Modules.BaseLib.csnBaseLib.baseLibTetgen

def cilabMacros():
    import Modules.CILabMacros.csnCILabMacros
    return Modules.CILabMacros.csnCILabMacros.cilabMacros

def meshLib():
    import Modules.MeshLib.csnMeshLib
    return Modules.MeshLib.csnMeshLib.meshLib

def dcmAPI():
    import Modules.DcmAPI.csnDcmAPI
    return Modules.DcmAPI.csnDcmAPI.dcmAPI
    
def dcmAPIData():
    import Modules.DcmAPI.csnDcmAPI
    return Modules.DcmAPI.csnDcmAPI.dcmAPIData
    
def dcmAPIIO():
    import Modules.DcmAPI.csnDcmAPI
    return Modules.DcmAPI.csnDcmAPI.dcmAPIIO
    
def pacsAPI():
    import Modules.PacsAPI.csnPacsAPI
    return Modules.PacsAPI.csnPacsAPI.pacsAPI



def CreateToolkitHeader(toolkit):
    """ Creates a header file with useful stuff for projects that use the CISTIB toolkit """
    filename = "%s/CISTIBToolkit.h" % toolkit.GetBuildFolder()
    f = open(filename, 'w')
    f.write("#ifndef CISTIBTOOLKIT_H\n")
    f.write("#define CISTIBTOOLKIT_H\n")
    f.write("\n")
    f.write("// Automatically generated file, do not edit.\n")
    f.write("\n")
    if hasattr(toolkit, "context"):
        f.write("#define CISTIB_TOOLKIT_FOLDER \"%s/..\"\n" % toolkit.GetSourceRootFolder())
        # check is build folder is public or private
        buildFolder = None
        if hasattr(toolkit.context, "buildFolder"):
            buildFolder = toolkit.context.buildFolder
        else:
            buildFolder = toolkit.context.GetBuildFolder()
        f.write("#define CISTIB_TOOLKIT_BUILD_FOLDER \"%s\"\n" % buildFolder)
    else:
        f.write("#define CISTIB_TOOLKIT_FOLDER \"%s/..\"\n" % toolkit.sourceRootFolder)
        f.write("#define CISTIB_TOOLKIT_BUILD_FOLDER \"%s\"\n" % toolkit.compiler.GetBuildFolder())
    
    f.write("\n")
    f.write("#endif // CISTIBTOOLKIT_H\n")
    f.close()
    
requiredVersion = 2.15

if not hasattr(csnBuild, "version") or csnBuild.version < requiredVersion:
    assert False, "\n\nCSnake version must be at least %d. Please update CSnake\n" % requiredVersion
    
toolkit = csnBuild.Project("CISTIBToolkit", "library")
toolkit.AddCustomCommand( CreateToolkitHeader )
