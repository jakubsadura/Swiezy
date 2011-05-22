/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef COREImageInfoWidget_H
#define COREImageInfoWidget_H

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
#include "coreImageInfoMediator.h"

class wxTextCtrl;

namespace Core{
typedef Core::DataHolder<Core::ImageInformation::Pointer> ImageInformationHolder;
} // Core

namespace Core{
namespace Widgets{

/**
This class is intended for displaying information on an image. 
Use ImageInfoMediator to connect this widget to an input image.

At the moment, the widget displays:

- current location of the mouse pointer in the image
- value at the current mouse pointer location

It uses an imageInformationHolder as input. When the image information in 
the holder changes, the widget will
update to show the changes.

\ingroup gmWidgets
\author Maarten Nieber
\date 25 Feb 2008
*/

class PLUGIN_EXPORT ImageInfoWidget : 
	public ImageWidgetUI, 
	public Core::BaseWindow
{
public:
	coreDefineBaseWindowFactory( ImageInfoWidget )
	coreClassNameMacro(ImageInfoWidget);

	//!
	ImageInfoWidget(wxWindow* parent, int id, const wxPoint&  pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = 0 );
	virtual ~ImageInfoWidget(void);
	//! Initialization done outside the constructor.
	//! This widget will display the information in \a imageInformationHolder.
	void Init
	(
		Core::ImageInformationHolder::Pointer imageInformationHolder
	);
	//!
	void OnInit( );
	//!
	void UpdateWidget(void);
	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:
	//! Used to store the pixel value at the current mouse position
	Core::ImageInformationHolder::Pointer m_ImageInformationHolder;

	//!
	Core::ImageInfoMediator::Pointer m_ImageMediator;
};

} // Core
} // Widgets)


#endif //COREImageInfoWidget_H

