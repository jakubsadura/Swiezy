// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreProcessorOutputsObserverBuilder.h"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

Core::Widgets::ProcessorOutputsObserverBuilder::ProcessorOutputsObserverBuilder( )
{
	m_list.clear();

}

Core::Widgets::ProcessorOutputsObserverBuilder::~ProcessorOutputsObserverBuilder()
{
}

void Core::Widgets::ProcessorOutputsObserverBuilder::Init( 
	Core::BaseFilter::Pointer processor,
	Core::RenderingTree::Pointer tree)
{
	m_list.clear();
	for ( size_t i = 0 ; i < processor->GetNumberOfOutputs() ; i++ )
	{
		Core::ProcessorOutputObserver::Pointer processorOutputObserver = 
				Core::ProcessorOutputObserver::New( );
		processorOutputObserver->SetOutputNumber( i );
		processorOutputObserver->SetProcessor( processor );
		processorOutputObserver->SetRenderingTree( tree );
		m_list.push_back(processorOutputObserver);
	}

}

Core::ProcessorOutputObserver::Pointer
Core::Widgets::ProcessorOutputsObserverBuilder::GetOutputObserver(int num)
{
	return m_list.at(num);
}

void Core::Widgets::ProcessorOutputsObserverBuilder::SetRenderingTree( 
	Core::RenderingTree::Pointer tree )
{
	std::vector<Core::ProcessorOutputObserver::Pointer>::iterator it;
	
	for ( it = m_list.begin() ; it != m_list.end() ; it++ )
	{
		(*it)->SetRenderingTree( tree );
	}
}

std::vector<Core::ProcessorOutputObserver::Pointer> 
Core::Widgets::ProcessorOutputsObserverBuilder::GetList() const
{
	return m_list;
}
