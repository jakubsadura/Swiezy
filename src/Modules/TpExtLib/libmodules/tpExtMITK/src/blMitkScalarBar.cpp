/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blMitkScalarBar.h"

#include <itkCommand.h>

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkScalarBarActor.h"
#include "vtkRenderer.h"

#include "mitkVtkLayerController.h"
#include "mitkLookupTableProperty.h"

#include "mitkRenderingManager.h"
#include "mitkDataTreeNode.h"
#include "mitkRenderingManager.h"

#include "itkCommand.h"

blMitkScalarBar::blMitkScalarBar()
{
	m_RenderWindow = NULL;
	m_ScalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New( );
	m_ColorLegendNode = NULL;
	m_ColorLegendNodeTagDelete = 0;
	m_ColorLegendNodeTagModified = 0;
	m_isEnabled = false;
}

blMitkScalarBar::~blMitkScalarBar()
{
	Enable( false );
}

void blMitkScalarBar::SetRenderWindow( vtkRenderWindow* renderWindow )
{
	m_RenderWindow = renderWindow;
}

void blMitkScalarBar::SetDataTreeNode( mitk::DataTreeNode* node )
{
	// Remove old observers
	if ( m_ColorLegendNode )
	{
		m_ColorLegendNode->GetPropertyList()->RemoveObserver( m_ColorLegendNodeTagDelete );
		m_ColorLegendNode->GetPropertyList()->RemoveObserver( m_ColorLegendNodeTagModified );
		m_ColorLegendNode->SetBoolProperty( "blMitkScalarBar", false );
	}

	m_ColorLegendNode = node;

	// Add new observers
	if ( m_ColorLegendNode )
	{
		itk::SimpleMemberCommand<blMitkScalarBar>::Pointer command;

		// Add an observer to the node, to remove the actor when the node is deleted
		command = itk::SimpleMemberCommand<blMitkScalarBar>::New();
		command->SetCallbackFunction(
			this,
			&blMitkScalarBar::OnColorLegendNodeDeleted );
		m_ColorLegendNodeTagDelete = m_ColorLegendNode->GetPropertyList()->AddObserver(itk::DeleteEvent(), command);

		// Add an observer to the node, to update the legend
		command = itk::SimpleMemberCommand<blMitkScalarBar>::New();
		command->SetCallbackFunction(
			this,
			&blMitkScalarBar::OnColorLegendNodeModified );
		m_ColorLegendNodeTagModified = m_ColorLegendNode->GetPropertyList()->AddObserver(itk::ModifiedEvent(), command);
	}
}

void blMitkScalarBar::Enable( 
	bool enable )
{
	// Get render window
	vtkRenderer	*renderer = NULL;
	mitk::VtkLayerController *layerController = NULL;
	layerController = mitk::VtkLayerController::GetInstance(m_RenderWindow);
	if ( layerController )
	{
		renderer = layerController->GetSceneRenderer();
	}

	// Check if it was enabled
	if ( enable == m_isEnabled || renderer == NULL )
	{
		return;
	}
	m_isEnabled = enable;

	if ( enable && GetLookUpTable().IsNotNull() )
	{
		// Add actor
		renderer->AddActor( m_ScalarBarActor );

		// Update lookupTable
		vtkLookupTable *vtkLut = GetLookUpTable()->GetVtkLookupTable();
		m_ScalarBarActor->SetLookupTable( vtkLut );

		m_ColorLegendNode->SetBoolProperty( "blMitkScalarBar", true );
	}
	else
	{
		// Remove actor
		renderer->RemoveActor( m_ScalarBarActor );

		m_ColorLegendNode->SetBoolProperty( "blMitkScalarBar", false );
	}

	mitk::RenderingManager::GetInstance()->RequestUpdate( m_RenderWindow );
}

void blMitkScalarBar::OnColorLegendNodeDeleted()
{
	// We need a special removing because we cannot
	// RemoveObservers when a m_ColorLegendNode event is triggered
	Enable( false );
	vtkRenderer	*renderer;
	if (mitk::VtkLayerController::GetInstance(m_RenderWindow)) 
	{
		renderer = mitk::VtkLayerController::GetInstance(m_RenderWindow)->GetSceneRenderer();
		renderer->RemoveActor( m_ScalarBarActor );
		m_ColorLegendNode = NULL;
	}

}

void blMitkScalarBar::OnColorLegendNodeModified()
{
	if ( GetLookUpTable().IsNull() )
	{
		return;
	}

	// Set the new properties
	vtkLookupTable *vtkLut = GetLookUpTable()->GetVtkLookupTable();
	m_ScalarBarActor->SetLookupTable( vtkLut );
	mitk::RenderingManager::GetInstance()->RequestUpdate( m_RenderWindow );
}

mitk::LookupTable::Pointer blMitkScalarBar::GetLookUpTable()
{
	if ( !m_ColorLegendNode )
	{
		return NULL;
	}

	mitk::LookupTableProperty::Pointer mitkLutProp;
	m_ColorLegendNode->GetProperty( mitkLutProp, "LookupTable" );

	mitk::LookupTable::Pointer mitkLut;
	if ( mitkLutProp.IsNotNull() )
	{
		mitkLut = mitkLutProp->GetLookupTable();
	}

	return mitkLut;
}

void blMitkScalarBar::SetPropertiesLegend(double xPos, double yPos, double Height, double Width)
{
	// Get render window
	vtkRenderer	*renderer = NULL;
	mitk::VtkLayerController *layerController = NULL;
	layerController = mitk::VtkLayerController::GetInstance(m_RenderWindow);
	if ( layerController )
	{
		renderer = layerController->GetSceneRenderer();
	}
	
	// Add actor
	renderer->AddActor( m_ScalarBarActor );

	if (GetLookUpTable().IsNotNull() )
	{
		// Change Position
		m_ScalarBarActor->GetPositionCoordinate()->SetValue(xPos, yPos);
		m_ScalarBarActor->SetHeight(Height);
		m_ScalarBarActor->SetWidth(Width);
	}
	else
	{
		// Remove actor
		renderer->RemoveActor( m_ScalarBarActor );
	}

	mitk::RenderingManager::GetInstance()->RequestUpdate( m_RenderWindow );
}

bool blMitkScalarBar::IsEnabled()
{
	return m_isEnabled;
}

mitk::DataTreeNode* blMitkScalarBar::GetDataTreeNode()
{
	return m_ColorLegendNode;
}
