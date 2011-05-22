/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreLandmarkSelectorWidget_H
#define coreLandmarkSelectorWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreLandmarkSelectorWidgetUI.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"
#include "corePointInteractor.h"
#include "coreVTKPolyDataHolder.h"
#include "corePointInteractorPointSet.h"
#include "coreSelectionToolWidget.h"

#include "blSignal.h"

class vtkStringArray;

#define wxID_LandmarkSelectorWidget wxID("wxID_LandmarkSelectorWidget")

namespace Core
{
namespace Widgets
{

	enum INTERACTOR_STATE
	{
		INTERACTOR_DISABLED,
		INTERACTOR_ENABLED
	};
	class InputControl;
	class PointsTableWidget;
	class RenderWindowBase;
	class DataEntityListBrowser;

/** 
\brief Widget for selecting landmarks

When the user starts interaction, a new point set data entity is created.
If the user unloads the image, the data entity is removed.
If the user stops interaction, the data entity is removed.

\ingroup gmWidgets
\author Xavi Planes
\date 06 April 2009
*/
class PLUGIN_EXPORT LandmarkSelectorWidget 
	: public coreLandmarkSelectorWidgetUI, 
	public SelectionToolWidget
{
	enum INPUT_TYPE {
		INPUT_DATA,
		INPUT_LANDMARKS
	};
	/**
	Empty processor to manager the inputs easily
	*/
	class Processor : public BaseProcessor
	{
	public:
		coreDeclareSmartPointerClassMacro(Processor, Core::BaseProcessor);
		Processor( );
	};

public:
	typedef Core::DataHolder<INTERACTOR_STATE> InteractorStateHolderType;

public:
	//!
	coreDefineBaseWindowFactory( LandmarkSelectorWidget );
	//!
	LandmarkSelectorWidget(
		wxWindow* parent, 
		int id = wxID_LandmarkSelectorWidget, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);
	~LandmarkSelectorWidget(void);

	//!
	void OnInit();

	//!
	void SetMultiRenderWindow(Core::Widgets::RenderWindowBase* val);

	//!
	void SetTimeStepHolder( Core::IntHolderType::Pointer timeStepHolder );

	//!
	void SetListBrowser(Core::Widgets::DataEntityListBrowser* val);

	//!
	bool Enable( bool enable = true );

	//!
	void StartInteractor( );

	//!
	void StopInteraction( );

	//!
	bool IsSelectionEnabled( );

	//!
	boost::any GetData( );

	//! Get DataEntity used to put the selected data
	Core::DataEntity::Pointer GetDataEntity( );

	//! 
	InteractorStateHolderType::Pointer GetInteractorStateHolder() const;

	//!
	Core::PointInteractor::Pointer GetPointInteractor() const;

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
		Core::PointInteractor::INTERACTOR_POINT_TYPE interactorType );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	std::string GetLandmarksName() const;
	void SetLandmarksName(std::string val);

	//!
	Core::vtkPolyDataPtr GetCurrentLandmarks( );

private:

	//@{ 
	/// \name Event handlers

	//! Update table of points
	void OnModifiedInput( );

	//!
	void OnModifiedLandmarks( );

	//!
	void OnNewLandmarksSubject( );

	//!
	void SetTimeStep( int time );

	//!
	void OnTxtCtrlLandmarkName(wxCommandEvent& event);

	//!
	void OnRadBoxInteractorType(wxCommandEvent& event);

	//!
	void OnSelectedLandmark( );

	//!
	void OnNewLandmarks(wxCommandEvent &event);

	//!
	void OnStart(wxCommandEvent &event);

	//@}


	//!
	void UpdateWidgets( );

	//!
	void UpdateData( );

	//!
	vtkStringArray *GetCurrentLandmarksNameArray( );

	//! Build m_InputLandmarksHolder and add it to the list
	void BuildLandmarksDataEntity( bool reuseChild = true );

	//!
	void CreateInteractor( );

	DECLARE_EVENT_TABLE()

private:

	//!
	Processor::Pointer m_Processor;

	//! Points table widget
	Core::Widgets::PointsTableWidget* m_PointsTableWidget;

	//! Connection to m_InputDataHolder
	boost::signals::connection m_InputDataHolderConnection;

	//!
	Core::PointInteractor::Pointer m_PointInteractor;

	//! State of the interactor to update the views
	InteractorStateHolderType::Pointer m_InteractorStateHolder;

	//! variable to set type of interactor
	Core::PointInteractor::INTERACTOR_POINT_TYPE m_interactorType;

	//!
	std::string m_LandmarksName;
};

} // namespace Widgets
} // namespace Core

#endif // coreLandmarkSelectorWidget_H
