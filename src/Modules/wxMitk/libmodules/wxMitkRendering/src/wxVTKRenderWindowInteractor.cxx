/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: wxVTKRenderWindowInteractor.cxx,v $
  Language:  C++
  Date:      $Date: 2008/04/14 15:12:00 $
  Version:   $Revision: 1.36 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <assert.h>

#include "wxVTKRenderWindowInteractor.h"

//This is needed for vtk 3.1 :
#ifndef VTK_MAJOR_VERSION
#  include "vtkVersion.h"
#endif

#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
#  include "vtkCommand.h"
#endif
#include "vtkDebugLeaks.h"

#ifdef __WXMAC__
#include "vtkCarbonRenderWindow.h"
#endif

#define ID_wxVTKRenderWindowInteractor_TIMER 1001

IMPLEMENT_DYNAMIC_CLASS(wxVTKRenderWindowInteractor, wxEvtHandler)

//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(wxVTKRenderWindowInteractor, wxEvtHandler)
	EVT_TIMER       (ID_wxVTKRenderWindowInteractor_TIMER, wxVTKRenderWindowInteractor::OnTimer)
END_EVENT_TABLE()

vtkCxxRevisionMacro(wxVTKRenderWindowInteractor, "$Revision: 1.36 $")
vtkInstantiatorNewMacro(wxVTKRenderWindowInteractor)


//---------------------------------------------------------------------------
wxVTKRenderWindowInteractor::wxVTKRenderWindowInteractor() : vtkRenderWindowInteractor()
      , timer(this, ID_wxVTKRenderWindowInteractor_TIMER)
{
#ifdef VTK_DEBUG_LEAKS
  vtkDebugLeaks::ConstructClass("wxVTKRenderWindowInteractor");
#endif
  this->RenderWindow = NULL;
  this->SetRenderWindow(vtkRenderWindow::New());
  this->RenderWindow->Delete();
}

//---------------------------------------------------------------------------
wxVTKRenderWindowInteractor::~wxVTKRenderWindowInteractor()
{
  SetRenderWindow(NULL);
  SetInteractorStyle(NULL);
}
//---------------------------------------------------------------------------
wxVTKRenderWindowInteractor * wxVTKRenderWindowInteractor::New()
{
  // we don't make use of the objectfactory, because we're not registered
  return new wxVTKRenderWindowInteractor;
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::Initialize()
{
  int *size = RenderWindow->GetSize();
  // enable everything and start rendering
  Enable();
  //RenderWindow->Start();

  // set the size in the render window interactor
  Size[0] = size[0];
  Size[1] = size[1];

  // this is initialized
  Initialized = 1;
}

//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::Start()
{
  // the interactor cannot control the event loop
  vtkErrorMacro( << "wxVTKRenderWindowInteractor::Start() "
    "interactor cannot control event loop.");
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::UpdateSize(int x, int y)
{
  if( RenderWindow )
  {
    // When x or y is 0, it crashes on Linux
    if ( x == 0 )
    {
        x = 20;
    }
    if ( y == 0 )
    {
        y = 20;
    }

    // if the size changed tell render window
    if ( x != Size[0] || y != Size[1] )
    {
      // adjust our (vtkRenderWindowInteractor size)
      Size[0] = x;
      Size[1] = y;
      // and our RenderWindow's size
      RenderWindow->SetSize(x, y);
    }
  }
}
//---------------------------------------------------------------------------
int wxVTKRenderWindowInteractor::CreateTimer(int WXUNUSED(timertype))
{
  // it's a one shot timer
  if (!timer.Start(10, TRUE))
    assert(false);

  return 1;
  
}
//---------------------------------------------------------------------------
int wxVTKRenderWindowInteractor::DestroyTimer()
{
  // do nothing
  return 1;
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnTimer(wxTimerEvent& WXUNUSED(event))
{
  if (!Enabled)
    return;
    
#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 0)
    // new style
    InvokeEvent(vtkCommand::TimerEvent, NULL);
#else
    // old style
    InteractorStyle->OnTimer();
#endif
}

void wxVTKRenderWindowInteractor::TerminateApp()
{

}

