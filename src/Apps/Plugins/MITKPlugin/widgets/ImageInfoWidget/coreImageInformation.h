/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreImageInformation_H
#define coreImageInformation_H

#include "coreObject.h"
#include "coreDataHolder.h"
#include <CILabNamespaceMacros.h>
#include <boost/shared_ptr.hpp>
#include <itkPoint.h>

namespace Core{

/**
\brief This class is used to store some information on an image, such as 
the current location of the mouse pointer, and the value
at the current mouse pointer location.

\ingroup gmWidgets
\author Maarten Nieber
\date 25 feb 2008
*/

class ImageInformation : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(
		ImageInformation, 
		Core::SmartPointerObject);
	itk::Point<double, 3> coordinate;
	double pixelValue;
	bool coordinateIsInsideImage;

public:
	//! Purposely private
	ImageInformation()
	{
		coordinateIsInsideImage = false;
	};

private:
	coreDeclareNoCopyConstructors(ImageInformation);
	
};


} // Core

#endif //coreImageInformation_H
