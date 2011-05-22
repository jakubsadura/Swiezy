/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreWorkingAreaManager_H
#define _coreWorkingAreaManager_H

#include "gmWidgetsWin32Header.h"
#include "coreMultiRenderWindow.h"
#include "coreCommonDataTypes.h"
#include "wxWidgetStackControl.h"
#include "coreBaseWindow.h"
#include "wxID.h"

class wxWindow;

namespace mitk{
	class wxMitkMultiRenderWindowConfig;
}

namespace Core
{
namespace Widgets
{

class AppearanceSuitcase;
class LandmarkSelectorWidget;
class ImageContrastWidget;
class Toolbar;
class MovieToolbar;

#define wxID_WorkingArea wxID("wxID_WorkingArea")

/**
\brief Stores all working areas and manages the active RenderWindowBase Holder

\ingroup gmWidgets
\sa FrontEndPlugin
\author Xavi Planes
\date 26 Feb 2010
*/
class GMWIDGETS_EXPORT WorkingAreaManager : 
	public wxWidgetStackControl,
	public Core::BaseWindow
{
public:

	//!
	coreDefineBaseWindowFactory( WorkingAreaManager );

	//!
	WorkingAreaManager( 
		wxWindow* parent, 
		wxWindowID id = wxID_WorkingArea, const wxPoint&  pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = 0, const wxString&  name = wxPanelNameStr );

	//!
	~WorkingAreaManager( );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//! Add a new Working Area using wxWindow::GetId( )
	void AddWorkingArea( wxWindow* aWorkingArea );

	//! Remove the reference to the working area using wxWindow::GetId( )
	void RemoveWorkingArea( int ID );

	//!
	void SetActiveWorkingArea( int ID );

	//!
	wxWindow* GetWorkingArea( int ID );

	//!
	wxWindow* GetWorkingArea( const wxString &label );

	//!
	int GetActiveWorkingAreaID( );

	//!
	wxWindow* GetActiveWorkingArea( );

	//!
	RenderWindowBase* GetActiveMultiRenderWindow() const;

	//!
	RenderWindowHolderType::Pointer GetActiveMultiRenderWindowHolder() const;

	//!
	blTagMap::Pointer GetActiveWindowMetadata();

	//!
	void AddObserverOnActiveWindow( Core::BaseWindow* baseWindow );

	//!
	void RemoveObserverOnActiveWindow( Core::BaseWindow* baseWindow );

	//!
	bool Enable( bool enable );

	//!
	IntHolderType::Pointer GetActiveWorkingAreaHolder() const;

	//!
	std::map<int, wxWindow*> GetWorkingAreaMap() const;

protected:	

private:
	//! Vector of WorkingAreas
	std::map<int, wxWindow*> m_WorkingAreaMap;

	//! Selected RenderWindowBase
	RenderWindowHolderType::Pointer m_ActiveMultiRenderWindowHolder;

	//! Selected working area to display in the tab
	IntHolderType::Pointer m_ActiveWorkingAreaHolder;
};

} // namespace Widgets
} // namespace Core

#endif // _coreWorkingAreaManager_H
