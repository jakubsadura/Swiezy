/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWorkflow_H
#define coreWorkflow_H

#include "gmFrontEndPluginWin32Header.h"
#include "coreObject.h"
#include "coreWorkflowStep.h"

namespace Core
{

/**
\brief Clinical Workflow: Set of steps used to go from input acquired
data (images) from a specific patient to information that is useful
for patient treatment.

\ingroup gmFrontEndPlugin
\author Xavi Planes
\date 27 May 2010
*/
class GMFRONTENDPLUGIN_EXPORT Workflow : public Core::SmartPointerObject
{
public:

	typedef std::list<WorkflowStep::Pointer> StepListType;
	typedef std::list<std::string> PluginNamesListType;

	coreDeclareSmartPointerClassMacro1Param(
		Workflow, 
		Core::SmartPointerObject,
		const char* );

	//!
	std::string GetName() const;
	void SetName(std::string val);

	//!
	std::string GetVersion() const;
	void SetVersion(std::string val);

	//!
	Core::Workflow::StepListType& GetStepVector();

	//!
	void AddStep( WorkflowStep::Pointer );

	//!
	Core::WorkflowStep::Pointer GetStep( const std::string &name );

	//!
	Core::Workflow::PluginNamesListType GetPluginNamesList() const;
	void SetPluginNamesList(Core::Workflow::PluginNamesListType val);

	//!
	void Clear( );

	//! GetName and version
	std::string GetFullName( );

protected:
	//!
	Workflow( const char* name );

	//! give access to private members for serialization
	template<class Archive> 
	friend void load(Archive &ar, Core::Workflow::Pointer workflow, const unsigned int version); 

	template<class Archive> 
	friend void save(Archive &ar, Core::Workflow::Pointer workflow, const unsigned int version); 

private:

	//!
	std::string m_Name;
	//!
	std::string m_Version;
	//!
	StepListType m_StepVector;
	//! List of plugin names needed to use this workflow
	PluginNamesListType m_PluginNamesList;
};

} // namespace Core

#endif // coreWorkflow_H

