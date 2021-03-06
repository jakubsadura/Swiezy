// -*- C++ -*- generated by wxGlade 0.6.3 on Fri Oct 01 11:22:09 2010

#include <wx/wx.h>
#include <wx/image.h>

#ifndef COREWORKFLOWNAVIGATIONWIDGETUI_H
#define COREWORKFLOWNAVIGATIONWIDGETUI_H

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode
#include "wxID.h"
#define wxID_BACK wxID( "wxID_BACK" )
#define wxID_ADD wxID("wxID_ADD")
#define wxID_NEXT wxID("wxID_NEXT")


// end wxGlade


class coreWorkflowNavigationWidgetUI: public wxScrolledWindow {
public:
    // begin wxGlade: coreWorkflowNavigationWidgetUI::ids
    // end wxGlade

    coreWorkflowNavigationWidgetUI(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: coreWorkflowNavigationWidgetUI::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: coreWorkflowNavigationWidgetUI::attributes
    wxStaticBox* sizer_3_staticbox;
    wxBitmapButton* m_bmpBack;
    wxBitmapButton* m_bmpAdd;
    wxBitmapButton* m_bmpNext;
    wxStaticText* m_lblCaption;
    wxPanel* m_EmptyPanel;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnBack(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnAdd(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnNext(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // COREWORKFLOWNAVIGATIONWIDGETUI_H
