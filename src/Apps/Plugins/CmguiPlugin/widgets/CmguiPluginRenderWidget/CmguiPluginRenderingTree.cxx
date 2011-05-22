/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginRenderingTree.h"
#include "CmguiPluginCmgui.h"
#include "CmguiImageImpl.h"
#include "CmguiMaterial.h"

#include "blTextUtils.h"

extern "C" {
#include "region/cmiss_region.h"
#include "graphics/scene.h"
#include "graphics/scene_viewer.h"
#include "graphics/cmiss_rendition.h"
#include "api/cmiss_scene_viewer.h"
#include "api/cmiss_material.h"
#include "api/cmiss_graphics_module.h"
}

#include <wx/wupdlock.h>

using namespace CmguiPlugin;

int RenderingTree::m_SceneCounter = 0;

//!
RenderingTree::RenderingTree(Cmgui *cmgui, wxWindow* parentWidget)
{
	m_cmgui = cmgui;

	InitOpenGLSharedContext( );


	struct Cmiss_scene_viewer_package *cmiss_scene_viewer_package;
	cmiss_scene_viewer_package = Cmiss_command_data_get_scene_viewer_package(m_cmgui->GetCommandData());
	m_sceneViewer = Cmiss_scene_viewer_create_wx(
		cmiss_scene_viewer_package,
		(void *)parentWidget, CMISS_SCENE_VIEWER_BUFFERING_DOUBLE,
		CMISS_SCENE_VIEWER_STEREO_ANY_MODE, 24, 24, 24);

	//!
	std::stringstream sceneName;
	sceneName << "RenderingTreeScene" << m_SceneCounter;
	m_Name = sceneName.str();

	// Change to manual to avoid automatic creation of scene objects
	// when a region is added to the tree
	std::stringstream command;
	command << "gfx create scene " << m_Name << " manual_g_element";
	m_cmgui->executeCommand( command.str() );

	Cmiss_scene_viewer_set_scene_by_name( m_sceneViewer, m_Name.c_str() );
	Cmiss_scene_viewer_set_perturb_lines( m_sceneViewer, 1 );

	struct Scene *src = Cmiss_command_data_get_default_scene(m_cmgui->GetCommandData());
	struct Scene *dst = Scene_viewer_get_scene( m_sceneViewer );
	Scene_disable_time_behaviour( dst );
	MANAGER_COPY_WITHOUT_IDENTIFIER(Scene,name)
		(dst, src);

	m_SceneCounter++;


	Cmiss_scene_viewer_view_all(m_sceneViewer);
	Cmiss_scene_viewer_set_antialias_mode(m_sceneViewer, 4);

	Cmiss_scene_viewer_set_projection_mode(m_sceneViewer, CMISS_SCENE_VIEWER_PROJECTION_PERSPECTIVE);
}

//!
RenderingTree::~RenderingTree(void)
{
	RemoveAll();

	if (m_sceneViewer != NULL) {
		Cmiss_scene_viewer_destroy(&m_sceneViewer);
	}
}

boost::any CmguiPlugin::RenderingTree::Add( 
	Core::DataEntity::Pointer dataEntity, bool bShow /*= true*/, bool bInitializeViews /*= true */ )
{

	if ( IsDataEntityRendered( dataEntity ) )
	{
		return GetNode( dataEntity );
	}

	if ( !AddToScene( dataEntity ) )
	{
		throw Core::Exceptions::Exception( "RenderingTree::Add", "Cannot render this data" );
	}

	ApplyCMGUICommands( dataEntity );

	// Add the connections so as to enable removing the nodes from the tree 
	// when the dataentity is destroyed
	this->ConnectToOnDestroySignal(dataEntity);

	Modified( );

	return GetNode( dataEntity );
}

bool CmguiPlugin::RenderingTree::Remove( 
	unsigned int dataEntityId, bool bInitializeViews /*= true*/ )
{
	return false;
}

