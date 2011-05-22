/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "coreMainContextMenu.h"
#include "coreMainMenu.h"

using namespace Core::Widgets;


MainContextMenu::MainContextMenu() : wxMenu()
{
	Append(wxID_CopyMenuItem, wxT("&Copy\tCtrl+C"), 
		wxT("Copy the current timepoint of the selected data"), wxITEM_NORMAL);
	Append(wxID_PasteMenuItem, wxT("&Paste\tCtrl+V"), 
		wxT("Paste to the selected data timepoint"), wxITEM_NORMAL);
	AppendSeparator();
}


void Core::Widgets::MainContextMenu::UpdateMenu()
{
}
