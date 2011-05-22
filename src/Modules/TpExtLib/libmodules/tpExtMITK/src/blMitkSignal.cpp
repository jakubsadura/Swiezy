/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blMitkSignal.h"

blMitk::Signal::Signal() : mitk::BaseData()
{
	InitializeTimeSlicedGeometry( 1 );
}

blMitk::Signal::~Signal()
{
}

void blMitk::Signal::SetRequestedRegionToLargestPossibleRegion()
{

}

bool blMitk::Signal::RequestedRegionIsOutsideOfTheBufferedRegion()
{
	return false;
}

bool blMitk::Signal::VerifyRequestedRegion()
{
	return true;
}

void blMitk::Signal::SetRequestedRegion( itk::DataObject *data )
{

}

void blMitk::Signal::SetSignal( blSignalCollective::Pointer signal )
{
	m_Signal = signal;
}
