/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


// own include file
#include "CmguiPluginMeshFittingReader.h"
#include "CmguiRegionImpl.h"

// Third Party Library includes
#include "boost/filesystem/path.hpp"
#include "boost/foreach.hpp"
#include "tinyxml.h"

#include <iomanip>

extern "C" {
#include "region/cmiss_region.h"
}

using namespace CmguiPlugin;


#define SCHEME_STEP "%%%STEP%%%"


MeshFittingReader::MeshFittingReader()
{
    m_ValidExtensionsList.push_back(".fitml");
    m_ValidTypesList.push_back(Core::ImageTypeId);
}

MeshFittingReader::~MeshFittingReader()
{
}

bool MeshFittingReader::ReadHeader() {
    return true;
}

void MeshFittingReader::Load(std::string filename) {
    // get directory from filename
    boost::filesystem::path filePath(filename);
    boost::filesystem::path parent = filePath.parent_path();
    
    // load file
    TiXmlDocument doc(filename);
    TiXmlHandle docHandle(&doc);
    bool loadOkay = doc.LoadFile();
    assert(loadOkay); // todo: replace this by an exception
    
    // get main node
    TiXmlHandle mainNode = docHandle.FirstChild("MeshFitting");
    
    // read mask
    std::string regionNameMask = filePath.filename() + "_Mask";
    TiXmlElement* maskNode = mainNode.FirstChild("Mask").ToElement();

	Core::DataEntity::Pointer dataEntity;
    dataEntity = LoadFiles(maskNode, parent.file_string(), regionNameMask);

	// set mask properties
	blTagMap::Pointer rendering = blTagMap::New( );
	blTagMap::Pointer commands = blTagMap::New( );
	commands->AddTag( "gfx create material red", 1 );
	commands->AddTag( "gfx modify g_element ${region} general element_discretization 1 scene ${scene}", 1 );
	commands->AddTag( "gfx modify g_element ${region} surfaces material red scene ${scene}", 1 );
	commands->AddTag( "gfx modify g_element ${region} lines material blue line_width 1 scene ${scene}", 1 );
	rendering->AddTag( "CMGUI commands", commands );
	dataEntity->GetMetadata()->AddTag( "Rendering", rendering );
	SetOutputDataEntity(0, dataEntity);

    // read fitted mesh
    std::string regionNameFitting = filePath.filename() + "_Fitting";
    TiXmlElement* fittingNode = mainNode.FirstChild("Fitting").ToElement();
    dataEntity = LoadFiles(fittingNode, parent.file_string(), regionNameFitting);

	// set fitted mesh properties
	rendering = blTagMap::New( );
	commands = blTagMap::New( );
	commands->AddTag( "gfx create material blue", 1 );
	commands->AddTag( "gfx modify g_element ${region} general element_discretization 3 scene ${scene}", 1 );
	commands->AddTag( "gfx modify g_element ${region} surfaces material blue scene ${scene}", 1 );
	commands->AddTag( "gfx modify g_element ${region} lines material red line_width 1 scene ${scene}", 1 );
	commands->AddTag( "gfx define face egroup ${region}", 1 );
	rendering->AddTag( "CMGUI commands", commands );
	dataEntity->GetMetadata()->AddTag( "Rendering", rendering );
	AddOutputDataEntity( dataEntity );

    // set camera
    TiXmlElement* cameraNode = mainNode.FirstChild("Camera").ToElement();

}

Core::DataEntity::Pointer MeshFittingReader::LoadFiles(
	TiXmlElement* filesNode, boost::filesystem::path directory, std::string name) 
{
	Core::DataEntity::Pointer dataEntity = Core::DataEntity::New( );
	dataEntity->GetMetadata()->SetName( name );
	dataEntity->Resize( 1, typeid( Cmiss_region_id ) );

	Cmiss_region_id region;
	dataEntity->GetProcessingData( region );
    
    std::list<std::string> files;
    LoadFileList(filesNode, files);
    BOOST_FOREACH(std::string filename, files) {

		std::string completeFilename = (directory / filename).file_string();
		Cmiss_region_read_file(region, completeFilename.c_str());
    }

	if ( CmguiRegionImpl::IsSurfaceMesh( region ) )
	{
		dataEntity->SetType( Core::SurfaceMeshTypeId );
	}
	else
	{
		dataEntity->SetType( Core::VolumeMeshTypeId );
	}

	return dataEntity;
}

void MeshFittingReader::LoadFileList(TiXmlElement* fileListNode, std::list<std::string>& files) {
    // for all elements that describe file lists
    TiXmlElement* child = fileListNode->FirstChild()->ToElement();
    while (child)
    {
        // read history and store it
        std::string type = child->ValueStr();
        if (type == "Scheme") {
            LoadFileListScheme(child, files);
        } else if (type == "File") {
            LoadFileListExplicit(child, files);
        }
        
        // next
        child = child->NextSiblingElement();
    }
}

void MeshFittingReader::LoadFileListExplicit(TiXmlElement* fileListNode, std::list<std::string>& files) {
    std::string text = fileListNode->GetText();
    files.push_back(text);
}

void MeshFittingReader::LoadFileListScheme(TiXmlElement* fileListNode, std::list<std::string>& files) {
    int rangeMin;
    fileListNode->QueryIntAttribute("rangeMin", &rangeMin);
    
    int rangeMax;
    fileListNode->QueryIntAttribute("rangeMax", &rangeMax);
    
    std::string scheme = fileListNode->Attribute("name");
    
    for (int i = rangeMin; i <= rangeMax; i++) {
        size_t found = scheme.find(SCHEME_STEP);
        std::string filename = scheme;
        if (found != std::string::npos) {
            std::stringstream numberString;
            numberString << std::setfill('0') << std::setw(3) << i;
            filename.replace(found, strlen(SCHEME_STEP), numberString.str());
        }
        files.push_back(filename);
    }
}

void MeshFittingReader::LoadCamera(TiXmlElement* cameraNode) {
}

void MeshFittingReader::Update() {
    BOOST_FOREACH(std::string filename, m_Filenames) {
        Load(filename);
    }
}

boost::any MeshFittingReader::ReadSingleTimeStep( 
        int iTimeStep, 
        const std::string &filename ) {
    return NULL;
}
