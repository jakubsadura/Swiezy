/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMainApp_H
#define coreMainApp_H

#include <wx/app.h>

namespace Core
{
/**
A version of wxApp is provided for wxMitk. When you use Lambda Core, 
you must create an instance of MainApp.

The reason behind this relies in specific routines and event filtering 
needed for the correct behavior of the mitk rendering engine. Read more 
about in wxMitkApp class.
\sa wxMitkApp

\ingroup gmMainApp
\author Juan Antonio Moya
\date 02 Jan 2008
*/
class MainApp : public wxApp
{
public:
	virtual bool OnInit(void);
	virtual int OnRun(void);
	virtual int OnExit(void);
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	//!
	void HandleEvent(wxEvtHandler *handler,
		wxEventFunction func,
		wxEvent& event) const;

};

DECLARE_APP(MainApp)

}

#endif // coreMainApp_H
