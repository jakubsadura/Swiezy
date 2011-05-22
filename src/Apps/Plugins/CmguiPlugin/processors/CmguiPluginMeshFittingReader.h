/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginMeshFittingReader_H
#define _CmguiPluginMeshFittingReader_H

#include "CmguiPluginCmgui.h"

#include "coreBaseDataEntityReader.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"

// Third Party Library includes
#include "boost/filesystem/path.hpp"
#include "tinyxml.h"


namespace CmguiPlugin {


/**
\brief 
\author Martin Steghöfer
\date Oct 2010
\ingroup CMGUIPlugin
*/
class MeshFittingReader : public Core::IO::BaseDataEntityReader
{
public:
	coreDeclareSmartPointerClassMacro(MeshFittingReader, Core::IO::BaseDataEntityReader)

    virtual bool ReadHeader( );
    
    virtual void Update();
    
    //virtual bool Read(const std::vector< std::string > &filenames);
    
    virtual boost::any ReadSingleTimeStep(int iTimeStep, const std::string &filename );
    
protected:
    MeshFittingReader();
    virtual ~MeshFittingReader();
    
private:
    void Load(std::string filename);
	Core::DataEntity::Pointer LoadFiles(TiXmlElement* filesNode, boost::filesystem::path directory, std::string name);
    void LoadFileList(TiXmlElement* fileListNode, std::list<std::string>& files);
    void LoadCamera(TiXmlElement* cameraNode);
    void LoadFileListScheme(TiXmlElement* fileListNode, std::list<std::string>& files);
    void LoadFileListExplicit(TiXmlElement* fileListNode, std::list<std::string>& files);
    
private:
    coreDeclareNoCopyConstructors(MeshFittingReader);
    
};

} // namespace CmguiPlugin

#endif //_CmguiPluginMeshFittingReader_H

