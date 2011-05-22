/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef COREMOVIETOOLBAR_H
#define COREMOVIETOOLBAR_H

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
// end wxGlade

#include "itkCommand.h"

#include "gmWidgetsWin32Header.h"
#include "coreCommonDataTypes.h"
#include <wx/spinctrl.h>
#include "coreMovieToolbarUI.h"
#include "coreBaseWindow.h"

#define wxID_MovieSlider	wxID("SliderScrolled")
#define wxID_PlayButton		wxID("PlayButton")
#define wxID_StopButton		wxID("StopButton")
#define wxID_PauseButton	wxID("PauseButton")

namespace Core
{
namespace Widgets
{

#define wxID_MovieToolbar wxID("wxID_MovieToolbar")

// begin wxGlade: ::extracode
// end wxGlade
/**
\brief A widget for managing 3D+t data. 
\ingroup gmWidgets
\author Martin Bianculli
\details This widget allows to play, pause and stop a 3D+t data. It doesn't allow to create a video.
*/
class GMWIDGETS_EXPORT  MovieToolbar: public MovieToolbarUI, public Core::BaseWindow{

public:
	//!
	coreDefineBaseWindowFactory( MovieToolbar );

	typedef MovieToolbar Self; 
	enum MovieMode {STOPPED = 0, PLAYING = 1, PAUSE = 2};

    // begin wxGlade: MovieToolbarUI::ids
    // end wxGlade
    MovieToolbar(
		wxWindow* parent, 
		int id = wxID_MovieToolbar, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=0);

	//! Destructor
	~MovieToolbar();

	//! Returns the movie slider
	wxSlider* GetSlider();

	//! This function returns the current time stem (the one shown)
	Core::IntHolderType::Pointer GetCurrentTimeStep();

	//! Redefined
	void SetRenderingTree( RenderingTree::Pointer tree );

	//! Set the 3D+t data to the slider position
	virtual void OnMovieSlider(wxScrollEvent &event);
	//! Pause playing 3D+t data
	virtual void OnPauseButton(wxCommandEvent &event);
	//! Start playing 3D+t data
	virtual void OnPlayButton(wxCommandEvent &event);
	//! Stop playing 3D+t data
	virtual void OnStopButton(wxCommandEvent &event);
	
	//! Process the spin box commands
	void OnFrameRateSlider(wxScrollEvent &event);

private:
	//! Stops the video
	void Stop();

	//! The callback function which observes the mitk::Stepper
	void OnRenderingTreeChanged();

	//! Displays next phase of the 3D+t data
	void OnTimer(wxTimerEvent& event);

	//! The callback function which observes the m_CurrentTimeStep
	void OnModifiedTimeStepHolder( );

	//! Prints the fame number on the TextCtrl Edit Box
	void UpdateWidget();

private:
	//! milliseconds
	unsigned int m_FrameRate; 
	//! Stores the movie current state
	MovieMode m_MovieMode;
	//! For displaying 3D+t data
	wxTimer* m_Timer;
	//!
	Core::IntHolderType::Pointer m_CurrentTimeStep;

	DECLARE_EVENT_TABLE()


}; // wxGlade: end class


} // namespace Widget
} // namespace Core

#endif // COREMOVIETOOLBAR_H
