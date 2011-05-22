/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBoundingBox.h"
#include "blNumericUtils.h"

Core::BoundingBox::BoundingBox()
{
	for( int i = 0; i < 3; ++i )
	{
		m_Box.SetIndex( i, 0 );
		m_Box.SetSize( i, 0 );
		m_Origin[ i ] = 0;
		m_Spacing[ i ] = 0;
		m_Min[i] = 0;
		m_Max[i] = 0;
	}
}

void Core::BoundingBox::CalculateBoundingBoxCoordinates()
{
	for( int i = 0; i < 3; ++i )
	{
		m_Min[i] = double(m_Origin[i] + m_Box.GetIndex(i) * m_Spacing[i]);
		m_Max[i] = double(m_Origin[i] + (m_Box.GetIndex(i) + m_Box.GetSize(i)) * m_Spacing[i]);
	}
}

double Core::BoundingBox::GetMeanSize()
{
	return (
		m_Max[0] - m_Min[0] + 
		m_Max[1] - m_Min[1] + 
		m_Max[2] - m_Min[2]) / 3.0;
}

void Core::BoundingBox::GetBounds( double bounds[ 6 ] )
{
	bounds[ 0 ] = m_Min[0];
	bounds[ 1 ] = m_Max[0];
	bounds[ 2 ] = m_Min[1];
	bounds[ 3 ] = m_Max[1];
	bounds[ 4 ] = m_Min[2];
	bounds[ 5 ] = m_Max[2];
}

double Core::BoundingBox::GetMean( int dimension )
{
	return ( 0.5 * (m_Min[ dimension ] + m_Max[ dimension ]) );
}


void Core::BoundingBox::GetHandleLocation(
	int dimension,
	itk::Point<double> &pointMin,
	itk::Point<double> &pointMax )
{
	pointMin[0] = GetMean( 0 );
	pointMin[1] = GetMean( 1 );
	pointMin[2] = GetMean( 2 );

	pointMax[0] = GetMean( 0 );
	pointMax[1] = GetMean( 1 );
	pointMax[2] = GetMean( 2 );

	pointMin[ dimension ] = m_Min[ dimension ];
	pointMax[ dimension ] = m_Max[ dimension ];

}

void Core::BoundingBox::SetHandleLocation( 
	int dimension, 
	itk::Point<double> &pointMin, 
	itk::Point<double> &pointMax )
{
	long index = m_Box.GetIndex( dimension );
	long size = m_Box.GetSize( dimension );

	index = blNumericUtils::round( ( pointMin[ dimension ] - m_Origin[ dimension ] ) / m_Spacing[ dimension ] );
	size = blNumericUtils::round( ( pointMax[ dimension ] - m_Origin[ dimension ] ) / m_Spacing[ dimension ] - index );
	m_Box.SetIndex( dimension, index );
	m_Box.SetSize( dimension, size );

	CalculateBoundingBoxCoordinates( );
}

itk::ImageBase<3>::PointType Core::BoundingBox::GetOrigin() const
{
	return m_Origin;
}

void Core::BoundingBox::SetOrigin( itk::ImageBase<3>::PointType val )
{
	m_Origin = val;
	CalculateBoundingBoxCoordinates( );
}

itk::ImageRegion<3> Core::BoundingBox::GetBox() const
{
	return m_Box;
}

void Core::BoundingBox::SetBox( itk::ImageRegion<3> val )
{
	m_Box = val;
	CalculateBoundingBoxCoordinates( );
}

itk::ImageBase<3>::SpacingType Core::BoundingBox::GetSpacing() const
{
	return m_Spacing;
}

void Core::BoundingBox::SetSpacing( itk::ImageBase<3>::SpacingType val )
{
	m_Spacing = val;
	CalculateBoundingBoxCoordinates( );
}
