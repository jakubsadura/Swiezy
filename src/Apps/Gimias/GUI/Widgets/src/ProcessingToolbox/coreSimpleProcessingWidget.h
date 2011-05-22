/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSimpleProcessingWidget_H
#define _coreSimpleProcessingWidget_H

#include "coreSimpleProcessingWidgetUI.h"
#include "coreProcessingWidget.h"


namespace Core{
namespace Widgets{

/**
\brief Processing widget with apply button that calls UpdateProcessor( )
\ingroup gmWidgets
\author Xavi Planes
\date 27 April 2010
*/
template <class T>
class SimpleProcessingWidget: 
	public coreSimpleProcessingWidgetUI,
	public ProcessingWidget
{
public:
	coreDefineBaseWindowFactory( SimpleProcessingWidget<T> )

	//!
    SimpleProcessingWidget(wxWindow* parent, int id = wxID_ANY, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

	//!
	virtual Core::BaseProcessor::Pointer GetProcessor( );

private:

	//!
	void OnApply(wxCommandEvent &event);

	//!
	typename T::Pointer m_Processor;
};

}
}

#include "coreSimpleProcessingWidget.txx"

#endif // _coreSimpleProcessingWidget_H
