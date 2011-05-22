/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMultiRenderWindowFocus_H
#define coreMultiRenderWindowFocus_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataEntityHolder.h"

namespace Core
{
namespace Widgets
{
	class MultiRenderWindowMITK;
/** 
\brief Center the MultiRenderWindow 2D views to one points
depending on the slected data from the data list

\ingroup gmWidgets
\author Xavi Planes
\date 16 July 2009
*/
class PLUGIN_EXPORT MultiRenderWindowFocus : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(MultiRenderWindowFocus, Core::SmartPointerObject);

	//!
	void Init( 
		Core::Widgets::MultiRenderWindowMITK* multiRenderWindow,
		Core::DataEntityHolder::Pointer selectedDataEntity );

protected:

	MultiRenderWindowFocus( );

	virtual ~MultiRenderWindowFocus( );

	//!
	void OnSelectedModified( );

protected:

	//! The window to center
	Core::Widgets::MultiRenderWindowMITK* m_MultiRenderWindow;

	//!
	Core::DataEntityHolder::Pointer m_SelectedDataEntity;

};

}
}

#endif // coreMultiRenderWindowFocus_H

