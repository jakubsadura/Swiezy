/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

/*****************************************************************
AR-KJ

  A tree class that handles all the Data as a tree
  which can be used as data structure class.
*******************************************************************/

#ifndef _TREE_R_H_
#define _TREE_R_H_


#include <vector>
#include "blLightObject.h"
#include "blSmartPointer.h"

#define MAX_PATH          260

typedef std::vector<std::string> StringArray;

class TNode : public blLightObject
{  
public:
	typedef TNode Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);

	std::string GetNodeData() const;
	void SetNodeData(std::string val);

	TNode::Pointer GetNode( int i );
	void SetNode( int i, TNode::Pointer node );

private:
	std::string csNodeData;
	std::vector<TNode::Pointer> pTreeNode;
	TNode()
	{
		pTreeNode.resize( MAX_PATH );
		for( int nSize=0; nSize <MAX_PATH; nSize++ )
		{
			pTreeNode[nSize] = 0;
		}
	}
};

/**
\brief Tree of strings for blPEFormatParser.

- Source: http://www.codeproject.com/KB/tips/PE_File.aspx?display=Print
- Modified to use it using STL

\author Xavi Planes
\date 24 March 2010
\ingroup DynLib
*/
class TreeR
{
public:
	TreeR();
	bool InsertNode(std::string csNode);
	bool DeleteNodes();
	bool DeleteNode(std::string csNode);
	TNode::Pointer GetNode();

private:

	bool Parse(std::string csNode, StringArray& csDataArray );

private:
	
	TNode::Pointer m_pTree;

};

#endif // end of _TREE_R_H_