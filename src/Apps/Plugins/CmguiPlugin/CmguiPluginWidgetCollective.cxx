/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPlugin.h"
#include "CmguiPluginMacros.h"
#include "CmguiPluginWidgetCollective.h"

// widgets
#include "CmguiPluginRenderWidget.h"
#include "CmguiPluginCommandlineWidget.h"
#include "CmguiPluginToolbar.h"
#include "CmguiPluginImageVisualProps.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTab.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"

using namespace CmguiPlugin;

WidgetCollective::WidgetCollective(Cmgui *cmgui)
{
    m_cmgui = cmgui;

	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

    Core::Runtime::Kernel::GetGraphicalInterface()->CreatePluginTab("Cmgui");
    
	RenderWidget::Factory::Pointer fRenderWidget = RenderWidget::Factory::New(cmgui);
	gIface->RegisterFactory( 
		fRenderWidget.GetPointer(), 
		Core::WindowConfig().RenderWindow().TabPage( "Cmgui").Show( )
		.Caption("Cmgui Rendering") );

    // Panel widgets
    Core::WindowConfig config;
    config.ProcessorObservers().TabPage("Cmgui").CommandPanel();
    
	CommandlineWidget::Factory::Pointer fCommandLine = CommandlineWidget::Factory::New(cmgui);
    gIface->RegisterFactory(
        fCommandLine.GetPointer(), config.Caption("Cmgui Commandline").Id( wxID_CmguiCommandline ) );

	gIface->RegisterFactory(
		Toolbar::Factory::NewBase( ),
		Core::WindowConfig( ).Toolbar( ).Top( ).TabPage("Cmgui").Show()
		.Caption( "Cmgui Toolbar" ).Id( wxID_CmguiToolbar ) );

	ImageVisualProps::Factory::Pointer f = ImageVisualProps::Factory::New( m_cmgui );
	gIface->RegisterFactory(
		f.GetPointer(),
		Core::WindowConfig( ).VisualProps( ).Caption( "Cmgui image Visual Props" ) );

	Core::Widgets::BaseMainWindow* mainWindow;
	mainWindow = Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( );
	if ( mainWindow && mainWindow->GetLastPluginTab( ) )
	{
		mainWindow->GetLastPluginTab( )->ShowWindow( "CMGUIWorkingArea" );
	}

}