bool CmguiPlugin::RenderingTree::Remove( Core::DataEntity* dataEntity, bool bInitializeViews /*= true */ )
{
	Cmiss_region_id region = GetRegion( dataEntity );
	if  ( region == NULL )
	{
		return NULL;
	}

	if ( !IsDataEntityRendered( dataEntity ) )
	{
		return false;
	}

	struct Scene *scene = Scene_viewer_get_scene( m_sceneViewer );
	int numChildRegions;
	Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
	for (int childRegionNr = 0; childRegionNr < numChildRegions; childRegionNr++) 
	{
		Cmiss_region_id childRegionID = Cmiss_region_get_child_region(region, childRegionNr);

		char *name = Cmiss_region_get_name( childRegionID );
		std::string regionName = name;
		delete[] name;

		Scene_remove_graphical_element_group( scene, childRegionID );

		// There's a reference somewhere to the material and cannot be removed
		if ( dataEntity->IsImage() || ( dataEntity->GetType() & Core::SliceImageTypeId ) )
		{
			CmguiMaterial::UnRegisterMaterial( regionName );
		}
	
	}

	Scene_remove_graphical_element_group( scene, region );

	// Remove from m_AllDataEntities
	DataEntityListType::iterator it;
	it = std::find( m_AllDataEntities.begin(), m_AllDataEntities.end(), dataEntity );
	if ( it != m_AllDataEntities.end() )
	{
		m_AllDataEntities.erase( it );
	}

	Modified( );

	return true;
}

void CmguiPlugin::RenderingTree::RemoveAll()
{
	while ( m_AllDataEntities.size() )
	{
		DataEntityListType::iterator it = m_AllDataEntities.begin();
		Remove( *it );
	}
}

boost::any CmguiPlugin::RenderingTree::GetNode( Core::DataEntity::Pointer dataEntity )
{
	return NULL;
}

boost::any CmguiPlugin::RenderingTree::GetNode( unsigned int dataEntityId )
{
	return NULL;
}

long CmguiPlugin::RenderingTree::GetNumberOfDataEntities()
{
	return m_AllDataEntities.size();
}

std::vector<Core::DataEntity::Pointer> CmguiPlugin::RenderingTree::GetAllDataEntities()
{
	std::vector<Core::DataEntity::Pointer> all;
	DataEntityListType::iterator it;
	for ( it = m_AllDataEntities.begin() ; it != m_AllDataEntities.end() ; it++ )
	{
		all.push_back( *it );
	}
	return all;
}

void CmguiPlugin::RenderingTree::Show( 
	Core::DataEntity::Pointer dataEntity, bool bShow, bool initializeViews /*= true */ )
{
	Cmiss_region_id region = GetRegion( dataEntity );
	if  ( region == NULL )
	{
		return ;
	}

	struct Scene *scene = Scene_viewer_get_scene( m_sceneViewer );
	GT_visibility_type visible;
	if ( bShow )
	{
		visible = g_VISIBLE;
	}
	else
	{
		visible = g_INVISIBLE;
	}

	Scene_set_element_group_visibility( scene, region, visible );
	int numChildRegions;
	Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
	for (int childRegionNr = 0; childRegionNr < numChildRegions; childRegionNr++) 
	{
		Cmiss_region_id childRegionID = Cmiss_region_get_child_region(region, childRegionNr);
		Scene_set_element_group_visibility( scene, childRegionID, visible );
	}


	if ( initializeViews )
		Modified();
}

bool CmguiPlugin::RenderingTree::IsDataEntityRendered( 
	Core::DataEntity* dataEntity )
{
	Cmiss_region_id region = GetRegion( dataEntity );
	if  ( region == NULL )
	{
		return false;
	}

	struct Scene *scene = Scene_viewer_get_scene( m_sceneViewer );
	struct Scene_object *scene_object;

	scene_object = first_Scene_object_in_Scene_that( scene, Scene_object_has_Cmiss_region, region );
	if ( scene_object == NULL )
	{
		return false;
	}

	int numChildRegions;
	Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
	for (int childRegionNr = 0; childRegionNr < numChildRegions; childRegionNr++) 
	{
		Cmiss_region_id childRegionID = Cmiss_region_get_child_region(region, childRegionNr);

		scene_object = first_Scene_object_in_Scene_that( scene, Scene_object_has_Cmiss_region, childRegionID );
		if ( scene_object == NULL )
		{
			return false;
		}
	}

	return true;
}

void CmguiPlugin::RenderingTree::OnModifiedDataEntity( 
	Core::SmartPointerObject* dataEntity )
{

}

Cmiss_scene_viewer_id CmguiPlugin::RenderingTree::GetSceneViewer() const
{
	return m_sceneViewer;
}

