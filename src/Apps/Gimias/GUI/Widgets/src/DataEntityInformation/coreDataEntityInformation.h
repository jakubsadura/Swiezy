/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityInformation_H
#define coreDataEntityInformation_H

#include "coreObject.h"
#include "coreDataEntityHolder.h"
#include "coreDataEntityListBrowser.h"
#include "coreMetadataWidget.h"
#include "coreDataInformationWidgetBase.h"
#include <wx/panel.h>

// wxMitk
#include <wxWidgetStackControl.h>
#include "coreNumericDataWidget.h"

// Forward declarations
class wxButton;
class wxStaticText;

namespace Core
{
	class DataEntity;
	class DataEntityList;
}

namespace Core{
namespace Widgets{

#define wxID_DataEntityInformationWidget wxID("wxID_DataEntityInformationWidget")

// Forward declarations
class InputControl;


/** 
\brief class that display the basic information about the data entity selected
\ingroup gmWidgets
\author Chiara Riccobene
\date 03 Jun 2009
*/
class GMWIDGETS_EXPORT DataEntityInformation : 
	public wxScrolledWindow,
	public Core::BaseWindow
{
public:
	//!
	coreDefineBaseWindowFactory( DataEntityInformation );
	
	coreClassNameMacro(DataEntityInformation);
	//!
	void SetListBrowser(DataEntityListBrowser* listBrowser);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	bool Show( bool show /* = true */ );

	DataEntityInformation(wxWindow* parent, 
						wxWindowID id = wxID_DataEntityInformationWidget, 
						const wxPoint& pos = wxDefaultPosition, 
						const wxSize& size = wxDefaultSize, 
						long style = wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE, 
						const wxString& name = wxT("DataEntity Information"));
private:
	virtual ~DataEntityInformation(void);

	//! Add observer to the data entity
	void SetInputHolder(Core::DataEntityHolder::Pointer inputHolder);
	
	/** Switch between the different data entity types in order to rise 
	the right panel widget
	*/
	void OnInputHolderChanged(void);

	//! A slot function for observing the meta data. Whenever the meta data changes, the metadata widget is updated
	void OnChangedTag(
		blTagMap* renamedDataEntity,
		unsigned long);
	
	//!
	void set_properties();
	void do_layout();

	void ClearOldData();

	//!
	void UpdateRegisteredWindows( );

private:
	//!
	wxStaticText* m_EmptyLabel;
	//!
	wxStaticText* m_EmptyLabelMetadata;

	//! Widget for altering the visualization properties 
	wxWidgetStackControl* m_StackControl; 
	//!
	wxWidgetStackControl* m_StackControlMetadata; 
	//!
	Core::Widgets::MetadataWidget* m_MetadataWidget;
	//!
	Core::Widgets::NumericDataWidget* m_NumericDataWidget;
	//!
	Core::Widgets::InputControl* m_AcquireInput;
	//!
	Core::DataEntityHolder::Pointer m_InputHolder;

	//!
	wxStaticText* m_lblDataEntityID;
	//!
	wxTextCtrl* m_txtDataEntityID;
	//!
	wxBoxSizer* m_SizerDataEntityID;

	//!
	std::list<std::string> m_RegisteredFactoryNameList;
};

} // Widgets
} // Core

#endif // coreDataEntityInformation_H
