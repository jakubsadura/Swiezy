/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _coreRenderingTreeManager_H
#define _coreRenderingTreeManager_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreRenderingTree.h"

namespace Core
{

class BaseWindow;

/**
\brief Stores a list of RenderingTree and the selected one.

\ingroup gmKernel
\author Xavi Planes
\date 01 Mar 2010
*/
class GMKERNEL_EXPORT RenderingTreeManager : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		RenderingTreeManager, 
		Core::SmartPointerObject);


	//!
	Core::RenderingTree::Pointer GetActiveTree( ) const;

	//!
	Core::RenderingTreeHolderType::Pointer GetActiveTreeHolder( ) const;

	//!
	void SetActiveTree( RenderingTree::Pointer tree );

	//!
	void Add( RenderingTree::Pointer tree );

	//!
	void Remove( RenderingTree::Pointer tree );

	//!
	void AddActiveTreeObserver( BaseWindow *observer );

	//!
	void RemoveActiveTreeObserver( BaseWindow *observer );

	//!
	std::list<RenderingTree::Pointer> GetTreeList() const;
protected:

	//!
	RenderingTreeManager(void);
	//!
	virtual ~RenderingTreeManager(void);
	//!
	coreDeclareNoCopyConstructors(RenderingTreeManager);

private:

	//!
	std::list<RenderingTree::Pointer> m_TreeList;
	//!
	RenderingTreeHolderType::Pointer m_ActiveTreeHolder;
};

} // namespace Core

#endif // _coreRenderingTreeManager_H

