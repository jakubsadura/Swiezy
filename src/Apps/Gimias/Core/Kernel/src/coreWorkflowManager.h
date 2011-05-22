/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWorkflowManager_H
#define coreWorkflowManager_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include "coreWorkflow.h"
#include "coreDataHolder.h"

namespace Core
{

/**
\brief Manages Workflow objects

\ingroup gmFrontEndPlugin
\author Xavi Planes
\date 1 June 2010
*/
class GMKERNEL_EXPORT WorkflowManager : public Core::SmartPointerObject
{
public:

	typedef Core::DataHolder<Workflow::Pointer> WorkflowHolderType;

	typedef std::list<Workflow::Pointer> WorkflowListType;

	coreDeclareSmartPointerClassMacro(
		WorkflowManager, 
		Core::SmartPointerObject );

	//! Call ScanDirectory
	void Initialize( );

	//! Add a workflow 
	void AddWorkflow( Workflow::Pointer workflow );

	//! Remove a workflow
	void RemoveWorkflow( const std::string &name );

	//!
	void RenameWorkflow( const std::string &name, const std::string &newName );

	//!
	Core::Workflow::Pointer GetWorkflow( const std::string &name );

	//!
	Workflow::Pointer GetActiveWorkflow() const;

	//!
	WorkflowHolderType::Pointer GetActiveWorkflowHolder() const;

	//! Add a workflow and set it as the active one
	void SetActiveWorkflow( const std::string &name );

	//!
	void SaveAll( );

	//!
	WorkflowListType GetWorkflowList() const;

	//!
	std::string GetWorkflowsFolder() const;

	//! Open a workflow file and return the workflow
	Core::Workflow::Pointer OpenWorkflow( const std::string &path );

	//! Scan projectPath + "Workflows" and open all workflows
	void ScanDirectory( const std::string &projectPath );

protected:
	//!
	WorkflowManager( );

	//!
	~WorkflowManager( );

	//!
	void SaveWorkflow( Core::Workflow::Pointer workflow );

	//!
	void CreateWorkflowsFolder();

	//!
	std::string GetWorkflowFileName( const std::string &name );

	//! Update a workflow from older version
	void UpdateOldWorkflow( Core::Workflow::Pointer workflow );

private:

	//!
	WorkflowHolderType::Pointer m_ActiveWorkflowHolder;

	//!
	WorkflowListType m_WorkflowList;

	//!
	std::string m_WorkflowsFolder;
};

} // namespace Core

#endif // coreWorkflowManager_H

