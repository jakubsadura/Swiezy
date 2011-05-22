/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMacroPannelWidget_H
#define coreMacroPannelWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include <wx/textctrl.h>
#include <string>


namespace Core
{
namespace Widgets
{

/** 
\brief This panel displays messages and warnings, and is intended to be 
used for typing script commands. Also, it displays the script-equivalent 
of operations performed in the GUI.

\ingroup gmWidgets
\author Juan Antonio Moya
\date 04 Jan 2008
*/
class GMWIDGETS_EXPORT MacroPannelWidget 
	: public wxTextCtrl, public virtual Core::Object
{
	
public:
	coreClassNameMacro(MacroPannelWidget)

	MacroPannelWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTE_MULTILINE | wxTE_READONLY | wxVSCROLL , 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxT("MacroPannel"));

	~MacroPannelWidget(void);
	
	void ReportWarning(const std::string& message);
	void ReportError(const std::string& message);
	void ReportMessage(const std::string& message);
	void WriteVerbatim(const std::string& message);
};

} // namespace Widgets
} // namespace Core

#endif // MacroPannelWidget_H
