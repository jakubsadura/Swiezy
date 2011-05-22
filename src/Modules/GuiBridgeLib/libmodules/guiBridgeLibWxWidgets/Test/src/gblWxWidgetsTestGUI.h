#ifndef GBLQTTESTGUI_H
#define GBLQTTESTGUI_H

#include "gblBridge.h"
#include "wx/button.h"
#include "wx/textctrl.h"

namespace gbl
{

class /*GUIBRIDGELIB_EXPORT*/ WxWidgetsTestGUI : public wxWindow
{
public:
	WxWidgetsTestGUI(	wxWindow* parent, 
						wxWindowID id, 
						const wxPoint& pos = wxPoint(5, 5), 
						const wxSize& size = wxSize(100, 100), 
						long style = 0, 
						const wxString& name = "WxWidgetsTestGUI"
	);
	void SetBridge( Bridge _bridge );
	void EmulateClick();

	Bridge bridge;
	wxButton *btnDoIt;
	wxTextCtrl* txtFilename;
};

} // namespace gbl

#endif //GBLQTTESTGUI_H
