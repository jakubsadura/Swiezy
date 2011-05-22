/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef wxMitkMouseOverHistogramEvent_H
#define wxMitkMouseOverHistogramEvent_H

#include "wxMitkWidgetsWin32Header.h"
#include <wx/event.h>



namespace mitk
{
// Definitions for enabling the custom wxMitkMouseOverHistogramEvent event 
// in wxWidgets ///////////
DECLARE_EXPORTED_EVENT_TYPE(WXMITKWIDGETS_EXPORT, wxEVT_MOUSEOVERHISTOGRAM, -1)

/**
\brief An event class responsible for signaling that the value of the 
TransferFunction under the mouse pointer.
\sa wxMitkColorFunctionControl

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 11 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkMouseOverHistogramEvent 
	: public wxCommandEvent
{

friend class wxMitkColorFunctionControl;
friend class wxMitkColorGradientControl;
friend class wxMitkScalarOpacityControl;
friend class wxMitkGradientOpacityControl;

public:
	wxMitkMouseOverHistogramEvent(
		int winid = 0, 
		wxEventType commandType = wxEVT_MOUSEOVERHISTOGRAM);
	virtual ~wxMitkMouseOverHistogramEvent(void);
	
	// required for sending it with wxPostEvent()
    wxEvent* Clone();

	double GetFunctionPoint(void);
	void GetRGBValue(double rgb[3]);
	double GetOpacityValue(void);
	bool ValuesFromColorFunction(void);
	bool ValuesFromOpacityFunction(void);
	
private:
	double rgb[3];
	double functionPoint;
	double opacityValue;
	bool isColorFunction;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxMitkMouseOverHistogramEvent)
};



// Definitions for enabling the custom wxMitkMouseOverHistogramEvent 
// event in wxWidgets ///////////
typedef void (wxEvtHandler::*wxMitkMouseOverHistogramEventFunction)
	(mitk::wxMitkMouseOverHistogramEvent&);
#define wxMitkMouseOverHistogramEventHandler(func) \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
    wxStaticCastEvent( wxMitkMouseOverHistogramEventFunction, & func )
#define EVT_MOUSEOVERHISTOGRAM(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(\
		mitk::wxEVT_MOUSEOVERHISTOGRAM, \
		id, wxID_ANY, wxMitkMouseOverHistogramEventHandler(fn), NULL), 


 
} // namespace mitk


#endif // wxMitkMouseOverHistogramEvent_H

