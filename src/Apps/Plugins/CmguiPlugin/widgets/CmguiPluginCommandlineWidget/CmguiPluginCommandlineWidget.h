/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginCommandlineWidget_H
#define _CmguiPluginCommandlineWidget_H

#include "wx/glcanvas.h"


// CoreLib
#include "coreProcessingWidget.h"

#include "CmguiPluginCmgui.h"
#include "CmguiPluginMacros.h"

extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_field.h"
#include "api/cmiss_texture.h"
#include "api/cmiss_scene_viewer.h"
#include "user_interface/message.h"
}

#define wxID_BTN_OK wxID("CmguiPluginCommandlineWidgetOK")
#define wxID_COMBO_CURRENT_COMMAND wxID("CmguiPluginCommandlineWidgetCurrentCommand")
#define wxID_BTN_LOAD wxID("CmguiPluginCommandlineWidgetLoad")


namespace Core{ namespace Widgets {
    class UserHelper;
    class DataEntityListBrowser;
}}

namespace CmguiPlugin {


/**
\brief 
\ingroup CMGUIPlugin
\author Martin Steghöfer
\date Oct 2010
*/
class CommandlineWidget : public wxScrolledWindow, public Core::Widgets::ProcessingWidget
{

public:
    coreDefineBaseWindowFactory1param(CommandlineWidget, Cmgui*)

    //!
    CommandlineWidget(Cmgui* cmgui, wxWindow* parent, int id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);

    //!
    ~CommandlineWidget();

    //!
    bool Enable(bool enable /*= true */);

	//!
	void execute(std::string command);

private:
    DECLARE_EVENT_TABLE()
    
    // ************* methods *************

    //! Update GUI from working data
    void UpdateWidget();

    //! Button has been pressed
    void OnBtnOK(wxCommandEvent& event);
    
	void OnBtnLoad(wxCommandEvent& event);
	
    void OnCurrentCommandTextChange(wxCommandEvent& event);
    
    void OnCurrentCommandEnter(wxCommandEvent& event);

    void do_layout();
    
    void execute();
    
    void message(std::string command, wxTextAttr attributes = wxTextAttr(*wxBLACK));
    
    int printCmguiMessageInformation(char *message);
    int printCmguiMessageWarning(char *message);
    int printCmguiMessageError(char *message);
    void printCmguiMessage(std::string prefix, std::string messageText, wxTextAttr attributes);

    // ************* attributes *************

    wxStaticBox* m_frame;
    wxStaticBox* m_sizer_frame;
    wxComboBox* m_ComboBoxCurrentCommand;
    //wxListBox* m_history;
    wxTextCtrl* m_history;
    wxButton* m_buttonOK;
	wxButton* m_buttonLoad;

    Cmgui *m_cmgui;
    
    int m_historyLines;
    
    static Display_message_function printCmguiMessageInformation;
    static Display_message_function printCmguiMessageWarning;
    static Display_message_function printCmguiMessageError;

};

/*
Display_message_function printCmguiMessageInformation;
Display_message_function printCmguiMessageWarning;
Display_message_function printCmguiMessageError;
*/

} // namespace CmguiPlugin

#endif //_CmguiPluginCommandlineWidget_H

