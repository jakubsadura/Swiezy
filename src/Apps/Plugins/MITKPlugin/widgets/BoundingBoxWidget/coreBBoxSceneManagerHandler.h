/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CORECROPPINGSCENEMANAGERHANDLER_H
#define CORECROPPINGSCENEMANAGERHANDLER_H

#include "coreDataEntity.h"
#include "coreRenderingTree.h"
#include "coreBoundingBox.h"
#include "blMitkLineBoundPointInteractor.h"

#include "mitkCuboid.h"
#include "mitkPointSet.h"

#include "boost/shared_ptr.hpp"

#include "itkCommand.h"

namespace Core{


/**
\brief Initializes the DataEntity, the LineBoundPointInteractor and enables or
disables it.
\ingroup ImageToolsPlugin
\author 
\date 
*/
class BBoxSceneManagerHandler : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro4Param(
		BBoxSceneManagerHandler, 
		Core::SmartPointerObject,
		Core::RenderingTree::Pointer , 
		int , 
		Core::BoundingBox::Pointer ,
		Core::DataEntity::Pointer  );

	//! 
	void Enable( bool enable = true, bool show = true );

	void OnHandleWasMoved();

	void SetHandleSize(float handleSize);

	void SetLocation( mitk::PointSet::PointType& point);

	void SetLocation( itk::Point<double>& point );

	void Show(bool visible);

	mitk::PointSet::Pointer GetHandlePointSet() const;

	itk::Point<double> GetLocation( );

private:

	BBoxSceneManagerHandler(
		Core::RenderingTree::Pointer renderingTree, 
		int position, 
		Core::BoundingBox::Pointer croppingData,
		Core::DataEntity::Pointer parentDataEntity );

	~BBoxSceneManagerHandler();

private:

	//!
	Core::RenderingTree::Pointer m_RenderingTree;

	//! Position of the handle within the bounding box 
	//! (there are six, each in the center of one of the sides of the box
	int m_Position;

	//! Holds the bounding box handles
	mitk::PointSet::Pointer m_HandlePointSet;

	//! Data entity (which is added to the rendering tree) visualizing the handle
	Core::DataEntity::Pointer m_DataEntity;

	//! For moving the handle along a line
	blMitkLineBoundPointInteractor::Pointer m_Interactor;

	//! Call back called when the user moves a point
	itk::SimpleMemberCommand<Core::BBoxSceneManagerHandler>::Pointer 
		m_HandleWasMovedCommand;

	//!
	Core::BoundingBox::Pointer m_CroppingData;

	//!
	unsigned long m_HandleWasMovedTag;

};

} // Core

#endif //CORECROPPINGSCENEMANAGER_H
