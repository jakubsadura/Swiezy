/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


// own include file
#include "CmguiPluginMechanicalSimulationReader.h"
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


MechanicalSimulationReader::MechanicalSimulationReader()
{
    m_ValidExtensionsList.push_back(".msml");
    m_ValidTypesList.push_back(Core::ImageTypeId);
}

MechanicalSimulationReader::~MechanicalSimulationReader()
{
}

bool MechanicalSimulationReader::ReadHeader() {
    return true;
}

void MechanicalSimulationReader::Load(std::string filename) {
    //std::cout << "!!!!!! Load " << filename << std::endl;
    
    // get directory from filename
    boost::filesystem::path filePath(filename);
    boost::filesystem::path parent = filePath.parent_path();
    
    // load file
    TiXmlDocument doc(filename);
    TiXmlHandle docHandle(&doc);
    bool loadOkay = doc.LoadFile();
    assert(loadOkay); // todo: replace this by an exception
    
    // get main node
    TiXmlHandle mainNode = docHandle.FirstChild("MechanicalSimulation");
    
    // read files
    TiXmlElement* filesNode = mainNode.FirstChild("Files").ToElement();
	Core::DataEntity::Pointer dataEntity;
    dataEntity = LoadFiles(filesNode, parent.file_string(), filePath.filename());

	blTagMap::Pointer rendering = blTagMap::New( );
	blTagMap::Pointer commands = blTagMap::New( );
	commands->AddTag( "gfx create material heart ambient 0.3 0 0.3 diffuse 1 0 0 specular 0.5 0.5 0.5 shininess 0.5", 1 );
	commands->AddTag( "gfx create material transouter ambient 1 1 1 diffuse 1 1 1 alpha 0.3", 1 );
	commands->AddTag( "gfx mod g_e ${region}/deformed_heart general element_discretization 3 scene ${scene}", 1 );
	commands->AddTag( "gfx mod g_e ${region}/deformed_heart surfaces   exterior face xi3_0 material heart render_shaded visible scene ${scene}", 1 );
	commands->AddTag( "gfx mod g_e ${region}/deformed_heart surfaces   exterior            material transouter  render_shaded visible scene ${scene}", 1 );
	//commands->AddTag( "gfx timekeeper default play minimum 0 maximum 10 speed 3", 1 );
	rendering->AddTag( "CMGUI commands", commands );
	dataEntity->GetMetadata()->AddTag( "Rendering", rendering );
	SetOutputDataEntity(0, dataEntity);
    
    // set camera
    TiXmlElement* cameraNode = mainNode.FirstChild("Camera").ToElement();
}

Core::DataEntity::Pointer MechanicalSimulationReader::LoadFiles(
	TiXmlElement* filesNode, boost::filesystem::path directory, std::string name) 
{
	Core::DataEntity::Pointer dataEntity = Core::DataEntity::New( );
	dataEntity->GetMetadata()->SetName( name );
	dataEntity->Resize( 1, typeid( Cmiss_region_id ) );

	Cmiss_region_id region;
	dataEntity->GetProcessingData( region );

    TiXmlElement* nodeFilesNode = filesNode->FirstChild("Nodes")->ToElement();
    std::list<std::string> nodeFiles;
    LoadFileList(nodeFilesNode, nodeFiles);
    int i = 0;
    BOOST_FOREACH(std::string filename, nodeFiles) {

		double time = 1.0*i;
		std::string completeFilename = (directory / filename).file_string();
		struct FE_import_time_index *timeIndex = new struct FE_import_time_index;
		timeIndex->time = time;
		Cmiss_region_read_file_time(region, completeFilename.c_str(), timeIndex);
        i++;
    }
	dataEntity->GetMetadata()->AddTag( "TimeMin", 0 );
	dataEntity->GetMetadata()->AddTag( "TimeMax", i );
    
    TiXmlElement* elementFilesNode = filesNode->FirstChild("Elements")->ToElement();
    std::list<std::string> elementFiles;
    LoadFileList(elementFilesNode, elementFiles);
    BOOST_FOREACH(std::string filename, elementFiles) {
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

	dataEntity->GetMetadata()->SetName( name );
	return dataEntity;
}

void MechanicalSimulationReader::LoadFileList(TiXmlElement* fileListNode, std::list<std::string>& files) {
    // for all elements that describe file lists
    TiXmlElement* child = fileListNode->FirstChild()->ToElement();
    while (child) {
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

void MechanicalSimulationReader::LoadFileListExplicit(TiXmlElement* fileListNode, std::list<std::string>& files) {
    std::string text = fileListNode->GetText();
    files.push_back(text);
}

void MechanicalSimulationReader::LoadFileListScheme(TiXmlElement* fileListNode, std::list<std::string>& files) {
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

void MechanicalSimulationReader::LoadCamera(TiXmlElement* cameraNode) {
}

void MechanicalSimulationReader::Update() {
    BOOST_FOREACH(std::string filename, m_Filenames) {
        Load(filename);
    }
}

boost::any MechanicalSimulationReader::ReadSingleTimeStep( 
        int iTimeStep, 
        const std::string &filename ) {
    return NULL;
}
