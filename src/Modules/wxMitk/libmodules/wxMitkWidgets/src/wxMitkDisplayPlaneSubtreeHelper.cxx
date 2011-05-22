/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"

#include "wxMitkDisplayPlaneSubtreeHelper.h"
#include <mitkProperties.h>
#include <mitkGeometry2DDataMapper2D.h>
#include <mitkStringProperty.h>
#include <mitkDataTreeNode.h>
#include "wxMitkMultiRenderWindow.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkSelectableGLWidget.h"
#include "mitkNodePredicateProperty.h"

using namespace mitk;

const char* wxMitkDisplayPlaneSubtreeHelper::PLANE_SUB_TREE_ROOT_NODE_NAME = "widgetPlanesRoot";

//!
wxMitkDisplayPlaneSubtreeHelper::wxMitkDisplayPlaneSubtreeHelper(void)
{
}


//!
wxMitkDisplayPlaneSubtreeHelper::~wxMitkDisplayPlaneSubtreeHelper(void)
{
}


mitk::DataTreeNode::Pointer wxMitkDisplayPlaneSubtreeHelper::AddDisplayPlaneSubTreeRoot(
	mitk::DataStorage* dataStorage )
{
	if ( dataStorage == NULL )
	{
		return false;
	}

	mitk::DataTreeNode::Pointer node = mitk::DataTreeNode::New();
	node->SetProperty(
		"name", 
		mitk::StringProperty::New( PLANE_SUB_TREE_ROOT_NODE_NAME) );
	node->SetProperty("helper object", mitk::BoolProperty::New(true));
	node->SetProperty("visible", mitk::BoolProperty::New(true));
	dataStorage->Add(node);

	return node;
}

bool wxMitkDisplayPlaneSubtreeHelper::RemoveDisplayPlaneSubTree(
	const mitk::DataStorage::Pointer dataStorage)
{
	if ( dataStorage.IsNull() )
	{
		return false;
	}

	mitk::DataTreeNode::Pointer rootNode = GetRootNode( dataStorage );
	if ( rootNode.IsNull() )
	{
		return false;
	}

	dataStorage->Remove( dataStorage->GetDerivations( rootNode ) );

	dataStorage->Remove( rootNode );

	return true;
}


bool wxMitkDisplayPlaneSubtreeHelper::HasDisplayPlaneSubTree(
	const mitk::DataStorage::Pointer dataStorage)
{
	return GetRootNode( dataStorage ).IsNotNull();
}

void wxMitkDisplayPlaneSubtreeHelper::SetDisplayPlaneSubTreeVisibility(
	const mitk::DataStorage::Pointer dataStorage, 
	bool visible)
{

	mitk::DataTreeNode::Pointer rootNode = GetRootNode( dataStorage );

	mitk::DataStorage::SetOfObjects::ConstPointer set;
	set = dataStorage->GetDerivations( rootNode );

	mitk::DataStorage::SetOfObjects::const_iterator it;
	for ( it = set->begin( ) ; it != set->end( ) ; it++ )
	{
		(*it)->SetVisibility( visible );
	}
}

void mitk::wxMitkDisplayPlaneSubtreeHelper::AddDisplayPlaneSubTree( 
	mitk::wxMitkMultiRenderWindow* renderWindow )
{
	mitk::DataTreeNode::Pointer rootNode = AddDisplayPlaneSubTreeRoot(renderWindow->GetDataStorage( ));
	if ( rootNode.IsNull() )
	{
		return;
	}

	wxMitkMultiRenderWindow::WidgetListType sliceViews;
	sliceViews = renderWindow->GetLayout()->GetSliceViews();
	wxMitkMultiRenderWindow::WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		UpdateDisplayPlaneSubTree( (*it) );
	}
}

void mitk::wxMitkDisplayPlaneSubtreeHelper::ChangeColorToDisplayPlanes( 
	float colorFactor[3],
	mitk::wxMitkMultiRenderWindow* renderWindow )
{
	wxMitkMultiRenderWindow::WidgetListType sliceViews;
	sliceViews = renderWindow->GetLayout()->GetSliceViews();
	wxMitkMultiRenderWindow::WidgetListType::iterator it;
	for ( it = sliceViews.begin() ; it != sliceViews.end() ; it++ )
	{
		mitk::DataTreeNode::Pointer node;
		node = (*it)->GetRenderer()->GetCurrentWorldGeometry2DNode();
		float color[3];
		(*it)->GetColourAsFloat( color );
		color[ 0 ] *= colorFactor[ 0 ];
		color[ 1 ] *= colorFactor[ 1 ];
		color[ 2 ] *= colorFactor[ 2 ];
		node->SetColor(color);
	}
}

