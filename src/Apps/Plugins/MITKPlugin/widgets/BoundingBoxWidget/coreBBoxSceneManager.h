/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CORECROPPINGSCENEMANAGER_H
#define CORECROPPINGSCENEMANAGER_H

#include "corePluginMacros.h"
#include "coreRenderingTree.h"
#include <mitkCuboid.h>
#include <mitkPointSet.h>
#include <boost/shared_ptr.hpp>

#include "coreBBoxSceneManagerHandler.h"

namespace Core{

/**
\brief 
\ingroup ImageToolsPlugin
\author 
\date 
*/
class PLUGIN_EXPORT BBoxSceneManager : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro2Param(
		BBoxSceneManager,
		SmartPointerObject,
		BoundingBoxHolder::Pointer,
		Core::RenderingTree::Pointer );

	//! 
	void Enable( bool enable = true );

	//!
	bool IsEnabled( );

	Core::DataEntity::Pointer GetInputDataEntity() const;
	void SetInputDataEntity(Core::DataEntity::Pointer val);

	Core::DataEntity::Pointer GetBoundingBoxDataEntity() const;

private:
	//! The scene manager displays the bounding box inside \a dataHolder
	BBoxSceneManager(		
		BoundingBoxHolder::Pointer dataHolder,
		Core::RenderingTree::Pointer renderingTree
	);

	void ConstructHandles();

	//@{ 
	//! \name Constructor helpers.
	void ConstructRenderingTree( Core::RenderingTree::Pointer &renderingTree );
	void ConstructDataHolder( 
		Core::BoundingBoxHolder::Pointer dataHolder );
	void ConstructBoundingBoxDataEntity();
	//@}

	//!
	~BBoxSceneManager();

	//! Respond to a change in the cropping data (i.e. the bounding box).
	void OnCroppingDataChanged();

	//!
	void OnHandleWasMoved( );

	//! Update the handles
	void UpdateHandleLocations();

	//! Updates m_BoundingBox based on m_DataHolder
	void UpdateBoundingBoxActor();

	//! Sets size of the interaction handles 
	void SetHandleSize( float handleSize );

	//! Update the bounding box based on the handlers
	void UpdateBondingBoxFromHandlers();

private:
	typedef mitk::PointSet::PointType::CoordRepType ScalarType;

	std::vector< BBoxSceneManagerHandler::Pointer > m_Handles;
	std::vector< unsigned long > m_HandleObserverHandleWasMoved;

	//!
	BoundingBoxHolder::Pointer m_DataHolder;

	//!
	Core::RenderingTree::Pointer m_RenderingTree;

	/** MITK object representing the bounding box (which is visualized 
	through m_BoundingBoxDataEntity)
	*/
	mitk::Cuboid::Pointer m_BoundingBox;

	//! Data entity (which is added to the rendering tree) visualizing m_BoundingBox 
	Core::DataEntity::Pointer m_BoundingBoxDataEntity;

	//!
	bool m_Enable;

	//! 
	Core::DataEntity::Pointer m_InputDataEntity;


};

} // Core

#endif //CORECROPPINGSCENEMANAGER_H
