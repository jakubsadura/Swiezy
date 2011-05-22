/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWorkflowStep_H
#define coreWorkflowStep_H

#include "gmFrontEndPluginWin32Header.h"
#include "coreObject.h"
#include "coreWorkflowSubStep.h"
#include "blTagMap.h"

namespace Core
{

/**
\brief Each step of a workflow

\ingroup gmFrontEndPlugin
\author Xavi Planes
\date 27 May 2010
*/
class GMFRONTENDPLUGIN_EXPORT WorkflowStep : public Core::SmartPointerObject
{
public:

	//!
	typedef std::list<WorkflowSubStep::Pointer> SubStepListType;
	//!
	typedef std::list<std::string> WindowListType;

	coreDeclareSmartPointerClassMacro1Param(
		WorkflowStep, 
		Core::SmartPointerObject,
		const char* );

	std::string GetName() const;
	void SetName(std::string val);

	Core::WorkflowStep::SubStepListType& GetSubStepVector();

	Core::WorkflowStep::WindowListType& GetWindowList();

	//!
	void AddSubStep( Core::WorkflowSubStep::Pointer subStep );

	//!
	void AddWindow( std::string factoryName );

	//! Find a sub step by name
	WorkflowSubStep::Pointer Get( int currentStep );

	//!
	blTagMap::Pointer GetProperties() const;

protected:
	//!
	WorkflowStep( const char* name );

	//! give access to private members for serialization
	template<class Archive> 
	friend void load(Archive &ar, Core::WorkflowStep::Pointer workflow, const unsigned int version); 

	template<class Archive> 
	friend void save(Archive &ar, Core::WorkflowStep::Pointer workflow, const unsigned int version); 

private:
	//!
	std::string m_Name;
	//!
	SubStepListType m_SubStepVector;
	//!
	WindowListType m_WindowList;
	//! Set of properties like Layout configuration
	blTagMap::Pointer m_Properties;
};

} // namespace Core

#endif // coreWorkflowStep_H

