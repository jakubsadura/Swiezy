/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SandboxPluginWidgetCollective.h"
#include "SandboxPluginShapeScalePanelWidget.h"
#include "SandboxPluginSubtractPanelWidget.h"
#include "SandboxPluginResamplePanelWidget.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTab.h"
#include "coreWxMitkGraphicalInterface.h"

SandboxPlugin::WidgetCollective::WidgetCollective( ) 
{
	Core::Runtime::Kernel::GetGraphicalInterface()->CreatePluginTab( "Sandbox" );

	// Panel widgets
	Core::WindowConfig config;
	config.CommandPanel().ProcessorObservers().TabPage( "Sandbox" );

	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
		ShapeScalePanelWidget::Factory::NewBase(), config.Caption( "Scale Shape" ) );

	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
		SubtractPanelWidget::Factory::NewBase(), config.Caption( "Substract Images" ) );

	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
		ResamplePanelWidget::Factory::NewBase(), config.Caption( "Resample an image" ) );

}

