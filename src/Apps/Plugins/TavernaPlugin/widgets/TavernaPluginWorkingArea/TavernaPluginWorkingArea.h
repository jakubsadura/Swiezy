/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginWorkingArea_H
#define _TavernaPluginWorkingArea_H

#include "TavernaPluginWorkingAreaUI.h"

class wxEmbeddedAppWindow;

namespace TavernaPlugin{

/** 

\ingroup TavernaPlugin
\author Xavi Planes
\date 23 Dec 2010
*/
class WorkingArea : 
	public TavernaPluginWorkingAreaUI,
	public Core::BaseWindow 
{

// OPERATIONS
public:
	coreDefineBaseWindowFactory( TavernaPlugin::WorkingArea )

	//!
	WorkingArea(
		wxWindow* parent, int id= wxID_ANY,
		const wxPoint&  pos = wxDefaultPosition, 
		const wxSize&  size = wxDefaultSize, 
		long style = 0);

	//!
	~WorkingArea( );

	//! Add button events to the bridge and call UpdateWidget()
	void OnInit(  );

	//!
	bool Enable(bool enable /* = true */);

private:

// ATTRIBUTES
private:

	//!
	wxEmbeddedAppWindow *m_EmbedWin;
};

} //namespace TavernaPlugin{

#endif //_TavernaPluginWorkingArea_H
