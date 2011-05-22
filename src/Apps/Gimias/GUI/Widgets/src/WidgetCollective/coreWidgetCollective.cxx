/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWidgetCollective.h"

#include "wxID.h"

#include "coreWxMitkCoreMainWindow.h"
#include "coreKernel.h"
#include "corePluginTabFactory.h"
#include "coreRenderingTree.h"
#include "coreProcessorWidgetsBuilder.h"
#include "coreWxMitkGraphicalInterface.h"

Core::WidgetCollective::WidgetCollective( )
{
}

Core::Widgets::RenderWindowBase *Core::WidgetCollective::GetActiveMultiRenderWindow()
{
	if ( GetPluginTab() == NULL )
	{
		return NULL;
	}

	Core::Widgets::RenderWindowBase *multiRenderWindow;
	multiRenderWindow = GetPluginTab( )->GetWorkingAreaManager()->GetActiveMultiRenderWindow( );
	return multiRenderWindow;
}

Core::Widgets::PluginTab* Core::WidgetCollective::GetPluginTab() const
{
	return Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow()->GetLastPluginTab( );
}

