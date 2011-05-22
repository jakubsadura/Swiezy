/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wx/wx.h>
#include <wx/image.h>

#ifndef COREMETADATAWIDGET_H
#define COREMETADATAWIDGET_H

#include <wx/panel.h>
#include "gmWidgetsWin32Header.h"
// GuiBridgeLib
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"
// Core
#include "coreDataEntity.h"

namespace Core{
namespace Widgets{

/**
\brief Widget for displaying information about metadata of dataentity
        such as EDFlag and ESflag, it also used to edit these flags
\ingroup gmWidgets
\author Chiara Riccobene
\date 06-08-09
*/



class GMWIDGETS_EXPORT MetadataWidget: public wxPanel 
{
public:
   
	//!
    MetadataWidget(wxWindow* parent, 
										int id, 
										const wxPoint& pos=wxDefaultPosition, 
										const wxSize& size=wxDefaultSize, 
										long style=0);

	//! Update the values in the metadata if the text control is changed								
	void UpdateData();
	//! Reset the widget
	void UpdateWidget( bool bResetModality = true);
	//! Set the data entity 
	void SetDataEntity( Core::DataEntity::Pointer data);
	
	
private:
    void set_properties();
    void do_layout();

protected:

	//! variables for the metadata widget
	wxStaticText* m_label_NrOfTimeStep;
	//!
	wxTextCtrl* m_txt_numberTimeSteps;
	//!
	wxStaticText* m_label_EDFlag;
	//! text for End diastole flag
	wxTextCtrl* m_txtCtrl_EDFlag;
	//!
	wxStaticText* m_label_ESFlag;
	//! text for End systole flag
	wxTextCtrl* m_txtCtrl_ESFlag;
	//!
	wxComboBox* m_comboBox_modality; 
	//! text for modality combo box
	wxStaticText* m_label_mod;

	//! observer for some variables
	gbl::wx::ConnectorOfWidgetChangesToSlotFunction m_changeInWidgetObserver;
	
	//! 
	Core::DataEntity::Pointer m_dataEntity;

	//!
	Core::DataEntityHolder::Pointer m_dataEntityHolder;
}; 

} // Widgets
} // Core
#endif // COREMETADATAWIDGET_H
