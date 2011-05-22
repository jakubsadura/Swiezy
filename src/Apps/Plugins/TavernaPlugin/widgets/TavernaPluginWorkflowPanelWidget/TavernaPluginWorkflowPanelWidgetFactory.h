/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginWorkflowPanelWidgetFactory_H
#define _TavernaPluginWorkflowPanelWidgetFactory_H

// CoreLib
#include "wflWorkflow.h"
#include "coreBaseWindowFactory.h"
#include "TavernaPluginWorkflowPanelWidget.h"

namespace TavernaPlugin{

/** 
Factory that creates WorkflowPanelWidget from wflWorkflow

\ingroup TavernaPlugin
\author Xavi Planes
\date 23 Dec 2010
*/
class WorkflowPanelWidgetFactory : public Core::BaseWindowFactory
{
public: 
		coreDeclareSmartPointerTypesMacro(WorkflowPanelWidgetFactory,Core::BaseWindowFactory)
		coreFactorylessNewMacro(WorkflowPanelWidgetFactory)
		coreClassNameMacro(WorkflowPanelWidgetFactory)

		static BaseWindowFactory::Pointer NewBase( const wflWorkflow::Pointer workflow ); 
		virtual Core::BaseWindow* CreateWindow( ); 
		std::string GetName() const; 
		wflWorkflow::Pointer GetWorkflow() const;
		void SetWorkflow(wflWorkflow::Pointer val);

		/** Scan Workflows folder and register a WorkflowPanelWidget
		for each workflow or update the workflow contents
		*/
		static void Scan( );

private:
	//!
	wflWorkflow::Pointer m_Workflow;
};

} //namespace TavernaPlugin{

#endif //_TavernaPluginWorkflowPanelWidgetFactory_H
