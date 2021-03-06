// -*- C++ -*- generated by wxGlade 0.6.3 on Wed Oct 13 11:04:52 2010

#include <wx/wx.h>
#include <wx/image.h>

#ifndef MULTILEVELROIPANELWIDGETUI_H
#define MULTILEVELROIPANELWIDGETUI_H

// begin wxGlade: ::dependencies
#include <wx/spinctrl.h>
// end wxGlade

// begin wxGlade: ::extracode
#include "wxMitkColorSelectorControl.h"
#include "wxID.h"

#define wxID_CMB_LEVELS wxID("CardiacManualSegmentationPanelWidgetUICmbLevels")
#define wxID_BTN_PROPAGATE wxID("CardiacManualSegmentationPanelWidgetUIBtnPropagate")
#define wxID_BTN_UPDATEMEASURES wxID("CardiacManualSegmentationPanelWidgetUIBtnUpdateMeasures")
#define wxID_BTN_CREATESIGNAL wxID("CardiacManualSegmentationPanelWidgetUIBtnCreateSignal")
#define wxID_BTN_ROITOMESH wxID("CardiacManualSegmentationPanelWidgetUIBtnROIToMesh")

// end wxGlade


class MultiLevelROIPanelWidgetUI: public wxScrolledWindow {
public:
    // begin wxGlade: MultiLevelROIPanelWidgetUI::ids
    // end wxGlade

    MultiLevelROIPanelWidgetUI(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: MultiLevelROIPanelWidgetUI::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: MultiLevelROIPanelWidgetUI::attributes
    wxStaticBox* sizer_1_staticbox;
    wxStaticBox* sizer_9_staticbox;
    wxButton* m_btnPropagate;
    wxStaticText* label_3;
    wxSpinCtrl* m_spinCtrlLeft;
    wxStaticText* label_8;
    wxSpinCtrl* m_spinCtrlRight;
    wxStaticText* label_4;
    wxTextCtrl* m_measureMean;
    wxStaticText* label_7;
    wxTextCtrl* m_measureSTD;
    wxStaticText* label_5;
    wxTextCtrl* m_measureMin;
    wxStaticText* label_6;
    wxTextCtrl* m_measureMax;
    wxButton* button_1;
    wxStaticText* label_1;
    wxComboBox* m_comboBoxLevels;
    wxButton* button_2;
    wxStaticText* label_2;
    wxRadioButton* m_rb3d;
    wxRadioButton* m_rbX;
    wxRadioButton* m_rbY;
    wxRadioButton* m_rbZ;
    // end wxGlade
}; // wxGlade: end class


#endif // MULTILEVELROIPANELWIDGETUI_H
