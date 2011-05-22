/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreToolbarIO.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "coreMainMenu.h"

//#include ".xpm"
#include "OpenFile.xpm"
#include "SaveFile.xpm"
#include "OpenFolder.xpm"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::ToolbarIO, wxToolBar)
END_EVENT_TABLE()


Core::Widgets::ToolbarIO::ToolbarIO(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxToolBar(parent, id, pos, size, style, name)
{
	wxBitmap bitmap;

	bitmap = wxBitmap( openfile_xpm );
	AddTool(wxID_OpenDataMenuItem, _T("Open File"),
		bitmap, wxNullBitmap, wxITEM_NORMAL,
		_T("Open File"), _T("Open File"));

	bitmap = wxBitmap( savefile_xpm );
	AddTool(wxID_SaveDataMenuItem, _T("Save File"),
		bitmap, wxNullBitmap, wxITEM_NORMAL,
		_T("Save File"), _T("Save File"));
	
	bitmap = wxBitmap( openfolder_xpm );
	AddTool(wxID_OpenDirectoryMenuItem, _T("Open directory"),
		bitmap, wxNullBitmap, wxITEM_NORMAL,
		_T("Open directory"), _T("Open directory"));

	Realize();
}

void Core::Widgets::ToolbarIO::UpdateState()
{
}

Core::BaseProcessor::Pointer Core::Widgets::ToolbarIO::GetProcessor()
{
	return NULL;
}

