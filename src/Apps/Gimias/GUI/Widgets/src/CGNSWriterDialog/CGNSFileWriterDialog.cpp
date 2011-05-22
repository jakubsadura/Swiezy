/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CGNSFileWriterDialog.h"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::CGNSFileWriterDialog, wxDialog)
END_EVENT_TABLE();

CGNSFileWriterDialog::CGNSFileWriterDialog() :
	CGNSFileWriterDialogUI(NULL,wxID_ANY,"")
{
	try 
	{
	
	} 	
	coreCatchExceptionsReportAndNoThrowMacro( 
		"CGNSFileWriterDialog::CGNSFileWriterDialog" );
}

CGNSFileWriterDialog::~CGNSFileWriterDialog( ) 
{
	std::cout << "CGNSFileWriterDialog destructor" << std::endl;
}

void CGNSFileWriterDialog::CheckBoxClicked(wxCommandEvent& event)
{
	int pos = event.GetId();
	wxCheckBox* checkBox = m_MapIdToCheckBox[pos];

	bool bChecked = checkBox->GetValue();

	std::pair<int,bool> posPair = m_MapSelected[pos];
	posPair.second = bChecked;

	m_MapSelected[pos] = posPair;
}

Core::BaseProcessor::Pointer Core::Widgets::CGNSFileWriterDialog::GetProcessor()
{
	return NULL;
}

void Core::Widgets::CGNSFileWriterDialog::SetProperties( blTagMap::Pointer tagMap )
{
	int idWx = 100;

	std::vector<std::string> scalars;
	std::vector<std::string> vectors;

	blTag::Pointer tag = tagMap->FindTagByName( "ScalarsPoint" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( scalars );
	}

	tag = tagMap->FindTagByName( "VectorsPoint" );
	if ( tag.IsNotNull() )
	{
		tag->GetValue( vectors );
	}

	std::vector<std::string>::iterator it;
	for ( it = scalars.begin(); it != scalars.end(); it++ ) 
	{
		wxCheckBox* checkbox = new wxCheckBox( this, ++idWx, wxT(*it) );
		checkbox->SetValue( true );
		sizer_2->Add(checkbox, 0, 0, 0);

		Connect( 
			idWx, 
			wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(CGNSFileWriterDialog::CheckBoxClicked) 
			);

		m_MapSelected[idWx] = std::pair<int,bool>(1,true);
		m_MapIdToString[idWx] = *it;
		m_MapIdToCheckBox[idWx] = checkbox;
	}

	for ( it = vectors.begin(); it != vectors.end(); it++ ) 
	{	
		wxCheckBox* checkbox = new wxCheckBox( this, ++idWx, wxT(*it) );
		checkbox->SetValue( true );
		sizer_3->Add(checkbox, 0, 0, 0);

		Connect( 
			idWx, 
			wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(CGNSFileWriterDialog::CheckBoxClicked) 
			);

		m_MapSelected[idWx] = std::pair<int,bool>(3,true);
		m_MapIdToString[idWx] = *it;
		m_MapIdToCheckBox[idWx] = checkbox;
	}

	sizer_1->Fit(this);
	Layout();	
}

void Core::Widgets::CGNSFileWriterDialog::GetProperties( blTagMap::Pointer tagMap )
{
	std::vector<std::string> scalars;
	std::vector<std::string> vectors;

	std::map<int,std::pair<int,bool> >::iterator it;
	for ( it = m_MapSelected.begin(); it!= m_MapSelected.end(); ++it )
	{
		if ( (*it).second.first == 1 && (*it).second.second == true )
		{
			scalars.push_back(m_MapIdToString[(*it).first]);
		}
		else if ( (*it).second.first == 3 && (*it).second.second == true )
		{
			vectors.push_back(m_MapIdToString[(*it).first]);
		}
	}
	
	tagMap->AddTag( "ScalarsPoint", scalars );
	tagMap->AddTag( "VectorsPoint", vectors );
}


