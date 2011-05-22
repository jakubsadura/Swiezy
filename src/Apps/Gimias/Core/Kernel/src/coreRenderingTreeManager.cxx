/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreRenderingTreeManager.h"
#include "coreBaseWindow.h"

using namespace Core;

//!
RenderingTreeManager::RenderingTreeManager(void)
{
	m_ActiveTreeHolder = RenderingTreeHolderType::New( );
}

//!
RenderingTreeManager::~RenderingTreeManager(void)
{
	while ( m_TreeList.size() )
	{
		m_TreeList.erase( m_TreeList.begin() ); 
	}
}


void RenderingTreeManager::Add( RenderingTree::Pointer tree )
{
	m_TreeList.push_back( tree );
}

void RenderingTreeManager::Remove( RenderingTree::Pointer tree )
{
	if ( tree == GetActiveTree( ) )
	{
		SetActiveTree( NULL );
	}

	std::list<RenderingTree::Pointer>::iterator it;
	it = std::find( m_TreeList.begin(), m_TreeList.end(), tree );
	if ( it != m_TreeList.end() )
	{
		m_TreeList.erase( it );
	}
}

Core::RenderingTree::Pointer 
RenderingTreeManager::GetActiveTree() const
{
	return m_ActiveTreeHolder->GetSubject();
}

void RenderingTreeManager::SetActiveTree( 
	RenderingTree::Pointer tree )
{
	m_ActiveTreeHolder->SetSubject( tree );
}

Core::RenderingTreeHolderType::Pointer 
RenderingTreeManager::GetActiveTreeHolder() const
{
	return m_ActiveTreeHolder;
}

void Core::RenderingTreeManager::AddActiveTreeObserver( 
	Core::BaseWindow *observer )
{
	GetActiveTreeHolder( )->AddObserver1( observer, &BaseWindow::SetRenderingTree, DH_NEW_SUBJECT );
}

void Core::RenderingTreeManager::RemoveActiveTreeObserver( 
	Core::BaseWindow *observer )
{
	GetActiveTreeHolder( )->RemoveObserver1( observer, &BaseWindow::SetRenderingTree, DH_NEW_SUBJECT );
}

std::list<RenderingTree::Pointer> Core::RenderingTreeManager::GetTreeList() const
{
	return m_TreeList;
}
