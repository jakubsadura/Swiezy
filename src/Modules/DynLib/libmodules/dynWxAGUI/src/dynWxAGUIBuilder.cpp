/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynWxAGUIBuilder.h"
#include "dynModuleExecutionImpl.h"
#include "wxID.h"

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "wx/collpane.h"

dynWxAGUIBuilder::dynWxAGUIBuilder()
{
	m_Panel = NULL;
	m_ParentWindow = NULL;
	m_ControlFactory = dynWxControlFactory::New();
}

dynWxAGUIBuilder::~dynWxAGUIBuilder()
{
}

ModuleDescription *dynWxAGUIBuilder::GetModule() const
{
	return m_Module;
}

void dynWxAGUIBuilder::SetModule( ModuleDescription *val )
{
	m_Module = val;
}

dynBasePanel* dynWxAGUIBuilder::GetPanel() const
{
	return m_Panel;
}

wxWindow* dynWxAGUIBuilder::GetParentWindow() const
{
	return m_ParentWindow;
}

void dynWxAGUIBuilder::SetParentWindow( wxWindow* val )
{
	m_ParentWindow = val;
}

void dynWxAGUIBuilder::Update()
{
	m_Panel = new dynBasePanel( m_ParentWindow, wxID_ANY );
	m_Panel->SetModule( GetModule() );

	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	m_Panel->SetSizer(sizer_1);
	sizer_1->Fit(m_Panel);

	AddHelp( );

	AddParameterGroups( );

	AddApplyButton( );

	AddAdvancedOptions( );

	// Layout parameter controls
	m_ControlFactory->LayoutAllControls( m_Panel, m_Module );

	RemoveEmptyGroups( );
}


void dynWxAGUIBuilder::AddParameterGroups()
{
	// iterate over each parameter group
	std::vector<ModuleParameterGroup>::iterator pgIt;
	for ( pgIt = m_Module->GetParameterGroups().begin();
		pgIt != m_Module->GetParameterGroups().end(); ++pgIt)
	{
		AddParameterGroup(*pgIt);
	}

}

void dynWxAGUIBuilder::AddParameterGroup( 
	const ModuleParameterGroup &parameterGroup )
{
	// Add a collapsible pane
	wxCollapsiblePane *collpane = new wxCollapsiblePane(
		m_Panel, wxID_ANY, wxT(parameterGroup.GetLabel().c_str()), wxDefaultPosition, wxDefaultSize, 
		wxTAB_TRAVERSAL | wxNO_BORDER | wxCP_NO_TLW_RESIZE );
	collpane->SetName( parameterGroup.GetLabel() );
	// add the pane with a zero proportion value to the 'sz' sizer which contains it
	m_Panel->GetSizer( )->Add(collpane, 0, wxGROW|wxALL, 5);

	wxWindow *win = collpane->GetPane();
	win->SetSizer(new wxBoxSizer(wxVERTICAL));

	// iterate over each parameter in this group
	std::vector<ModuleParameter>::const_iterator pBeginIt = parameterGroup.GetParameters().begin();
	std::vector<ModuleParameter>::const_iterator pEndIt = parameterGroup.GetParameters().end();
	for (std::vector<ModuleParameter>::const_iterator pIt = pBeginIt; pIt != pEndIt; ++pIt)
	{
		AddParameter(win, *pIt);
	}

	win->GetSizer( )->SetSizeHints(win);
}

void dynWxAGUIBuilder::AddParameter( 
	wxWindow* parent, 
	const ModuleParameter& moduleParameter )
{
	if (moduleParameter.GetHidden() == "true")
	{
		return;
	}

	std::string label = moduleParameter.GetLabel();
	std::string description = moduleParameter.GetDescription();

	// TODO Parameters with flags can support the None node because they are optional
	int noneEnabled = 0; 
	if (moduleParameter.GetLongFlag() != "" || moduleParameter.GetFlag() != "")
	{
		noneEnabled = 1;
	}

	wxSizer *sizer = m_ControlFactory->CreateTagWidget( m_Panel, parent, moduleParameter);
	if ( sizer )
	{
		parent->GetSizer()->Add(sizer, 0, wxEXPAND | wxALL, 1);
	}

}


void dynWxAGUIBuilder::AddApplyButton()
{
	if ( m_Module->GetUpdateFunc().empty() && m_Module->GetType() == "RawDynLibModule" )
	{
		return;
	}

	wxButton* button = new wxButton(m_Panel, wxID_RESET_BUTTON, wxT("Reset"));
	wxButton* button1 = new wxButton(m_Panel, wxID_APPLY, wxT("Apply"));
	wxButton* button2 = new wxButton(m_Panel, wxID_SAVE_AS_SCRIPT, wxT("Save Script"));

	wxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(button, wxSizerFlags( ).Expand().Border().Right() );
	sizer->Add(button1, wxSizerFlags( ).Border().Right() );
	sizer->Add(button2, wxSizerFlags( ).Border().Right() );

	m_Panel->GetSizer()->Add(sizer, wxSizerFlags( ).Right().Border() );
}

void dynWxAGUIBuilder::SetControlFactory( dynWxControlFactoryBase::Pointer val )
{
	m_ControlFactory = val;
}

void dynWxAGUIBuilder::RemoveEmptyGroups()
{
	std::vector<ModuleParameterGroup>::iterator pgIt;
	for ( pgIt = m_Module->GetParameterGroups().begin();
		pgIt != m_Module->GetParameterGroups().end(); ++pgIt)
	{
		wxWindow* win = m_Panel->FindWindowByLabel( pgIt->GetLabel(), m_Panel );
		wxCollapsiblePane* pane = wxDynamicCast( win, wxCollapsiblePane );

		if ( pane && pane->GetPane()->GetChildren( ).empty( ) )
		{
			pane->Hide();
		}
	}
}

void dynWxAGUIBuilder::AddAdvancedOptions()
{
	if ( m_Module->GetAlternativeType() == "CommandLineModule" )
	{
		wxCheckBox* checkbox = new wxCheckBox(m_Panel, wxID_RUN_AS_COMMAND_LINE, wxT("Run as executable"));
		m_Panel->GetSizer()->Add(checkbox, wxSizerFlags( ).Border() );
	}
	
}

void dynWxAGUIBuilder::AddHelp()
{
	// Add a collapsible pane
	wxCollapsiblePane *collpane = new wxCollapsiblePane(
		m_Panel, wxID_ANY, wxT("Help and Acknowledgment"), wxDefaultPosition, wxDefaultSize, 
		wxTAB_TRAVERSAL | wxNO_BORDER | wxCP_NO_TLW_RESIZE );
	// add the pane with a zero proportion value to the 'sz' sizer which contains it
	m_Panel->GetSizer( )->Add(collpane, 0, wxGROW|wxALL, 5);

	wxWindow *win = collpane->GetPane();
	win->SetSizer(new wxBoxSizer(wxVERTICAL));

	wxSizer *sizer = m_ControlFactory->CreateHelpWidget( win, m_Module );
	if ( sizer )
	{
		win->GetSizer()->Add(sizer, 0, wxEXPAND | wxALL, 1);
	}

	win->GetSizer( )->SetSizeHints(win);
}
