/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreFuraPopupWidget_H
#define coreFuraPopupWidget_H

#include <wx/panel.h>

#include <string>

#include "coreFuraPopupWidgetUI.h"
#include "gmWidgetsWin32Header.h"

namespace Core
{
namespace Widgets
{

/** 
\brief Widget for connecting to Fura


\ingroup gmWidgets
\author Mohommed Rizwan
\date 06 March 2009
*/
class GMWIDGETS_EXPORT FuraPopupWidget : public coreFuraPopupWidgetUI
{
public:

public:
	//!
	FuraPopupWidget(
		wxWindow* parent, 
		int id, 
		const wxString& title, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);

	//!
	~FuraPopupWidget(void);

private:

	//! Windows only
	#ifdef WIN32
	void OnOK(wxCommandEvent& event);

	void ExecuteFuraConnector( 
		std::string host,
		std::string port,
		std::string resultsPath,
		std::string module,
		std::string exGroup );
	#endif
	//!
	void OnCancel(wxCommandEvent& event);
	
	//!
	void OnBrowse(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	std::string m_FilenamePath;
	std::string m_Filename;

};

} // namespace Widgets
} // namespace Core

#endif // coreFuraPopupWidget_H

