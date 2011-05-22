/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "mitkTransform.h"

mitk::Transform::Transform() : mitk::BaseData()
{
}

mitk::Transform::~Transform()
{
}

void mitk::Transform::SetRequestedRegionToLargestPossibleRegion()
{

}

bool mitk::Transform::RequestedRegionIsOutsideOfTheBufferedRegion()
{
	return false;
}

bool mitk::Transform::VerifyRequestedRegion()
{
	return true;
}

void mitk::Transform::SetRequestedRegion( itk::DataObject *data )
{

}

void mitk::Transform::SetTransform( int pos, TransformPointer transform )
{
	m_Transform[ pos ] = transform;
}

void mitk::Transform::Initialize( int numTimeSteps )
{
	InitializeTimeSlicedGeometry( numTimeSteps );
	m_Transform.resize( numTimeSteps );
}

mitk::Transform::TransformPointer mitk::Transform::GetTransform( int pos )
{
	return m_Transform[ pos ];
}
