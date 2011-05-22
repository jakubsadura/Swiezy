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

#include "medWidgetCollective.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTabFactory.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreSimpleProcessingWidget.h"

#include "ptExtractMainSurfacePanelWidget.h"
#include "ptTaubinSmoothSurfacePanelWidget.h"
#include "ptEdgeSwappingPanelWidget.h"
#include "ptLocalRefinerPanelWidget.h"
#include "ptLoopSubdivisionPanelWidget.h"
#include "ptThresholdPanelWidget.h"
#include "ptVolumeClosingPanelWidget.h"
#include "ptSkeletonizationPanelWidget.h"
#include "ptTetraGeneratorPanelWidget.h"
#include "ptNGOptimizationPanelWidget.h"
#include "ptRingCutPanelWidget.h"
#include "ptSkeletonCutPanelWidget.h"
#include "ptMeshStatisticsPanelWidget.h"
#include "ptExtractScalarWidget.h"

#include "ptRadiographVisualizationWidget.h"
#include "coreSurfaceSelectorWidget.h"
#include "SelectCellsSurfaceButton.xpm"

#include "ptMeshCreationWidget.h"

medWidgetCollective::medWidgetCollective( ) 
{
}

void medWidgetCollective::Init(  )
{
	Core::Runtime::Kernel::RuntimeGraphicalInterfacePointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	Core::WindowConfig windowConfig = Core::WindowConfig( ).ProcessingTool( );
	windowConfig.Category( "Basic Surface Editing" );
	gIface->RegisterFactory(
		ptExtractMainSurfacePanelWidget::Factory::NewBase(),
		windowConfig.Caption( "Extract Surface" ) );
	gIface->RegisterFactory(
		ptTaubinSmoothSurfacePanelWidget::Factory::NewBase(), 
		windowConfig.Caption( "Taubin Smooth Surface" ) );
	gIface->RegisterFactory(
		ptVolumeClosingPanelWidget::Factory::NewBase(), 
		windowConfig.Caption( "Volume Closing Surface" ) );
	gIface->RegisterFactory(
		ptLoopSubdivisionPanelWidget::Factory::NewBase(), 
		windowConfig.Caption( "Loop Subdivision Refiner Surface" ) );
	//gIface->RegisterFactory(
	//	ptLocalRefinerPanelWidget::Factory::NewBase(), 
	//	windowConfig.Caption( "Local Refiner Surface" ) );
	gIface->RegisterFactory(
		ptEdgeSwappingPanelWidget::Factory::NewBase(), 
		windowConfig.Caption( "Edge Swapped Surface" ) );
	gIface->RegisterFactory(
		ptThresholdPanelWidget::Factory::NewBase(), 
		windowConfig.Caption( "Threshold Surface" ) );
	gIface->RegisterFactory(
		ptMeshCreationWidget::Factory::NewBase(), 
		windowConfig.Caption( "Mesh Creation" ) );


	windowConfig.Category( "&Advanced Surface Editing" );
	gIface->RegisterFactory(
		ptSkeletonizationPanelWidget::Factory::NewBase(), 
		windowConfig.Caption( "Skeletonization" ) );
	gIface->RegisterFactory(
		ptTetraGeneratorPanelWidget::Factory::NewBase(), 
		windowConfig.Caption( "Tetra generation" ) );
	//gIface->RegisterFactory(
	//	ptNGOptimizationPanelWidget::Factory::NewBase(), 
	//	windowConfig.Caption( "Netgen Tools" ) );
	gIface->RegisterFactory(
		ptRingCutPanelWidget::Factory::NewBase(), 
		windowConfig.Caption( "Ring Cut" ) );
	//gIface->RegisterFactory(
	//	ptSkeletonCutPanelWidget::Factory::NewBase(), 
	//	windowConfig.Caption( "Skeleton Cut" ) );
	gIface->RegisterFactory(
		ptExtractScalarWidget::Factory::NewBase(), 
		windowConfig.Caption( "Extract Scalar" ) );

	windowConfig.Category("");
	gIface->RegisterFactory(
		ptMeshStatisticsPanelWidget::Factory::NewBase(), 
		windowConfig.Category("Statistics").Caption( "Statistics on Tetra mesh" ) );
	//gIface->RegisterFactory(
	//	ptRadiographVisualizationWidget::Factory::NewBase(), 
	//	windowConfig.Caption( "Radiograph Visualization" ) );

	gIface->RegisterFactory(
		Core::Widgets::SurfaceSelectorWidget::Factory::NewBase(), 
		Core::WindowConfig( ).SelectionTool()
		.Caption("Surface").Bitmap( selectcellssurfacebutton_xpm )
		.Id( wxID_SurfaceSelectorWidget ) );
}

