/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wx/wx.h>
#include <wx/image.h>

#ifndef _SignalReaderWidget_H
#define _SignalReaderWidget_H

#include "SignalReaderWidgetUI.h"
#include "coreBaseWindow.h"


/**
\brief Show signal properties and sub sampling option
\ingroup SignalViewerPlugin
\author Xavi Planes
\date 21 June 2010
*/
class SignalReaderWidget : 
	public SignalReaderWidgetUI,
	public Core::BaseWindow {
public:

	coreDefineBaseWindowReaderFactory( SignalReaderWidget, Core::SignalTypeId, ".vhdr" )

	//!
    SignalReaderWidget(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	/** Set and Get properties of the window to serialize them
	or visualize in the GUI
	*/
	void SetProperties( blTagMap::Pointer tagMap );
	void GetProperties( blTagMap::Pointer tagMap );

	//!
	void UpdateWidget ();

	//!
	void UpdateData ();

private:

	//!
	void OnSpinFactor(wxSpinEvent &event);

protected:

	//!
	blTagMap::Pointer m_TagMap;
};


#endif // _SignalReaderWidget_H
