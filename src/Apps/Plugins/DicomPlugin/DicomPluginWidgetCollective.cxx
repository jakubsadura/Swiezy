/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "DicomPluginWidgetCollective.h"

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTabFactory.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreWxMitkCoreMainWindow.h"
#include "coreKernel.h"
#include "coreCommandPanel.h"
#include "corePluginTabFactory.h"

#include "DicomWorkingAreaPanelWidget.h"
#include "DicomConnectToPacsDialogWidget.h"

DicomPlugin::WidgetCollective::WidgetCollective( ) 
{
	Core::Runtime::Kernel::GetGraphicalInterface()->CreatePluginTab( "Dicom" );

	
	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory( 
		WorkingAreaPanelWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).TabPage( "Dicom" ).WorkingArea().Show()
		.Id( wxID_WorkingAreaPanelWidget ).Caption( "DICOM working area" ) );

	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory( 
		ConnectToPacsDialogWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).TabPage( "Dicom" ).VerticalLayout( ).Float( )
		.Id( wxID_ConnectToPacsDialogWidget ).Caption( "PACS Connect/Query/Retrieve/Send" ) );

	// disable unwanted functionality
	if ( GetPluginTab( ) != NULL )
	{
		GetPluginTab( )->EnableWindow( wxID_CommandPanel, false);
		GetPluginTab( )->EnableAllToolbars( false );
		GetPluginTab( )->ShowWindow( wxID_IOToolbar );
	}
}
