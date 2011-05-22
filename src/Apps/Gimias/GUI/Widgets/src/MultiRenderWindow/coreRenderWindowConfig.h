/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreRenderWindowConfig_H
#define _coreRenderWindowConfig_H

#include "gmWidgetsWin32Header.h"
#include "coreRenderWindowConfigUI.h"
#include "coreBaseWindow.h"
#include "coreRenderWindowConfigBase.h"

namespace Core
{
namespace Widgets
{

class RenderWindowContainer;

/** 
\brief GIMIAS specific implementation of wxMitkMultiRenderWindowConfig.

\ingroup gmWidgets
\author Xavi Planes
\date 3 Mar 2010
*/
class GMWIDGETS_EXPORT RenderWindowConfig :
	public coreRenderWindowConfigUI,
	public BaseWindow
{
public:
	//!
	coreDefineBaseWindowFactory( RenderWindowConfig );

	//!
	RenderWindowConfig(
		wxWindow* parent, 
		int id , const wxPoint&  pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = 0);

	//!
	void SetMultiRenderWindow( RenderWindowBase* multiRenderWindow );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:
	void OnAddCol(wxCommandEvent &event);
	void OnAddRow(wxCommandEvent &event);
	void OnRemove(wxCommandEvent &event);

	//!
	RenderWindowContainer* GetWorkingArea( );

	//!
	void UpdateWidget( );

	//!
	void InitComboBox( );

	//!
	RenderWindowBase* CreateView( RenderWindowContainer* workingArea );

	//!
	void UpdateRegisteredWindows( );

private:
	//!
	std::vector<std::string> m_ViewList;
	//!
	RenderWindowConfigBase* m_MultiRenderWindowConfigBase;
};

}
}

#endif // _coreRenderWindowConfig_H
