// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef wxMitkScrolledWindow_H
#define wxMitkScrolledWindow_H

#include "wx/app.h"

/**
\brief Test wxScrolledWindow problem when inserting controls with
proportion > 0
\group wxMitk
\author Xavi Planes
\date 13-05-08
*/
class wxMitkScrolledWindow : public wxApp
{
public:
	virtual int OnRun(void);

private:
	DECLARE_EVENT_TABLE();
};

DECLARE_APP(wxMitkScrolledWindow)

#endif // wxMitkScrolledWindow_H
