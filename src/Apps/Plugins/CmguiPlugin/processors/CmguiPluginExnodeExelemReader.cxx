/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginExnodeExelemReader.h"
#include "CmguiRegionImpl.h"

#include "boost/foreach.hpp"
#include "itksys/SystemTools.hxx"
#include "coreScanFolder.h"
#include "coreFile.h"

extern "C" {
#include "region/cmiss_region.h"
}

#define FOREACH_CONST(elementVariable, collection, collectionType, elementType) for (std::collectionType<elementType>::const_iterator elementVariable = collection.begin(); elementVariable != collection.end(); elementVariable++)

#define FOREACH(elementVariable, collection, collectionType, elementType) for (std::collectionType<elementType>::iterator elementVariable = collection.begin(); elementVariable != collection.end(); elementVariable++)


using namespace CmguiPlugin;
using namespace itksys;

ExnodeExelemReader::ExnodeExelemReader()
{
    m_ValidExtensionsList.push_back(".exnode");
    m_ValidExtensionsList.push_back(".exelem");
    m_ValidTypesList.push_back(Core::SurfaceMeshTypeId);
    //std::cout << m_Filenames.size() << std::endl;
}

ExnodeExelemReader::~ExnodeExelemReader()
{
}

bool ExnodeExelemReader::ReadHeader() {
    return true;
}

void ExnodeExelemReader::Update() {

	if ( m_Filenames.empty() )
	{
		return;
	}

	// Groups names that only change the number
	Core::IO::ScanFolder::Pointer scan = Core::IO::ScanFolder::New();
	scan->SetFileNames( m_Filenames );
	scan->Update();
	Core::IO::ScanFolder::GroupMapType groups = scan->GetGroups();

	// Find .exelem and .exnode groups
	Core::IO::ScanFolder::GroupMapType::iterator it = groups.begin();
	std::string exelemGroup;
	std::string exnodeGroup;
	while ( ( exelemGroup.empty() || exnodeGroup.empty() ) && it != groups.end() )
	{
		std::vector< std::string > filenames = it->second.GetFilenames();
		std::string extension = SystemTools::LowerCase(SystemTools::GetFilenameLastExtension( it->first ));
		if ( extension == ".exelem" )
		{
			exelemGroup = it->first;
		}
		else if ( extension == ".exnode" )
		{
			exnodeGroup = it->first;
		}
		it++;
	}


	// Create data entity
	Core::DataEntity::Pointer dataEntity = Core::DataEntity::New();
	std::string regionName = Core::IO::File::GetFilenameWithoutExtension( exelemGroup );
	dataEntity->GetMetadata()->SetName( regionName );
	dataEntity->Resize( 1, typeid( Cmiss_region_id ) );
	Cmiss_region_id region;
	dataEntity->GetProcessingData( region );

	// Read nodes
	if ( !exnodeGroup.empty() )
	{
		if ( groups[exnodeGroup].GetFilenames().size() == 1 )
		{
			std::string filename = groups[exnodeGroup].GetFilenames()[ 0 ];
			Cmiss_region_read_file(region, filename.c_str());
		}
		else
		{
			int i = 0;
			BOOST_FOREACH(std::string filename, groups[exnodeGroup].GetFilenames() ) 
			{
				double time = 1.0*i;
				struct FE_import_time_index *timeIndex = new struct FE_import_time_index;
				timeIndex->time = time;
				Cmiss_region_read_file_time(region, filename.c_str(),timeIndex);
				i++;
			}
			dataEntity->GetMetadata()->AddTag( "TimeMin", 0 );
			dataEntity->GetMetadata()->AddTag( "TimeMax", i );
		}
	}

	// Read elements
	if ( !exelemGroup.empty() )
	{
		if ( groups[exelemGroup].GetFilenames().size() == 1 )
		{
			std::string filename = groups[exelemGroup].GetFilenames()[ 0 ];
			Cmiss_region_read_file(region, filename.c_str());
		}
		else
		{
			int i = 0;
			BOOST_FOREACH(std::string filename, groups[exelemGroup].GetFilenames() ) 
			{
				double time = 1.0*i;
				struct FE_import_time_index *timeIndex = new struct FE_import_time_index;
				timeIndex->time = time;
				Cmiss_region_read_file_time(region, filename.c_str(),timeIndex);
				i++;
			}
			dataEntity->GetMetadata()->AddTag( "TimeMin", 0 );
			dataEntity->GetMetadata()->AddTag( "TimeMax", i );
		}
	}

	// Add rendering options
	blTagMap::Pointer rendering = blTagMap::New( );
	blTagMap::Pointer commands = blTagMap::New( );
	rendering->AddTag( "CMGUI commands", commands );
	dataEntity->GetMetadata()->AddTag( "Rendering", rendering );

	commands->AddTag( "gfx modify g_element ${region} lines material white line_width 1 scene ${scene}", 1 );

	if ( CmguiRegionImpl::IsSurfaceMesh( region ) )
	{
		dataEntity->SetType( Core::SurfaceMeshTypeId );
	}
	else
	{
		dataEntity->SetType( Core::VolumeMeshTypeId );
	}

	// Out data entity
    SetOutputDataEntity(0, dataEntity);
}

boost::any ExnodeExelemReader::ReadSingleTimeStep( 
        int iTimeStep, 
        const std::string &filename ) {
    return NULL;
}
