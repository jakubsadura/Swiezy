/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginCommandlineWidget.h"
#include "coreDirectory.h"
#include "coreDataContainer.h"
#include "coreKernel.h"

// STD
#include <limits>

// Cmgui
extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_field_image.h"
#include "api/cmiss_field.h"
#include "api/cmiss_field_composite.h"
#include "api/cmiss_region.h"
#include "api/cmiss_field.h"
#include "api/cmiss_scene_viewer.h"
#include "user_interface/message.h"
}

#include <cstdio>

#include <boost/algorithm/string.hpp>


using namespace CmguiPlugin;


// Event the widget
BEGIN_EVENT_TABLE(CommandlineWidget, wxScrolledWindow)
    EVT_BUTTON(wxID_BTN_OK, CommandlineWidget::OnBtnOK)
	EVT_BUTTON(wxID_BTN_LOAD, CommandlineWidget::OnBtnLoad)
    EVT_TEXT(wxID_COMBO_CURRENT_COMMAND, CommandlineWidget::OnCurrentCommandTextChange)
    EVT_TEXT_ENTER(wxID_COMBO_CURRENT_COMMAND, CommandlineWidget::OnCurrentCommandEnter)
END_EVENT_TABLE()

CommandlineWidget::CommandlineWidget(Cmgui* cmgui, wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: wxScrolledWindow(parent, id, pos, size, style)
{
    m_cmgui = cmgui;
    
    m_frame = new wxStaticBox(this, -1, wxT("CMGUI Commandline"));
    
    const wxString *m_ComboBoxSelection_choices = NULL;
    m_ComboBoxCurrentCommand = new wxComboBox(this, wxID_COMBO_CURRENT_COMMAND, wxT(""), wxDefaultPosition, wxDefaultSize, 0, m_ComboBoxSelection_choices, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    
    //const wxString *list_box_1_choices = NULL;
    //m_history = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, list_box_1_choices, 0);
    m_history = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
    
    m_buttonOK = new wxButton(this, wxID_BTN_OK, wxT("OK"));
	m_buttonLoad = new wxButton(this, wxID_BTN_LOAD, wxT("Load"));
    
    do_layout();
    
    wxString name = wxT("CMGUI Command-Line Interface");
    SetLabel(name);
    SetName(name);
    
    m_historyLines = 0;
    
    set_display_message_function(INFORMATION_MESSAGE, printCmguiMessageInformation, this);
    set_display_message_function(WARNING_MESSAGE, printCmguiMessageWarning, this);
    set_display_message_function(ERROR_MESSAGE, printCmguiMessageError, this);
}

CommandlineWidget::~CommandlineWidget()
{
    // We don't need to destroy anything because all the child windows
    // of this wxWindow are destroyed automatically
	set_display_message_function(INFORMATION_MESSAGE, NULL, NULL);
	set_display_message_function(WARNING_MESSAGE, NULL, NULL);
	set_display_message_function(ERROR_MESSAGE, NULL, NULL);
}

void CommandlineWidget::do_layout()
{
    // vertical layout
    wxStaticBoxSizer* sizer = new wxStaticBoxSizer(m_frame, wxVERTICAL);
    sizer->Add(m_history, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    
        // horizontal layout
        wxBoxSizer* sizerCommand = new wxBoxSizer(wxHORIZONTAL);
        sizerCommand->Add(m_ComboBoxCurrentCommand, 1, wxEXPAND, 0);
        sizerCommand->Add(m_buttonOK, 0, wxALIGN_CENTER, 0);
    sizer->Add(sizerCommand, 0, wxEXPAND, 0);
	sizer->Add(m_buttonLoad, 0, wxEXPAND, 0);
    
    // ready
    SetSizer(sizer);
    sizer->Fit(this);
    
    // save history
    //Core::Runtime::Settings::Pointer settings;
    //settings = Core::Runtime::Kernel::GetApplicationSettings();
    //Core::IO::PluginConfigDatabase *pluginConfiguration = settings->GetPluginConfiguration("cmgui");
}


void CommandlineWidget::UpdateWidget() {
}


void CommandlineWidget::OnBtnOK(wxCommandEvent& event) {
    execute();
}

void CommandlineWidget::OnCurrentCommandEnter(wxCommandEvent& event) {
    execute();
}

void CommandlineWidget::OnCurrentCommandTextChange(wxCommandEvent& event) {
    // nothing yet - later we can implement auto-complete functionality here
}

int CommandlineWidget::printCmguiMessageInformation(char *messageText) {
    printCmguiMessage("Information", messageText, wxTextAttr(*wxBLUE));
    return 0;
}

int CommandlineWidget::printCmguiMessageWarning(char *messageText) {
    printCmguiMessage("Warning", messageText, wxTextAttr(*wxRED));
    return 0;
}

int CommandlineWidget::printCmguiMessageError(char *messageText) {
    printCmguiMessage("Error", messageText, wxTextAttr(*wxRED));
    return 0;
}

void CommandlineWidget::printCmguiMessage(std::string prefix, std::string messageText, wxTextAttr attributes) {
    boost::trim(messageText);
    //std::cout << "??? " << messageText << std::endl;
    message(std::string("[") + prefix + std::string("] ") + messageText, attributes);
}

void CommandlineWidget::execute() {
    // get the command
    wxString command = m_ComboBoxCurrentCommand->GetValue();
    
    if (!command.IsEmpty()) {
        // remember the command in the combo box (so you can redo it with the arrow-down key)
        m_ComboBoxCurrentCommand->Insert(command, 0);
        
        // write the command to the console output and execute it
        execute(std::string(command));
        
        // clear the input
        m_ComboBoxCurrentCommand->SetValue(wxEmptyString);
    }
}


bool CommandlineWidget::Enable(bool enable)
{
    bool bReturn = wxScrolledWindow::Enable(enable);
    
    // If this panel widget is selected -> Update the widget
    if (enable) {
        UpdateWidget();
    }
    
    return bReturn;
}

void CommandlineWidget::execute(std::string command) 
{
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
	Core::DataEntity::Pointer dataEntity = list->GetSelectedDataEntity();

	if ( dataEntity.IsNotNull() && !command.empty() )
	{
		blTextUtils::StrSub( command, "${region}", "\"" + dataEntity->GetMetadata()->GetName() + "\"" );
	}
	else
	{
		blTextUtils::StrSub( command, "${region}", "" );
	}
	if ( GetRenderingTree().IsNotNull() && !command.empty() )
	{
		blTextUtils::StrSub( command, "${scene}", GetRenderingTree()->GetName( ) );
	}
	else
	{
		blTextUtils::StrSub( command, "${scene}", "" );
	}

	message(command, wxTextAttr(*wxBLACK));
	m_cmgui->executeCommand( command );
}

void CommandlineWidget::message(std::string command, wxTextAttr attributes) {
    m_history->SetDefaultStyle(attributes);
    if (m_historyLines == 0) {
        m_history->AppendText(command);
    } else {
        m_history->AppendText("\n" + command);
    }
    m_historyLines++;
}

int CommandlineWidget::printCmguiMessageInformation(char *message, void *data) {
    return ((CommandlineWidget*)data)->printCmguiMessageInformation(message);
}

int CommandlineWidget::printCmguiMessageWarning(char *message, void *data) {
    return ((CommandlineWidget*)data)->printCmguiMessageWarning(message);
}

int CommandlineWidget::printCmguiMessageError(char *message, void *data) {
    return ((CommandlineWidget*)data)->printCmguiMessageError(message);
}


void CmguiPlugin::CommandlineWidget::OnBtnLoad( wxCommandEvent& event )
{
	std::string dataPath;
	wxFileDialog* openFileDialog = 
		new wxFileDialog(this, wxT("Open a Command file"), wxT(""), wxT(""), wxT(""), 
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxMULTIPLE );
	if(openFileDialog->ShowModal() == wxID_OK)
	{


		wxArrayString wxFilenames;
		openFileDialog->GetFilenames( wxFilenames );
		dataPath = openFileDialog->GetDirectory().c_str();
		
		std::list<std::string> filenames;
		for ( unsigned i = 0 ; i < wxFilenames.size() ; i++ )
		{
			filenames.push_back( dataPath + Core::IO::SlashChar + 
				wxFilenames[ i ].c_str() );
		}

		BOOST_FOREACH(std::string filename, filenames) {

			std::ifstream file(filename.c_str(), std::ifstream::in);
			while (file.good()) 
			{
				std::string command;
				getline(file, command);
				execute( command );
			}
		}
	}

	openFileDialog->Destroy();

}
