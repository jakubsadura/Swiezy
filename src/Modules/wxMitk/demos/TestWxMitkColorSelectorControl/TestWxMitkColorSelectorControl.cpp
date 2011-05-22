// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "TestWxMitkColorSelectorControl.h"
#include "wxMitkColorSelectorControl.h"
#include "wxMitkColorSpaceHelper.h"
#include <iostream>

using namespace mitk;

#define ID_wxMitkColorSelectorControl 12345

BEGIN_EVENT_TABLE(TestWxMitkColorSelectorControl, wxMitkApp)
	EVT_COLOURPICKER_CHANGED(ID_wxMitkColorSelectorControl, TestWxMitkColorSelectorControl::OnColorChange)
END_EVENT_TABLE()

IMPLEMENT_APP_CONSOLE(TestWxMitkColorSelectorControl);

//!
int TestWxMitkColorSelectorControl::OnRun(void)
{
	wxFrame* mainWindow = new wxFrame(NULL, wxID_ANY, wxT("wxMitk test"), wxDefaultPosition, wxSize(300, 64));

	// create the control
	control = new mitk::wxMitkColorSelectorControl(mainWindow, ID_wxMitkColorSelectorControl);
	
	mainWindow->SetAutoLayout(true);
	wxBoxSizer* layout = new wxBoxSizer(wxHORIZONTAL);
	layout->Add(control, 0, wxEXPAND);
	mainWindow->SetSizer(layout);

	mainWindow->Show(true);

	this->wxMitkApp::OnRun();

	return 0;
}

//!
void TestWxMitkColorSelectorControl::OnColorChange(wxColourPickerEvent& event)
{
	wxString nameCSS = event.GetColour().GetAsString(wxC2S_CSS_SYNTAX);

	std::cout << std::endl << " -- With type UINT:" << std::endl;

	{
	unsigned int rgb[3], hsl[3], rgbDest[3], hslDest[3];
	const int e = 1;
	bool succeededRGB, succeededHLS;
	this->control->GetRGBColour(rgb);
	this->control->GetHSLColour(hsl);
	std::cout << "Selected a color: " << nameCSS << std::endl
		<< ", Got as R(" << rgb[0] << ")G(" << rgb[1] << ")B(" << rgb[2] << ") - H(" 
		<< hsl[0] << ")S(" << hsl[1] << ")L(" << hsl[2] << ")" << std::endl;
	wxMitkColorSpaceHelper::RGBtoHSL(rgb, hsl);
	wxMitkColorSpaceHelper::HSLtoRGB(hsl, rgbDest);
	succeededRGB = ((int)rgbDest[0] <= (int)rgb[0] + e) && ((int)rgbDest[1] <= (int)rgb[1] + e) && ((int)rgbDest[2] <= (int)rgb[2] + e);
	succeededRGB &= ((int)rgbDest[0] >= (int)rgb[0] - e) && ((int)rgbDest[1] >= (int)rgb[1] - e) && ((int)rgbDest[2] >= (int)rgb[2] - e);
	wxMitkColorSpaceHelper::HSLtoRGB(hsl, rgb);
	wxMitkColorSpaceHelper::RGBtoHSL(rgb, hslDest);
	succeededHLS = ((int)hslDest[0] <= (int)hsl[0] + e) && ((int)hslDest[1] <= (int)hsl[1] + e) && ((int)hslDest[2] <= (int)hsl[2] + e);
	succeededHLS &= ((int)hslDest[0] >= (int)hsl[0] - e) && ((int)hslDest[1] >= (int)hsl[1] - e) && ((int)hslDest[2] >= (int)hsl[2] - e);

	std::cout << "  Testing conversions: " << std::endl
		<< "    Got R(" << rgbDest[0] << ")G(" << rgbDest[1] << ")B(" << rgbDest[2] << ") - H(" 
		<< hslDest[0] << ")S(" << hslDest[1] << ")L(" << hslDest[2] << ")" << std::endl

		<< "    RGB->HLS->RGB(" << (succeededRGB?"OK":"ERROR")
		<< ")    HLS->RGB->HLS(" << (succeededHLS?"OK":"ERROR") << ")" << std::endl;
	}

	std::cout << std::endl << " -- Now with type DOUBLE:" << std::endl;

	{
	double rgb[3], hsl[3], rgbDest[3], hslDest[3];
	const double e = 0.0001;
	bool succeededRGB, succeededHLS;
	this->control->GetRGBColour(rgb);
	this->control->GetHSLColour(hsl);
	std::cout << "Selected a color: " << nameCSS << std::endl
		<< ", Got as R(" << rgb[0] << ")G(" << rgb[1] << ")B(" << rgb[2] << ") - H(" 
		<< hsl[0] << ")S(" << hsl[1] << ")L(" << hsl[2] << ")" << std::endl;
	wxMitkColorSpaceHelper::RGBtoHSL(rgb, hsl);
	wxMitkColorSpaceHelper::HSLtoRGB(hsl, rgbDest);
	succeededRGB = (rgbDest[0] <= rgb[0] + e) && (rgbDest[1] <= rgb[1] + e) && (rgbDest[2] <= rgb[2] + e);
	succeededRGB &= (rgbDest[0] >= rgb[0] - e) && (rgbDest[1] >= rgb[1] - e) && (rgbDest[2] >= rgb[2] - e);
	wxMitkColorSpaceHelper::HSLtoRGB(hsl, rgb);
	wxMitkColorSpaceHelper::RGBtoHSL(rgb, hslDest);
	succeededHLS = (hslDest[0] <= hsl[0] + e) && (hslDest[1] <= hsl[1] + e) && (hslDest[2] <= hsl[2] + e);
	succeededHLS &= (hslDest[0] >= hsl[0] - e) && (hslDest[1] >= hsl[1] - e) && (hslDest[2] >= hsl[2] - e);

	std::cout << "  Testing conversions: " << std::endl
		<< "    Got R(" << rgbDest[0] << ")G(" << rgbDest[1] << ")B(" << rgbDest[2] << ") - H(" 
		<< hslDest[0] << ")S(" << hslDest[1] << ")L(" << hslDest[2] << ")" << std::endl

		<< "    RGB->HLS->RGB(" << (succeededRGB?"OK":"ERROR")
		<< ")    HLS->RGB->HLS(" << (succeededHLS?"OK":"ERROR") << ")" << std::endl;
	}

	std::cout << " ---------------------------------------------- " << std::endl;

	event.Skip();
}





