/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreCommandPanel_H
#define coreCommandPanel_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataHolder.h"
#include <wxToolBoxControl.h>
#include <wxToolBoxItem.h>
#include "coreBaseWindow.h"

namespace Core
{

namespace Widgets
{

/** 
\brief An item for the Command Panel
\ingroup gmWidgets
\author Juan A. Moya
\date 10 Jan 2008
\sa CommandPanel
*/
class GMWIDGETS_EXPORT CommandPanelItem : public wxToolBoxItem
{
public:

	coreClassNameMacro(CommandPanelItem);

CommandPanelItem(
	wxWindow* widget, 
	const wxString& caption, 
	wxWindow* parent = NULL, 
	wxWindowID id = wxID_ANY, 
	const wxPoint& pos = wxDefaultPosition, 
	const wxSize& size = wxDefaultSize, 
	long style = wxBORDER_NONE, 
	const wxString& name = wxT("CommandPanelItem"))
		: wxToolBoxItem(parent, id, widget, caption, pos, size, style, name) { };

	virtual ~CommandPanelItem(void);
};

/**
\brief This widget is responsible for displaying a toolbox of items that 
the user can show or collapse. Every plugin has a PluginTab that holds 
one CommandPanel.

The items to attach will be widgets designed as views that provide 
parameters for algorithms, that run those algorithms, other GUI elements 
that enable tools to work with inside the working area, or those widgets that
display miscellaneous information.

\ingroup gmWidgets
\author Juan A. Moya
\date 10 Jan 2008
*/
class GMWIDGETS_EXPORT CommandPanel : 
	public wxToolBoxControl,
	public Core::BaseWindow
{
public:
	//!
	coreDefineBaseWindowFactory( CommandPanel );
	coreClassNameMacro(CommandPanel);

	CommandPanel(
		wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("CommandPanel"),
		bool exclusive = true);
	virtual ~CommandPanel(void);

	void Add(CommandPanelItem* item);
	void Remove(CommandPanelItem* item);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

};

} // namespace Widgets
} // namespace Core

#endif // corePluginTab_H

