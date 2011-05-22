/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkColorSelectorControl_H
#define wxMitkColorSelectorControl_H

#include "wxMitkWidgetsWin32Header.h"
#include <wx/clrpicker.h>


namespace mitk
{
/*
The wxMitkColorSelectorControl is a widget component used for simple selection of colour.
It allows the user to select a colour and to retrieve it as HSV, RGB or wxColour

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 12 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkColorSelectorControl 
	: public wxColourPickerCtrl
{
public:
	wxMitkColorSelectorControl(
		wxWindow* parent, 
		wxWindowID id, 
		const wxColour& colour = *wxWHITE, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxCLRP_USE_TEXTCTRL, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxT("wxMitkColorSelectorControl"));
	virtual ~wxMitkColorSelectorControl(void);

	void SetRGBColour(const double rgb[3]);
	void SetHSVColour(const double hsv[3]);
	void SetHSLColour(const double hsl[3]);
	void SetRGBColour(const unsigned int rgb[3]);
	void SetHSLColour(const unsigned int hsl[3]);

	void GetRGBColour(double rgb[3]) const;
	void GetHSVColour(double hsv[3]) const;
	void GetHSLColour(double hsl[3]) const;
	void GetRGBColour(unsigned int rgb[3]) const;
	void GetHSLColour(unsigned int hsl[3]) const;

};
 
} // namespace mitk


#endif // wxMitkColorGradientCanvas_H

