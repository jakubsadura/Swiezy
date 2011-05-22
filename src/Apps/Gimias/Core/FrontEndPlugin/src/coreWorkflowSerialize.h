/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreWorkflowSerialize_H
#define _coreWorkflowSerialize_H

#include "gmFrontEndPluginWin32Header.h"
#include "coreObject.h"
#include "coreWorkflow.h"
#include "coreWorkflowStep.h"
#include "blTagMapSerializer.h"

#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/version.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_VERSION(Core::Workflow::Pointer, 2)
BOOST_CLASS_VERSION(Core::WorkflowStep::Pointer, 1)
BOOST_CLASS_VERSION(Core::WorkflowSubStep::Pointer, 1)

namespace Core {

template<class Archive> 
void save(Archive &ar, Core::Workflow::Pointer workflow, const unsigned int version)
{
	using boost::serialization::make_nvp;
	ar << make_nvp("Name", workflow->m_Name);
	ar << make_nvp("Version", workflow->m_Version);

	size_t size = workflow->m_StepVector.size();
	ar << make_nvp("StepVectorSize", size );
	Core::WorkflowStep::Pointer workflowStep;
	BOOST_FOREACH(workflowStep, workflow->m_StepVector) {
		ar << make_nvp("WorkflowStep", workflowStep);
	}

	ar << make_nvp( "PluginNamesList", workflow->m_PluginNamesList );
}


template<class Archive> 
void load(Archive &ar, Core::Workflow::Pointer workflow, const unsigned int version)
{
	using boost::serialization::make_nvp;
	ar >> make_nvp("Name", workflow->m_Name);
	if ( version > 1 )
	{
		ar >> make_nvp("Version", workflow->m_Version);
	}

	size_t size;
	ar >> make_nvp("StepVectorSize", size );
	for ( size_t i = 0 ; i < size ; i++ )
	{
		Core::WorkflowStep::Pointer workflowStep = Core::WorkflowStep::New( "" );
		ar >> make_nvp("WorkflowStep", workflowStep);
		workflow->m_StepVector.push_back( workflowStep );
	}

	ar >> make_nvp( "PluginNamesList", workflow->m_PluginNamesList );
}

//!
template<class Archive> 
void save(Archive &ar, Core::WorkflowStep::Pointer workflowStep, const unsigned int version)
{
	using boost::serialization::make_nvp;
	ar << make_nvp("Name", workflowStep->m_Name);
	ar << make_nvp("WindowList", workflowStep->m_WindowList);

	size_t size = workflowStep->m_SubStepVector.size();
	ar << make_nvp("SubStepVectorSize", size );
	Core::WorkflowSubStep::Pointer workflowSubStep;
	BOOST_FOREACH(workflowSubStep, workflowStep->m_SubStepVector) {
		ar << make_nvp("WorkflowSubStep", workflowSubStep);
	}

	ar << make_nvp("Properties", workflowStep->m_Properties );
}


//!
template<class Archive> 
void load(Archive &ar, Core::WorkflowStep::Pointer workflowStep, const unsigned int version)
{
	using boost::serialization::make_nvp;
	ar >> make_nvp("Name", workflowStep->m_Name);
	ar >> make_nvp("WindowList", workflowStep->m_WindowList);

	size_t size;
	ar >> make_nvp("SubStepVectorSize", size );
	for ( size_t i = 0 ; i < size ; i++ )
	{
		Core::WorkflowSubStep::Pointer workflowSubStep = Core::WorkflowSubStep::New( "" );
		ar >> make_nvp("WorkflowSubStep", workflowSubStep);
		workflowStep->m_SubStepVector.push_back( workflowSubStep );
	}

	if ( version > 0 )
		ar >> make_nvp("Properties", workflowStep->m_Properties );
}

//!
template<class Archive> 
void save(Archive &ar, Core::WorkflowSubStep::Pointer workflowSubStep, const unsigned int version)
{
	using boost::serialization::make_nvp;
	ar << make_nvp("Name", workflowSubStep->m_Name);
	ar << make_nvp("Alternatives", workflowSubStep->m_Alternatives);
}

//!
template<class Archive> 
void load(Archive &ar, Core::WorkflowSubStep::Pointer workflowSubStep, const unsigned int version)
{
	using boost::serialization::make_nvp;
	ar >> make_nvp("Name", workflowSubStep->m_Name);
	ar >> make_nvp("Alternatives", workflowSubStep->m_Alternatives);
}

} // namespace Core


BOOST_SERIALIZATION_SPLIT_FREE( Core::Workflow::Pointer )

BOOST_SERIALIZATION_SPLIT_FREE( Core::WorkflowStep::Pointer )

BOOST_SERIALIZATION_SPLIT_FREE( Core::WorkflowSubStep::Pointer )


#endif // _coreWorkflowSerialize_H

