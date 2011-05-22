/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef WXMITK_STRINGPROPERTYVIEW_H_INCLUDED
#define WXMITK_STRINGPROPERTYVIEW_H_INCLUDED

#include "wxMitkWidgetsWin32Header.h"
#include <mitkPropertyObserver.h>
#include <mitkStringProperty.h>
#include "wx/stattext.h"

/// @ingroup wxMitkWidgets
class WXMITKWIDGETS_EXPORT wxMitkStringPropertyView 
	: public wxStaticText, 
	  public mitk::PropertyView
{
  public:
    
    wxMitkStringPropertyView( 
		const mitk::StringProperty*, 
		wxWindow* parent, 
		const char* name = 0 );
    virtual ~wxMitkStringPropertyView();
      
  protected:

    virtual void PropertyChanged();
    virtual void PropertyRemoved();

    const mitk::StringProperty* m_StringProperty;
  
  private:
};

#endif

