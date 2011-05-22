#include "gblWxWidgetsTestGUI.h"
#include "gblBridgeWxWidgets.h"
#include "gblWxButtonEventProxy.h"
#include "gblWxTextCtrlValueProxy.h"

gbl::WxWidgetsTestGUI::WxWidgetsTestGUI(
	wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name
)
: wxWindow(parent, id, pos, size, style, name)
{
	this->btnDoIt = new wxButton(parent, -1, "ButtonDoIt");
}

void gbl::WxWidgetsTestGUI::SetBridge( Bridge _bridge )
{
	if( !gbl::InitBridge(this->bridge, _bridge) )
		return;

	gbl::AddBridgeEvent(this->bridge, "SetFilename", this->btnDoIt );
}

void gbl::WxWidgetsTestGUI::EmulateClick()
{
	//this->btnDoIt->animateClick();
}
