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

#include "wx/dir.h"
#include "wxID.h"

#include <wxUnicode.h>

#include "coreThumbnailWidget.h"

const long wxID_ThumbnailCtrl = wxNewId( );

/// Returns the image type, or -1, determined from the extension.
static int DetermineImageType(const wxString& filename);

// Event the widget
BEGIN_EVENT_TABLE(Core::Widgets::ThumbnailWidget, coreThumbnailWidgetUI)
END_EVENT_TABLE()

//!
Core::Widgets::ThumbnailWidget::ThumbnailWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: coreThumbnailWidgetUI(parent, id, pos, size, style)
{

	m_ThumbnailCtrl = new wxThumbnailCtrl( 
		this, 
		wxID_ThumbnailCtrl, 
		wxDefaultPosition, 
		wxDefaultSize,
		wxTH_SHOW_TITLE | wxTH_IMAGE_LABEL | wxTH_EXTENSION_LABEL );
		//long style = wxTH_TEXT_LABEL|wxTH_IMAGE_LABEL|wxTH_EXTENSION_LABEL );
	GetSizer()->Insert( 0, m_ThumbnailCtrl, 1, wxEXPAND, 0 );
}

//!
Core::Widgets::ThumbnailWidget::~ThumbnailWidget(void)
{

}

/// Shows the images in the given dir
void Core::Widgets::ThumbnailWidget::ShowFolder(const wxString& path)
{
	wxBusyCursor busy;
	m_ThumbnailCtrl->Clear();

	m_ThumbnailCtrl->Freeze();
	wxDir dir;
	if (dir.Open(path))
	{
		wxString filename;

		bool cont = dir.GetFirst(&filename, wxT("*.*"), wxDIR_FILES);
		while ( cont )
		{
			wxString file = path + wxFILE_SEP_PATH + filename;
			Append( file );

			cont = dir.GetNext(&filename);
		}                
	}
	m_ThumbnailCtrl->Thaw();
}

void Core::Widgets::ThumbnailWidget::Append( wxString file, wxString title )
{
	if (wxFileExists(file) && DetermineImageType(file) != -1)
	{
		m_ThumbnailCtrl->Append(new wxImageThumbnailItem(file, title));
	}
}

wxThumbnailCtrl* Core::Widgets::ThumbnailWidget::GetThumbnailCtrl() const
{
	return m_ThumbnailCtrl;
}

Core::BaseProcessor::Pointer Core::Widgets::ThumbnailWidget::GetProcessor()
{
	return NULL;
}

// Returns the image type, or -1, determined from the extension.
static int DetermineImageType(const wxString& filename)
{
	wxString path, name, ext;

	wxSplitPath(filename, & path, & name, & ext);

	ext.MakeLower();
	if (ext == wxT("jpg") || ext == wxT("jpeg"))
		return wxBITMAP_TYPE_JPEG;
	else if (ext == wxT("gif"))
		return wxBITMAP_TYPE_GIF;
	else if (ext == wxT("bmp"))
		return wxBITMAP_TYPE_BMP;
	else if (ext == wxT("png"))
		return wxBITMAP_TYPE_PNG;
	else if (ext == wxT("pcx"))
		return wxBITMAP_TYPE_PCX;
	else if (ext == wxT("tif") || ext == wxT("tiff"))
		return wxBITMAP_TYPE_TIF;
	else
		return -1;
}
