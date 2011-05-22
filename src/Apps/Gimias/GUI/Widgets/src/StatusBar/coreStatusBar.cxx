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

#include "coreStatusBar.h"

#include "wxMemoryUsageIndicator.h"

#include "wxID.h"

BEGIN_EVENT_TABLE(Core::Widgets::StatusBar, wxStatusBar)
	EVT_SIZE(Core::Widgets::StatusBar::OnSize)
END_EVENT_TABLE()

#define wxID_MemoryUsageWidget		wxID("wxID_MemoryUsageWidget")

const int FIELD_MAX = 2;
static const int BITMAP_SIZE_X = 32;
static const int BITMAP_SIZE_Y = 15;

enum
{
	Field_Text,
	Field_Bitmap,
	Field_Max
};

Core::Widgets::StatusBar::StatusBar(wxWindow *parent)
	: wxStatusBar(parent, wxID_ANY)
{

//takes to long in debug, thus, slows down the start up speed
	m_MemoryUsageWidget = new wxMemoryUsageIndicator( this, wxID_MemoryUsageWidget );

	wxSize size = m_MemoryUsageWidget->GetMinSize();

	static const int widths[ Field_Max ] = { -1, size.GetX() };

	SetFieldsCount( Field_Max );
	SetStatusWidths( Field_Max, widths);

	SetMinHeight(BITMAP_SIZE_Y);
}

void Core::Widgets::StatusBar::OnSize(wxSizeEvent& event)
{
	//takes to long in debug, thus, slows down the start up speed
	wxRect rect;

	// Move the widget to the center of the Field_Bitmap
    GetFieldRect( Field_Bitmap, rect );
    wxSize size = m_MemoryUsageWidget->GetSize();
    m_MemoryUsageWidget->Move( rect.x, rect.y );

    event.Skip();
}

Core::Widgets::StatusBar::~StatusBar()
{

}

