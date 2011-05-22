/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-04-23 13:50:34 +0200 (Do, 23 Apr 2009) $
Version:   $Revision: 16947 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "mitkTransformMapper2D.h"
#include "mitkPlanarFigure.h"

#include "mitkBaseRenderer.h"
#include "mitkPlaneGeometry.h"
#include "mitkColorProperty.h"
#include "mitkProperties.h"
#include "mitkGL.h"
#include "mitkTransform.h"


mitk::TransformMapper2D::TransformMapper2D()
{
}


mitk::TransformMapper2D::~TransformMapper2D()
{
}




void mitk::TransformMapper2D::Paint( mitk::BaseRenderer *renderer )
{
  if ( !this->IsVisible( renderer ) ) 
  {
    return;
  }

  // Get current world 2D geometry from renderer
  Geometry2D::ConstPointer rendererGeometry2D = renderer->GetCurrentWorldGeometry2D();

  // Get display geometry
  mitk::DisplayGeometry *displayGeometry = renderer->GetDisplayGeometry();
  assert( displayGeometry != NULL );

  // Apply visual appearance properties from the PropertyList
  this->ApplyProperties( renderer );

  // Get node properties
  const mitk::DataTreeNode* node = this->GetDataTreeNode();
  mitk::Vector3D gridSpacing;
  gridSpacing[ 0 ] = 0.5;
  gridSpacing[ 1 ] = 0.5;
  gridSpacing[ 2 ] = 0.5;
  mitk::Vector3D gridOffset;
  node->GetFloatProperty( "gridSpacingX", gridSpacing[ 0 ] );
  node->GetFloatProperty( "gridSpacingY", gridSpacing[ 1 ] );
  node->GetFloatProperty( "gridSpacingZ", gridSpacing[ 2 ] );
  node->GetFloatProperty( "gridOffsetX", gridOffset[ 0 ] );
  node->GetFloatProperty( "gridOffsetY", gridOffset[ 1 ] );
  node->GetFloatProperty( "gridOffsetZ", gridOffset[ 2 ] );

  // Get ITK transform
  typedef itk::Transform< double, 3, 3 > GenericTransformType;
  mitk::Transform::Pointer transfData = dynamic_cast<mitk::Transform*> ( node->GetData() );
  if ( transfData.IsNull() )
  {
	  return;
  }
  mitk::Transform::TransformPointer transf = transfData->GetTransform( GetTimestep() );
  if ( transf.IsNull( ) )
  {
	  return;
  }
  GenericTransformType::Pointer trans = dynamic_cast<GenericTransformType*> ( transf.GetPointer() );
  if ( trans.IsNull( ) )
  {
	  return;
  }

  PlaneGeometry::ConstPointer worldPlaneGeometry = dynamic_cast<const PlaneGeometry*>(rendererGeometry2D.GetPointer());
  if(worldPlaneGeometry.IsNull())
  {
	  return;
  }

  // Get the 4 corners
  mitk::Point3D worldPoint;
  std::list<mitk::Point3D> pointsList;
  mitk::Point3D corner1 = worldPlaneGeometry->GetCornerPoint( 1 );
  mitk::Point3D corner2 = worldPlaneGeometry->GetCornerPoint( 2 );
  mitk::Point3D corner4 = worldPlaneGeometry->GetCornerPoint( 4 );
  mitk::Point3D corner6 = worldPlaneGeometry->GetCornerPoint( 6 );

  mitk::Vector3D incY = ( corner2 - corner1 );
  int gridSizeY = abs( incY[ 0 ] / gridSpacing[ 0 ] + incY[ 1 ] / gridSpacing[ 1 ] + incY[ 2 ] / gridSpacing[ 2 ] );
  incY = ( corner2 - corner1 ) / gridSizeY;

  mitk::Vector3D incX = ( corner6 - corner2 );
  int gridSizeX = abs( incX[ 0 ] / gridSpacing[ 0 ] + incX[ 1 ] / gridSpacing[ 1 ] + incX[ 2 ] / gridSpacing[ 2 ] );
  incX = ( corner6 - corner2 ) / gridSizeX;

  mitk::Point3D point1 = corner1;
  mitk::Point3D point2 = corner1 + incY;
  // Horizontal lines
  for ( int i = 0 ; i < gridSizeX + 1 ; i++ )
  {
	mitk::Point3D currentPoint = corner1 + incX * i + gridOffset;
	mitk::Point3D currentPoint1 = corner1 + incX * (i+1) + gridOffset;
	// Vertical lines
	mitk::Vector3D currentIncY;
	currentIncY.Fill( 0 );
	for ( int j = 0 ; j < gridSizeY + 1; j++ )
	{
		if ( i != gridSizeX )
		{
			point1 = currentPoint + currentIncY;
			point2 = currentPoint1 + currentIncY;
			pointsList.push_back( point1 );
			pointsList.push_back( point2 );
		}
		if ( j != gridSizeY )
		{
			point1 = currentPoint + currentIncY;
			point2 = currentPoint + currentIncY + incY;
			pointsList.push_back( point1 );
			pointsList.push_back( point2 );
		}
		currentIncY = currentIncY + incY;
	}
  }


  // Draw this 2D point as OpenGL vertex
  std::list<mitk::Point3D>::iterator it;
  glBegin ( GL_LINES );
  for( it = pointsList.begin(); it != pointsList.end(); ++it)
  {
	mitk::Point3D transformedPoint = trans->TransformPoint( *it );

    mitk::Point2D worldPoint;
	rendererGeometry2D->Map( transformedPoint, worldPoint );

	mitk::Point2D displayPoint;
	displayGeometry->WorldToDisplay( worldPoint, displayPoint );

    glVertex2f( displayPoint[0], displayPoint[1] );

  }
  glEnd();

}

void mitk::TransformMapper2D::SetDefaultProperties( mitk::DataTreeNode* node, mitk::BaseRenderer* renderer, bool overwrite )
{
  node->AddProperty( "visible", mitk::BoolProperty::New(true), renderer, overwrite );
  node->AddProperty( "color", mitk::ColorProperty::New(1.0, 1.0, 1.0), renderer, overwrite );
  node->AddProperty( "gridSpacingX", mitk::FloatProperty::New( 1.5 ) );
  node->AddProperty( "gridSpacingY", mitk::FloatProperty::New( 1.5 ) );
  node->AddProperty( "gridSpacingZ", mitk::FloatProperty::New( 1.5 ) );
  node->AddProperty( "gridOffsetX", mitk::FloatProperty::New( 0.0 ) );
  node->AddProperty( "gridOffsetY", mitk::FloatProperty::New( 0.0 ) );
  node->AddProperty( "gridOffsetZ", mitk::FloatProperty::New( 0.0 ) );
}
