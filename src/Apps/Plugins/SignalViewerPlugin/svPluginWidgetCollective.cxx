/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svPluginWidgetCollective.h"
#include "svPlotManagerWidget.h"
#include "ptSignalTimePropagationPanelWidget.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTabFactory.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreDoubleBananaProcessor.h"
#include "coreSimpleProcessingWidget.h"
#include "SignalReaderWidget.h"

svWidgetCollective::svWidgetCollective( ) 
{
}

void svWidgetCollective::Init(  )
{
	try
	{
		Core::Runtime::Kernel::RuntimeGraphicalInterfacePointer gIface;
		gIface = Core::Runtime::Kernel::GetGraphicalInterface();

		gIface->RegisterFactory(
			svPlotManagerWidget::Factory::NewBase(),
			Core::WindowConfig( ).Layer(1).Bottom().Category( "Visualization" )
			.Caption( "Signal viewer" ).Id( wxID_PlotManagerWidget ) );

		Core::WindowConfig windowConfig = Core::WindowConfig( ).ProcessingTool( )
			.Category( "Signals" );
		gIface->RegisterFactory(
			ptSignalTimePropagationPanelWidget::Factory::NewBase(),
			windowConfig.Caption( "Propagate Signal time" ) );

		typedef Core::Widgets::SimpleProcessingWidget<Core::DoubleBananaProcessor> DoubleBananaWidget;
		gIface->RegisterFactory( 
			DoubleBananaWidget::Factory::NewBase(),
			windowConfig.Caption( "Double Banana" ) );

		gIface->RegisterFactory( 
			SignalReaderWidget::Factory::NewBase(),
			Core::WindowConfig( ).Caption( "Signal header reader" ).ReaderHeaderWindow() );
	}
	coreCatchExceptionsReportAndNoThrowMacro( svWidgetCollective::Init )



}

