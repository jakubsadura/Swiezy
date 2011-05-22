/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkCustomTransferFunctionSelector_H
#define wxMitkCustomTransferFunctionSelector_H

#include "wxMitkWidgetsWin32Header.h"

namespace mitk
{
/*
The wxMitkCustomTransferFunctionSelector is responsible for providing 
custom transferfunctions to the user, so he can quickly select the one he 
likes. 

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 14 Dic 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkCustomTransferFunctionSelector 
	: public wxChoice
{
public:

	wxMitkCustomTransferFunctionSelector(
		wxWindow *parent, 
		wxWindowID id, 
		const wxPoint& pos, 
		const wxSize& size, 
		long style = 0, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxT("wxMitkCustomTransferFunctionSelector"));
	virtual ~wxMitkCustomTransferFunctionSelector(void);

};
 
} // namespace mitk


#endif // wxMitkCustomTransferFunctionSelector_H
