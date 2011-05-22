/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreCroppingBoundingBox_H
#define _coreCroppingBoundingBox_H

#include "itkImageBase.h"
#include "itkImageRegion.h"
#include "CILabItkMacros.h"
#include "corePluginMacros.h"

namespace Core{

/**
Stores information about the bounding box used for cropping.
All pixels inside CroppingProcessorData::m_Box are preserved in the 
cropped image.

\ingroup gmInteractors
\author Maarten Nieber
\date 18 nov 2008
*/

class PLUGIN_EXPORT BoundingBox : public itk::LightObject
{
public:
	CILAB_ITK_CLASS(
		BoundingBox, 
		itk::LightObject, 
		itkFactorylessNewMacro);

public:
	//! Get mean size on all dimensions
	double GetMeanSize( );

	//!
	void GetBounds( double bounds[ 6 ] );

	//!
	double GetMean( int dimension );

	//!
	void GetHandleLocation(
		int dimension,
		itk::Point<double> &pointMin,
		itk::Point<double> &pointMax );

	//! 
	void SetHandleLocation(
		int dimension,
		itk::Point<double> &pointMin,
		itk::Point<double> &pointMax );

	//!
	itk::ImageBase<3>::PointType GetOrigin() const;
	void SetOrigin(itk::ImageBase<3>::PointType val);

	//!
	itk::ImageRegion<3> GetBox() const;
	void SetBox(itk::ImageRegion<3> val);

	//!
	itk::ImageBase<3>::SpacingType GetSpacing() const;
	void SetSpacing(itk::ImageBase<3>::SpacingType val);

private:
	//! Update m_Min and m_Max based on m_Origin, m_Box and m_Spacing
	void CalculateBoundingBoxCoordinates( );

	//!
	BoundingBox( );

private:
	//! Box position in voxels used for cropping and to adjust it
	itk::ImageRegion<3> m_Box;

	//! Origin in mm
	itk::ImageBase<3>::PointType m_Origin;
	//! Spacing
	itk::ImageBase<3>::SpacingType m_Spacing;
	//! Point with the start of the bounding m_Box in each dimension (mm)
	double m_Min[ 3 ];
	//! Point with the end of the bounding m_Box in each dimension (mm)
	double m_Max[ 3 ];

};

typedef Core::DataHolder< Core::BoundingBox::Pointer > BoundingBoxHolder;

} // Core

#endif //_coreCroppingBoundingBox_H
