/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkflowSubStep.h"

Core::WorkflowSubStep::WorkflowSubStep( const char* name )
{
	SetName( name );
}

std::string Core::WorkflowSubStep::GetName() const
{
	return m_Name;
}

void Core::WorkflowSubStep::SetName( std::string val )
{
	m_Name = val;
	AddAlternative( val );
}

Core::WorkflowSubStep::AlternativesListType& Core::WorkflowSubStep::GetAlternatives()
{
	return m_Alternatives;
}

void Core::WorkflowSubStep::AddAlternative( std::string name )
{
	AlternativesListType::iterator it;
	it = std::find( m_Alternatives.begin(), m_Alternatives.end(), name );
	if ( it != m_Alternatives.end() )
	{
		return;
	}

	m_Alternatives.push_back( name );
}
