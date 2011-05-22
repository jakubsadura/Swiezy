// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#pragma once

#include "blCxxTest.h"
#include "wx/string.h"
#include "wx/textctrl.h"
#include "wxCxxTestFrame.h"
#include "wxMitkApp.h"


/**
\brief Tests for 
\ingroup wxMitkTests
\author Xavi Planes
\date 06-06-08

Tests wxWidgets memory leaks found in GlowCode:
-a block @0x375CB90, 48 bytes, malloc called from  wxStringBase::AllocBuffer();  file f:\toolkit\thirdparty_wxwidgets\thirdparty\wxwidgets-2.8\wxwidgets-2.8.6-src\src\common\string.cpp; line 221 + 30 bytes; module wxbase28d_vc_custom.dll; 0x1009915E : lReq:1820
  -call stack during allocation
     caller(1): wxStringBase::AllocBuffer();  file f:\toolkit\thirdparty_wxwidgets\thirdparty\wxwidgets-2.8\wxwidgets-2.8.6-src\src\common\string.cpp; line 221 + 35 bytes; module wxbase28d_vc_custom.dll; 0x10099163
	 ...
     caller(8): wxGetMessageName();  file f:\toolkit\thirdparty_wxwidgets\thirdparty\wxwidgets-2.8\wxwidgets-2.8.6-src\src\msw\window.cpp; line 6517 + 22 bytes; module wxmsw28d_core_vc_custom.dll; 0xF4C2AA

*/
class CoreSettingsTest : public CxxTest::TestSuite, public blCxxTest
{
// OPERATIONS
public:
	void setUp( )
	{
		//blCxxTest::setUp( );
		this->m_MainFrame = new wxCxxTestFrame(NULL, wxID_ANY, wxEmptyString);
	}

	void tearDown( )
	{
		// We cannot use this function because some wxString variables
		// are static
		//blCxxTest::tearDown( );
	}

	void test0( void )
	{
		this->m_MainFrame->StartMainLoop();
	}

// ATTRIBUTES
protected:

	wxCxxTestFrame* m_MainFrame;
};

