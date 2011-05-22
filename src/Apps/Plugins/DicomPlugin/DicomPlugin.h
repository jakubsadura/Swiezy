/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomPlugin_H
#define DicomPlugin_H

#include "coreFrontEndPlugin.h"

#include "DicomPluginWidgetCollective.h"
#include "DicomProcessorCollective.h"

namespace DicomPlugin{

class WorkingAreaPanelWidget;

/** 
A class that places all the graphical interface items for the Dicom Plugin

\ingroup DicomPlugin
\author Jakub Lyko
\date 07 May 2008
*/

class PLUGIN_EXPORT DicomPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
public:
	coreDeclareSmartPointerClassMacro(DicomPlugin, Core::FrontEndPlugin::FrontEndPlugin);

protected:
	//!
	DicomPlugin(void);

	//!
	virtual ~DicomPlugin(void);

private:

	WidgetCollective::Pointer m_WidgetCollective;

	//!
	ProcessorCollective::Pointer m_ProcessorCollective;
};

} // DicomPlugin

#endif // DicomPlugin_H
