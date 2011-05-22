/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blMitkAnnotatedCube.h"

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

blMitkAnnotatedCube::blMitkAnnotatedCube()
{
	m_RenderWindow = NULL;

	CreateAnnotatedCube();
}

blMitkAnnotatedCube::~blMitkAnnotatedCube()
{
}

void blMitkAnnotatedCube::SetRenderWindow( vtkRenderWindow* renderWindow )
{
	m_RenderWindow = renderWindow;
	m_Marker->SetInteractor( m_RenderWindow->GetInteractor() );
}

void blMitkAnnotatedCube::Enable (bool a)
{
	m_Marker->SetEnabled (a);
	if( a )
	{
		m_Marker->InteractiveOff();
	}
}

void blMitkAnnotatedCube::CreateAnnotatedCube()
{

	m_CubeActor = vtkSmartPointer<vtkAnnotatedCubeActor>::New();
	m_CubeActor->SetXPlusFaceText ("L");
	m_CubeActor->SetXMinusFaceText ("R");
	m_CubeActor->SetYPlusFaceText ("P");
	m_CubeActor->SetYMinusFaceText ("A");
	m_CubeActor->SetZPlusFaceText ("S");
	m_CubeActor->SetZMinusFaceText ("I");
	m_CubeActor->SetZFaceTextRotation (90);
	m_CubeActor->SetFaceTextScale (0.65);
	m_CubeActor->GetCubeProperty()->SetColor (0.5, 1, 1);
	m_CubeActor->GetTextEdgesProperty()->SetLineWidth (1);
	m_CubeActor->GetTextEdgesProperty()->SetDiffuse (0);
	m_CubeActor->GetTextEdgesProperty()->SetAmbient (1);
	m_CubeActor->GetTextEdgesProperty()->SetColor (0.18, 0.28, 0.23);

	m_CubeActor->SetTextEdgesVisibility (1);  
	m_CubeActor->SetCubeVisibility(1);  
	m_CubeActor->SetFaceTextVisibility(1);

	m_CubeActor->GetXPlusFaceProperty()->SetColor (1, 0, 0);
	m_CubeActor->GetXPlusFaceProperty()->SetInterpolationToFlat();
	m_CubeActor->GetXMinusFaceProperty()->SetColor (1, 0, 0);
	m_CubeActor->GetXMinusFaceProperty()->SetInterpolationToFlat();
	m_CubeActor->GetYPlusFaceProperty()->SetColor (0, 1, 0);
	m_CubeActor->GetYPlusFaceProperty()->SetInterpolationToFlat();
	m_CubeActor->GetYMinusFaceProperty()->SetColor (0, 1, 0);
	m_CubeActor->GetYMinusFaceProperty()->SetInterpolationToFlat();
	m_CubeActor->GetZPlusFaceProperty()->SetColor (0, 0, 1);
	m_CubeActor->GetZPlusFaceProperty()->SetInterpolationToFlat();
	m_CubeActor->GetZMinusFaceProperty()->SetColor (0, 0, 1);
	m_CubeActor->GetZMinusFaceProperty()->SetInterpolationToFlat();

	m_CubeAxes = vtkSmartPointer<vtkAxesActor>::New(); 
	m_CubeAxes->SetShaftTypeToCylinder();
	m_CubeAxes->SetXAxisLabelText ("x");
	m_CubeAxes->SetYAxisLabelText ("y");
	m_CubeAxes->SetZAxisLabelText ("z");
	m_CubeAxes->SetTotalLength (1.5, 1.5, 1.5);
	vtkSmartPointer<vtkTextProperty> tprop = vtkSmartPointer<vtkTextProperty>::New();
	tprop->ItalicOn();
	tprop->ShadowOn();
	tprop->SetFontFamilyToTimes();
	m_CubeAxes->GetXAxisCaptionActor2D()->SetCaptionTextProperty (tprop);
	m_CubeAxes->GetYAxisCaptionActor2D()->SetCaptionTextProperty (tprop);
	m_CubeAxes->GetZAxisCaptionActor2D()->SetCaptionTextProperty (tprop);

	vtkSmartPointer<vtkPropAssembly> assembly = vtkSmartPointer<vtkPropAssembly>::New();
	assembly->AddPart ( m_CubeAxes );
	assembly->AddPart ( m_CubeActor );

	m_Marker = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	m_Marker->SetOutlineColor (0.93, 0.57, 0.13);
	m_Marker->SetOrientationMarker (assembly);
	m_Marker->SetViewport (0.0, 0.0, 0.15, 0.3);
}

void blMitkAnnotatedCube::SetAxisColors( float colorXf[3], float colorYf[3], float colorZf[3] )
{
	double colorX[3] = {colorXf[0], colorXf[1], colorXf[2]};
	double colorY[3] = {colorYf[0], colorYf[1], colorYf[2]};
	double colorZ[3] = {colorZf[0], colorZf[1], colorZf[2]};
/*
	// To avoid to change the default orientation of the axis, 
	// we will switch the colors to the correct arrow
	m_CubeActor->GetXPlusFaceProperty()->SetColor (colorY);
	m_CubeActor->GetXMinusFaceProperty()->SetColor (colorY);
	m_CubeActor->GetYPlusFaceProperty()->SetColor (colorZ);
	m_CubeActor->GetYMinusFaceProperty()->SetColor (colorZ);
	m_CubeActor->GetZPlusFaceProperty()->SetColor (colorX);
	m_CubeActor->GetZMinusFaceProperty()->SetColor (colorX);

	m_CubeAxes->GetXAxisShaftProperty( )->SetColor( colorY );
	m_CubeAxes->GetYAxisShaftProperty( )->SetColor( colorZ );
	m_CubeAxes->GetZAxisShaftProperty( )->SetColor( colorX );

	m_CubeAxes->GetXAxisTipProperty( )->SetColor( colorY );
	m_CubeAxes->GetYAxisTipProperty( )->SetColor( colorZ );
	m_CubeAxes->GetZAxisTipProperty( )->SetColor( colorX );
*/
	m_CubeActor->GetXPlusFaceProperty()->SetColor (colorX);
	m_CubeActor->GetXMinusFaceProperty()->SetColor (colorX);
	m_CubeActor->GetYPlusFaceProperty()->SetColor (colorY);
	m_CubeActor->GetYMinusFaceProperty()->SetColor (colorY);
	m_CubeActor->GetZPlusFaceProperty()->SetColor (colorZ);
	m_CubeActor->GetZMinusFaceProperty()->SetColor (colorZ);

	m_CubeAxes->GetXAxisShaftProperty( )->SetColor( colorX );
	m_CubeAxes->GetYAxisShaftProperty( )->SetColor( colorY );
	m_CubeAxes->GetZAxisShaftProperty( )->SetColor( colorZ );

	m_CubeAxes->GetXAxisTipProperty( )->SetColor( colorX );
	m_CubeAxes->GetYAxisTipProperty( )->SetColor( colorY );
	m_CubeAxes->GetZAxisTipProperty( )->SetColor( colorZ );

}

