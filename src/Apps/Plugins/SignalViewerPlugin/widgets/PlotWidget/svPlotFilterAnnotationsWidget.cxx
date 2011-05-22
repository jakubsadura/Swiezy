/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svPlotFilterAnnotationsWidget.h"
#include "coreReportExceptionMacros.h"

#include "blSignalEvent.h"

class wxTreeItemAnnotation : public wxTreeItemData
{
public:
};


svPlotFilterAnnotationsWidget::svPlotFilterAnnotationsWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
svPlotFilterAnnotationsWidgetUI(parent, id, pos, size, style)
{
	SetName( "FilterAnnotationsWidget" );

	InitTree( );
}


/**
*/
svPlotFilterAnnotationsWidget::~svPlotFilterAnnotationsWidget()
{
}

void svPlotFilterAnnotationsWidget::InitTree()
{
	wxTreeItemId rootItemId = m_TreeCtrl->AddRoot( "Root" );
	blSignalEventTable::LoadAllEventTables( );

	for ( int i = 0 ; i < blSignalEventTable::GetNumberOfGroups( ) ; i++ )
	{
		blSignalEventGroup group;
		group = blSignalEventTable::GetGroup( i );
		wxTreeItemId groupItemID = m_TreeCtrl->AppendItem( rootItemId, group.m_Description );

		std::vector<blSignalEvent> eventTable;
		eventTable = blSignalEventTable::GetGroupEvents( i );
		for ( int j = 0 ; j < eventTable.size() ; j++ )
		{
			m_TreeCtrl->AppendItem( groupItemID, eventTable[ j ].m_Description );
		}

	}

	//blSignalEventTable::SelectGroup( 0, false );
}
