// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreProcessorWidgetsBuilder.h"
#include "coreProcessorInputWidget.h"
#include "coreBaseWindow.h"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

Core::ProcessorWidgetsBuilder::ProcessorWidgetsBuilder( )
{
}

Core::ProcessorWidgetsBuilder::~ProcessorWidgetsBuilder()
{
}

void Core::ProcessorWidgetsBuilder::Init( 
	Core::BaseFilter::Pointer processor, 
	Core::BaseWindow* baseWindow, 
	Core::Widgets::DataEntityListBrowser* localList,
	bool automaticSelection )
{
	if ( processor.IsNull() )
	{
		return;
	}

	wxWindow* window = dynamic_cast<wxWindow*> (baseWindow);
	Core::Widgets::ProcessorInputWidget* inputWidget;

	for ( size_t i = 0 ; i < processor->GetNumberOfInputs() ; i++ )
	{
		// If there's another input port with the same input data,
		// don't use automatic selection
		bool currentAutomaticSelection = automaticSelection;
		Core::DataEntityType type2;
		type2 = processor->GetInputPort( i )->GetDataEntityType( );
		for ( size_t j = 0 ; j < processor->GetNumberOfInputs() ; j++ )
		{
			Core::DataEntityType type1;
			type1 = processor->GetInputPort( j )->GetDataEntityType( );
			if ( type1 == type2 && i != j )
			{
				currentAutomaticSelection = false;
			}
		}

		// Create the widget
		inputWidget = Init( 
			i, processor->GetInputPort( i ), 
			window, localList, currentAutomaticSelection );
		
		// Remove old widget
		Core::Widgets::ProcessorInputWidget* oldWidget;
		oldWidget = baseWindow->GetInputWidget( i );
		baseWindow->SetInputWidget( i, inputWidget );
		if ( oldWidget )
		{
			window->GetSizer()->Detach( oldWidget );
		}
	}


}

Core::Widgets::ProcessorInputWidget* 
Core::ProcessorWidgetsBuilder::Init( 
	int iPos,
	Core::BaseFilterInputPort::Pointer port, 
	wxWindow* parent, 
	Core::Widgets::DataEntityListBrowser* localList,
	bool automaticSelection )
{
	Core::Widgets::ProcessorInputWidget *inputControl;
	inputControl = new Core::Widgets::ProcessorInputWidget(
		parent, 
		wxID_ANY,
		automaticSelection );
	parent->GetSizer( )->Insert(iPos, inputControl, wxSizerFlags( ).Expand().Border() );

	// Disable the control to avoid setting default data entity 
	// at initialization
	inputControl->Enable( false );
	inputControl->SetDataEntityListBrowser( localList );
	inputControl->SetInputPort( port );

	// If the data list contains data entities, this function will 
	// set the input of the processor
	inputControl->Enable( parent->IsEnabled() );

	return inputControl;
}
