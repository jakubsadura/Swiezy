/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxMitkSliderDouble.h"

using namespace mitk;

#define ID_txtValue (1 + wxID_HIGHEST)
#define ID_sldValue (2 + wxID_HIGHEST)

BEGIN_EVENT_TABLE(wxMitkSliderDouble, wxPanel)
	EVT_TEXT_ENTER(ID_txtValue, wxMitkSliderDouble::OnTextChange)
	EVT_COMMAND_SCROLL(ID_sldValue, wxMitkSliderDouble::OnSliderChange)
END_EVENT_TABLE()

//!
wxMitkSliderDouble::wxMitkSliderDouble(wxWindow* parent, wxWindowID id, double minValue, double maxValue, double stepResolution, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxPanel(parent, id, pos, size, style, name), minValue(minValue), maxValue(maxValue), step(stepResolution)
{
	int min = (int)(minValue * stepResolution);
	int max = (int)(maxValue * stepResolution);
	int value = (int)((maxValue + minValue) * stepResolution / 2.0);

	// Create controls
	this->txtValue = new wxTextCtrl(this, ID_txtValue, wxT(""), wxDefaultPosition, wxSize(48,12));
	this->sldValue = new wxSlider(this, ID_sldValue, value, min, max, wxDefaultPosition, wxDefaultSize, style);

	// layout
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(this->txtValue, 0, wxEXPAND);
	sizer->Add(this->sldValue, 1, wxEXPAND);
	this->SetSizer(sizer);

	// Assign values
	this->currentValue = (maxValue + minValue) / 2.0;
	this->SetValueToTextControl(this->currentValue);
	this->SetValueToSlider(this->currentValue);
}

//!
wxMitkSliderDouble::~wxMitkSliderDouble(void)
{
}

//!
void wxMitkSliderDouble::SetValue(double value)
{
	this->currentValue = value;
	if(value < this->minValue)
        this->currentValue = this->minValue;
    if(value > this->maxValue)
        this->currentValue = this->maxValue;
	this->SetValueToSlider(this->currentValue);
	this->SetValueToTextControl(this->currentValue);
}

//!
double wxMitkSliderDouble::GetValue(void) const
{
	return this->currentValue;
}


//!
void wxMitkSliderDouble::OnTextChange(wxCommandEvent& event)
{
	this->currentValue = GetValueFromTextControl();
	SetValueToSlider(this->currentValue);
	
	// Cast event to the wxMitkSliderChangeEvent type
	wxMitkSliderChangeEvent newEvent(this->GetId());
    newEvent.SetEventObject(this);
	newEvent.value = this->currentValue;

	// Cast the event
	this->GetEventHandler()->ProcessEvent(newEvent);

	// Consume the cought event
	event.Skip(false);
}

//!
void wxMitkSliderDouble::OnSliderChange(wxScrollEvent& event)
{
	this->currentValue = GetValueFromSlider();
	SetValueToTextControl(this->currentValue);

	// Cast event to the wxMitkSliderChangeEvent type
	wxMitkSliderChangeEvent newEvent(this->GetId());
    newEvent.SetEventObject(this);
	newEvent.value = this->currentValue;

	// Cast the event
	this->GetEventHandler()->ProcessEvent(newEvent);

	// Consume the caught event
	event.Skip(false);
}

//!
double wxMitkSliderDouble::GetValueFromTextControl(void)
{
	wxString value = this->txtValue->GetValue();
	float x;

    if((wxSscanf(value, wxT("%f"), &x) != 1))
	{
        x = this->minValue;
		SetValueToTextControl((double)x); // override it with a valid value
	}
    if(x < this->minValue)
    {
        x = this->minValue;
		SetValueToTextControl((double)x); // override it with a valid value
	}
    if(x > this->maxValue)
    {
        x = this->maxValue;
		SetValueToTextControl((double)x); // override it with a valid value
	}

	return (double)x;
}

//!
double wxMitkSliderDouble::GetValueFromSlider(void) const
{
	return this->sldValue->GetValue() / this->step;
}


//!
void wxMitkSliderDouble::SetValueToTextControl(double value)
{
	wxString text = wxString::Format(wxT("%.2f"), value);
	this->txtValue->SetValue(text);
}

//!
void wxMitkSliderDouble::SetValueToSlider(double value)
{
	this->sldValue->SetValue((int)(value * this->step));
}

//////////////////////////////////////////////////////////////////////////////////
//
// Code for the wxWidgetRisenEvent
//
//////////////////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(mitk::wxEVT_SLIDER_CHANGE)
IMPLEMENT_DYNAMIC_CLASS(wxMitkSliderChangeEvent, wxCommandEvent)

//!
wxMitkSliderChangeEvent::wxMitkSliderChangeEvent(int winid, wxEventType commandType)
: wxCommandEvent(commandType, winid)
{
}

//!
wxMitkSliderChangeEvent::~wxMitkSliderChangeEvent(void)
{
}

/**
This function is used to create a copy of the event polymorphically and
all derived classes must implement it because otherwise wxPostEvent()
for them wouldn't work (it needs to do a copy of the event)
*/
wxEvent* wxMitkSliderChangeEvent::Clone(void)
{
	return new wxMitkSliderChangeEvent(*this);
}

//!
double wxMitkSliderChangeEvent::GetValue(void)
{
	return this->value;
}
