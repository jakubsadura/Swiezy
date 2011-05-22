// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "wxMitkIDTest.h"
#include "wxID.h"
#include <wx/defs.h>

void wxMitkIDTest::TestWxMitkID()
{
	int id1 = wxID("Hello");
	int id2 = wxID("Hello");
	int id3 = wxID("Bye");

	TS_ASSERT(wxID_HIGHEST < id1);
	TS_ASSERT_EQUALS(id1, id2);
	TS_ASSERT_DIFFERS(id1, id3);
}
