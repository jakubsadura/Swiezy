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

#include "DicomPlugin.h"
#include "coreException.h"
#include "wxID.h"
#include "corePluginTab.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "corePluginTabFactory.h"

// Declaration of the plugin
coreBeginDefinePluginMacro(DicomPlugin::DicomPlugin)
	coreDefinePluginAddProfileMacro("Dicom")
coreEndDefinePluginMacro()

// Constructor for class DicomPlugin
DicomPlugin::DicomPlugin::DicomPlugin() 
{
	try
	{
		m_ProcessorCollective = ProcessorCollective::New();
		m_WidgetCollective = WidgetCollective::New( );

	}
	coreCatchExceptionsReportAndNoThrowMacro(DicomPlugin::DicomPlugin)
}

// Destructor for class DicomPlugin
DicomPlugin::DicomPlugin::~DicomPlugin(void)
{
}
