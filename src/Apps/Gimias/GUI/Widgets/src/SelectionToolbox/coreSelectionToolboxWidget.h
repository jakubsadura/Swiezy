/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSelectionToolboxWidget_H
#define _coreSelectionToolboxWidget_H

#include "coreBaseWindow.h"
#include "coreSelectionToolWidget.h"
#include "coreBaseToolboxWidget.h"

// Forward declarations
class wxButton;
class wxStaticText;

namespace Core
{
	class DataEntity;
	class DataEntityList;
}

namespace Core{
namespace Widgets{


/** 
\brief class that display the parameters for the processors of the tools menu
\ingroup gmWidgets
\author Chiara Riccobene
\date 05 Nov 2009
*/
class GMWIDGETS_EXPORT SelectionToolboxWidget : public BaseToolboxWidget
{
public:
	//!
	coreDefineBaseWindowFactory( SelectionToolboxWidget );

	coreClassNameMacro(SelectionToolboxWidget);
	
	SelectionToolboxWidget(wxWindow* parent, 
						wxWindowID id = wxID_ANY, 
						const wxPoint& pos = wxDefaultPosition, 
						const wxSize& size = wxDefaultSize, 
						long style = wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE, 
						const wxString& name = wxPanelNameStr);

	//!
	void SetToolWindow(wxWindow *win);

	//!
	template <class T>
	T* GetTool()
	{
		return dynamic_cast<T*> ( m_SelectionToolWidget );
	}

	//!
	wxWindow* GetToolWindow();

	//!
	SelectionToolWidget* GetSelectionToolWidget() const;

	//!
	void Start( );

	//!
	void Stop( );

	//!
	bool IsSelectionEnabled( );

private:
	virtual ~SelectionToolboxWidget(void);	
	
	//!
	virtual std::string GetNameMenuItem( );

	//!
	virtual WIDGET_TYPE GetWidgetState( );

private:
	//!
	SelectionToolWidget* m_SelectionToolWidget;
};

} // Widgets
} // Core

#endif // coreSelectionToolboxWidget_H
