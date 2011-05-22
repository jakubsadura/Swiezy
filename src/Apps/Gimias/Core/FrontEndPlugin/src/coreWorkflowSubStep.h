/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWorkflowSubStep_H
#define coreWorkflowSubStep_H

#include "gmFrontEndPluginWin32Header.h"
#include "coreObject.h"

namespace Core
{

/**
\brief Each step of a step of a workflow

\ingroup gmFrontEndPlugin
\author Xavi Planes
\date 27 May 2010
*/
class GMFRONTENDPLUGIN_EXPORT WorkflowSubStep : public Core::SmartPointerObject
{
public:

	coreDeclareSmartPointerClassMacro1Param(
		WorkflowSubStep, 
		Core::SmartPointerObject,
		const char* );

	//!
	typedef std::list<std::string> AlternativesListType;

	std::string GetName() const;
	void SetName(std::string val);

	Core::WorkflowSubStep::AlternativesListType& GetAlternatives();

	//!
	void AddAlternative( std::string name );

protected:
	//!
	WorkflowSubStep( const char* name );

	//! give access to private members for serialization
	template<class Archive> 
	friend void load(Archive &ar, Core::WorkflowSubStep::Pointer workflow, const unsigned int version); 

	template<class Archive> 
	friend void save(Archive &ar, Core::WorkflowSubStep::Pointer workflow, const unsigned int version); 

private:
	//!
	std::string m_Name;
	//!
	AlternativesListType m_Alternatives;
};

} // namespace Core

#endif // coreWorkflowSubStep_H

