/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _DicomPluginWidgetCollective_H
#define _DicomPluginWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "DicomProcessorCollective.h"

#define wxID_WorkingAreaPanelWidget wxID("WorkingAreaPanelWidget")
#define wxID_ConnectToPacsDialogWidget wxID("ConnectToPacsDialogWidget")

namespace DicomPlugin{

class WorkingAreaPanelWidget;
class ConnectToPacsDialogWidget;


/**

\ingroup DicomPlugin
\author Xavi Planes
\date 23 dec 2009
*/

class WidgetCollective : public Core::WidgetCollective
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		WidgetCollective, 
		Core::WidgetCollective );


private:
	WidgetCollective( );



private:


};

} // namespace DicomPlugin

#endif //_DicomPluginWidgetCollective_H
