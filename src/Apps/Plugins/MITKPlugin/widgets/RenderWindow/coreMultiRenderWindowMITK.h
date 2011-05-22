/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMultiRenderWindowMITKMITK_H
#define coreMultiRenderWindowMITKMITK_H

#include "gmWidgetsWin32Header.h"
#include "coreMultiRenderWindow.h"
#include "coreRenderingTreeMITK.h"
#include "coreCommonDataTypes.h"
#include "coreMultiRenderWindowFocus.h"
#include "coreMultiRenderWindowOverlay.h"

#include <mitkDataTreeNode.h>

#include "wxMitkMultiRenderWindow.h"

class vtkPolyData;

namespace mitk { class LevelWindowProperty; }

namespace Core
{
namespace Widgets
{

class MultiRenderWindowMITK;

/** 
\brief Refresh all linked views when the camera is modified

\ingroup gmWidgets
\author Xavi Planes
\date 08 Mar 2010
*/
class vtkCameraCallback : public vtkCommand
{
public:

	//!
	vtkCameraCallback(){}
	//!
	static vtkCameraCallback *New()
	{
		return new vtkCameraCallback( );
	}
	//!
	void Execute(vtkObject *caller, unsigned long event, void *vtkNotUsed(callData));
	//!
	MultiRenderWindowMITK* m_MultiRenderWindowMITK;
};

class MultiRenderWindowEventHandler;
/** 
\brief GIMIAS specific implementation of wxMitkMultiRenderWindowMITK.

The MultiRenderWindowMITK is responsible for holding some wxRenderWindow
and manage the layout so as to display many different views over the same data.

In order to update the views each time the configuration of this class changes,
you should use MultiRenderWindowMITKState class.

\ingroup gmWidgets
\author Juan Antonio Moya
\date 17 Oct 2007
*/
class PLUGIN_EXPORT MultiRenderWindowMITK 
	: public mitk::wxMitkMultiRenderWindow, public Core::Widgets::RenderWindowBase
{
public:
	coreDefineBaseWindowFactory( MultiRenderWindowMITK )

	MultiRenderWindowMITK(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("View"));
	virtual ~MultiRenderWindowMITK(void);

	//!
	void Init(
		Core::DataEntityHolder::Pointer selectedDataEntity,
		bool enablePlanes = true);

	void RenderScene(void);

	void SetEnableInitialization( bool bEnableContinuousInitialization );

	//! Enable or disable axis
	void EnableAxis( bool enable );

	//! Redefined
	void SetPlaneRotationLinked(bool enable);

	//!
	void EnableAnnotatedCube(bool enable);

	//!
	void EnableCornerAnnotations(bool enable);

	//!
	void SetCurrentLayout( 
		mitk::LayoutConfiguration layout,
		mitk::wxMitkSelectableGLWidget* widget = NULL );

	//! Enable plane sub tree in the rendering tree
	void EnableDisplayPlaneSubtree( bool bEnable );

	//!
	void EnableWindowLevelInteractor(bool enable = true);

	//! Enable the render windows (optimize speed) and interactors
	bool Enable(bool enable = true );

	//! Redefined
	void GetProperties( blTagMap::Pointer tagMap );

	//! Redefined
	void SetProperties( blTagMap::Pointer tagMap );

	//! Enable/Disable m_MultiRenderWindowMITKEventHandler
	void SetEvtHandlerEnabled(bool enabled);

	//!
	MultiRenderWindowOverlay::Pointer GetMultiRenderWindowOverlay();

	//!
	Core::RenderingTree::Pointer GetPrivateRenderingTree() const;

	//!
	void SaveImage3DWindow( const std::string& completeFileName );

	//!
	void CenterToPoint( double p[3] );

	//!
	std::string GetName( );

	//! Called when a DataEntity has been selected on DataTree
	void OnDataSelected( Core::DataEntity::Pointer dataEntity );

	//! Redefined
	int GetTime( );

	//! Redefined
	void SetTime( int time );

	//! Redefined
	void GetTimeRange( int &min, int &max );

	//!
	void SetTimeStep( int time );

	//! Redefined
	void UpdateCamera3D( Core::Camera3D::Pointer cam );

	//! Redefined
	void UpdateSlicePlane( SlicePlane::Pointer slicePlane );

	//!
	void InitDefaultLayout();

	//! Redefined
	void RequestUpdateAll( );

	//! Redefined
	virtual void GetSlicePlanesPositions( std::list<SlicePlane::Pointer> &slicePlanesPositions );


private:

	//! Change color to disabled or enabled
	void ChangeColorDisplayPlaneSubtree( bool bEnable );

	//!
	void SetRenderingTreeInstance(Core::RenderingTree::Pointer val);

	//!
	void OnModifiedSlicePlane( 
		const itk::Object* o, const itk::EventObject &event );

	//!
	void OnModifiedMetadata( );


private:

	//!
	Core::RenderingTreeMITK::Pointer m_RenderingTree;

	//! Remove it at destructor
	boost::signals::connection	m_RenderingTreeConnection;

	//! 
	MultiRenderWindowFocus::Pointer m_MultiRenderWindowFocus;

	//!
	MultiRenderWindowOverlay::Pointer m_MultiRenderWindowOverlay;

	//!
	MultiRenderWindowEventHandler* m_MultiRenderWindowEventHandler;

	//!
	vtkSmartPointer<vtkCameraCallback> m_vtkCameraCallback;

};


typedef DataHolder<MultiRenderWindowMITK*> MultiRenderWindowMITKHolderType;



}
}

#endif // coreMultiRenderWindowMITKMITK_H
