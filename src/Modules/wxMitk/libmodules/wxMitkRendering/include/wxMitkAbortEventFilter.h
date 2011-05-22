/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkAbortEventFilter_H
#define wxMitkAbortEventFilter_H

#include "wxMitkRenderingWin32Header.h"
#include <itkLightObject.h>

#include <queue>

class wxEvent;

namespace mitk
{
/*
\brief wxWidgets specific implementation of mitk::RenderWindow.

The wxMitkAbortEventFilter is a singleton class responsible for filtering 
the events of the wxWidgets application unique instance, in order to detect 
when to abort rendering and lower the level of detail of the rendering so 
as to improve user interaction.

\note This static class must be called from your wxApp::FilterEvent() in 
order to tell the application to do it.

The main picture is that, when volume rendering is initiated, the volume 
is first drawn with a low resolution (visually not so nice but very fast). 
While the user interacts with the dataset (or with MITK in general), 
volume rendering remains in this low-res mode. 
When interaction stops, MITK switches successively to higher resolution 
rendering modes, ultimately drawing a high quality version of the volume. 
This takes time though, even seconds for large datasets.

In order to make sure that the application does not block during such a 
long rendering process, VTK provides a mechanism to frequently check for 
an abort condition during rendering. For example, if the users clicks 
into the 3D window during rendering, this will be a signal for the 
rendering to stop its rendering process. VTK uses a callback mechanism 
for this: during rendering, a user-specified method is called n times 
where the user application (and GUI system) can check if an abort condition 
is present.

Using wxWidgets, the options for checking for such a conditions are not 
very straightforward. There is simply no way to check directly whether a 
specific event (e.g. a mouse click into a window) has occurred while 
<b>NOT</b> being in the program’s main wxWidgets event loop (where all 
events are usually processed).

The way to achieve this apparently lies in the creation of an event filter, 
which pre-filters all system events before processing them via the 
wxWidgets event loop. This can be triggered by issuing a ProcessEvents() 
call on the event loop.

\sa mitk::VolumeDataVtkMapper3D::AbortCallback()

\ingroup wxMitkCore
\author Juan Antonio Moya
\date 17 Oct 2007
*/
class WXMITKRENDERING_EXPORT wxMitkAbortEventFilter : public itk::LightObject
{
	// ------------------------------------------------------------
	// Standard class typedefs
	// ------------------------------------------------------------
	typedef wxMitkAbortEventFilter			Self;
	typedef itk::LightObject				Superclass;
	typedef itk::SmartPointer<Self>			Pointer;
	typedef itk::SmartPointer<const Self>	ConstPointer;
	
	itkTypeMacro(Self, Superclass);

public:
	static wxMitkAbortEventFilter* GetInstance(void);

	//! 
	int eventFilter(wxEvent& event);
	
	//!
	void ProcessEvents();

	//!
	void IssueQueuedEvents();

protected:
	wxMitkAbortEventFilter(void);
	virtual ~wxMitkAbortEventFilter(void);

private:
	itkFactorylessNewMacro(Self);
	static wxMitkAbortEventFilter::Pointer filterInstance;

	//!
	bool m_ButtonPressed;

	//!
	bool m_MouseMoved;

	// Renderer of RenderWindow in which mouse button was pressed
	mitk::BaseRenderer *m_LODRendererAtButtonPress;

	//!
	typedef std::pair< wxObject*, wxEvent* > ObjectEventPair;

	//!
	typedef std::queue< ObjectEventPair > EventQueue;

	//!
	EventQueue m_EventQueue;

};

} // namespace mitk


#endif // wxMitkAbortEventFilter_H
