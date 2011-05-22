/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef WXMITK_BOOLPROPERTYEDITOR_H_INCLUDED
#define WXMITK_BOOLPROPERTYEDITOR_H_INCLUDED

#include <wxMitkBoolPropertyView.h>

/// @ingroup wxMitkWidgets
class WXMITKWIDGETS_EXPORT wxMitkBoolPropertyEditor 
	: public wxMitkBoolPropertyView
{
  public:
    
    wxMitkBoolPropertyEditor( 
		const mitk::BoolProperty*, 
		wxWindow* parent, 
		const char* name = 0 );
    virtual ~wxMitkBoolPropertyEditor();
      
  protected:
    virtual void PropertyRemoved();
	void onToggle(wxCommandEvent &event); // wxGlade: <event_handler>

DECLARE_EVENT_TABLE()
};

#endif

