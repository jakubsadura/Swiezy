// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "wxMitkScrolledWindow.h"

IMPLEMENT_APP_CONSOLE(wxMitkScrolledWindow)

BEGIN_EVENT_TABLE(wxMitkScrolledWindow, wxApp)
END_EVENT_TABLE()

//! this is executed upon startup, like 'main()' in non-wxWidgets programs
int wxMitkScrolledWindow::OnRun(void)
{

	wxFrame* mainWindow = new wxFrame(NULL, wxID_ANY, wxT("wxMitk test"));
	wxPanel* panel = new wxScrolledWindow(mainWindow, wxID_ANY);

	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* sizer_horiz = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* lbOutputImage = new wxStaticText(panel, wxID_ANY, wxT("Output file name"));
    wxTextCtrl* leOutputImage = new wxTextCtrl(panel, wxID_ANY, wxT("Output file name"));
    sizer_horiz->Add(lbOutputImage, 1, wxEXPAND);
    sizer_horiz->Add(leOutputImage, 6, wxEXPAND);
    topsizer->Add(sizer_horiz, 0, wxEXPAND);

    panel->SetSizer(topsizer);
    topsizer->SetSizeHints( panel );   // set size hints to honour minimum size

	mainWindow->Show(true);

	wxApp::OnRun();

	return 0;
}

