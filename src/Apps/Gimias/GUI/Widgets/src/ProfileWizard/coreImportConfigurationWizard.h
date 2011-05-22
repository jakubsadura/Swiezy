/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreImportConfigurationWizard_H
#define coreImportConfigurationWizard_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreProfile.h"
#include <wx/wizard.h>

class wxWizardEvent;

namespace Core
{
namespace Widgets
{
/** 
\brief Wizard for importing previous Gimias configuration.

\ingroup gmWidgets
\author Xavi Planes
\date 26 Nov 2010
*/
class GMWIDGETS_EXPORT ImportConfigurationWizard : public wxWizard
{
public:
	ImportConfigurationWizard(wxFrame* frame);
	~ImportConfigurationWizard(void);
	wxWizardPageSimple* GetStartPage() const;
	wxWizardPageSimple* GetSecondPage() const;

	//! return true if something is found
	bool ScanPreviousConfigurations( );

	//!
	bool ImportPreviousConfiguration( );

protected:
	void SetupFirstPage(void);
	void SetupSecondPage(void);
	void SetupEndPage( );
	void OnPageChanged( wxWizardEvent& event );

private:

	wxWizardPageSimple* m_StartPage;
	wxWizardPageSimple* m_SecondPage;
	wxWizardPageSimple* m_EndPage;
	
	//!
	wxListBox* m_ConfigurationList;
	//!
	wxStaticText* m_lblTextTitleEndPage;
	//!
	wxStaticText* m_lblTextLabelEndPage;


	//! Map of configuration version and path
	std::map<std::string,std::string> m_ConfigurationMap;
	//!
	bool m_ImportConfigurationSuccess;

	DECLARE_EVENT_TABLE();
};
}
}

#endif // coreImportConfigurationWizard_H
