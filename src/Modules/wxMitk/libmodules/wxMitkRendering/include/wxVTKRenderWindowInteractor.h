/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _wxVTKRenderWindowInteractor_h_
#define _wxVTKRenderWindowInteractor_h_

#include "wxMitkRenderingWin32Header.h"

// vtk includes
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"


class WXMITKRENDERING_EXPORT wxVTKRenderWindowInteractor : public wxEvtHandler, public vtkRenderWindowInteractor
{
	DECLARE_DYNAMIC_CLASS(wxVTKRenderWindowInteractor)

  public:
    static wxVTKRenderWindowInteractor * New();
	vtkTypeRevisionMacro(wxVTKRenderWindowInteractor,vtkRenderWindowInteractor);

	  //destructor
    ~wxVTKRenderWindowInteractor();

    // vtkRenderWindowInteractor overrides
    void Initialize();
    void Start();
    void UpdateSize(int x, int y);
    int CreateTimer(int timertype);
    int DestroyTimer();
    void TerminateApp();

    // event handlers
    void OnTimer(wxTimerEvent &event);

  private:
    wxTimer timer;
    
  private:

	//constructors
	wxVTKRenderWindowInteractor();

	DECLARE_EVENT_TABLE()
};

#endif //_wxVTKRenderWindowInteractor_h_
