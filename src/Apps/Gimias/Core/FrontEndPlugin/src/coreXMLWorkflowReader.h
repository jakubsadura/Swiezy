/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _XMLWorkflowReader_H
#define _XMLWorkflowReader_H

#include "coreBaseWorkflowIO.h"

class TiXmlNode;

namespace Core{

/**
XML Reader for Workflow.

\sec Example

\code
<?xml version="0" ?>
<Workflow name="AngioImages" version="0.0.0">
    <RequiredPlugins>
        <Item name="ImageToolsPlugin" />
        <Item name="MeshEditorPlugin" />
        <Item name="Plugin0" />
    </RequiredPlugins>
    <Step name="Visualize &amp; Crop">
        <Windows>
            <Item name="class Core::Widgets::WorkingAreaManager" />
            <Item name="class Core::Widgets::DataEntityListBrowser" />
            <Item name="class Core::Widgets::CommandPanel" />
            <Item name="class Core::Widgets::WorkflowNavigationWidget" />
...
\endcode

\author Xavi Planes
\date Nov 2010
\ingroup gmFrontEndPlugin
*/
class XMLWorkflowReader : public BaseWorkflowIO 
{
public:
	coreDeclareSmartPointerClassMacro( XMLWorkflowReader, BaseWorkflowIO );

protected:
	XMLWorkflowReader( );

	~XMLWorkflowReader( );

	//!
	void InternalUpdate( );

	//!
	void LoadData( TiXmlNode* node, Workflow::Pointer workflow );

private:
};

} // Core

#endif //_XMLWorkflowReader_H
