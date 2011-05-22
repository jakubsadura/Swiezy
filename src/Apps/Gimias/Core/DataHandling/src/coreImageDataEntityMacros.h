/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef COREIMAGEDATAENTITYMACROS_H
#define COREIMAGEDATAENTITYMACROS_H

#include "gmDataHandlingWin32Header.h"
#include "CILabExceptionMacros.h"
#include "itkImage.h"
#include "CILabNamespaceMacros.h"
#include "CILabAssertMacros.h"
#include "coreException.h"
#include "coreCreateExceptionMacros.h"
#include "itkSmartPointer.h"
#include "itkVTKImageToImageFilter.h"
#include "coreVTKImageDataHolder.h"

/**
Used when the user tries to convert an unknown image type to an itk image.
\ingroup gmDataHandling
\author Maarten Nieber
\date 15 sep 2008
*/
coreCreateRuntimeExceptionClassMacro(
	UnknownImageTypeException, 
	"Cannot convert data inside DataEntity to an itk image", 
	GMDATAHANDLING_EXPORT
)

namespace Core{

/**
Helper function for coreImageDataEntityItkMacro (I like to be able to 
put a breakpoint and check the conversion result
\ingroup gmDataHandling
\author Maarten Nieber
\date 15 sep 2008
*/
inline void ImageDataEntityItkMacroFinalize(bool foundCast)
{
	if( !foundCast )
	{
		throw Core::Exceptions::UnknownImageTypeException("ImageDataEntityItkMacroFinalize");
	}
}

} // Core

/**
Helper macro (used only internally). It tries to directly cast the image 
inside argProcessingData to an itk image, and then calls argFunction 
with that itk image.

\ingroup gmDataHandling
\author Maarten Nieber
\date 15 sep 2008
*/
#define coreImageDataEntityItkMacroImplItk(argFoundCast, argType, argProcessingData, argFunction) \
	if( !argFoundCast ) \
	{ \
		typedef itk::Image<argType, 3> ItkImageType; \
		ItkImageType::Pointer image; \
		if( argFoundCast = Core::CastAnyProcessingData(argProcessingData, image) ) \
		{ \
			argFunction< ItkImageType >(image); \
		} \
	}

/**
Helper macro (used only internally). It tries to cast the image inside 
argProcessingData to a vtk image, and then convert that vtk image to an 
itk image, and then calls argFunction with the itk image.

\ingroup gmDataHandling
\author Maarten Nieber
\date 15 sep 2008
*/
#define coreImageDataEntityItkMacroImplVtk(argFoundCast, argType, argVtkTypeId, argProcessingData, argFunction) \
	if( !argFoundCast ) \
	{ \
		Core::vtkImageDataPtr imageVtk; \
		if( Core::CastAnyProcessingData(argProcessingData, imageVtk) ) \
		{ \
			if( imageVtk->GetScalarType() == argVtkTypeId ) \
			{ \
				typedef itk::Image<argType, 3> ItkImageType; \
				ItkImageType::Pointer imageItk; \
				typedef itk::VTKImageToImageFilter< ItkImageType > VtkToItkAdapterType; \
				VtkToItkAdapterType::Pointer vtkToItkFilter = VtkToItkAdapterType::New(); \
				vtkToItkFilter->SetInput( imageVtk );	\
				vtkToItkFilter->Update(); \
				imageItk = ItkImageType::Pointer(const_cast<ItkImageType*>(vtkToItkFilter->GetOutput())); \
				argFoundCast = imageItk.IsNotNull();\
				if( argFoundCast ) \
				{ \
					argFunction< ItkImageType >(imageItk); \
				} \
			} \
		} \
	}

/**
This macro tries to convert the image inside argDataEntity to an itk image,
without copying any voxels, and then calls argFunction with that itk image.
Throws Core::UnknownImageTypeException if the input image type is unknown.

\ingroup gmDataHandling
\author Maarten Nieber
\date 15 sep 2008
*/
#define coreImageDataEntityItkMacro(argDataEntity, argFunction) \
	bool foundCast = false; \
	Core::AnyProcessingData processingData = argDataEntity->GetProcessingData(); \
	coreImageDataEntityItkMacroImplVtk(foundCast, char, VTK_SIGNED_CHAR, processingData, argFunction) \
	coreImageDataEntityItkMacroImplVtk(foundCast, unsigned char, VTK_UNSIGNED_CHAR, processingData, argFunction) \
	coreImageDataEntityItkMacroImplVtk(foundCast, short, VTK_SHORT, processingData, argFunction) \
	coreImageDataEntityItkMacroImplVtk(foundCast, unsigned short, VTK_UNSIGNED_SHORT, processingData, argFunction) \
	coreImageDataEntityItkMacroImplVtk(foundCast, int, VTK_INT, processingData, argFunction) \
	coreImageDataEntityItkMacroImplVtk(foundCast, unsigned int, VTK_UNSIGNED_INT, processingData, argFunction) \
	coreImageDataEntityItkMacroImplVtk(foundCast, long, VTK_LONG, processingData, argFunction) \
	coreImageDataEntityItkMacroImplVtk(foundCast, unsigned long, VTK_UNSIGNED_LONG, processingData, argFunction) \
	coreImageDataEntityItkMacroImplVtk(foundCast, float, VTK_FLOAT, processingData, argFunction) \
	coreImageDataEntityItkMacroImplVtk(foundCast, double, VTK_DOUBLE, processingData, argFunction) \
	\
	coreImageDataEntityItkMacroImplItk(foundCast, char, processingData, argFunction) \
	coreImageDataEntityItkMacroImplItk(foundCast, unsigned char, processingData, argFunction) \
	coreImageDataEntityItkMacroImplItk(foundCast, short, processingData, argFunction) \
	coreImageDataEntityItkMacroImplItk(foundCast, unsigned short, processingData, argFunction) \
	coreImageDataEntityItkMacroImplItk(foundCast, int, processingData, argFunction) \
	coreImageDataEntityItkMacroImplItk(foundCast, unsigned int, processingData, argFunction) \
	coreImageDataEntityItkMacroImplItk(foundCast, long, processingData, argFunction) \
	coreImageDataEntityItkMacroImplItk(foundCast, unsigned long, processingData, argFunction) \
	coreImageDataEntityItkMacroImplItk(foundCast, float, processingData, argFunction) \
	coreImageDataEntityItkMacroImplItk(foundCast, double, processingData, argFunction) \
	Core::ImageDataEntityItkMacroFinalize(foundCast);

#endif //COREIMAGEDATAENTITYMACROS_H
