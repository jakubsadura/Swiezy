/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynWxControlFactory.h"

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include <wx/hyperlink.h>

#include "wxCheckableControl.h"
#include "wxSliderWithTextCtrl.h"

dynWxControlFactory::dynWxControlFactory()
{
}

dynWxControlFactory::~dynWxControlFactory()
{
}

wxSizer* dynWxControlFactory::CreateTagWidget( 
	dynBasePanel* panel, 
	wxWindow* parent, 
	const ModuleParameter& moduleParameter )
{
	wxSizer *widget = 0;

	if (moduleParameter.GetTag() == "double")
	{
		widget = CreateNumericControlTagWidget(panel, parent, moduleParameter);
	}
	else if (moduleParameter.GetTag() == "integer")
	{
		widget = CreateNumericControlTagWidget(panel, parent, moduleParameter);
	}
	else if (moduleParameter.GetTag() == "uinteger")
	{
		widget = CreateNumericControlTagWidget(panel, parent, moduleParameter);
	}
	else if (moduleParameter.GetTag() == "boolean")
	{
		widget = CreateCheckBoxControlTagWidget(panel, parent, moduleParameter);
	}
	else if (moduleParameter.GetTag() == "float")
	{
		widget = CreateNumericControlTagWidget(panel, parent, moduleParameter);
	}
	else if (moduleParameter.GetTag() == "string" ||
			moduleParameter.GetTag() == "integer-vector" ||
			moduleParameter.GetTag() == "float-vector" ||
			moduleParameter.GetTag() == "double-vector" ||
			moduleParameter.GetTag() == "string-vector" )
	{
		widget = CreateTextControlTagWidget(panel, parent, moduleParameter);
	}
	else if (moduleParameter.GetTag() == "region")
	{
		widget = CreateTextControlTagWidget(panel, parent, moduleParameter);
	}
	else if (moduleParameter.GetTag() == "point")
	{
		widget = CreateTextControlTagWidget(panel, parent, moduleParameter);
	}
	else if(moduleParameter.GetTag() == "string-enumeration" ||
		moduleParameter.GetTag() == "integer-enumeration" ||
		moduleParameter.GetTag() == "float-enumeration" ||
		moduleParameter.GetTag() == "double-enumeration")
	{
		widget = CreateEnumerationTagWidget(panel, parent, moduleParameter);
	}
	else if (moduleParameter.GetTag() == "file")
	{
		widget = CreateFileTagWidget(panel, parent, moduleParameter);
	}
	else if (moduleParameter.GetTag() == "directory")
	{
		widget = CreateFileTagWidget(panel, parent, moduleParameter);
	}
	else if ( moduleParameter.GetChannel() == "input" )
	{

	}
	else if ( moduleParameter.GetChannel() == "output" )
	{
		// This is an optional parameter that should have a check box
		if ( ( moduleParameter.GetTag() == "image" ||
			moduleParameter.GetTag() == "geometry" ||
			moduleParameter.GetTag() == "transform" ) &&
			moduleParameter.GetIndex() == "" )
		{
			widget = CreateCheckBoxControlTagWidget(panel, parent, moduleParameter);
		}
	}
	else
	{
		wxStaticText* staticText;
		staticText = new wxStaticText(panel, wxID_ANY, wxT(moduleParameter.GetLabel()));
		widget = new wxBoxSizer(wxHORIZONTAL);
		widget->Add(staticText, 1, 0, 0);
	}

	// Not implemented yet
	//else if (moduleParameter.GetTag() == "table")
	//{
	//	widget = CreateTableTagWidget(moduleParameter);
	//}

	return widget; 
}

wxSizer* dynWxControlFactory::CreateTextControlTagWidget( 
	dynBasePanel* panel,
	wxWindow* parent, 
	const ModuleParameter& moduleParameter )
{
	wxWindow* staticText;
	wxTextCtrl* text;
	staticText = new wxStaticText(parent, wxID_ANY, wxT(moduleParameter.GetLabel()));
	staticText->SetToolTip( moduleParameter.GetDescription() );
	text = new wxTextCtrl(parent, wxID_ANY, wxT( moduleParameter.GetDefault() ) );
	text->SetName( moduleParameter.GetName() );
	text->SetToolTip( moduleParameter.GetDescription() );

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(staticText, wxSizerFlags().Proportion( 1 ).Align( wxALIGN_CENTER_VERTICAL ) );
	sizer->Add(text, wxSizerFlags( ) );

	panel->GetUpdater()->AddControlUpdater( 
		new gbl::ControlUpdater<wxTextCtrl>( moduleParameter.GetName(), text ) );

	return sizer; 
}

wxSizer* dynWxControlFactory::CreateCheckBoxControlTagWidget( 
	dynBasePanel* panel, 
	wxWindow* parent, 
	const ModuleParameter& moduleParameter )
{
	wxCheckableControl* checkBox;
	checkBox = new wxCheckableControl(parent, wxID_ANY);
	checkBox->SetLabel( wxT(moduleParameter.GetLabel()) );
	checkBox->SetName( moduleParameter.GetName() );
	checkBox->SetToolTip( moduleParameter.GetDescription() );

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(checkBox, wxSizerFlags( ).Align( wxALIGN_CENTER_VERTICAL ).Border( wxUP | wxDOWN ) );

	panel->GetUpdater()->AddControlUpdater( 
		new gbl::ControlUpdater<wxCheckableControl>( moduleParameter.GetName(), checkBox ) );

	return sizer; 
}

