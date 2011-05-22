/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseToolboxWidget_H
#define _coreBaseToolboxWidget_H

#include "coreBaseWindow.h"

namespace Core{
namespace Widgets{


/** 
\brief class that display the parameters for the processors of the tools menu
\ingroup gmWidgets
\author Chiara Riccobene
\date 05 Nov 2009
*/
class GMWIDGETS_EXPORT BaseToolboxWidget 
	: public wxScrolledWindow, public BaseWindow
{
public:
	coreClassNameMacro(BaseToolboxWidget);
	
	BaseToolboxWidget(wxWindow* parent, 
						wxWindowID id = wxID_ANY, 
						const wxPoint& pos = wxDefaultPosition, 
						const wxSize& size = wxDefaultSize, 
						long style = wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE, 
						const wxString& name = wxPanelNameStr);

	//!
	void SetToolByName( const std::string &caption );

	//!
	void OnInit();

	//!
	void SetToolByID( wxWindowID id );

	//!
	virtual wxWindow* GetToolWindow() = 0;

	//!
	virtual void SetToolWindow( wxWindow* win ) = 0;

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	bool Enable(bool enable = true );

protected:
	virtual ~BaseToolboxWidget(void);	

	//!
	void set_properties();
	void do_layout();

	//!
	void SetToolByFactoryName( const std::string &factoryName );

	//!
	void Clean( );

	//!
	void CreateCaptionLabel( );

	//!
	void OnMouseClick( wxMouseEvent& event );
	
	//!
	virtual std::string GetNameMenuItem( ) = 0;

	//!
	virtual WIDGET_TYPE GetWidgetState( ) = 0;

	//!
	void InitTool( );
private:
	//!
	wxStaticText* m_EmptyLabel;
	//!
	wxStaticText* m_lblCaption;
};

} // Widgets
} // Core

#endif // coreBaseToolboxWidget_H
