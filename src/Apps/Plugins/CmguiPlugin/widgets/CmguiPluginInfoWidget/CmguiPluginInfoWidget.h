/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginInfoWidget_H
#define _CmguiPluginInfoWidget_H

#include "wx/treectrl.h"

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
#include "region/cmiss_region.h"
}


namespace Core{ namespace Widgets {
    class UserHelper;
    class DataEntityListBrowser;
}}

namespace CmguiPlugin{


/**
\brief 
\ingroup CMGUIPlugin
\author Martin Steghöfer
\date Oct 2010
*/
class InfoWidget : public wxScrolledWindow, public Core::Widgets::ProcessingWidget
{

public:
    //!
    coreDefineBaseWindowFactory1param(InfoWidget, Cmgui*)
    
    //!
    InfoWidget(Cmgui* cmgui, wxWindow* parent, int id = wxID_ANY, const wxPoint&  pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = 0);

    //!
    ~InfoWidget();

    //!
    bool Enable(bool enable /*= true */);
    
    void OnSelectionChanged(wxTreeEvent& event);
    
    virtual Core::BaseProcessor::Pointer GetProcessor() {
        return NULL;
    }

private:
    DECLARE_EVENT_TABLE()
    
    // ************* methods *************

    //! Update GUI from working data
    void UpdateWidget();

    void do_layout();
    
    void printRegions(struct Cmiss_region *region, int indent = 0, int indentIncrement = 2);
    
    void updateRegions(struct Cmiss_region *region, wxTreeItemId treeItem);

    // ************* attributes *************

    //wxListBox* m_listBoxRegions;
    wxPanel* m_notebookPaneRegions;
    wxTreeCtrl *m_treeRegions;
    wxNotebook* m_notebook;

    
    CmguiPlugin::Cmgui *m_cmgui;
};


} // namespace CmguiPlugin

#endif //_CmguiPluginInfoWidget_H

