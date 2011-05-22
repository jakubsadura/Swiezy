/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef ProcessingWidget_H
#define ProcessingWidget_H

#include <wx/wx.h>
#include <wx/image.h>

#include "coreDataEntityHolder.h"
#include "coreRenderingTree.h"
#include "coreDataEntityListBrowser.h"
#include "coreBaseFilter.h"
#include "coreProcessorOutputObserver.h"
#include "coreProcessorOutputsObserverBuilder.h"
#include "coreUserHelperWidget.h"
#include "coreBaseWindow.h"
#include "coreSelectionToolWidget.h"
#include "coreSelectionToolboxWidget.h"
#include "corePluginTabFactory.h"

namespace Core{
namespace Widgets{

/**
\brief Base widget for processing tools
\ingroup gmWidgets
\author Chiara Riccobene
\date 5 Nov 09
*/

class GMWIDGETS_EXPORT ProcessingWidget: public BaseWindow {
public:
    ProcessingWidget();
	 
	virtual ~ProcessingWidget( );

	//!	
	virtual void UpdateData();

	//!
	virtual void UpdateWidget();

	//!
	void SetSelectionTool( wxWindowID id );

	//!
	void SetSelectionTool( const std::string &caption );

	//!
	template<class T>
	T* GetSelectionToolWidget( const std::string &name )
	{
		if ( GetPluginTab() == NULL )
		{
			return NULL;
		}

		SetSelectionTool( name );

		Core::Widgets::SelectionToolboxWidget* selectionToolboxWidget;
		GetPluginTab()->GetWidget( wxID_SelectionToolboxWidget, selectionToolboxWidget );

		return selectionToolboxWidget->GetTool<T>();
	}

protected:
	//! Call m_Processor->Update()
	void UpdateProcessor( );
	//!
	void SetInfoUserHelperWidget(std::string text);

private:

private:

}; // wxGlade: end class

}
}

#endif // ProcessingWidget_H
