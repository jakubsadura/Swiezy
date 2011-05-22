/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreMultiRenderWindowMITKConfig_H
#define _coreMultiRenderWindowMITKConfig_H

#include "wxMitkMultiRenderWindowConfig.h"
#include "coreRenderWindowConfig.h"

namespace Core
{
namespace Widgets
{

/** 
\brief MITK specific implementation of wxMitkMultiRenderWindowConfig.
\ingroup gmWidgets
\author Xavi Planes
\date Nov 2010
*/
class MultiRenderWindowMITKConfig : 
	public mitk::wxMitkMultiRenderWindowConfig, 
	public RenderWindowConfigBase
{
public:
	//!
	coreDefineBaseWindowFactory( MultiRenderWindowMITKConfig );

	//!
	MultiRenderWindowMITKConfig(wxWindow* parent, 
		int id, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=0) : mitk::wxMitkMultiRenderWindowConfig( parent, id, pos, size, style ) {}
	//!
	virtual bool SetView( Core::Widgets::RenderWindowBase* multiRenderWindow )
	{
		MultiRenderWindowMITK* mitkWindow;
		mitkWindow = dynamic_cast<MultiRenderWindowMITK*> ( multiRenderWindow );
		// If it's NULL, set it to NULL
		mitk::wxMitkMultiRenderWindowConfig::SetView( mitkWindow );
		return true;
	}

};

}
}

#endif // _coreMultiRenderWindowMITKConfig_H
