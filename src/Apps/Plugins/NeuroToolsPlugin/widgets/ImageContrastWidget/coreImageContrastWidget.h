/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef coreImageContrastWidget_H
#define coreImageContrastWidget_H

#include "coreImageContrastWidgetUI.h"

#include "coreObject.h"
#include "coreDataHolder.h"
#include "CILabItkMacros.h"
#include <CILabNamespaceMacros.h>
#include "coreInputControl.h"
#include "wxMitkLevelWindowWidget.h"
#include "mitkLevelWindowManager.h"
#include "itkCommand.h"
#include "coreBaseWindow.h"

#define wxID_ImageContrastWidget wxID("wxID_ImageContrastWidget")

namespace Core{
namespace Widgets{

/**
This class is intended for changing the contrast of a difference image (an image
that has positive and negative contrast values).

A difference image has negative and positive contrast values. This widget
allows to change the negative and the positive values independently

It uses an imageInformationHolder as input. When the image information in 
the holder changes, the widget update the range of the slider.

When the level window of the working area is changed, the widget will be notify and it will update its state

\ingroup gmWidgets
\author Martin Bianculli
\date Aug 2009
*/

class PLUGIN_EXPORT ImageContrastWidget : public coreImageContrastWidgetUI, public Core::BaseWindow
{
public:
	coreClassNameMacro(ImageContrastWidget);
	coreDefineBaseWindowFactory( ImageContrastWidget )

	typedef Core::DataHolder< mitk::LevelWindowProperty::Pointer > LevelWindowPropertyHolder;

	//!
	ImageContrastWidget(
		wxWindow* parent, 
		int id = wxID_ImageContrastWidget, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

	virtual ~ImageContrastWidget(void);

	//! Initialization done outside the constructor.
	void SetListBrowser(DataEntityListBrowser* dataEntityListBrowser);

	//! Updates the widget state whenever the mitk::levelWindow is changed or whenever the input is changed
	void UpdateWidget(void);

	//!
	void OnModifiedInput(void);

	//!
	void OnNewInput(void);

	//!
	void OnSliderNegativeContrast(wxScrollEvent &event);
	
	//!
	void OnSliderPositiveContrast(wxScrollEvent &event);

	//!
	void SetMultiRenderWindow( Core::Widgets::RenderWindowBase *multiRenderWindow );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	void UpdateNode( );

	//!
	bool Enable(bool enable /* = true */);

	//!
	void OnChkBoxEnable(wxCommandEvent &event);

private:
	//! Widget
	Core::Widgets::InputControl* m_AcquireInputWidget;

	//! Input data
	Core::DataEntityHolder::Pointer m_InputDataHolder;

	//! Window Level Widget
	mitk::wxMitkLevelWindowWidget* m_LevelWindowWidget;
	
	// The tag of the of the observer
	unsigned long m_Tag;

	// 
	itk::SimpleMemberCommand< ImageContrastWidget >::Pointer m_Command;
};

} // Core
} // Widgets)


#endif //coreImageContrastWidget_H

