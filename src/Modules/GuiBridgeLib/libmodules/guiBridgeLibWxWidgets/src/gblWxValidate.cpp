/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxValidate.h"
#include "wx/wx.h"

bool gbl::wx::SetAppearance(wxWindow* widget, bool valid)
{
	widget->SetBackgroundColour(valid ? wxColour(200, 200, 200) : wxColour(255, 0, 0) );
	widget->Refresh();
	return valid;
}
