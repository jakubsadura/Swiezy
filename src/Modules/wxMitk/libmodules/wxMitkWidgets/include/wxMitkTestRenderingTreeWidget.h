/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkTestRenderingTreeWidget_H
#define wxMitkTestRenderingTreeWidget_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkMultiRenderWindow.h"
#include "blMitkPointSelectInteractor.h"
#include "blMitkPointSetInteractor.h"
#include "boost/function.hpp"

#include "mitkStandaloneDataStorage.h"

class vtkSphereSource;

namespace mitk { 
	class wxMitkMaterialEditorWidget;
	class wxMitkTransferFunctionWidget;
	class wxMitkSphereViewCanvas;
	class wxMitkDataTreeListView;
	class wxMitkMultiRenderWindowConfig;

//! \ingroup wxMitkWidgets
class wxMitkTestRenderingTreeWidgetParams
{
public:
	typedef enum
	{
		INPUT_TYPE_NONE = 0,
		INPUT_TYPE_IMAGE = 1,
		INPUT_TYPE_MESH = 2,
		INPUT_TYPE_VOLUME_MESH = 4
	} INPUT_TYPE;

	typedef enum{
		POINT_INTERACTOR_TYPE_SELECTED_AT_RUNTIME,
		POINT_INTERACTOR_TYPE_SELECT,
		POINT_INTERACTOR_TYPE_SET,
		POINT_INTERACTOR_TYPE_NONE
	} POINT_INTERACTOR_TYPE;

public:
	//! Input data type
	INPUT_TYPE m_inputType;
	
	//! enable multirender window
	bool m_bEnableMultiRenderWindow;

	bool m_bEnableVolumeRendering;

	//! layout configuration for multi render window
	LayoutConfiguration m_layoutConfiguration;

	POINT_INTERACTOR_TYPE m_pointInteractorType;

	//! Path for input data
	std::string m_strDataPath;

public:
	wxMitkTestRenderingTreeWidgetParams( )
	{
		m_inputType = INPUT_TYPE_NONE;
		m_bEnableMultiRenderWindow = true;
		m_bEnableVolumeRendering = true;
		m_layoutConfiguration = Default2x2;
		m_pointInteractorType = POINT_INTERACTOR_TYPE_SELECT;
	}
};


/**
\brief Widget with a rendering tree and some widgets to modify
the properties of the rendering tree. It is used to test the code.
\group wxMitkWidgets
\author Xavi Planes
\date 13-06-08
*/
class WXMITKWIDGETS_EXPORT wxMitkTestRenderingTreeWidget : public wxPanel
{
public:
	//! Type of the call back function that can be installed with SetCallback.
	typedef boost::function<mitk::Interactor::Pointer (mitk::DataTreeNode::Pointer)> CallbackType;

	//!
	wxMitkTestRenderingTreeWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("wxMitkTestRenderingTreeWidget"));

	//!
	~wxMitkTestRenderingTreeWidget( );

	//!
	bool Initialize( wxMitkTestRenderingTreeWidgetParams &params );

	/**
	*/
	void TestTextOutput();

	//! See m_CreatePointInteractorCallback
	void SetCallback(CallbackType val);

public:

	void ModifiedInteractorPoints( );

	/**
	*/
	void CreateInteractor( 
		wxMitkTestRenderingTreeWidgetParams &params );

	/**
	\brief Test the memory leaks produced by creating an instance of
	mitk::wxMitkMultiRenderWindow( )
	
	There's some commented code that instantiates objects of intermediate
	classes involved in the creation of wxMitkMultiRenderWindow.
	*/
	void CreatewxMitkMultiRenderWindow( 
						wxMitkTestRenderingTreeWidgetParams &params );

	/**
	\brief Test a crash when loading an image, add it to the rendering tree
	and closing wxMitkApp because one rendering window was destroyed before
	the renderer and the renderer accesses the destroyed window memory.
	*/
	void CreateRenderingTree( );

	/**
	\brief Create an image node

	If the visibility of the image is switched to off and level window is 
	changed, it crashes.
	 */
	void CreateImageNode( const std::string &inputFilename );

	/**
	\brief Test volume rendering with the transfer function widget.
	
	When OnPaint event is sent to wxMitkRenderWindow, nothing is done. This
	has been fixed.

	If the size of the rendering window is 0, it crahses sometimes. This
	has been fixed.
	*/
	void EnableVolumeRendering( );

	/** 
	\brief If m_sphereViewCanvas is created the LOD is not working properly.
	A new vtkRenderWindow is added to the mitk::RenderingManager. 
	The 3D render window from the wxMitkMultiRenderWindow will not be 
	udpated when calling RequestUpdateAll( REQUEST_UPDATE_3DWINDOWS ). 
	This has been fixed.
	 */
	void CreateAdditional3DView( );

	//! Update m_renderWindow with m_renderingTree
	void UpdateRenderWindowTree( );

	/**
	 */
	void CreateSurfaceNode( const std::string &inputFilename );

	void RemoveSurfaceNode( );

	//!
	void CreateVolumeMeshNode( const std::string &inputFilename );

	//!
	void CreateViewConfiguration( );

	/**
	 */
	void ComposeWindows( );

private:
	
	//! Rendering tree
	//mitk::DataTree::Pointer m_renderingTree;
	mitk::StandaloneDataStorage::Pointer m_dataStorage;

	//! Node of the rendering tree for the loaded image
	mitk::DataTreeNode::Pointer m_ImageNode;

	//! Node of the rendering tree for the loaded surface
	mitk::DataTreeNode::Pointer m_SurfaceNode;

	//! Node of the rendering tree for the loaded surface
	mitk::DataTreeNode::Pointer m_VolumeMeshNode;

	//! Interactor
	mitk::Interactor::Pointer m_blPointSelectInteractor;

	//! Interactor
	blMitkPointSetInteractor::Pointer	m_blPointSetInteractor;

	//! Loaded mesh for interactor. This should be m_SurfaceNode
	vtkPolyData *m_MeshForInteractor;

	//! Point set to interact with
	mitk::PointSet::Pointer m_PointSet;

	//! Node displaying m_PointSet
	mitk::DataTreeNode::Pointer m_PointSetNode;

	//! Render window
	mitk::wxMitkMultiRenderWindow* m_renderWindow;

	//! Widget to change the properties of the volume image
	mitk::wxMitkTransferFunctionWidget	*m_transferFunctionWidget;

	//! Widget to change the properties of the mesh
	mitk::wxMitkMaterialEditorWidget *m_wxMitkMaterialEditorWidget;

	//! Widget to show the properties of the surface mesh
	mitk::wxMitkSphereViewCanvas	*m_sphereViewCanvas;

	//! Data tree list view
	//mitk::wxMitkDataTreeListView	*m_wxMitkDataTreeListView;

	//!
	wxMitkMultiRenderWindowConfig* m_viewConfiguration;

	vtkSphereSource* m_sphere;

	//! This call back function is used to create the interactor at runtime 
	//! (in case of POINT_INTERACTOR_TYPE_SELECTED_AT_RUNTIME).
	CallbackType m_CreatePointInteractorCallback;
};

}


#endif // wxMitkTestRenderingTreeWidget_H
