/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMissingDataEntityFieldsWizard_H
#define coreMissingDataEntityFieldsWizard_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreProfile.h"
#include "coreDataEntity.h"
#include <wx/wizard.h>

class wxWizardEvent;

namespace Core
{
namespace Widgets
{
/** 
\brief The MissingDataEntityFieldsWizard class provides a Wizard to the 
user for selecting the set of profiles to work with.

The application will show only the widgets designed (related) to each
of the chosen profiles, and the others will not be loaded nor displayed.

\ingroup gmWidgets
\author Juan Antonio Moya
\date 26 Mar 2008
*/
class GMWIDGETS_EXPORT MissingDataEntityFieldsWizard : public wxWizard
{
public:
	MissingDataEntityFieldsWizard(
		wxFrame* frame, 
		Core::DataEntity::Pointer dataEntity);
	~MissingDataEntityFieldsWizard(void);
	wxWizardPageSimple* GetStartPage(void);

	Core::ModalityType GetSelectedModality() const;

protected:
	typedef std::map<int, Core::ModalityType> ModalityMap;
	void SetupModalityPage(void);

private:
	void OnModalityItemSelected(wxCommandEvent& event);

	wxWizardPageSimple* m_ModalityPage;
	Core::ModalityType m_SelectedModality;
	ModalityMap m_ModalityMap;
	wxListBox* m_ListModality;

	DECLARE_EVENT_TABLE();
};
}
}

#endif // coreMissingDataEntityFieldsWizard_H
