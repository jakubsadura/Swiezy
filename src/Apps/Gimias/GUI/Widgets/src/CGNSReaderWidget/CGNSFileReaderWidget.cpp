/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CGNSFileReaderWidget.h"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::CGNSFileReaderWidget, CGNSFileReaderWidgetUI)
	EVT_CHECKBOX(wxID_COMP, CGNSFileReaderWidget::CompressionOrNot)
END_EVENT_TABLE();

void Core::Widgets::CGNSFileReaderWidget::SetProperties( blTagMap::Pointer tagMap )
{
	try 
	{
		m_tagMap = tagMap;
		//for each tag...

		blTagMap::Iterator it;

		for ( it = tagMap->GetIteratorBegin(); it != tagMap->GetIteratorEnd(); ++it )
		{
			blTag::Pointer tag = tagMap->GetTag(it);

			//create a checkbox
			int idWx = 101+checkBoxVector.size();
			std::string checkBoxName = tag->GetName();

			if (checkBoxName == "UseCompression" ) continue;

			// Try to get the int value
			int tagValue;
			if ( !tag->GetValue( tagValue ) )
			{
				continue;
			}

			if (checkBoxName[checkBoxName.size()-1]=='X')
			{
				checkBoxName = checkBoxName.substr(0,checkBoxName.size()-1) + " vector";
			}

			wxCheckBox* checkbox = new wxCheckBox( this, idWx, wxT(checkBoxName) );
			checkbox->SetValue( true );
			checkBoxVector.push_back(checkbox);
			namesVector.push_back(tag->GetName());
			originalValues.push_back( tagValue );

			//add a checkbox
			wxSizerItemList list = GetSizer( )->GetChildren();
			wxSizerItemList::iterator it = list.begin( );
			wxSizer* sizer_2 = NULL;
			while ( it != list.end( ) && sizer_2 == NULL )
			{
				if ( (*it)->IsSizer( ) )
				{
					sizer_2 = (*it)->GetSizer( );
				}
				it++;
			}

			sizer_2->Add(checkBoxVector[checkBoxVector.size()-1], 0, 0, 0);

			//connect the checkbox to CheckBoxClicked
			Connect( 
				idWx, 
				wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(CGNSFileReaderWidget::CheckBoxClicked) 
				);

			GetSizer()->Fit(this);
			Layout();
		}

	} 	
	coreCatchExceptionsReportAndNoThrowMacro( 
		"CGNSFileReaderWidget::SetProperties" );
}

void Core::Widgets::CGNSFileReaderWidget::GetProperties( blTagMap::Pointer tagMap )
{
	tagMap->AddTags( m_tagMap );
}

Core::BaseProcessor::Pointer Core::Widgets::CGNSFileReaderWidget::GetProcessor()
{
	return NULL;
}

CGNSFileReaderWidget::CGNSFileReaderWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
	CGNSFileReaderWidgetUI(parent, id, pos, size, style)
{

}

CGNSFileReaderWidget::~CGNSFileReaderWidget( ) 
{
	std::cout << "CGNSFileReaderWidget destructor" << std::endl;
}

void CGNSFileReaderWidget::CheckBoxClicked(wxCommandEvent& event)
{
	int pos;
	bool bChecked;
	blTag::Pointer tag;

	pos = event.GetId() - 101;
	if ( pos < 0 || size_t(pos) >= checkBoxVector.size() )
	{
		return;
	}

	wxCheckBox* checkBox = checkBoxVector[pos];
	bChecked = checkBox->IsChecked();
	tag = m_tagMap->FindTagByName(namesVector[pos]);

	if (bChecked)
	{
		tag->SetValue(originalValues[pos]);
	}
	else 
	{
		tag->SetValue(-1);
	}
}

void CGNSFileReaderWidget::CompressionOrNot(wxCommandEvent& event)
{
	bool bChecked = checkbox_2->GetValue();
	if (bChecked)
	{
		m_tagMap->AddTag("UseCompression",4);
	}
	else
	{
		m_tagMap->AddTag("UseCompression",0);
	}

}

