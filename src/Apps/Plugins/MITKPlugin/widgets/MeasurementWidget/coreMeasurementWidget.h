/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreMeasurementWidget_H
#define _coreMeasurementWidget_H


#include "coreObject.h"
#include <mitkCommon.h>
#include <mitkPointSet.h>
#include "coreSmartPointerMacros.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"
#include "corePointInteractorPointSet.h"
#include "coreSelectionToolWidget.h"

namespace Core
{
namespace Widgets
{

namespace Widgets{ 
	class RenderWindowBase; 
}

#define wxID_MeasurementWidget wxID("wxID_MeasurementWidget")

/**
\brief This class is used by the PluginTab and the ViewToolBar to set the 
currentDataEntityHolder
       and the rendering tree that are needed by the pointset Interactor
\ingroup gmInteractors
\author Chiara Riccobene
27 apr 2009
*/
class PLUGIN_EXPORT MeasurementWidget 
	: public wxScrolledWindow, 
	public SelectionToolWidget
{
public:

	enum INPUT_TYPE {
		INPUT_DATA,
		INPUT_MEASUREMENT
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

	enum INTERACTOR_STATE
	{
		INTERACTOR_DISABLED,
		INTERACTOR_ENABLED
	};

	typedef Core::DataHolder<INTERACTOR_STATE> InteractorStateHolderType;

	//!
	coreDefineBaseWindowFactory( MeasurementWidget );

	//!
	MeasurementWidget(
		wxWindow* parent, 
		int id = wxID_MeasurementWidget,
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=0);

	//!
	~MeasurementWidget( );

	//!
	void OnInit( );

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
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	Core::DataEntityHolder::Pointer GetMeasurementHolder();

	//!
	void SetMeasureName(std::string measureName);

	//!
	void SetMeasureColor(double color[3]);


private:

	//!
	void OnModifiedInput( );

	//!
	void OnStart(wxCommandEvent &event);

	//!
	void UpdateWidget( );

	DECLARE_EVENT_TABLE();

private:

	//!
	Processor::Pointer m_Processor;

	//!
	Core::PointInteractorPointSet::Pointer m_PointInteractor;
	
	//!
	bool m_enable;

	//! State of the interactor to update the views
	InteractorStateHolderType::Pointer m_InteractorStateHolder;

	//!
	std::string m_MeasureName;

	//!
	double m_measureColor[3];

	//!
	wxToggleButton* m_btnStart;

};

}
}// Core

#endif //_coreMeasurementWidget_h

