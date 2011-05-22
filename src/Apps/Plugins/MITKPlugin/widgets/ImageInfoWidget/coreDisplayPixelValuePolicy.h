/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DISPLAYPIXELVALUEPOLICY_H
#define DISPLAYPIXELVALUEPOLICY_H

#include "corePointTrackingInteractorWithPolicy.h"
#include "coreImageInformation.h"
#include <CILabNamespaceMacros.h>
#include <itkImage.h>

namespace Core{

/**
\brief This is a concrete implementation of PointTrackingInteractorPolicy
that responds to the event of a new selected point by
displaying the pixel intensity in the view.

\ingroup gmWidgets
\author Maarten Nieber & Martin Bianculli
\date 28 feb 2008
*/

template< class TItkImageType >
class DisplayPixelValuePolicy : public PointTrackingInteractorPolicy
{
public:
	typedef TItkImageType ImageType;

	//Type of the information of the image
	typedef Core::DataHolder< Core::ImageInformation::Pointer > 
		ImageInformationHolder;

	coreDeclareSmartPointerClassMacro(
		DisplayPixelValuePolicy, 
		PointTrackingInteractorPolicy);	

	//! Sets the itk Image Pointer 
	void SetImage(typename TItkImageType::Pointer image);

	/**
	Sets the image information holder.
	\ param _imageInformationHolder - May not be NULL (or an assertion 
	is raised). If the holder has no
	subject, then a subject will be assigned to the holder automatically 
	using _imageInformationHolder->SetSubject.
	*/
	void SetInformationHolder(
		ImageInformationHolder::Pointer imageInformationHolder);

	/** Respond to the event that a new point has been selected in 
	PointTrackingInteractorWithPolicy.
	*/
	virtual void OnNewPoint(const Superclass::PointType& worldCoordinate);

protected:
	//! Default Constructor
	DisplayPixelValuePolicy(void);

	//! The volume Image
	typename ImageType::Pointer image;
	//! The image information holder
	ImageInformationHolder::Pointer m_ImageInformationHolder;
	//! Image coordinates
	itk::Point<double, 3> m_Coordinates;
	//! The pixel intensity value
	typename ImageType::PixelType pixelValue;
	//! The index point types
	typename ImageType::IndexType pixelIndex;
	//for transforming the point coordinates
	itk::Point<double, 3> m_PhysicalPoint;
	
	coreDeclareNoCopyConstructors(DisplayPixelValuePolicy);
};

} // Core

#include "coreDisplayPixelValuePolicy.txx"

#endif //DISPLAYPIXELVALUEPOLICY_H
