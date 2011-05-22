/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreThumbnailWidget_H
#define _coreThumbnailWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreThumbnailWidgetUI.h"
#include "coreBaseWindow.h"

#include "thumbnailctrl.h"

namespace Core
{
namespace Widgets
{

/** 
\brief Widget for Showing thumbnails

\ingroup gmWidgets
\author Xavi Planes
\date 06 August 2009
*/
class GMWIDGETS_EXPORT ThumbnailWidget : 
	public coreThumbnailWidgetUI,
	public Core::BaseWindow
{
public:

	coreDefineBaseWindowFactory( ThumbnailWidget )

	//!
	ThumbnailWidget(
		wxWindow* parent, 
		int id, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);

	//!
	~ThumbnailWidget(void);

	//!
	void ShowFolder(const wxString& path);

	//!
	void Append( wxString file, wxString title = wxEmptyString );

	//!
	wxThumbnailCtrl* GetThumbnailCtrl() const;

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:

	DECLARE_EVENT_TABLE()

private:

	//! Thumbnail control
	wxThumbnailCtrl* m_ThumbnailCtrl;
};

} // namespace Widgets
} // namespace Core

#endif // _coreThumbnailWidget_H
