/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _BaseWorkflowIO_H
#define _BaseWorkflowIO_H

#include "gmFrontEndPluginWin32Header.h"
#include "coreObject.h"
#include "coreWorkflow.h"

namespace Core
{

/**
Base reader/writer for all Workflow reader/writers. 

\author Xavi Planes
\date Nov 2010
\ingroup gmFrontEndPlugin
*/
class GMFRONTENDPLUGIN_EXPORT BaseWorkflowIO : public SmartPointerObject
{
public:
	coreDeclareSmartPointerTypesMacro(BaseWorkflowIO,SmartPointerObject)
	coreClassNameMacro(BaseWorkflowIO)
	
public:

	//! Set the complete filename 
	void SetFilename( const char* );

	//!
	void SetInput( Workflow::Pointer workflow );

	//! execute
	virtual void Update( );

	//! Returns the output of the reader
	Workflow::Pointer GetOutput( );


protected:
	BaseWorkflowIO( );

	~BaseWorkflowIO( );

	//!
	virtual void InternalUpdate( ) = 0;

protected:

	//!
	Workflow::Pointer m_Data;

	//!
	std::string m_Filename;

	//!
	unsigned int m_Version;
};

} // Core

#endif //_BaseWorkflowIO_H
