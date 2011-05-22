/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreRenderWindowContainer_H
#define _coreRenderWindowContainer_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreMultiRenderWindow.h"
#include "coreCommonDataTypes.h"
#include "coreRenderingTreeManager.h"
#include "coreRenderWindowLinker.h"
#include "coreBaseWindow.h"

class vtkPolyData;

namespace Core
{
namespace Widgets
{

/** 
\brief Container of RenderWindowBase.

Each RenderWindowBase has its own rendering tree. 

The layout of this wxPanel is managed using a wxAuiManager, so the user
can move the MultiRenderWindows.

The layout of this wxPanel is managed using a wxAuiManager. By default,
all RenderWindowBase are configured as “Center” position to allow

MultiRenderWindows can be synchronized using the RenderWindowLinker.
When the user moves 3D view or a slice, the event is processed by all
MultiRenderWindows.

\sec Window focus
When the user clicks on a RenderWindowBase, this will be the selected one. 
You need to handle the left mouse click event and call SetFocus( ) on your 
RenderWindowBase implementation or a child. This call will send an 
wxChildFocusEvent to the parent and will be handled by wxAuiManager that 
will set this as the active window.
This class will handle the left mouse click event and will call 
SetActiveWindow( ) for the window that has been activated by wxAuiManager.

\note All children of RenderWindowBase should be derived from wxPanel. If
you derive it from wxScrolledWindow, the SetFocus( ) will not active the window

\ingroup gmWidgets
\author Xavi Planes
\date 25 Feb 2010
*/
class GMWIDGETS_EXPORT RenderWindowContainer : 
	public wxPanel,
	public BaseWindow
{
public:
	class State{
	public:
		State( ){
			m_LinkedWindows = false;
		}

		//! Enable or disable RenderWindowLinker
		bool m_LinkedWindows;
	};
public:
	coreClassNameMacro(RenderWindowContainer);

	RenderWindowContainer(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("RenderWindowContainer"));
	virtual ~RenderWindowContainer(void);

	//!
	RenderWindowBase* GetActiveWindow();

	//!
	RenderWindowBase* GetWindow( int pos );

	//!
	void SetActiveWindow( RenderWindowBase* window );

	//! Position from 0 to size
	void SetActiveWindow( int pos );

	//!
	RenderWindowHolderType::Pointer GetActiveWindowHolder() const;

	//!
	void SetActiveWindowHolder(RenderWindowHolderType::Pointer val);

	//!
	bool Enable( bool enable /* = true */ );

	//!
	void Add( RenderWindowBase*, int layer = -1 );

	//!
	void Destroy( RenderWindowBase* );

	//!
	void SetRenderingTreeManager(RenderingTreeManager::Pointer val);

	//!
	Core::DataHolder<State>::Pointer GetStateHolder() const;

	//!
	void LinkWindows( bool enable );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	int GetNumberOfWindows( );

	//! Redefined
	void GetProperties( blTagMap::Pointer tagMap );

	//! Redefined
	void SetProperties( blTagMap::Pointer tagMap );

	//!
	wxAuiPaneInfo &GetAuiPaneInfo( RenderWindowBase* win );

	//!
	void UpdateAui( );

	//!
	int GetMaxLayer( );

		//! Check if properties is valid and all windows can be created
	bool CheckValidProperties( );

private:
	//!
	void OnWxMouseEvent( wxMouseEvent& event );

	//! Configure the first window as Center and the rest as right
	void UpdateWindowsPanes();

	//!
	void InitializeUsingProperties( );

	//!
	RenderingTreeManager::Pointer GetRenderingTreeManager( );

	//!
	std::string GetPropertyWindowFactory( int i );

	DECLARE_EVENT_TABLE();

private:

	//!
	std::list<RenderWindowBase*> m_MultiRenderWindowList;

	//!
	wxAuiManager m_wxAuiManager;

	//! Generic active window holder for all plugin windows
	RenderWindowHolderType::Pointer m_ActiveWindowHolder;

	//! m_ActiveWindow for this container
	RenderWindowBase* m_ActiveWindow;

	//!
	RenderWindowLinker::Pointer m_MultiRenderWindowLinker;

	//!
	Core::DataHolder<State>::Pointer m_StateHolder;

	/** Properties loaded using XML and will be used to initialize this
	window when Enable(true) is called
	*/
	blTagMap::Pointer m_Properties;
};

}
}

#endif // _coreRenderWindowContainer_H
