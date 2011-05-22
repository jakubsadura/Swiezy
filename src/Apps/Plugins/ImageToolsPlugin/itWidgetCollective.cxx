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

#include "itWidgetCollective.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTabFactory.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreSimpleProcessingWidget.h"
#include "coreKernel.h"

#include "coreOverlayProcessor.h"
#include "ptChangeOrientationWidget.h"
#include "ptCroppingWidget.h"
#include "coreImageInfoWidget.h"

itWidgetCollective::itWidgetCollective( ) 
{
}

void itWidgetCollective::Init(  )
{
	Core::Runtime::Kernel::RuntimeGraphicalInterfacePointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();


	Core::WindowConfig windowConfig = Core::WindowConfig( ).ProcessingTool( );
	windowConfig.Category("Image processing");
	gIface->RegisterFactory( 
		ptCroppingWidget::Factory::NewBase(),
		windowConfig.Caption( "Cropping tool" ) );
	gIface->RegisterFactory( 
		ptChangeOrientationWidget::Factory::NewBase(),
		windowConfig.Caption( "Change Orientation" ) );
	typedef Core::Widgets::SimpleProcessingWidget<Core::OverlayProcessor> 
		OverlayWidget;
	gIface->RegisterFactory( 
		OverlayWidget::Factory::NewBase(),
		windowConfig.Caption( "Overlay Images" ) );

	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
		Core::Widgets::ImageInfoWidget::Factory::NewBase( ),
		Core::WindowConfig( ).VerticalLayout( ).Show()
		.Id( wxID_ImageInformationWidget ).Category("Windows").Caption( "Image information" ) );
}

