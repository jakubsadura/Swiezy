/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxMitkApplicationCursorImplementation.h"
#include "vtkRenderWindow.h"
#include "wx/cursor.h"
#include "wx/window.h"
#include "wx/image.h"

using namespace mitk;

mitk::wxMitkApplicationCursorImplementation::wxMitkApplicationCursorImplementation()
{
	m_vtkWindow = NULL;
	m_wxwindow = NULL;
}

mitk::wxMitkApplicationCursorImplementation::~wxMitkApplicationCursorImplementation()
{
	mitk::ApplicationCursor::UnRegisterImplementation( m_vtkWindow );
}

void mitk::wxMitkApplicationCursorImplementation::PushCursor( const char* XPM[], int hotspotX, int hotspotY )
{
	wxImage image(XPM);
	wxCursor cursor( image);
	m_wxwindow->SetCursor( cursor );
	m_CursorList.push_back( cursor );
}

void mitk::wxMitkApplicationCursorImplementation::PopCursor()
{
	wxCursor cursor = wxCURSOR_DEFAULT;
	m_CursorList.pop_back();
	if ( m_CursorList.size() )
	{
		cursor = m_CursorList.back( );
	}
	m_wxwindow->SetCursor( cursor );
}

void mitk::wxMitkApplicationCursorImplementation::SetWindow( 
						vtkRenderWindow *vtkWindow,
						wxWindow *wxwindow )
{
	m_vtkWindow = vtkWindow;
	m_wxwindow = wxwindow;
	mitk::ApplicationCursor::RegisterImplementation( m_vtkWindow, this );
}
