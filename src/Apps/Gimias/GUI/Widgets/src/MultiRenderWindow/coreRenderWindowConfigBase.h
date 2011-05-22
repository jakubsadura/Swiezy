/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreRenderWindowConfigBase_H
#define _coreRenderWindowConfigBase_H

#include "coreBaseWindow.h"

namespace Core
{
namespace Widgets
{

class RenderWindowBase;

/** 
\brief Base class for all render window configuration widgets

\ingroup gmWidgets
\author Xavi Planes
\date Nov 2010
*/
class RenderWindowConfigBase : public BaseWindow
{
public:
	//!
	virtual bool SetView( Core::Widgets::RenderWindowBase* window ) = 0;
};

}
}

#endif // _coreRenderWindowConfigBase_H
