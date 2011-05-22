/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef WXMITK_STRINGPROPERTYEDITOR_H_INCLUDED
#define WXMITK_STRINGPROPERTYEDITOR_H_INCLUDED

#include "wxMitkWidgetsWin32Header.h"
#include <mitkPropertyObserver.h>
#include <mitkStringProperty.h>
#include "wx/textctrl.h"

/// @ingroup wxMitkWidgets
class WXMITKWIDGETS_EXPORT wxMitkStringPropertyEditor 
	: public wxTextCtrl, public mitk::PropertyEditor
{
  public:
    
    wxMitkStringPropertyEditor( 
		mitk::StringProperty*, 
		wxWindow* parent, 
		const char* name = 0 );
    virtual ~wxMitkStringPropertyEditor();
      
  protected:
    virtual void PropertyChanged();
    virtual void PropertyRemoved();
    mitk::StringProperty* m_StringProperty;
    void onTextChanged(wxKeyEvent&);

	DECLARE_EVENT_TABLE();
};

#endif

