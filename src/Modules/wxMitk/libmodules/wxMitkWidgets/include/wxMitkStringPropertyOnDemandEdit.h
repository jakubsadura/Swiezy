/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef WXMITK_STRINGPROPERTYONDEMANDDEDITOR_H_INCLUDED
#define WXMITK_STRINGPROPERTYONDEMANDDEDITOR_H_INCLUDED

#include "wxMitkWidgetsWin32Header.h"
#include <mitkPropertyObserver.h>
#include <mitkStringProperty.h>
#include "wx/stattext.h"
#include "wx/frame.h"

/// @ingroup wxMitkWidgets
class WXMITKWIDGETS_EXPORT wxMitkStringPropertyOnDemandEdit 
	: public wxFrame, public mitk::PropertyEditor
{
  public:
    
    wxMitkStringPropertyOnDemandEdit( 
		mitk::StringProperty*, 
		wxWindow* parent, 
		const char* name = 0 );
    virtual ~wxMitkStringPropertyOnDemandEdit();
  protected:

    virtual void PropertyChanged();
    virtual void PropertyRemoved();
    
    mitk::StringProperty* m_StringProperty;

    wxStaticText* m_label;
    wxStaticText* m_toolbutton;
    void onToolButtonClicked(wxMouseEvent& event);

	DECLARE_EVENT_TABLE();
};

#endif
