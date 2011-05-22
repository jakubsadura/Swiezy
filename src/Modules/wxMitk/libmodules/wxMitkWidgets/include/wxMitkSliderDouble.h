/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef wxMitkSliderDouble_H
#define wxMitkSliderDouble_H

#include "wxMitkWidgetsWin32Header.h"

class wxTextCtrl;

namespace mitk
{
// Definitions for enabling the custom wxMitkSliderChangeEvent event 
// in wxWidgets ///////////
DECLARE_EXPORTED_EVENT_TYPE(WXMITKWIDGETS_EXPORT, wxEVT_SLIDER_CHANGE, -1)

/**
\brief An event class responsible for signaling that the value has changed
within wxMitkSliderDouble control
\sa wxMitkSliderDouble

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 14 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkSliderChangeEvent : public wxCommandEvent
{

friend class wxMitkSliderDouble;

public:
	wxMitkSliderChangeEvent(
		int winid = 0, 
		wxEventType commandType = wxEVT_SLIDER_CHANGE);
	virtual ~wxMitkSliderChangeEvent(void);
	
	// required for sending it with wxPostEvent()
    wxEvent* Clone();

	double GetValue(void);

private:
	double value;

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxMitkSliderChangeEvent)
};


// Definitions for enabling the custom wxMitkSliderChangeEvent event 
// in wxWidgets ///////////
typedef void (wxEvtHandler::*wxMitkSliderChangeEventFunction)
(mitk::wxMitkSliderChangeEvent&);
#define wxMitkSliderChangeEventHandler(func) \
	(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) \
    wxStaticCastEvent( wxMitkSliderChangeEventFunction, & func )
#define EVT_SLIDER_CHANGE(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(\
		mitk::wxEVT_SLIDER_CHANGE, \
			id, wxID_ANY, wxMitkSliderChangeEventHandler(fn), NULL), 


// ------------------------------------------------------------------------
/*
wxMitkSliderDouble provides a slider that accepts DOUBLE types instead of 
int, like the standard slider.

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 18 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkSliderDouble : public wxPanel
{
	
public:
	wxMitkSliderDouble(
		wxWindow* parent, 
		wxWindowID id, 
		double minValue = 0.0, 
		double maxValue = 1.0, 
		double stepResolution = 100.0, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxSL_HORIZONTAL, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxT("wxMitkSliderDouble"));
	virtual ~wxMitkSliderDouble(void);
	virtual void SetValue(double value);
	virtual double GetValue(void) const;

protected:
	void SetValueToTextControl(double value);
	void SetValueToSlider(double value);
	double GetValueFromTextControl(void);
	double GetValueFromSlider(void) const;

private:
	void OnTextChange(wxCommandEvent& event);
	void OnSliderChange(wxScrollEvent& event);

	double step;
	double currentValue;
	double minValue;
	double maxValue;

	wxTextCtrl* txtValue;
	wxSlider* sldValue;

	DECLARE_EVENT_TABLE();
};

} // namespace mitk


#endif // wxMitkSliderDouble