void mitk::wxMitkDisplayPlaneSubtreeHelper::ChangeColorToDisplayPlanes( 
	bool enable, mitk::wxMitkMultiRenderWindow* renderWindow )
{
	float colorFactor[3];
	if ( enable )
	{
		colorFactor[ 0 ] = 1;
		colorFactor[ 1 ] = 1;
		colorFactor[ 2 ] = 1;
	}
	else{
		colorFactor[ 0 ] = 0.5;
		colorFactor[ 1 ] = 0.5;
		colorFactor[ 2 ] = 0.5;
	}

	mitk::wxMitkDisplayPlaneSubtreeHelper::ChangeColorToDisplayPlanes( 
		colorFactor, renderWindow );
}

mitk::DataTreeNode::Pointer mitk::wxMitkDisplayPlaneSubtreeHelper::GetRootNode(
	const mitk::DataStorage::Pointer dataStorage )
{
	if ( dataStorage.IsNull() )
	{
		return NULL;
	}

	mitk::DataTreeNode::Pointer node;
	node = dataStorage->GetNamedNode( PLANE_SUB_TREE_ROOT_NODE_NAME );
	return node;
}

void mitk::wxMitkDisplayPlaneSubtreeHelper::RemoveDisplayPlaneSubTree( 
	wxMitkSelectableGLWidget* widget )
{
	if ( widget == NULL ||
		 widget->GetRenderer() == NULL || 
		 widget->GetRenderer()->GetDataStorage( ).IsNull() )
	{
		return;
	}
	mitk::DataStorage::Pointer dataStorage = widget->GetRenderer( )->GetDataStorage( );

	mitk::DataTreeNode::Pointer node = dataStorage->GetNamedNode( widget->GetName() );

	dataStorage->Remove( node );

}

mitk::DataTreeNode::Pointer 
mitk::wxMitkDisplayPlaneSubtreeHelper::UpdateDisplayPlaneSubTree( 
	wxMitkSelectableGLWidget* widget )
{
	// Check if node is already present
	mitk::DataStorage::Pointer dataStorage = widget->GetRenderer( )->GetDataStorage( );
	if ( dataStorage.IsNull() )
	{
		return NULL;
	}

	mitk::DataTreeNode::Pointer node;
	node = dataStorage->GetNamedNode( widget->GetName() );

	mitk::DataTreeNode::Pointer rootNode = GetRootNode( dataStorage );

	// If is not present, add it
	if ( rootNode.IsNotNull( ) && node.IsNull() )
	{
		node = widget->GetRenderer()->GetCurrentWorldGeometry2DNode();
		node->SetProperty("name", mitk::StringProperty::New( widget->GetName() ) );
		node->SetProperty("visible", mitk::BoolProperty::New( true ));
		node->SetProperty("helper object", mitk::BoolProperty::New(true));
		node->SetProperty("includeInBoundingBox", mitk::BoolProperty::New(false));

		mitk::Geometry2DDataMapper2D::Pointer mapper;
		mapper = mitk::Geometry2DDataMapper2D::New();
		node->SetMapper(mitk::BaseRenderer::Standard2D, mapper);

		dataStorage->Add(node, rootNode);
		mapper->SetDatastorageAndGeometryBaseNode(dataStorage, rootNode);
	}
	// Remove node
	else if ( rootNode.IsNull() && node.IsNotNull() )
	{
		dataStorage->Remove( node );
		node = NULL;
	}

	if ( node.IsNull() )
	{
		return NULL;
	}

	// Update color, because this can change
	float color[3];
	widget->GetColourAsFloat( color );
	node->SetColor(color);

	// Update visibility
	bool visible;
	rootNode->GetPropertyValue( "visible", visible );
	node->SetVisibility( visible );

	return node;
}

