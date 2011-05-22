/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreWorkingAreaManagerWidget_H
#define _coreWorkingAreaManagerWidget_H

#include "coreWorkingAreaManagerWidgetUI.h"
#include "coreBaseWindow.h"
#include "coreRenderWindowContainer.h"

namespace Core
{
namespace Widgets
{

/** 
\brief Manage all working areas: Add, Delete, Save, Rename, ...

\ingroup gmWidgets
\author Xavi Planes
\date 27 August 2010
*/
class WorkingAreaManagerWidget: 
	public coreWorkingAreaManagerWidgetUI,
	public BaseWindow{
public:

    WorkingAreaManagerWidget(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

	Core::BaseProcessor::Pointer GetProcessor();

	//!
	void SetPluginTab(Core::Widgets::PluginTab* val);

	//!
	bool Enable(bool enable = true );

private:
	void OnNew(wxCommandEvent &event);
	void OnSave(wxCommandEvent &event);
	void OnDelete(wxCommandEvent &event);
	void OnSelectedWorkingArea(wxCommandEvent &event);
	void OnRename(wxCommandEvent &event);

	//!
	void UpdateWidget();

	//!
	std::string AskName( std::string name );

	//!
	RenderWindowContainer* GetActiveWorkingArea();
protected:

};

}
}

#endif // _coreWorkingAreaManagerWidget_H
