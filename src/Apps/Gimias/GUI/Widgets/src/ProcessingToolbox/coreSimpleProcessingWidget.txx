/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSimpleProcessingWidget.h"


template <class T>
Core::Widgets::SimpleProcessingWidget<T>::SimpleProcessingWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
		coreSimpleProcessingWidgetUI(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	SetId( wxNewId( ) );
	m_Processor = T::New( );
}

template <class T>
Core::BaseProcessor::Pointer Core::Widgets::SimpleProcessingWidget<T>::GetProcessor( )
{
	return m_Processor.GetPointer( );
}

template <class T>
void Core::Widgets::SimpleProcessingWidget<T>::OnApply(wxCommandEvent &event)
{
	UpdateProcessor( );
}

