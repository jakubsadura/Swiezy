/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef WXMITK_BOOLPROPERTYVIEW_H_INCLUDED
#define WXMITK_BOOLPROPERTYVIEW_H_INCLUDED

#include "wxMitkWidgetsWin32Header.h"
#include <mitkPropertyObserver.h>
#include <mitkProperties.h>
#include "wx/checkbox.h"

enum
{
	wxID_Checked = 1,
};

/// @ingroup wxMitkWidgets
class WXMITKWIDGETS_EXPORT wxMitkBoolPropertyView 
	: public wxCheckBox, public mitk::PropertyView
{
  public:
    
    wxMitkBoolPropertyView( 
		const mitk::BoolProperty*, 
		wxWindow* parent, 
		const char* name = 0 );
    virtual ~wxMitkBoolPropertyView();
      
  protected:
    virtual void PropertyChanged();
    virtual void PropertyRemoved();
    const mitk::BoolProperty* m_BoolProperty;
};

#endif

