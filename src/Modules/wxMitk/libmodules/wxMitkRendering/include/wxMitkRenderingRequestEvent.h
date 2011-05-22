/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkRenderingRequestEvent_H
#define wxMitkRenderingRequestEvent_H

#include "wxMitkRenderingWin32Header.h"

namespace mitk
{

DECLARE_EXPORTED_EVENT_TYPE(WXMITKRENDERING_EXPORT, wxEVT_RENDERING_REQUEST, -1)

/**
\brief Event for request a rendering

\ingroup wxMitkRendering
\author Xavi Planes
\date 18 Nov 2009
*/
class WXMITKRENDERING_EXPORT wxMitkRenderingRequestEvent 
	: public wxCommandEvent
{

public:
	wxMitkRenderingRequestEvent(
		int winid = 0, 
		wxEventType commandType = wxEVT_RENDERING_REQUEST);
	virtual ~wxMitkRenderingRequestEvent(void);
	
	// required for sending it with wxPostEvent()
    wxEvent* Clone();

private:

	DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxMitkRenderingRequestEvent)
};


} // namespace mitk


#endif // wxMitkRenderingRequestEvent_H

