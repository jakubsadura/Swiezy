/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef XMLWorkflowWriter_H
#define XMLWorkflowWriter_H

#include "coreBaseWorkflowIO.h"

class TiXmlNode;

namespace Core{

/**
XML Writer for Workflow

Please see XMLWorkflowReader for the format of the XML file.

\author Xavi Planes
\date Nov 2010
\ingroup gmFrontEndPlugin
*/
class XMLWorkflowWriter : public BaseWorkflowIO 
{
public:
	coreDeclareSmartPointerClassMacro( XMLWorkflowWriter, BaseWorkflowIO );
	
public:

protected:
	XMLWorkflowWriter( );

	~XMLWorkflowWriter( );

	//!
	void InternalUpdate( );

	//!
	void SaveData( TiXmlNode* node, Workflow::Pointer workflow );
private:

};

} // Core

#endif //blSignalWriter_H
