/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

/**
This is a concrete implementation of PointTrackingInteractorPolicy that 
responds to the event of a new selected point by
displaying the pixel intensity in the view.

\author Martin Bianculli
28 feb 2008
*/

#include "coreDisplayPixelValuePolicy.h"
#include <coreImageInfoWidget.h>
#include <coreReportExceptionMacros.h>
#include <CILabAssertMacros.h>
#include "itkImage.h"
using namespace Core;

template< class TItkImageType >
void DisplayPixelValuePolicy<TItkImageType>::SetImage(
	typename TItkImageType::Pointer image)
{
	this->image = image;
}

template< class TItkImageType >
void DisplayPixelValuePolicy<TItkImageType>::SetInformationHolder( 
	ImageInformationHolder::Pointer imageInformationHolder) 
{
	cilabAssertMacro(imageInformationHolder && "Arg imageInformationHolder may not be NULL");
	this->m_ImageInformationHolder = imageInformationHolder;
	if( !this->m_ImageInformationHolder->GetSubject() )
	{
		this->m_ImageInformationHolder->SetSubject( 
			Core::ImageInformation::New() );
	}
}

template< class TItkImageType >
void DisplayPixelValuePolicy<TItkImageType>::OnNewPoint(
	const Superclass::PointType& worldCoordinate)
{
	Core::ImageInformation::Pointer imageInformation = 
		this->m_ImageInformationHolder->GetSubject();
	if( imageInformation )
	{
		imageInformation->coordinateIsInsideImage = false;
	}

	try
	{
		/*Perform a picking: find the x,y,z world coordinate of a display 
		x,y coordinate. 
		_worldCoordinates are a PhysicalPoint. */
		
		//Transformation between world and index coordinates
		this->m_PhysicalPoint[0] = worldCoordinate[0];
		this->m_PhysicalPoint[1] = worldCoordinate[1];
		this->m_PhysicalPoint[2] = worldCoordinate[2];

		if( this->image.IsNotNull( ) 
			&& this->image->TransformPhysicalPointToIndex(
				this->m_PhysicalPoint,this->pixelIndex) )
		{
			//Get the image pixel intensity value
			//this->pixelIndex[1] = this->image->GetLargestPossibleRegion().GetSize()[1] 
			// - 1 - this->pixelIndex[1];
			this->pixelValue = this->image->GetPixel( this->pixelIndex );

			//Set the coordinates to pixel index
			this->m_Coordinates[0] = this->pixelIndex[0]; // x position
			this->m_Coordinates[1] = this->pixelIndex[1]; // y position
			this->m_Coordinates[2] = this->pixelIndex[2]; // z position

			//put the values in the data image information holder
			Core::ImageInformation::Pointer imageInformation = 
				this->m_ImageInformationHolder->GetSubject();
			if( imageInformation )
			{
				imageInformation->coordinateIsInsideImage = true;
				imageInformation->pixelValue = static_cast<double>(this->pixelValue);
				imageInformation->coordinate = this->m_Coordinates;
				this->m_ImageInformationHolder->NotifyObservers();
			}
		}
		else
		{
			this->m_Coordinates[0] = 0; // x position
			this->m_Coordinates[1] = 0; // y position
			this->m_Coordinates[2] = 0; // z position
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(DisplayPixelValuePolicy::OnNewPoint);
}

template< class TItkImageType >
DisplayPixelValuePolicy<TItkImageType>::DisplayPixelValuePolicy()
{
}
