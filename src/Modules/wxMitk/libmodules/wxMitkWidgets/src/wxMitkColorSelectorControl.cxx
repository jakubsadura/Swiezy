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

#include "wxMitkColorSelectorControl.h"
#include "wxMitkColorSpaceHelper.h"

using namespace mitk;

//!
wxMitkColorSelectorControl::wxMitkColorSelectorControl(wxWindow* parent, wxWindowID id, const wxColour& colour, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxColourPickerCtrl(parent, id, colour, pos, size, style, validator, name)
{
}

//!
wxMitkColorSelectorControl::~wxMitkColorSelectorControl(void)
{
}

/**
\brief Sets the selected colour from RGB colourspace, using range [0.0, 1.0]
*/
void wxMitkColorSelectorControl::SetRGBColour(const double rgb[3])
{
	wxColour color((unsigned char)(floor(255 * rgb[0])), (unsigned char)(floor(255 * rgb[1])), 
		(unsigned char)(floor(255 * rgb[2])));
	this->SetColour(color);
}

/**
\brief Sets the selected colour from HSV colourspace, using range H[0.0, 360.0] S[0.0, 1.0] V[0.0, 1.0]
*/
void wxMitkColorSelectorControl::SetHSVColour(const double hsv[3])
{
	double rgb[3];
	wxMitkColorSpaceHelper::HSVtoRGB(hsv, rgb);
	this->SetRGBColour(rgb);
}

/**
\brief Sets the selected colour from HSL colourspace, using range H[0.0, 360.0] S[0.0, 1.0] L[0.0, 1.0]
*/
void wxMitkColorSelectorControl::SetHSLColour(const double hsl[3])
{
	double rgb[3];
	wxMitkColorSpaceHelper::HSLtoRGB(hsl, rgb);
	this->SetRGBColour(rgb);
}


/**
\brief Sets the selected colour from RGB colourspace, using range [0, 255]
*/
void wxMitkColorSelectorControl::SetRGBColour(const unsigned int rgb[3])
{
	wxColour color(rgb[0], rgb[1], rgb[2]);
	this->SetColour(color);
}


/**
\brief Sets the selected colour from HSL colourspace, using range [0, 255]
*/
void wxMitkColorSelectorControl::SetHSLColour(const unsigned int hsl[3])
{
	unsigned int rgb[3];
	wxMitkColorSpaceHelper::HSLtoRGB(hsl, rgb);
	this->SetRGBColour(rgb);
}



/**
\brief Returns the selected colour in RGB colourspace, using range [0.0, 1.0]
*/
void wxMitkColorSelectorControl::GetRGBColour(double rgb[3]) const
{
	wxColour color = this->GetColour(); 
	rgb[0] = (double)color.Red() / 255;
	rgb[1] = (double)color.Green() / 255;
	rgb[2] = (double)color.Blue() / 255;
}

/**
\brief Returns the selected colour in RGB colourspace, using range [0, 255]
*/
void wxMitkColorSelectorControl::GetRGBColour(unsigned int rgb[3]) const
{
	wxColour color = this->GetColour(); 
	rgb[0] = color.Red();
	rgb[1] = color.Green();
	rgb[2] = color.Blue();
}

/**
\brief Returns the selected colour in HSV colourspace, using range H[0.0, 360.0] S[0.0, 1.0] V[0.0, 1.0]

*/
void wxMitkColorSelectorControl::GetHSVColour(double hsv[3]) const
{
	double rgb[3];
	this->GetRGBColour(rgb);
	wxMitkColorSpaceHelper::RGBtoHSV(rgb, hsv);
}


/**
\brief Returns the selected colour in HSL colourspace, using range H[0.0, 360.0] S[0.0, 1.0] L[0.0, 1.0]
*/
void wxMitkColorSelectorControl::GetHSLColour(double hsl[3]) const
{
	double rgb[3];
	this->GetRGBColour(rgb);
	wxMitkColorSpaceHelper::RGBtoHSL(rgb, hsl);
}


/**
\brief Returns the selected colour in HSL colourspace, using range [0, 255]
*/
void wxMitkColorSelectorControl::GetHSLColour(unsigned int hsl[3]) const
{
	unsigned int rgb[3];
	this->GetRGBColour(rgb);
	wxMitkColorSpaceHelper::RGBtoHSL(rgb, hsl);
}