bool CmguiPlugin::RenderingTree::IsDataEntityShown( Core::DataEntity* dataEntity )
{
	Cmiss_region_id region = GetRegion( dataEntity );
	if  ( region == NULL )
	{
		return false;
	}

	if ( !IsDataEntityRendered( dataEntity ) )
	{
		return false;
	}

	struct Scene *scene = Scene_viewer_get_scene( m_sceneViewer );
	bool shown = true;
	shown &= Scene_get_element_group_visibility( scene, region ) == g_VISIBLE;
	int numChildRegions;
	Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
	for (int childRegionNr = 0; childRegionNr < numChildRegions; childRegionNr++) 
	{
		Cmiss_region_id childRegionID = Cmiss_region_get_child_region(region, childRegionNr);
		shown &= Scene_get_element_group_visibility( scene, childRegionID ) == g_VISIBLE;
	}

	return shown;
}

void CmguiPlugin::RenderingTree::SetProperty( Core::DataEntity::Pointer, blTag::Pointer property )
{
	
}

void CmguiPlugin::RenderingTree::ApplyCMGUICommands( Core::DataEntity::Pointer dataEntity )
{
	// Apply rendering properties
	blTagMap::Pointer rendering;
	rendering = dataEntity->GetMetadata()->GetTagValue<blTagMap::Pointer>( "Rendering" );
	if ( rendering.IsNotNull() )
	{
		blTagMap::Pointer commands;
		commands = rendering->GetTagValue<blTagMap::Pointer>( "CMGUI commands" );
		if ( commands.IsNotNull() )
		{
			blTagMap::Iterator it;
			for ( it = commands->GetIteratorBegin(); it != commands->GetIteratorEnd() ; it++ )
			{
				std::string command = it->second->GetName();
				blTextUtils::StrSub( command, "${scene}", GetName() );
				// Replace without "" if there's a /
				blTextUtils::StrSub( command, "${region}/", dataEntity->GetMetadata()->GetName() + "/" );
				blTextUtils::StrSub( command, "${region}", "\"" + dataEntity->GetMetadata()->GetName() + "\"" );
				m_cmgui->executeCommand( command );
			}
		}
	}
}

bool CmguiPlugin::RenderingTree::AddToScene( Core::DataEntity::Pointer dataEntity )
{
	if ( dataEntity->IsImage() || ( dataEntity->GetType() & Core::SliceImageTypeId ) )
	{
		CmguiPlugin::CmguiImage3D::Pointer image;

		if ( !dataEntity->GetProcessingData( image ) )
		{
			// Get rendering data entity
			Core::DataEntity::Pointer renDataEntity;
			boost::any anyData = dataEntity->GetRenderingData( typeid( image ) );
			Core::CastAnyProcessingData( anyData, renDataEntity );
			if ( renDataEntity.IsNull() )
			{
				// Create new data entity of type cmgui
				renDataEntity = Core::DataEntity::New( Core::SliceImageTypeId );
				renDataEntity->GetMetadata()->SetName( dataEntity->GetMetadata()->GetName() );
				renDataEntity->SwitchImplementation( typeid( image ) );

				// Copy the processing data to rendering data, referencing the memory
				renDataEntity->Copy( dataEntity, Core::gmReferenceMemory );
			}

			if ( !renDataEntity->GetProcessingData( image ) )
			{
				return false;
			}

			dataEntity->SetRenderingData( renDataEntity );
		}

		AddRegionToScene( image->m_Region );

		std::vector<CmguiImage2D::Pointer>::iterator it;
		for ( it  = image->m_Slices.begin() ; it != image->m_Slices.end() ; it++ )
		{
			AddImageToScene( (*it)->m_Region, (*it)->m_CmguiImage );
		}

	}
	else
	{
		Cmiss_region_id region;

		if ( !dataEntity->GetProcessingData( region ) )
		{
			Core::DataEntity::Pointer renDataEntity;
			boost::any anyData = dataEntity->GetRenderingData( typeid( region ) );
			Core::CastAnyProcessingData( anyData, renDataEntity );
			if ( renDataEntity.IsNull() )
			{
				// Create new data entity of type cmgui
				renDataEntity = Core::DataEntity::New( Core::SurfaceMeshTypeId );
				renDataEntity->GetMetadata()->SetName( dataEntity->GetMetadata()->GetName() );
				renDataEntity->SwitchImplementation( typeid( region ) );

				// Copy the processing data to rendering data, referencing the memory
				renDataEntity->Copy( dataEntity, Core::gmReferenceMemory );
			}

			if ( !renDataEntity->GetProcessingData( region ) )
			{
				return false;
			}

			dataEntity->SetRenderingData( renDataEntity );
		}

		AddRegionToScene( region );

	}

	m_AllDataEntities.push_back( dataEntity );

	return true;
}

