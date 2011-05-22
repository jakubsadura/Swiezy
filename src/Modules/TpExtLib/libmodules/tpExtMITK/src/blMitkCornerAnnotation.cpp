/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blMitkCornerAnnotation.h"

#include "vtkAnnotatedCubeActor.h"
#include "mitkVtkLayerController.h"

#include "vtkProperty.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkPropAssembly.h"
#include "vtkAxesActor.h"
#include "vtkTextProperty.h"
#include "vtkCaptionActor2D.h"
#include "vtkCornerAnnotation.h"
#include "vtkOrientationAnnotation.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"

blMitkCornerAnnotation::blMitkCornerAnnotation()
{
	m_RenderWindow = NULL;

	m_bShowCornerAnnotations = true; 
	m_bShowOrientationAnnotations = true;

	CreateCornerAnnotations( );
}

blMitkCornerAnnotation::~blMitkCornerAnnotation()
{
}

void blMitkCornerAnnotation::SetRenderWindow( vtkRenderWindow* renderWindow )
{
	m_RenderWindow = renderWindow;
	vtkRenderer	*renderer;
	renderer = mitk::VtkLayerController::GetInstance(renderWindow)->GetSceneRenderer();
	renderer->AddActor( m_CornerAnnotation );
	renderer->AddActor( m_OrientationAnnotation );
}

void blMitkCornerAnnotation::CreateCornerAnnotations()
{
	m_CornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
	m_CornerAnnotation->SetNonlinearFontScaleFactor (0.3);
	m_CornerAnnotation->SetTextProperty ( vtkSmartPointer<vtkTextProperty>::New() );
	m_OrientationAnnotation = vtkSmartPointer<vtkOrientationAnnotation>::New();
	m_OrientationAnnotation->SetNonlinearFontScaleFactor (0.25);
}

std::string blMitkCornerAnnotation::GetNorthAnnotation() const
{
	return m_NorthAnnotation;
}

void blMitkCornerAnnotation::SetNorthAnnotation( const char* val )
{
	m_NorthAnnotation = val;
	UpdateAnnotations();
}

void blMitkCornerAnnotation::SetSouthAnnotation( const char* val )
{
	m_SouthAnnotation = val;
	UpdateAnnotations();
}

std::string blMitkCornerAnnotation::GetSouthAnnotation() const
{
	return m_SouthAnnotation;
}

std::string blMitkCornerAnnotation::GetEastAnnotation() const
{
	return m_EastAnnotation;
}

void blMitkCornerAnnotation::SetEastAnnotation( const char* val )
{
	m_EastAnnotation = val;
	UpdateAnnotations();
}

std::string blMitkCornerAnnotation::GetOuestAnnotation() const
{
	return m_OuestAnnotation;
}

void blMitkCornerAnnotation::SetWestAnnotation( const char* val )
{
	m_OuestAnnotation = val;
	UpdateAnnotations();
}


void blMitkCornerAnnotation::UpdateAnnotations()
{
	if ( GetShowCornerAnnotations() )
	{
		m_CornerAnnotation->SetText (1, GetDownRightAnnotation().c_str() );
		m_CornerAnnotation->SetText (2, GetUpLeftAnnotation().c_str() );
		m_CornerAnnotation->SetText (3, GetUpRightAnnotation().c_str() );
	}
	else
	{
		m_CornerAnnotation->SetText (1, "" );
		m_CornerAnnotation->SetText (2, "" );
		m_CornerAnnotation->SetText (3, "" );
	}
	//always show about data...
	m_CornerAnnotation->SetText (0, GetDownLeftAnnotation().c_str() );

	if ( GetShowOrientationAnnotations( ) )
	{
		m_OrientationAnnotation->SetText (0, GetEastAnnotation().c_str() );
		m_OrientationAnnotation->SetText (1, GetNorthAnnotation().c_str() );
		m_OrientationAnnotation->SetText (2, GetOuestAnnotation().c_str() );
		m_OrientationAnnotation->SetText (3, GetSouthAnnotation().c_str() );
	}
	else
	{
		m_OrientationAnnotation->SetText (0, "" );
		m_OrientationAnnotation->SetText (1, "" );
		m_OrientationAnnotation->SetText (2, "" );
		m_OrientationAnnotation->SetText (3, "" );
	}
}

std::string blMitkCornerAnnotation::GetUpLeftAnnotation() const
{
	return m_upLeftAnnotation;
}

void blMitkCornerAnnotation::SetUpLeftAnnotation( const char* val )
{
	m_upLeftAnnotation = val;
	UpdateAnnotations();
}

std::string blMitkCornerAnnotation::GetUpRightAnnotation() const
{
	return m_upRightAnnotation;
}

void blMitkCornerAnnotation::SetUpRightAnnotation( const char* val )
{
	m_upRightAnnotation = val;
	UpdateAnnotations();
}

std::string blMitkCornerAnnotation::GetDownLeftAnnotation() const
{
	return m_downLeftAnnotation;
}

void blMitkCornerAnnotation::SetDownLeftAnnotation( const char* val )
{
	m_downLeftAnnotation = val;
	UpdateAnnotations();
}

std::string blMitkCornerAnnotation::GetDownRightAnnotation() const
{
	return m_downRightAnnotation;
}

void blMitkCornerAnnotation::SetDownRightAnnotation( const char* val )
{
	m_downRightAnnotation = val;
	UpdateAnnotations();
}

bool blMitkCornerAnnotation::GetShowCornerAnnotations() const
{
	return m_bShowCornerAnnotations;
}

void blMitkCornerAnnotation::SetShowCornerAnnotations( bool val )
{
	m_bShowCornerAnnotations = val;
	UpdateAnnotations();
}

bool blMitkCornerAnnotation::GetShowOrientationAnnotations() const
{
	return m_bShowOrientationAnnotations;
}

void blMitkCornerAnnotation::SetShowOrientationAnnotations( bool val )
{
	m_bShowOrientationAnnotations = val;
	UpdateAnnotations();
}

