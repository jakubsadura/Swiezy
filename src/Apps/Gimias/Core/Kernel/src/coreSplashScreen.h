/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSplashScreen_H
#define coreSplashScreen_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include <wx/splash.h>
#include <time.h>
#include <string>

class wxTextCtrl;
class wxBitmap;


namespace Core
{
namespace Widgets
{
/** 
\brief Displays the Core splash screen and allows displaying 
startup messages.
It also displays the version number, year and copyright.

\ingroup gmKernel
*/
class GMKERNEL_EXPORT SplashScreen : public wxSplashScreen
{
public:
	enum Behaviour { Behaviour_ABOUT, Behaviour_SPLASH };
	coreClassNameMacro(SplashScreen)

	Core::Widgets::SplashScreen::Behaviour GetBehaviour() const;

	SplashScreen(
		const wxBitmap& bitmap, 
		Behaviour type, 
		wxWindow* parent = NULL);
	~SplashScreen(void);
	
	void AppendToStartupMessages(const std::string& message);

private:
	void SetTimeout(long milis);
	void OnCloseWindow(wxCloseEvent& event);

	clock_t timeout;
	wxTextCtrl* m_LblMessage;
	Behaviour m_Behaviour;
};
}
}

#endif
