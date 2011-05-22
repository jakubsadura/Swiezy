/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSurfaceSelectorWidget_H
#define coreSurfaceSelectorWidget_H

#include "corePluginMacros.h"
#include "coreSurfaceSelectorWidgetUI.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"
#include "coreVTKPolyDataHolder.h"
#include "coreSurfaceInteractor.h"
#include "coreSurfaceInteractorProcessor.h"
#include "coreSelectionToolWidget.h"


#define wxID_SurfaceSelectorWidget wxID("wxID_SurfaceSelectorWidget")

namespace Core
{
namespace Widgets
{

	class InputControl;
	class RenderWindowBase;
	class DataEntityListBrowser;

/** 
\brief Widget for selecting Surfaces

When the user starts interaction, a region on a surface is created.
If the user unloads the surface, the data entity is removed.
If the user stops interaction, the data entity is removed.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 13 April 2010
*/
class PLUGIN_EXPORT SurfaceSelectorWidget 
	: public coreSurfaceSelectorWidgetUI, 
	public SelectionToolWidget
{
public:

	enum INTERACTOR_STATE
	{
		INTERACTOR_DISABLED,
		INTERACTOR_ENABLED
	};
	typedef Core::DataHolder<INTERACTOR_STATE> InteractorStateHolderType;

public:
	coreDefineBaseWindowFactory( SurfaceSelectorWidget )

	SurfaceSelectorWidget(
		wxWindow* parent, 
		int id = wxID_SurfaceSelectorWidget, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);
	~SurfaceSelectorWidget(void);

	//!
	void SetListBrowser(Core::Widgets::DataEntityListBrowser* val);

	//!
	bool Enable( bool enable = true );

	//!
	void StartInteractor( );

	//!
	void StopInteraction( );

	//!
	virtual bool IsSelectionEnabled( );

	//! 
	InteractorStateHolderType::Pointer GetInteractorStateHolder() const;

	//! 
	void SetAllowedInputDataTypes( Core::DataEntityType val );

	//! 
	void SetDefaultAllowedInputDataTypes( );

	//!
	void SetInputDataEntity( Core::DataEntity::Pointer val );

	//!
	Core::DataEntity::Pointer GetInputDataEntity( );

	//!
	void SetInteractorType( 
		Core::SurfaceInteractor::INTERACTOR_TYPE interactorType );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:

	//@{ 
	/// \name Event handlers

	//! Update table of points
	void OnModifiedInput( );

	//!
	void OnModifiedInteractorState( );

	//!
	void SetTimeStep( int time );

	//!
	void OnRadBoxInteractorType(wxCommandEvent& event);
	//!
	void OnRemoveCells(wxCommandEvent& event);
	void OnLocalRefiner(wxCommandEvent& event);
	void OnLoopSubdivision(wxCommandEvent& event);
	void OnEdgeSwap(wxCommandEvent& event);
	void OnTaubinSmooth(wxCommandEvent& event);
	void OnEnableSelection(wxCommandEvent &event);
	void OnCheckBoxSelect(wxCommandEvent &event);
	void OnCheckBoxUnselect(wxCommandEvent &event);
	void OnBrushSizeChangedEvent(wxCommandEvent &event);
	void OnBrushSizeChanged(wxSpinEvent &event);

	//!
	void OnSelectedSurface( );

	//@}


	//!
	void UpdateWidgets( );

	//!
	void UpdateData( );

	//!
	void CreateInteractor( );

	//!
	void OnModifiedOutputDataEntity();

	//!
	void CreateSphere( 
		Core::DataEntity::Pointer dataEntity );

	DECLARE_EVENT_TABLE()

private:

	//! Input data
	Core::DataEntityHolder::Pointer m_InputDataHolder;

	//!
	Core::Widgets::InputControl *m_AcquireInputWidget;

	//! Connection to m_InputDataHolder
	boost::signals::connection m_InputDataHolderConnection;

	//! State of the interactor to update the views
	InteractorStateHolderType::Pointer m_InteractorStateHolder;

	//!
	Core::DataEntityHolder::Pointer m_InputSurfacesHolder;

	//! variable to set type of interactor
	Core::SurfaceInteractor::INTERACTOR_TYPE m_interactorType;

	Core::SurfaceInteractor::Pointer m_SurfaceInteractor;

	//! brief Selected point of the mesh. If no point is selected, it's empty.
	Core::DataEntityHolder::Pointer m_selectedPoint;
	//! brief Selected point of the mesh. If no point is selected, it's empty.
	Core::DataEntityHolder::Pointer m_selectionSphereHolder;

	bool m_removeArray;

	Core::SurfaceInteractorProcessor::Pointer m_Processor;

	double radius;

	//!
	std::string m_PreviousSelectedScalar;

};

} // namespace Widgets
} // namespace Core

#endif // coreSurfaceSelectorWidget_H
