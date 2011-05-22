/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wx/wx.h>
#include <wx/image.h>

#ifndef WXMITKSignalINFORMATIONWIDGET_H
#define WXMITKSignalINFORMATIONWIDGET_H

#include "wxMitkWidgetsWin32Header.h"
#include <mitkProperties.h>
#include <wx/panel.h>

#include "vtkSmartPointer.h"
#include "blSignal.h"


namespace mitk
{


/**
\brief Widget for displaying information about Signal dataentity
\group wxMitkWidgets
\author Chiara Riccobene
\date 07-06-09
*/

class WXMITKWIDGETS_EXPORT wxMitkSignalInformationWidget: public wxPanel {
public:
   
    wxMitkSignalInformationWidget(wxWindow* parent, 
										int id, 
										const wxPoint& pos=wxDefaultPosition, 
										const wxSize& size=wxDefaultSize, 
										long style=0);

	/** Display number of Cells, Points , type of data and whatever 
	information of the Signal
	*/
	void UpdateData(blSignal::Pointer SignalInput);
	//!
	void ResetWidget( );

private:
   
    void set_properties();
    void do_layout();
  

protected:

   wxStaticText* label_1;
   wxTextCtrl* m_timesteps;
   wxStaticText* label_2;
   wxTextCtrl* m_values;
   wxStaticText* label_3;
   wxTextCtrl* m_minvalue;
   wxStaticText* label_4;
   wxTextCtrl* m_maxvalue;
}; 

}// mitk
#endif // WXMITKSignalINFORMATIONWIDGET_H
