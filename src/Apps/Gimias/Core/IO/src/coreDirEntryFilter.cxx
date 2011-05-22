/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDirEntryFilter.h"

using namespace Core::IO;

//!
DirEntryFilter::DirEntryFilter(void) : m_Mode(AllEntries)
{
}

//!
DirEntryFilter::~DirEntryFilter(void)
{
}

//!
void DirEntryFilter::AddGlobbingExpression(const std::string& expr)
{
	this->m_GlobbingExpressions.push_back(expr);
}


//!
void DirEntryFilter::ResetGlobbingExpressions(void)
{
	this->m_GlobbingExpressions.clear();
}

//!
bool DirEntryFilter::HasGlobbingExpression(void)
{
	return !this->m_GlobbingExpressions.empty();
}

/**
 */
Core::IO::GlobExprList Core::IO::DirEntryFilter::GetGlobbingExpressions() const
{
	return m_GlobbingExpressions;
}

Core::IO::DirEntryFilter::FilterMode Core::IO::DirEntryFilter::GetMode() const
{
	return m_Mode;
}

void Core::IO::DirEntryFilter::SetMode( FilterMode val )
{
	m_Mode = val;
}
