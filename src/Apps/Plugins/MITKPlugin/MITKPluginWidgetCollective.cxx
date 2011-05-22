/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "MITKPluginWidgetCollective.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTab.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreSettings.h"
#include "coreWindowConfig.h"

#include "wxMitkApp.h"

#include "mitkGlobalInteraction.h"

#include "coreToolbarAppearance.h"
#include "coreToolbarMultiRenderWindow.h"
#include "coreToolbarLayout.h"

#include "coreLandmarkSelectorWidget.h"
#include "coreBBoxWidget.h"
#include "coreMeasurementWidget.h"

#include "coreMultiRenderWindowMITK.h"
#include "coreMultiRenderWindowMITKConfig.h"

#include "coreVolumeMeshMaterialPropertiesWidget.h"
#include "coreVolumeImagePropertiesWidget.h"
#include "coreSurfaceMeshMaterialPropertiesWidget.h"
#include "coreITKTransformVisualPropertiesWidget.h"

#include "coreDataInformation.h"

#include "PointInteractorOn.xpm"
#include "MeasurementInteractorOn.xpm"
#include "BoundingBox.xpm"

const char* GIMIAS_STATE_MACHINES_FILENAME = "/GimiasStateMachines.xml";


MITKPlugin::WidgetCollective::WidgetCollective( ) 
{
	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();
	mitk::wxMitkApp::Initialize( settings->GetApplicationPathSubPath("StateMachine.xml") );

	LoadStateMachines( );

	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	// Toolbars
	gIface->RegisterFactory(
		Core::Widgets::ToolbarMultiRenderWindow::Factory::NewBase( ),
		Core::WindowConfig( ).Toolbar( ).Top( ).Show().Caption( "MultiRenderWindow Toolbar" ) );

	gIface->RegisterFactory(
		Core::Widgets::ToolbarAppearance::Factory::NewBase( ),
		Core::WindowConfig( ).Toolbar( ).Top( ).Caption( "Appearance Toolbar" ) );

	gIface->RegisterFactory(
		Core::Widgets::ToolbarLayout::Factory::NewBase( ),
		Core::WindowConfig( ).Toolbar( ).Top( ).Caption( "Layout Toolbar" ) );

	// Selection tools
	gIface->RegisterFactory( 
		Core::Widgets::LandmarkSelectorWidget::Factory::NewBase( ),
		Core::WindowConfig( ).SelectionTool()
		.Caption(wxT("Landmark selector")).Bitmap( pointinteractoron_xpm )
		.Id( wxID_LandmarkSelectorWidget ) );

	gIface->RegisterFactory( 
		Core::Widgets::MeasurementWidget::Factory::NewBase( ),
		Core::WindowConfig( ).SelectionTool()
		.Caption("Measurement").Bitmap( measurementinteractoron_xpm )
		.Id( wxID_MeasurementWidget ) );

	gIface->RegisterFactory( 
		Core::Widgets::BBoxWidget::Factory::NewBase( ),
		Core::WindowConfig( ).SelectionTool()
		.Caption(wxT("Bounding Box")).Bitmap( boundingbox_xpm )
		.Id( wxID_BBoxWidget ) );

	// MultiRenderWindow
	gIface->RegisterFactory(
		Core::Widgets::MultiRenderWindowMITK::Factory::NewBase( ), 
		Core::WindowConfig( ).RenderWindow( ).Caption( wxT( "Multi Render Window MITK" ) ) );

	gIface->RegisterFactory(
		Core::Widgets::MultiRenderWindowMITKConfig::Factory::NewBase( ), 
		Core::WindowConfig( ).RenderWindowConfig( ).Caption( wxT( "Multi Render Window MITK Config" ) ) );

	// Information
	gIface->RegisterFactory(
		Core::Widgets::VolumeImageInformationWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).DataInformation( ).Caption( wxT( "Volume Image" ) ) );

	gIface->RegisterFactory(
		Core::Widgets::MeasurementInformationWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).DataInformation( ).Caption( wxT( "Measurement" ) ) );

	gIface->RegisterFactory(
		Core::Widgets::PolyDataInformationWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).DataInformation( ).Caption( wxT( "Surface mesh" ) ) );

	gIface->RegisterFactory(
		Core::Widgets::UnstructuredGridInformationWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).DataInformation( ).Caption( wxT( "Volume mesh" ) ) );

	gIface->RegisterFactory(
		Core::Widgets::SignalInformationWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).DataInformation( ).Caption( wxT( "Signal" ) ) );

	// Visual Props
	gIface->RegisterFactory(
		Core::Widgets::VolumeMeshMaterialPropertiesWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).VisualProps( ).Caption( wxT( "VolumeMeshVisualProps" ) ) );

	gIface->RegisterFactory(
		Core::Widgets::VolumeImagePropertiesWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).VisualProps( ).Caption( wxT( "ImageVisualProps" ) ) );

	gIface->RegisterFactory(
		Core::Widgets::SurfaceMeshMaterialPropertiesWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).VisualProps( ).Caption( wxT( "SurfaceMeshVisualProps" ) ) );

	gIface->RegisterFactory(
		Core::Widgets::ITKTransformVisualPropertiesWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).VisualProps( ).Caption( wxT( "ITKTransformVisualProps" ) ) );

}

void MITKPlugin::WidgetCollective::LoadStateMachines()
{
	std::string strPath;
	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();
	strPath = settings->GetResourcePath() + GIMIAS_STATE_MACHINES_FILENAME;

	// load interaction patterns from XML-file
	bool bResult = mitk::GlobalInteraction::GetInstance()->GetStateMachineFactory()->LoadBehavior( strPath );
}

MITKPlugin::WidgetCollective::~WidgetCollective()
{
	mitk::wxMitkApp::CleanUpMITK( );
}

