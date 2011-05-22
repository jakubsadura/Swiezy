/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreWorkingAreaConfig_H
#define _coreWorkingAreaConfig_H

#include "gmWidgetsWin32Header.h"
#include "coreBaseWindow.h"

namespace mitk 
{ 
	class wxWidgetStackControl;
}

namespace Core
{
namespace Widgets
{

class WorkingAreaManagerWidget;
class RenderWindowConfig;

/** 
\brief Configure current working area and manager

\ingroup gmWidgets
\author Xavi Planes
\date 27 August 2010
*/
class GMWIDGETS_EXPORT WorkingAreaConfig :
	public wxPanel,
	public Core::BaseWindow
{
public:
	//!
	class Factory : public Core::BaseWindowFactory
	{
	public:
		coreDeclareSmartPointerTypesMacro(Factory,BaseWindowFactory)
		coreFactorylessNewMacro(Factory)
		coreClassNameMacro(Factory)
		coreCommonFactoryFunctionsNewBase( WorkingAreaConfig )
		coreCommonFactoryFunctionsCreateWindow( WorkingAreaConfig )
		std::string GetName() const
		{
			// Backwards compatibility
			return "class Core::Widgets::MultiRenderWindowConfig";
		}
	};

	//!
	WorkingAreaConfig(
		wxWindow* parent, 
		int id = wxID_ANY, const wxPoint&  pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = 0);

	//!
	void SetMultiRenderWindow( RenderWindowBase* multiRenderWindow );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	void SetPluginTab(Core::Widgets::PluginTab* val);

protected:
	DECLARE_EVENT_TABLE( );

	void OnBtnSwitch(wxCommandEvent &event);

private:
	//!
	WorkingAreaManagerWidget* m_WorkingAreaManagerWidget;
	//!
	RenderWindowConfig* m_MultiRenderWindowConfig;
	//!
	wxButton* m_btnSwitch;
	//!
	wxWidgetStackControl* m_StackControl; 
};

}
}

#endif // _coreWorkingAreaConfig_H
