/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wflWorkflowFactory_H
#define _wflWorkflowFactory_H

#include "WflLibWin32Header.h"
#include "blLightObject.h"
#include <map>
#include "wflWorkflow.h"

/**
Creates wflWorkflow instances reading XML files from disk

\author Xavi Planes
\date 4 Jan 2011
\ingroup WflLib
*/
class WFLLIB_EXPORT wflWorkflowFactory : public blLightObject
{
public:
	typedef wflWorkflowFactory Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	typedef std::map<std::string, wflWorkflow::Pointer> WorkflowMapType;

	//!
	void SetScanDirectory( const std::string &path );

	//!
	void Update( );

	//!
	WorkflowMapType &GetWorkflows();

protected:
	//!
	wflWorkflowFactory( );

	//!
	~wflWorkflowFactory( );

protected:

	//!
	std::string m_SearchPaths;

	//!
	WorkflowMapType m_Workflows;
};

#endif //_wflWorkflowFactory_H
