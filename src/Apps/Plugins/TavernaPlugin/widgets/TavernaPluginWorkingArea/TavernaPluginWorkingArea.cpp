/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "TavernaPluginWorkingArea.h"
#include "wxEmbeddedAppWindow.h"
#include "coreKernel.h"
#include "coreSettings.h"
#include "itksys/SystemTools.hxx"

TavernaPlugin::WorkingArea::WorkingArea(  
	wxWindow* parent, int id/*= wxID_ANY*/,
	const wxPoint&  pos /*= wxDefaultPosition*/, 
	const wxSize&  size /*= wxDefaultSize*/, 
	long style/* = 0*/ )
: TavernaPluginWorkingAreaUI(parent, id,pos,size,style)
{
	m_EmbedWin = NULL;
}

TavernaPlugin::WorkingArea::~WorkingArea( )
{
	// We don't need to destroy anything because all the child windows 
	// of this wxWindow are destroyed automatically
}

void TavernaPlugin::WorkingArea::OnInit( )
{
}


bool TavernaPlugin::WorkingArea::Enable( bool enable /* = true */ )
{
	if (enable )
	{
		if ( m_EmbedWin == NULL )
		{
			Core::Runtime::Settings::Pointer settings;
			settings = Core::Runtime::Kernel::GetApplicationSettings();

			std::string tavernaPath;
			std::string tavernaCaption;
			if ( !settings->GetPluginProperty( "Taverna Plugin", "TavernaPath", tavernaPath ) ||
				!itksys::SystemTools::FileExists( tavernaPath.c_str() ) ||
				!settings->GetPluginProperty( "Taverna Plugin", "TavernaCaption", tavernaCaption ) )
			{
				m_lblEmpty->Show( );
				return false;
			}

			m_lblEmpty->Hide( );

			m_EmbedWin = new wxEmbeddedAppWindow( this, wxID_ANY );
			GetSizer()->Add( m_EmbedWin, wxSizerFlags().Expand().Proportion(1) );

			m_EmbedWin->SetAppFilePath( tavernaPath );
			m_EmbedWin->SetWindowName( tavernaCaption );

		}

		m_EmbedWin->LaunchApp( );
	}


	return TavernaPluginWorkingAreaUI::Enable( enable );
}