void CmguiPlugin::RenderingTree::AddRegionToScene( Cmiss_region_id region )
{
	struct Scene *scene = Scene_viewer_get_scene( m_sceneViewer );

	char *name = Cmiss_region_get_path(region);
	Scene_add_graphical_element_group(scene, region, 0, name);
	delete[] name;

	int numChildRegions;
	Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
	for (int childRegionNr = 0; childRegionNr < numChildRegions; childRegionNr++) {
		struct Cmiss_region *childRegionID = Cmiss_region_get_child_region(region, childRegionNr);
		//std::string name = std::string("rootRegion/")
		char *name = Cmiss_region_get_path(childRegionID);
		Scene_add_graphical_element_group(scene, childRegionID, 0, name);
		delete[] name;
	}
}

void CmguiPlugin::RenderingTree::AddImageToScene( 
	Cmiss_region_id region, 
	struct Cmgui_image *cmgui_image )
{
	char *name = Cmiss_region_get_name( region );
	std::string regionName = name;
	delete[] name;

	CmguiMaterial::Pointer mat = CmguiMaterial::FindMaterial( regionName );
	if ( mat.IsNull() )
	{
		mat = CmguiMaterial::New( m_cmgui, regionName, cmgui_image );
	}
	CmguiMaterial::RegisterMaterial( regionName, mat );
	Cmiss_material_id material = mat->GetMaterialId();

	// Set surface material and texture coordinates
	GT_element_settings* settings = CREATE(GT_element_settings)(GT_ELEMENT_SETTINGS_SURFACES);
	GT_element_settings_set_material(settings, material);
	GT_element_settings_set_selected_material(settings, material);
	GT_element_settings_set_name( settings, "ImageTexture" );

	struct Scene *scene = Scene_viewer_get_scene( m_sceneViewer );
	manager_Computed_field* cfm = Cmiss_region_get_Computed_field_manager(region);
	Computed_field* c_field = FIND_BY_IDENTIFIER_IN_MANAGER(Computed_field, name)("xi",cfm);
	GT_element_settings_set_texture_coordinate_field(settings,c_field);

	struct GT_element_group *elem_group;
	elem_group = Scene_get_graphical_element_group( scene, region );
	GT_element_group_add_settings(elem_group, settings, -1);
}

Cmiss_region_id CmguiPlugin::RenderingTree::GetRegion( Core::DataEntity* dataEntity )
{
	if ( dataEntity == NULL )
	{
		return NULL;
	}

	DataEntityListType::iterator it;
	it = std::find( m_AllDataEntities.begin(), m_AllDataEntities.end(), dataEntity );
	if ( it == m_AllDataEntities.end() )
	{
		return NULL;
	}

	Cmiss_region_id region;
	if ( dataEntity->GetProcessingData( region ) )
	{
		return region;
	}

	if ( dataEntity->IsImage() || ( dataEntity->GetType() & Core::SliceImageTypeId ) )
	{
		CmguiPlugin::CmguiImage3D::Pointer image;
		Core::DataEntity::Pointer renDataEntity;
		boost::any anyData = (*it)->GetRenderingData( typeid( image ) );
		Core::CastAnyProcessingData( anyData, renDataEntity );
		if ( renDataEntity.IsNotNull( ) && renDataEntity->GetProcessingData( image ) )
		{
			return image->m_Region;
		}
	}
	else if ( dataEntity->IsSurfaceMesh() )
	{
		Core::DataEntity::Pointer renDataEntity;
		boost::any anyData = (*it)->GetRenderingData( typeid( region ) );
		Core::CastAnyProcessingData( anyData, renDataEntity );
		if ( renDataEntity.IsNotNull( ) && renDataEntity->GetProcessingData( region ) )
		{
			return region;
		}
	}

	return NULL;
}

void CmguiPlugin::RenderingTree::InitOpenGLSharedContext()
{
	struct Cmiss_scene_viewer_package *cmiss_scene_viewer_package;
	cmiss_scene_viewer_package = Cmiss_command_data_get_scene_viewer_package(m_cmgui->GetCommandData());
}

blTag::Pointer CmguiPlugin::RenderingTree::GetProperty( Core::DataEntity::Pointer, const std::string& name )
{
	return NULL;
}

