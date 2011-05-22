/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVisualProperties_H
#define coreVisualProperties_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataHolder.h"
#include "coreRenderingTree.h"
#include <wx/panel.h>
#include "wx/scrolwin.h"
#include "coreBaseWindow.h"
#include "wxID.h"
#include "coreVisualPropertiesBase.h"

// Forward declarations
class wxButton;
class wxStaticText;
namespace mitk 
{ 
	class wxMitkRenderWindow;
}

namespace Core{
namespace Widgets{

// Forward declarations
class SurfaceMeshMaterialPropertiesWidget;
class VolumeMeshMaterialPropertiesWidget;
class ITKTransformVisualPropertiesWidget;
class VolumeImagePropertiesWidget;
class InputControl;
class DataEntityListBrowser;

#define wxID_VisualProperties wxID("wxID_VisualProperties")

/** 
A widget for changing the appearance of a data entity (a mesh or an image).

\ingroup gmWidgets
\author Juan Antonio Moya
\date 06 Feb 2008
*/
class GMWIDGETS_EXPORT VisualProperties : public wxScrolledWindow, public BaseWindow
{
public:
	//!
	coreDefineBaseWindowFactory( VisualProperties );

	coreClassNameMacro(VisualProperties);
	//!
	VisualProperties(
		wxWindow* parent, 
		wxWindowID id = wxID_VisualProperties, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE, 
		const wxString& name = wxT("VisualProperties"));
	//!
	virtual ~VisualProperties(void);
	
	//!
	void SetListBrowser( Core::Widgets::DataEntityListBrowser* val );

	//! Set the rendering window
	void SetMultiRenderWindow( Core::Widgets::RenderWindowBase *multiRenderWindow );

	//! Show or hide the Appearance suitecase widget
	bool Show( bool show /* = true */ );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	Core::Widgets::VolumeImagePropertiesWidget* GetVolumeImagePropertiesWidget()
	{return m_VolumeImageWidget;};

private:

	//!
	void OnInputRenderingHolderChanged(void);
	//!
	void OnInputDataEntityHolderChanged(void);
	//!
	void OnUnloadDataEntity(Core::DataEntity* dataEntity);
	//! Update the rendering node of the input data if selected
	void SetRenderingTree( RenderingTree::Pointer tree );
	//!
	void SetProperties();
	
	//!
	void DoLayout();

	//!
	void OnModifiedRenderingTree( );

	//!
	void UpdateRegisteredWindows( );

	DECLARE_EVENT_TABLE()

private:
	//!
	wxStaticText* m_EmptyLabel;
	//!
	Core::Widgets::SurfaceMeshMaterialPropertiesWidget* m_MaterialWidget;
	//!
	Core::Widgets::VolumeMeshMaterialPropertiesWidget* m_VolumeMaterialWidget;
	//!
	Core::Widgets::VolumeImagePropertiesWidget* m_VolumeImageWidget;
	//!
	Core::Widgets::ITKTransformVisualPropertiesWidget* m_ITKTransformWidget;

	//! Widget for altering the visualization properties
	wxWidgetStackControl* m_StackControl; 
	//!
	Core::Widgets::InputControl* m_AcquireInput;
	//! Input rendering holder
	Core::DataEntityHolder::Pointer m_InputRenderingHolder;
	//! Input data entity holder
	Core::DataEntityHolder::Pointer m_InputDataEntityHolder;
	//! 
	boost::signals::connection m_RenderingTreeObserver;

	static bool resetTargetAfterResize;

	//! true if a renderWindow as been associated to the widget
	bool m_hasRenderWindowAttached;

	//!
	std::list<std::string> m_RegisteredFactoryNameList;
};

} // Widgets
} // Core

#endif // coreVisualProperties_H
