/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "ntWidgetCollective.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTabFactory.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreSimpleProcessingWidget.h"
#include "coreKernel.h"

#include "coreImageContrastWidget.h"


ntWidgetCollective::ntWidgetCollective( ) 
{
}

void ntWidgetCollective::Init(  )
{
	Core::Runtime::Kernel::RuntimeGraphicalInterfacePointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	gIface->RegisterFactory(
		Core::Widgets::ImageContrastWidget::Factory::NewBase(),
		Core::WindowConfig( ).VerticalLayout()
		.Caption( "Image Contrast Widget" ).Category( "Visualization" ) );

}

