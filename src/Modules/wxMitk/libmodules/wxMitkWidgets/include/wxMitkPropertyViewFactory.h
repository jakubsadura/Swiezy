/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <mitkProperties.h>
#include "wx/window.h"

//! \ingroup wxMitkWidgets
class wxMitkPropertyViewFactory
{
  public:

    enum ViewTypes { vtDEFAULT = 0  };
    enum EditorTypes { etDEFAULT = 0, etALWAYS_EDIT = 1, etON_DEMAND_EDIT = 2 };

    static wxMitkPropertyViewFactory* GetInstance(); // singleton

    /** Views and editors are created via operator new. After creation 
	they belong to the caller of CreateView()/CreateEditor(),
     i.e. the caller has to ensure, that the objects are properly deleted!
	 */
    wxWindow* CreateView  (
		const mitk::BaseProperty* property, 
		unsigned int type = 0, 
		wxWindow* parent = 0, 
		const char* name = 0);

    wxWindow* CreateEditor(
		mitk::BaseProperty* property, 
		unsigned int type = 0, 
		wxWindow* parent = 0, 
		const char* name = 0);

  protected:

    wxMitkPropertyViewFactory();   // hidden, access through GetInstance()
    ~wxMitkPropertyViewFactory();

  private:

};

