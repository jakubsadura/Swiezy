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

#include "msWidgetCollective.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTabFactory.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "coreSimpleProcessingWidget.h"

#include "msImageCreationProcessor.h"
#include "ManualSegmentationPanelWidget.h"
#include "MultiLevelROIPanelWidget.h"
#include "ContourToolOn.xpm"

msWidgetCollective::msWidgetCollective( ) 
{
}

void msWidgetCollective::Init(  )
{
	Core::Runtime::Kernel::RuntimeGraphicalInterfacePointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	gIface->RegisterFactory(
		Core::Widgets::ManualSegmentationPanelWidget::Factory::NewBase(),
		Core::WindowConfig( ).SelectionTool()
		.Caption("Manual Segmentation").Bitmap( contourtoolon_xpm )
		.Id( wxID_ManualSegmentationWidget ) );

	typedef Core::Widgets::SimpleProcessingWidget<Core::msImageCreationProcessor> 
		ImageCreationWidget;
	gIface->RegisterFactory( 
		ImageCreationWidget::Factory::NewBase(),
		Core::WindowConfig( ).ProcessingTool( ).Category("Mesh processing").Caption( "Image Creation" ) );

	gIface->RegisterFactory(
		Core::Widgets::MultiLevelROIPanelWidget::Factory::NewBase(),
		Core::WindowConfig( ).ProcessingTool( ).Category("Statistics")
		.Caption("ROI Statistics").Bitmap( contourtoolon_xpm )
		.Id( wxID_MultiLevelROIWidget ) );
}

