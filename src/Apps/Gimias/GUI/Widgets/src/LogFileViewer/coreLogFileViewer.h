/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef LogFileViewer_H
#define LogFileViewer_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include <wx/dialog.h>

class wxTextCtrl;

namespace Core
{
namespace Widgets
{

/** 
\brief The LogFileViewer class provides a widget to display the log file of the
application (see the core Settings class for finding out where this file is).

\ingroup gmWidgets
\author Juan Antonio Moya
\date 18 Jan 2008
\sa Settings
*/
class GMWIDGETS_EXPORT LogFileViewer : public wxDialog
{

public:
	coreClassNameMacro(LogFileViewer);

	LogFileViewer(wxWindow* parent);
	virtual ~LogFileViewer(void);

	void ClearLogFile(void);
	void ReadLogFile(void);

private:
	void OnButtonClear(wxCommandEvent& WXUNUSED(event));
	void OnButtonClose(wxCommandEvent& WXUNUSED(event));

	wxTextCtrl* txtBox;

	DECLARE_EVENT_TABLE();
};
}
}

#endif
