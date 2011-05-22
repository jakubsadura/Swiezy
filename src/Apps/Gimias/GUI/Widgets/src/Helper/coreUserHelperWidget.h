/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreUserHelperWidget_H
#define coreUserHelperWidget_H

#include "gmWidgetsWin32Header.h"
#include <wx/panel.h>
#include <string>
#include "coreUserHelperWidgetUI.h"
#include "coreBaseWindow.h"
#include "wxID.h"

namespace Core
{
namespace Widgets
{

typedef enum
{
	//! Only a line of text 
	HELPER_INFO_ONLY_TEXT,
	//! Left button icon + text like "+ SHIFT Place seed point"
	HELPER_INFO_LEFT_BUTTON,
	HELPER_INFO_RIGHT_BUTTON,
	HELPER_INFO_KEY_A,
	HELPER_INFO_KEY_DEL,
	HELPER_INFO_MAX
} HELPER_INFO_TYPE;

#define wxID_UserHelper wxID("wxID_UserHelper")

/** 
\brief Show messages to the user to help him to use the interface

You can add an ICON + text or simple text

You can add several lines of text

\ingroup gmWidgets
\author Xavi Planes
\date 31 July 2008
*/
class GMWIDGETS_EXPORT UserHelper : 
	public coreUserHelperWidgetUI,
	public Core::BaseWindow
{

public:
	//!
	coreDefineBaseWindowFactory( UserHelper );

	//!
	UserHelper(
		wxWindow* parent, 
		int id = wxID_UserHelper, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);
	~UserHelper(void);

	//!
	void EnableManualUpdate( bool manualUpdate );

	//!
	bool GetManualUpdate( );

	//! Add new info at the bottom of the widget
	void AddInfo( HELPER_INFO_TYPE type, std::string strText );

	//! Reset the widget and set this info
	void SetInfo( HELPER_INFO_TYPE type, std::string strText );

	//!
	void Update( );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );


private:

	//! Hide the horizontal sizers
	void HideSizers( int iStart, int iEnd );

	//! Add new info to a horizontal sizer
	void SetSizerInfo( int iPos, HELPER_INFO_TYPE type, std::string strText );

	//!
	void LoadBitmaps( );

private:

	//! Current next empty position for horizontal sizer
	int m_iNextEmptyPos;

	//!
	wxBitmap m_BitmapArray[ HELPER_INFO_MAX ];

	//!
	std::string m_strResourceBitmap[ HELPER_INFO_MAX ];

	//!
	bool m_ManualUpdate;
};

} // namespace Widgets
} // namespace Core

#endif // coreUserHelperWidget_H
