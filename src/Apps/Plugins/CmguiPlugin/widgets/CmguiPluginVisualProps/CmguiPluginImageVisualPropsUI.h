// -*- C++ -*- generated by wxGlade 0.6.3 on Thu Nov 11 13:16:44 2010

#include <wx/wx.h>
#include <wx/image.h>

#ifndef CMGUIPLUGINIMAGEVISUALPROPSUI_H
#define CMGUIPLUGINIMAGEVISUALPROPSUI_H

// begin wxGlade: ::dependencies
#include <wx/treectrl.h>
#include <wx/tglbtn.h>
// end wxGlade

// begin wxGlade: ::extracode
#include "wxID.h"

#define wxID_TREE wxID( "wxID_TREE" )
#define wxID_BTN_SHOW_ALL wxID( "wxID_BTN_SHOW_ALL" )
#define wxID_BTN_HIDE_ALL wxID( "wxID_BTN_HIDE_ALL" )
#define wxID_BTN_SHOW_SELECTED wxID( "wxID_BTN_SHOW_SELECTED" )



// end wxGlade


class CmguiPluginImageVisualPropsUI: public wxScrolledWindow {
public:
    // begin wxGlade: CmguiPluginImageVisualPropsUI::ids
    // end wxGlade

    CmguiPluginImageVisualPropsUI(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: CmguiPluginImageVisualPropsUI::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: CmguiPluginImageVisualPropsUI::attributes
    wxTreeCtrl* m_Tree;
    wxButton* m_btnShowAll;
    wxButton* m_btnHideAll;
    wxToggleButton* m_btnShowSelected;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnTreeSelChanged(wxTreeEvent &event); // wxGlade: <event_handler>
    virtual void OnShowAll(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnHideAll(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnShowSelected(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // CMGUIPLUGINIMAGEVISUALPROPSUI_H
