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

#include "svWidgetCollective.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTabFactory.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"

#include "ptExecuteCommandPanelWidget.h"

svWidgetCollective::svWidgetCollective( ) 
{
}

void svWidgetCollective::Init(  )
{
	Core::Runtime::Kernel::GetGraphicalInterface()->CreatePluginTab( "Scene View" );

	Core::Runtime::Kernel::RuntimeGraphicalInterfacePointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	Core::WindowConfig windowConfig = Core::WindowConfig( ).ProcessingTool( );

	windowConfig.Category("");
	gIface->RegisterFactory(
		ptExecuteCommandPanelWidget::Factory::NewBase(), 
		windowConfig.Caption( "Execute command" ) );
}
