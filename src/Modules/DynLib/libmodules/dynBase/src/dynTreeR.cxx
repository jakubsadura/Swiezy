/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

/*****************************************************************
AR-KJ
*******************************************************************/


#include "dynTreeR.h"

// Default constructor
TreeR::TreeR() : m_pTree( 0 )
{
	
}

bool TreeR::InsertNode(std::string csNode)
{
	StringArray csDataArray;
	Parse(csNode, csDataArray );
	unsigned nSize = 0;
	TNode::Pointer pLastNode = m_pTree;
	TNode::Pointer pCurrent = m_pTree;
	while( nSize < csDataArray.size())
	{
		if( m_pTree.IsNull() )
		{
			m_pTree = TNode::New();
			m_pTree->SetNodeData( csDataArray.at( nSize ) );
			pLastNode = m_pTree;
			pCurrent = m_pTree;
		}
		else
		{
			if( nSize > 0 )
			{
				int nIndex = 0;
				bool bData = false;
				while( pCurrent->GetNode( nIndex ).IsNotNull( ) )
				{
					std::string csCheck = csDataArray.at( nSize  );
					if( pCurrent->GetNode( nIndex )->GetNodeData() == csCheck )
					{
						bData = true;
						break;
					}
					nIndex++;
				}
				if( !bData )
				{
					TNode::Pointer pDataNode = TNode::New();
					pDataNode->SetNodeData( csDataArray.at( nSize ) );
					pCurrent->SetNode( nIndex, pDataNode );
					pLastNode = pDataNode;
					pCurrent = pLastNode;
				}
				else
				{
					pCurrent = pCurrent->GetNode( nIndex );
					pLastNode = pLastNode->GetNode( nIndex );
				}
			}
		}
		nSize++;
	}
	return true;
}

TNode::Pointer TreeR::GetNode()
{
	return m_pTree;
}


bool TreeR::DeleteNode(std::string csNode)
{
	return true;
}

bool TreeR::DeleteNodes()
{
	m_pTree =0;
	return 1;
}

bool TreeR::Parse(std::string csNode, StringArray& csDataArray )
{
	std::string csArray;
	int nPos =0;
	int nPrev = 0;
	while( -1 != ( nPos = csNode.find( "\\", nPos )))
	{
		csArray = csNode.substr( nPrev, ( nPos - nPrev ));
		csDataArray.push_back( csArray );
		nPos++;
		nPrev = nPos;
	}
	csArray = csNode.substr( nPrev, csNode.size());
	csDataArray.push_back( csArray );
	return true;
}

std::string TNode::GetNodeData() const
{
	return csNodeData;
}

void TNode::SetNodeData( std::string val )
{
	csNodeData = val;
}

TNode::Pointer TNode::GetNode( int i )
{
	return pTreeNode[ i ];
}

void TNode::SetNode( int i, Pointer node )
{
	pTreeNode[i] = node;
}


