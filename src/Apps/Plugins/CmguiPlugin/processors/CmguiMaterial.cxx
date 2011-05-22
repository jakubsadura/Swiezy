/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiMaterial.h"

extern "C" {
#include "api/cmiss_graphics_module.h"
}

CmguiPlugin::CmguiMaterial::MaterialMapType CmguiPlugin::CmguiMaterial::m_MaterialMap;

CmguiPlugin::CmguiMaterial::CmguiMaterial( 
	Cmgui *cmgui, const std::string &regionName, struct Cmgui_image *cmgui_image ) 
{
	m_cmgui = cmgui;
	m_RegionName = regionName;

	// Create texture "image_texture".
	std::string texture_name = regionName + "_texture";
	Cmiss_texture_id texture = CREATE(Texture)(texture_name.c_str());
	Texture_set_image(texture, cmgui_image, "", "", 0, 0, 1, 0, 0, 0, 0);
	Cmiss_texture_set_filter_mode(texture, CMISS_TEXTURE_FILTER_LINEAR);
	Cmiss_texture_manager_id texture_manager = Cmiss_command_data_get_texture_manager( m_cmgui->GetCommandData() );
	ADD_OBJECT_TO_MANAGER(Texture)(texture, texture_manager);

	// Create material "image_material"
	Cmiss_graphics_module_id gModule = Cmiss_command_data_get_graphics_module( m_cmgui->GetCommandData() );
	Cmiss_material_id material = Cmiss_graphics_module_create_material(gModule);
	std::string material_name = regionName + "_material";
	Cmiss_material_set_name(material, material_name.c_str() );
	Graphical_material_set_texture( material, texture );
	Cmiss_material_set_volatile( material, true );

	m_MaterialId = material;
	// Texture has access_count 0 when created
	// Material has access_count 1 when created
	DEACCESS(Graphical_material)(&material);
}

CmguiPlugin::CmguiMaterial* CmguiPlugin::CmguiMaterial::FindMaterial( 
	const std::string &regionName )
{
	MaterialMapType::iterator it = m_MaterialMap.find( regionName );
	if ( it == m_MaterialMap.end() )
	{
		return NULL;
	}
	CmguiPlugin::CmguiMaterial* material = it->second;
	return material;
}

void CmguiPlugin::CmguiMaterial::RegisterMaterial( 
	const std::string &regionName, CmguiMaterial* material )
{
	m_MaterialMap[ regionName ] = material;
	material->Register();
}

Cmiss_material_id CmguiPlugin::CmguiMaterial::GetMaterialId() const
{
	return m_MaterialId;
}

CmguiPlugin::CmguiMaterial::~CmguiMaterial()
{
	// Decrease access count because when material is deaccessed
	// it's not removed from manager
	std::string material_name = m_RegionName + "_material";
	Cmiss_graphics_module_id gModule = Cmiss_command_data_get_graphics_module( m_cmgui->GetCommandData() );
	Cmiss_material_id material;
	material = Cmiss_graphics_module_find_material_by_name( gModule, material_name.c_str() );
	Cmiss_material_destroy(&material);

	Cmiss_texture_manager_id texture_manager;
	texture_manager = Cmiss_command_data_get_texture_manager( m_cmgui->GetCommandData() );
	std::string texture_name = m_RegionName + "_texture";
	Cmiss_texture_id texture;
	texture = Cmiss_texture_manager_get_texture( texture_manager, texture_name.c_str() ); 
	REMOVE_OBJECT_FROM_MANAGER(Texture)(texture, texture_manager);
}

void CmguiPlugin::CmguiMaterial::UnRegisterMaterial( const std::string &regionName )
{
	MaterialMapType::iterator it = m_MaterialMap.find( regionName );
	if ( it == m_MaterialMap.end() )
	{
		return ;
	}

	CmguiPlugin::CmguiMaterial* material = it->second;
	if ( material->GetReferenceCount() == 1 )
	{
		m_MaterialMap.erase( regionName );
	}
	material->UnRegister();
}
