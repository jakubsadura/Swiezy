/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreProfileWizard_H
#define coreProfileWizard_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreProfile.h"
#include <wx/wizard.h>
#include "corePluginSelectorWidget.h"

class wxWizardEvent;

namespace Core
{
namespace Widgets
{
/** 
\brief The ProfileWizard class provides a Wizard to the user for selecting 
the set of profiles to work with.

The application will show only the widgets designed (related) to each of
the chosen profiles, and 
the others will not be loaded nor displayed.

\ingroup gmWidgets
\author Juan Antonio Moya
\date 14 Mar 2008
*/
class GMWIDGETS_EXPORT ProfileWizard : public wxWizard
{
public:
	ProfileWizard(wxFrame* frame);
	~ProfileWizard(void);
	Core::Profile::Pointer GetSelectedProfile() const;
	wxWizardPageSimple* GetStartPage() const;
	wxWizardPageSimple* GetSecondPage() const;
	void UpdateData( );

protected:
	void SetupFirstPage(void);
	void SetupSecondPage(void);

private:

	wxWizardPageSimple* m_StartPage;
	wxWizardPageSimple* m_PageEnd;
	PluginSelectorWidget* m_PluginSelectorWidget;

	DECLARE_EVENT_TABLE();
};
}
}

#endif // coreProfileWizard_H
