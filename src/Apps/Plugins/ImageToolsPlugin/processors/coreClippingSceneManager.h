/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CORECLIPPINGSCENEMANAGER_H
#define CORECLIPPINGSCENEMANAGER_H

#include "CILabNamespaceMacros.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreRenderingTreeMITK.h"
#include "coreClippingProcessor.h"
#include <mitkCuboid.h>
#include <mitkSurface.h>
#include <mitkPointSet.h>
//#include "blMitkLineBoundPointInteractor.h"
//#include "blMitkClipperInteractor.h"
//#include "corePointInteractor.h"

namespace Core{

/**

\ingroup ImageToolsPlugin
\author 
\date dec 2008
*/
class PLUGIN_EXPORT ClippingSceneManager  
	: public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro2Param(
		Core::ClippingSceneManager,
		Core::SmartPointerObject,
		ClippingProcessor::ClippingProcessorDataHolder::Pointer,
		Core::RenderingTreeMITK::Pointer);

	//! Returns true if the manager is in the cropping mode
	bool GetIsInClippingMode() const;
	//! Sets whether the manager is in the cropping mode
	void SetIsInClippingMode(bool flag);
	//! Update output
	void UpdateModifiedOutput(Core::ClippingProcessor::Pointer processor);

private:
	//! The scene manager displays the bounding box inside \a dataHolder
	ClippingSceneManager(		
		ClippingProcessor::ClippingProcessorDataHolder::Pointer dataHolder,
		Core::RenderingTreeMITK::Pointer renderingTree
	);

	void ConstructMarkerSetDataEntity();
	void ConstructClippingPlaneDataEntity();

	//@{ 
	//! \name Constructor helpers.
	void ConstructRenderingTree( Core::RenderingTreeMITK::Pointer &renderingTree );
	void ConstructDataHolder( 
		Core::ClippingProcessor::ClippingProcessorDataHolder::Pointer dataHolder );
	void ConstructBoundingBoxDataEntity();
	//@}

	//! Holds the bounding box handles
	mitk::PointSet::Pointer m_MarkerSet;

	//!
	~ClippingSceneManager();
	//! Respond to a change in the clipping data (i.e. the plane).
	void OnClippingDataChanged();

	void UpdateMarkerLocations();

	/** Updates the actor displaying the clipping plane based on the 
	two points/markers
	*/
	void UpdateClippingPlaneActor();

	void UpdateBoundingBox(Core::ClippingProcessorData::Pointer clippingData );

	void CalculateClippingPlane( Core::ClippingProcessorData::Pointer clippingData );

	//! Sets size of the interaction handles 
	void SetMarkerSize( float markerSize );

private:
	typedef mitk::PointSet::PointType::CoordRepType ScalarType;

	//!
	ClippingProcessor::ClippingProcessorDataHolder::Pointer m_DataHolder;
	Core::RenderingTreeMITK::Pointer m_RenderingTree;
	/** MITK object representing the bounding box (which is visualized 
	through m_BoundingBoxDataEntity)
	*/
	mitk::Cuboid::Pointer m_BoundingBox;
	/**  Data entity (which is added to the rendering tree) visualizing 
	m_BoundingBox 
	*/
	Core::DataEntity::Pointer m_BoundingBoxDataEntity;
	/**  MITK object representing the clipping plane (which is visualized 
	through m_PlaneDataEntity)
	*/
	mitk::Surface::Pointer m_Plane;
	//! Data entity (which is added to the rendering tree) visualizing m_Plane 
	Core::DataEntity::Pointer m_PlaneDataEntity;
	//! Data entity (which is added to the rendering tree) visualizing m_MarkerSet
	Core::DataEntity::Pointer m_MarkerSetDataEntity;
	//! Point with the center of the bounding box
	mitk::PointSet::PointType m_Center;
	//! Point with the end 
	mitk::PointSet::PointType m_Max;
	//! Indicates whether the manager is in the clipping mode
	bool m_IsInClippingMode;
	//! Interactor for displacement of the plane through a line
//	blMitkLineBoundPointInteractor::Pointer m_LineBoundInteractor;
	//! Interactor for the marker
//	blMitkClipperInteractor::Pointer m_ClipperInteractor;
	
};

} // Core

#endif //CORECLIPPINGSCENEMANAGER_H
