/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef COREImageInfoMediator_H
#define COREImageInfoMediator_H

#include "coreObject.h"
#include "coreDataHolder.h"
#include "CILabItkMacros.h"
#include <CILabNamespaceMacros.h>
#include <boost/shared_ptr.hpp>
#include <itkPoint.h>
#include <wx/panel.h>
#include "coreImageWidgetUI.h"
#include "coreDataEntity.h"
#include "coreDisplayPixelValuePolicy.h"
#include "coreBaseWindow.h"

class wxTextCtrl;

namespace Core{
typedef Core::DataHolder<Core::ImageInformation::Pointer> ImageInformationHolder;
} // Core

namespace Core{

	namespace Widgets{ class ImageInfoWidget; };

/**
This class mediates between a DataEntityHolder and ImageInformationWidget.
When the DataEntityHolder contains an image, then the pixel values of this image 
(at the current mouse location) will be displayed in the widget.

Example:

Core::Widgets::ImageInfoWidget* widget = new Core::Widgets::ImageInfoWidget(NULL, wxNewId());
Core::DataEntityHolder::Pointer inputImageDataEntityHolder = Core::DataEntityHolder::New();

ImageInfoMediator::Pointer mediator = ImageInfoMediator::New();
mediator->Init(widget, inputImageDataEntityHolder);

\ingroup gmWidgets
\author Maarten Nieber
\date 30 oct 2008
*/

class PLUGIN_EXPORT ImageInfoMediator : public itk::LightObject
{
public:
	CILAB_ITK_CLASS(ImageInfoMediator, itk::LightObject, itkFactorylessNewMacro);

	/** After calling this function, the mediator will listen for mouse 
	events that signal that the mouse is moved over a pixel
	 and copy them to this->m_ImageInformationHolder.
	Also, it connects \a widget to m_ImageInformationHolder, so that 
	pixel values are displayed in the widget.
	*/
	void Init
	(
		Core::Widgets::ImageInfoWidget* widget,
		Core::DataEntityHolder::Pointer inputImageDataEntityHolder
	);

private:
	//!
	ImageInfoMediator();
	//!
	~ImageInfoMediator();

	//! Respond to a change in m_InputImageDataEntityHolderConnection
	void OnNewImageDataEntity();

	//! Remove m_MouseTracker interactor from mitk::GlobalInteraction
	void RemoveInteractor();

	//! Add m_MouseTracker interactor to mitk::GlobalInteraction
	void AddInteractor();

	/** Helper function that instantiates a PointTrackingInteractorPolicy 
	that copies the pixel value at the current mouse position to 
	m_ImageInformationHolder.
	*/
	template< class ItkImageType > void InstantiatePixelPolicy(
		typename ItkImageType::Pointer image)
	{
		typename Core::DisplayPixelValuePolicy<ItkImageType>::Pointer policy = 
			Core::DisplayPixelValuePolicy<ItkImageType>::New();
		policy->SetImage( image );
		policy->SetInformationHolder(m_ImageInformationHolder);
		this->m_IntensityValuePolicy = policy;
	}

private:
	//! Used to store the pixel value at the current mouse position
	Core::ImageInformationHolder::Pointer m_ImageInformationHolder;
	//! Holds the image for which pixel information is displayed
	Core::DataEntityHolder::Pointer m_InputImageDataEntityHolder;
	//! Tracks the mouse position on the input image
	Core::PointTrackingInteractorWithPolicy::Pointer m_MouseTracker;
	/** A point tracking policy for acquiring an image point coordinates 
	and intensity values
	*/
	Core::PointTrackingInteractorPolicy::Pointer m_IntensityValuePolicy;
};

} // Core

#endif //COREImageInfoMediator_H

