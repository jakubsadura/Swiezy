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

#include "coreUserHelperWidget.h"
#include "coreKernel.h"
#include "coreDirectory.h"
#include "coreWxMitkGraphicalInterface.h"
#include "Helper.xpm"

#include <wxUnicode.h>

using namespace Core::Widgets;

//!
UserHelper::UserHelper(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: coreUserHelperWidgetUI(parent, id, pos, size, style)
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
	graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
	coreAssertMacro(graphicalIface.IsNotNull());
	Core::Runtime::StyleManager::Pointer styleManager = graphicalIface->GetStyleManager();
	coreAssertMacro(styleManager.IsNotNull());

	SetMinSize( wxSize(314, 96) );

	SetBitmap( helper_xpm );

	// Init file names
	m_strResourceBitmap[ HELPER_INFO_LEFT_BUTTON ] = Core::IO::SlashChar + "MouseLeftBtnClk.png";
	m_strResourceBitmap[ HELPER_INFO_RIGHT_BUTTON ] = Core::IO::SlashChar + "MouseRightBtnClk.png";
	m_strResourceBitmap[ HELPER_INFO_KEY_A ] = Core::IO::SlashChar + "letter_a.png";
	m_strResourceBitmap[ HELPER_INFO_KEY_DEL ] = Core::IO::SlashChar + "delete.png";

	LoadBitmaps( );

	m_iNextEmptyPos = 0;
	m_ManualUpdate = false;
}

//!
UserHelper::~UserHelper(void)
{
}

void UserHelper::HideSizers( int iStart, int iEnd )
{
	for ( int i = iStart ; i < iEnd ; i++ )
	{
		m_sizer[ i ]->Show( false );
	}
}

void UserHelper::SetSizerInfo( int iPos, HELPER_INFO_TYPE type, std::string strText )
{
	switch ( type )
	{
	case HELPER_INFO_ONLY_TEXT:
		{
			m_sizer[ iPos ]->Show( true );
			m_bitmap[ iPos ]->Show( false );
			m_label[ iPos ]->Show( true );
			m_label[ iPos ]->SetLabel( _U(strText) );
		}
		break;
	default:
		{
			m_sizer[ iPos ]->Show( true );
			m_bitmap[ iPos ]->SetBitmap( m_BitmapArray[ type ] );
			m_bitmap[ iPos ]->Show( true );
			m_label[ iPos ]->Show( true );
			m_label[ iPos ]->SetLabel( _U(strText) );
		}
		break;
	}

}

void UserHelper::AddInfo( HELPER_INFO_TYPE type, std::string strText )
{
	if ( m_iNextEmptyPos < MAX_SIZERS )
	{
		HideSizers( m_iNextEmptyPos, MAX_SIZERS );

		SetSizerInfo( m_iNextEmptyPos, type, strText );
		m_iNextEmptyPos++;

		if ( !m_ManualUpdate )
		{
			FitInside( );
		}
	}
}


void Core::Widgets::UserHelper::SetInfo( 
	HELPER_INFO_TYPE type, 
	std::string strText )
{
	m_iNextEmptyPos = 0;

	AddInfo( type, strText );
}

void Core::Widgets::UserHelper::LoadBitmaps()
{
	for ( int i = 0 ; i < HELPER_INFO_MAX ; i++ )
	{
		if ( i != HELPER_INFO_ONLY_TEXT )
		{
			m_BitmapArray[ i ].LoadFile( 
				_U(Core::Runtime::Kernel::GetApplicationSettings()->GetResourcePath( ) +
					    m_strResourceBitmap[ i ]), 
				wxBITMAP_TYPE_ANY );
		}
	}
}

void Core::Widgets::UserHelper::Update()
{
	FitInside( );
}

void Core::Widgets::UserHelper::EnableManualUpdate( bool manualUpdate )
{
	m_ManualUpdate = manualUpdate;
}

bool Core::Widgets::UserHelper::GetManualUpdate()
{
	return m_ManualUpdate;
}

Core::BaseProcessor::Pointer Core::Widgets::UserHelper::GetProcessor()
{
	return NULL;
}

