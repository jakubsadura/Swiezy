/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _CmguiPluginRenderingTree_H
#define _CmguiPluginRenderingTree_H

#include "corePluginMacros.h"
#include "coreRenderingTree.h"

extern "C"
{
#include "api/cmiss_scene_viewer.h"
}

namespace CmguiPlugin
{

	class Cmgui;

/**
\brief Rendering Tree for CMGUI library
\ingroup CMGUIPlugin
\author Xavi Planes
\date Oct 2010
*/
class PLUGIN_EXPORT RenderingTree : public Core::RenderingTree
{
public:
	coreDeclareSmartPointerClassMacro2Param(RenderingTree, Core::RenderingTree, Cmgui*, wxWindow* );

	typedef std::list<Core::DataEntity*> DataEntityListType;

	//! Redefined
	boost::any Add(
		Core::DataEntity::Pointer dataEntity,
		bool bShow = true,
		bool bInitializeViews = true );
	//! Redefined
	bool Remove(unsigned int dataEntityId, bool bInitializeViews = true);
	//! Redefined
	bool Remove(Core::DataEntity* dataEntity, bool bInitializeViews = true );
	//! Redefined
	void RemoveAll();
	//! Redefined
	boost::any GetNode(Core::DataEntity::Pointer dataEntity);
	//! Redefined
	boost::any GetNode(unsigned int dataEntityId);
	//! Redefined
	long GetNumberOfDataEntities( );
	//! Redefined
	std::vector<Core::DataEntity::Pointer> GetAllDataEntities( );
	//! Redefined
	void Show( Core::DataEntity::Pointer dataEntity, bool bShow, bool initializeViews = true );
	//! Redefined
	bool IsDataEntityRendered(Core::DataEntity* dataEntity);
	//! Redefined
	bool IsDataEntityShown(Core::DataEntity* dataEntity);
	//! Redefined
	void SetProperty( Core::DataEntity::Pointer, blTag::Pointer property );
	//! Redefined
	blTag::Pointer GetProperty( Core::DataEntity::Pointer, const std::string& name );

	//!
	Cmiss_scene_viewer_id GetSceneViewer() const;

protected:

	//!
	void InitOpenGLSharedContext( );

	//!
	virtual void OnModifiedDataEntity(Core::SmartPointerObject* dataEntity);

	//!
	RenderingTree(Cmgui*, wxWindow* parentWidget );
	virtual ~RenderingTree(void);

	coreDeclareNoCopyConstructors(RenderingTree);

	//!
	bool AddToScene( Core::DataEntity::Pointer dataEntity );

	//!
	void AddRegionToScene( Cmiss_region_id region );

	//!
	void AddImageToScene( Cmiss_region_id region, struct Cmgui_image *image );

	//!
	void ApplyCMGUICommands( Core::DataEntity::Pointer dataEntity );

	//!
	Cmiss_region_id GetRegion( Core::DataEntity* dataEntity );

protected:

	//!
	Cmgui *m_cmgui;

	//!
	Cmiss_scene_viewer_id m_sceneViewer;

	//!
	static int m_SceneCounter;

	//! Data entity and the rendering data
	DataEntityListType m_AllDataEntities;
};

} // namespace CmguiPlugin

#endif // _CmguiPluginRenderingTree_H