wxSizer* dynWxControlFactory::CreateFileTagWidget( 
	dynBasePanel* panel, 
	wxWindow* parent, 
	const ModuleParameter& moduleParameter )
{
	wxWindow* staticText;
	staticText = new wxStaticText(parent, wxID_ANY, wxT(moduleParameter.GetLabel()));
	staticText->SetToolTip( moduleParameter.GetDescription() );

	wxTextCtrl* text;
	text = new wxTextCtrl(parent, wxID_ANY, wxT( moduleParameter.GetDefault() ) );
	text->SetName( moduleParameter.GetName() );
	text->SetToolTip( moduleParameter.GetDescription() );

	wxButton* button;
	button = new wxButton(parent, wxID_BROWSE_BUTTON, wxT("..."), wxDefaultPosition, wxSize(21,21) );
	button->SetName( moduleParameter.GetName() + "Button" );

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(staticText, wxSizerFlags().Proportion( 1 ).Align( wxALIGN_CENTER_VERTICAL ) );
	sizer->Add(text, wxSizerFlags( ).Proportion( 1 ) );
	sizer->Add(button, wxSizerFlags( ).Proportion( 0 ) );

	panel->GetUpdater()->AddControlUpdater( 
		new gbl::ControlUpdater<wxTextCtrl>( moduleParameter.GetName(), text ) );

	return sizer;
}

wxSizer* dynWxControlFactory::CreateEnumerationTagWidget( 
	dynBasePanel* panel, 
	wxWindow* parent, 
	const ModuleParameter& moduleParameter )
{
	std::vector<std::string>::const_iterator sbeginit= moduleParameter.GetElements().begin();
	std::vector<std::string>::const_iterator sendit= moduleParameter.GetElements().end();
	std::vector<std::string>::const_iterator sit;
	wxArrayString stringArray;
	for (sit = sbeginit; sit != sendit; ++sit)
	{
		stringArray.Add( (*sit).c_str() );
	}

	wxRadioBox* radioBox;
	radioBox = new wxRadioBox(parent, wxID_ANY, wxT( moduleParameter.GetLabel() ), wxDefaultPosition, wxDefaultSize, stringArray );
	radioBox->SetName( moduleParameter.GetName() );
	radioBox->SetToolTip( moduleParameter.GetDescription() );


	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(radioBox, wxSizerFlags().Proportion( 1 ) );

	panel->GetUpdater()->AddControlUpdater( 
		new gbl::ControlUpdater<wxRadioBox>( moduleParameter.GetName(), radioBox ) );

	return sizer; 
}

wxSizer* dynWxControlFactory::CreateHelpWidget( wxWindow* parent, ModuleDescription *module )
{
	wxNotebook* notebook = new wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);


	wxPanel* panel = new wxPanel( notebook, wxID_ANY, wxDefaultPosition, wxSize(30,100) );
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer( sizer );

	wxTextCtrl* textHelp;
	textHelp = new wxTextCtrl(panel, wxID_ANY, wxT( module->GetDescription() ), 
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY );
	sizer->Add(textHelp, wxSizerFlags( ).Proportion(1).Expand() );

	if ( !module->GetDocumentationURL().empty() )
	{
		wxHyperlinkCtrl* hyperLink;
		hyperLink = new wxHyperlinkCtrl( panel, wxID_ANY, 
			wxT( "Online Documentation" ), module->GetDocumentationURL() );
		sizer->Add(hyperLink, wxSizerFlags( ).Right() );
	}

	notebook->AddPage( panel, wxT( "Help" ) );


	wxTextCtrl* textAck;
	textAck = new wxTextCtrl(notebook, wxID_ANY, wxT( module->GetAcknowledgements() ), 
		wxDefaultPosition, wxSize(30,100), wxTE_MULTILINE | wxTE_READONLY );

	notebook->AddPage( textAck, wxT( "Acknowledgment" ) );

	wxSizer * outSizer = new wxBoxSizer(wxVERTICAL);
	outSizer->Add(notebook, wxSizerFlags( ).Proportion( 0 ).Expand() );

	return outSizer;
	
}

wxSizer* dynWxControlFactory::CreateNumericControlTagWidget( 
	dynBasePanel* panel, wxWindow* parent, const ModuleParameter& moduleParameter )
{

	if ( moduleParameter.GetConstraints().empty() )
	{
		return CreateTextControlTagWidget( panel, parent, moduleParameter );
	}
	else
	{
		return CreateSliderControlTagWidget( panel, parent, moduleParameter );
	}
}

wxSizer* dynWxControlFactory::CreateSliderControlTagWidget( 
	dynBasePanel* panel, wxWindow* parent, const ModuleParameter& moduleParameter )
{
	wxWindow* staticText;
	wxSliderWithTextCtrl* slider;

	float defVal = atof( moduleParameter.GetDefault().c_str() );
	float minVal = atof( moduleParameter.GetMinimum().c_str() );
	float maxVal = atof( moduleParameter.GetMaximum().c_str() );
	float step = atof( moduleParameter.GetStep().c_str() );
	if ( step == 0 )
	{
		step = 1.0f;
	}

	slider = new wxSliderWithTextCtrl(parent, wxID_ANY, defVal, minVal, maxVal, step );

	staticText = new wxStaticText(parent, wxID_ANY, wxT(moduleParameter.GetLabel()));
	staticText->SetToolTip( moduleParameter.GetDescription() );
	slider->SetName( moduleParameter.GetName() );
	slider->SetToolTip( moduleParameter.GetDescription() );

	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(staticText, wxSizerFlags().Expand() );
	sizer->Add(slider, wxSizerFlags( ).Expand() );

	panel->GetUpdater()->AddControlUpdater( 
		new gbl::ControlUpdater<wxSliderWithTextCtrl>( moduleParameter.GetName(), slider ) );

	return sizer;
}
