/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blSliceImage.h"

#include <algorithm>

blSliceImage::blSliceImage()
{
}

blSliceImage::~blSliceImage()
{
}

void blSliceImage::SetNumberOfSlices(int number)
{
	m_slices.resize(number);
}

void blSliceImage::SetSlicePosition( base::VectorFloatType slicePosition, int id )
{
	m_slices.at(id).position = slicePosition;
}

void blSliceImage::SetSlicePosition( double slicePosition[3], int id )
{
	m_slices.at(id).position.resize(3);
	for (size_t i =0; i < 3; i++) 
		m_slices.at(id).position.at(i) = (float)slicePosition[i];
}

base::VectorFloatType blSliceImage::GetSlicePosition(  int id  )
{
	return m_slices.at(id).position;
}

void blSliceImage::SetSliceOrientation( base::VectorFloatType sliceOrientation, int id )
{
	m_slices.at(id).orientation = sliceOrientation;
}

void blSliceImage::SetSliceOrientation( double sliceOrientation[6], int id )
{
	m_slices.at(id).orientation.resize(6);
	for (size_t i =0; i < 6; i++) 
		m_slices.at(id).orientation.at(i) = sliceOrientation[i];
}
	
base::VectorFloatType blSliceImage::GetSliceOrientation( int id  )
{
	return m_slices.at(id).orientation;
}

int blSliceImage::GetNumberOfSlices()
{
	return m_slices.size( );
}

SliceBase blSliceImage::GetBaseSlice( int i ) const
{
	SliceBase slice;
	slice.orientation = m_slices.at( i ).orientation;
	slice.position = m_slices.at( i ).position;
	slice.pixelTypeAsString = m_slices.at( i ).pixelTypeAsString;
	return slice;
}

