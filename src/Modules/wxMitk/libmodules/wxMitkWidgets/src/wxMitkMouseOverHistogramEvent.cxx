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

#include "wxMitkMouseOverHistogramEvent.h"


using namespace mitk;

DEFINE_EVENT_TYPE(mitk::wxEVT_MOUSEOVERHISTOGRAM)
IMPLEMENT_DYNAMIC_CLASS(wxMitkMouseOverHistogramEvent, wxCommandEvent)

//!
wxMitkMouseOverHistogramEvent::wxMitkMouseOverHistogramEvent(int winid, wxEventType commandType)
: wxCommandEvent(commandType, winid), isColorFunction(false), opacityValue(0.0), functionPoint(0.0)
{
	this->rgb[0] = this->rgb[1] = this->rgb[2] = 0.0;
}

//!
wxMitkMouseOverHistogramEvent::~wxMitkMouseOverHistogramEvent(void)
{
}

/**
This function is used to create a copy of the event polymorphically and
all derived classes must implement it because otherwise wxPostEvent()
for them wouldn't work (it needs to do a copy of the event)
*/
wxEvent* wxMitkMouseOverHistogramEvent::Clone(void)
{
	return new wxMitkMouseOverHistogramEvent(*this);
}

//!
double wxMitkMouseOverHistogramEvent::GetFunctionPoint(void)
{
	return this->functionPoint;
}

//!
void wxMitkMouseOverHistogramEvent::GetRGBValue(double rgb[3])
{
	rgb[0] = this->rgb[0];
	rgb[1] = this->rgb[1];
	rgb[2] = this->rgb[2];
}

//!
double wxMitkMouseOverHistogramEvent::GetOpacityValue(void)
{
	return this->opacityValue;
}

bool wxMitkMouseOverHistogramEvent::ValuesFromColorFunction(void)
{
	return this->isColorFunction;
}

bool wxMitkMouseOverHistogramEvent::ValuesFromOpacityFunction(void)
{
	return !this->isColorFunction;
}

